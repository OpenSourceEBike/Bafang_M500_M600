/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * NXP Confidential. This software is owned or controlled by NXP and may only be
 * used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.
 */

#include "sdk_project_config.h"
#include "interrupt_manager.h"

/* This example is setup to work by default with EVB. For running other applications is recommended
 * to move the SBC in FNMC after running this example, otherwise CAN transceiver can be
 * disabled because SBC goes in LIMP mode due to lack of watchdog feed.
 * To use it with other boards please comment the following line:
 */

#define RXD_GPIO_MASK      0x0010U
#define RXD_PORT           PORTE
#define RXD_GPIO           PTE
#define RXD                (4U)
#define SW3_GPIO           PTC
#define SW3_PORT           PORTC
#define SW3_GPIO_MASK      0x2000U
#define SW3               (13U)
#define SW2_GPIO          PTC
#define SW2_PORT          PORTC
#define SW2_GPIO_MASK     0x1000U
#define SW2               (12U)
#define LED_PORT          PORTD
#define LED_GPIO          PTD
#define PCC_CLOCK         PCC_PORTD_CLOCK
#define LED_RED           (15U)
#define LED_GREEN         (16U)
#define LED_BLUE          (0U)
#define LPIT_CHANNEL      (0U)
#define POWER_MODES_NUM   (2u)
#define RUN               (0u) /* High speed run      */
#define SLEEP             (1u) /* Very low power stop */

uint8_t timerCount = 0;
volatile uint8_t wakeUpEvnt = false;

/* @brief Interrupt service routine for RDX.
 * MCU is wake-up using CAN wake-up signal. */
static void CanWakeUpISR(void);

/* @brief Interrupt service routine for WatchDog refreshing. */
static void SBC_FeedWachdogAndLeds(void);

/* Interrupt service routine for RDX.
 * MCU is wake-up using CAN wake-up signal. */
static void CanWakeUpISR(void)
{
    /* Clear button IRQ flag */
    PINS_DRV_ClearPortIntFlagCmd(RXD_PORT);
    /* Flag for wake up event. */
    wakeUpEvnt = true;
    /* Start Feeding of WachDog by LPIT timer interrupts. */
    LPIT_DRV_StartTimerChannels(INST_LPIT1, 1U << LPIT_CHANNEL);    /* Check if some event on SBC occurred. */
}

/* Interrupt service routine for WachDog refreshing. */
static void SBC_FeedWachdogAndLeds(void)
{
    SBC_FeedWatchdog();
    /* Toggle with blue led to show that WachDog is triggered. */
    PINS_DRV_TogglePins(LED_GPIO, 1 << LED_BLUE);

    /* Clear interrupt flag.*/
    LPIT_DRV_ClearInterruptFlagTimerChannels(INST_LPIT1, 1 << LPIT_CHANNEL);
}

/*!
 \brief The main function for the project.
 */
int main(void)
{
    /* Initialize and configure clocks
     * 	-	see clock manager component for details
     */
    status_t status = STATUS_SUCCESS;
    sbc_factories_conf_t factoriesConf;

    /* Initialization clocks . */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
            g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, POWER_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *	-	See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize Power Manager */
    POWER_SYS_Init(&powerConfigsArr, POWER_MODES_NUM, &powerStaticCallbacksConfigsArr, 0);

    /* Install hander for CanWakeUp pin ISR. For wake up event is used RXD -
     * SBC_CAN_RX pin of SBC_UJA1669. This pin is used for wake up MCU
     * over CAN. There are only interrupt pin instead of Can driver in order
     * to fit simpler example and make impress on state machine.
     * This pin is assert low when wake up event occurred.
     * Wake up event can occurred if there some error condition or using
     * WAKE pin of SBC or over can Can.
     *  */
    INT_SYS_InstallHandler(PORTE_IRQn, CanWakeUpISR, NULL);
    /* Enable button port IRQ */
    INT_SYS_EnableIRQ(PORTE_IRQn);

    INT_SYS_SetPriority(PORTE_IRQn, 2);

    /* Set Output value LED0 & LED_RED */
    PINS_DRV_ClearPins(LED_GPIO, 1 << LED_RED);
    PINS_DRV_ClearPins(LED_GPIO, 1 << LED_GREEN);
    PINS_DRV_ClearPins(LED_GPIO, 1 << LED_BLUE);

    INT_SYS_SetPriority(LPSPI1_IRQn, 1);

    status = LPSPI_DRV_MasterInit(INST_LPSPICOM1, &lpspiCom1State, &lpspiCom1_MasterConfig0);

    DEV_ASSERT(status == STATUS_SUCCESS);

    /* PIT initialization for periodic 100ms WachDog refresh. */

    /* Initialize LPIT instance 0
     *  -   Reset and enable peripheral
     */
    LPIT_DRV_Init(INST_LPIT1, &lpit1_InitConfig);
    /* Initialize LPIT channel 0 and configure it as a periodic counter
     * which is used to generate an interrupt every second.
     */

    /* Channel 1 configuration. */
    LPIT_DRV_InitChannel(INST_LPIT1, LPIT_CHANNEL, &lpit1_ChnConfig0);

    /* Install LPIT_ISR as LPIT interrupt handler */
    INT_SYS_InstallHandler(LPIT0_Ch0_IRQn, &SBC_FeedWachdogAndLeds, NULL);
    /* Enable LPIT interrupt. */
    INT_SYS_EnableIRQ(LPIT0_Ch0_IRQn);
    /* Set Priority lower to third place. */
    INT_SYS_SetPriority(LPIT0_Ch0_IRQn, 3U);

    /* Initialize SBC. If status is != STATUS_SUCCESS probably SBC is in FNMC. This mode will be 
     * changed with SBC_ChangeFactoriesSettings.
     * Additional delay to allow SBC to leave reset mode - MCU starts running as soon as it's powered
     * instead of after SBC RST is released, because the reset pins are not connected on the EVB. */
    SBC_InitDriver(INST_LPSPICOM1);
    OSIF_TimeDelay(20);
    status = SBC_InitDevice(&sbc_uja116x_InitConfig0);

    /* Start Feeding of WachDog by LPIT timer interrupts. */
    LPIT_DRV_StartTimerChannels(INST_LPIT1, 1U << LPIT_CHANNEL);

    /* Test require correct factory settings for example.
     * FNMC bit must be disabled, SBC_UJA_SBC_SDMC_DIS must be disabled
     * and slpc must be allowed. */
    status = SBC_GetFactoriesSettings(&factoriesConf);
    DEV_ASSERT(status == STATUS_SUCCESS);
    if(factoriesConf.control.fnmc == SBC_UJA_SBC_FNMC_EN ||
            factoriesConf.control.sdmc == SBC_UJA_SBC_SDMC_EN ||
            factoriesConf.control.slpc == SBC_UJA_SBC_SLPC_IG)
    {
        /* Set correct settings. */
        factoriesConf.control.fnmc = SBC_UJA_SBC_FNMC_DIS;
        factoriesConf.control.sdmc = SBC_UJA_SBC_SDMC_DIS;
        factoriesConf.control.slpc = SBC_UJA_SBC_SLPC_AC;

        /* If this function return SBC_NVN_ERROR. Factory settings must be preset
         * to default state by Hardware configuration. Please read
         * SBC_ChangeFactoriesSettings descriptions for correct settings.
         * Conditions for Hardware reset value are
         * -pin RSTN is held LOW
         * -CANH is pulled up to VBAT
         * -CANL is pulled down to GND
         * When HW reset is done you can change factory settings by running
         * this example. Be carefully device will be reseted,
         * initialization sequence must be done again. Run the example again
         * with new configuration.
         * */
        status = SBC_ChangeFactoriesSettings(&factoriesConf);
        DEV_ASSERT(status == STATUS_SUCCESS);
    }
   SBC_SetMode(SBC_UJA_MODE_MC_NORMAL);
    /* Set interrupt priorities. */
    INT_SYS_SetPriority(SysTick_IRQn, 0);

    /* SBC has two power save mode. StandBy mode is first power safe mode.
     * MCU stay powered by SBC. In this example SBC is set to StandBy mode and
     * MCU moves to deeply sleep mode. MCU is wake up by SBC_CAN_RX(RXD) where Low
     * level is occurred if there are some wake up event on SBC otherwise it is High.
     * Use WAKE (SW9 button) pin or external CAN to wake up MCU.
     * Sleep mode is second mode which turn of power for MCU.
     * Use SW9 button for wake up SBC.
     * Move SBC to StandBy mode using SW2 button. Use SW9 button for wake up
     * event. Press SW3 button to move to Sleep mode and use SW9
     * for wake up device.
     * LED_GREEN is on in Normal mode otherwise is off.
     * LED_RED is on for 1 second and SBC goes in StandBy mode
     * LED_BLUE is toggle when MCU run and SBC_FeedWatchdog is called.
     * SBC WatchDog is in autonomous mode. The WatchDog is turn of
     * in Stand by mode if RXD is High
     *  */

    sbc_mode_mc_t mode = SBC_UJA_MODE_MC_SLEEP;
    sbc_evn_capt_t event;
    while(1)
    {
        /* Go to StandBy mode if SW2 is preset. */
        if((PINS_DRV_ReadPins(SW2_GPIO)& SW2_GPIO_MASK) >> SW2)
        {
            status = SBC_SetMode(SBC_UJA_MODE_MC_STANDBY);
        }

        /* Go to normal mode from SBC wake up event. */
        if(wakeUpEvnt == true)
        {
            status = SBC_SetMode(SBC_UJA_MODE_MC_NORMAL);

            status = SBC_GetEventsStatus(&event);
            /* Clean Events if some occurred. */
            SBC_CleanEvents(&event);
            wakeUpEvnt = false;
        }

        /* Go to sleep mode if SW3 is preset. */
        if((PINS_DRV_ReadPins(SW3_GPIO)& SW3_GPIO_MASK) >> SW3)
        {
            status = SBC_SetMode(SBC_UJA_MODE_MC_SLEEP);
        }

        /* Check SBC mode each 400ms. */
        if(timerCount >= 2)
        {
            status = SBC_GetMode(&mode);
            if(( mode== SBC_UJA_MODE_MC_NORMAL))
            {
                /* Turn on Green Led. */
                PINS_DRV_SetPins(LED_GPIO, 1 << LED_GREEN);
                /* Turn off Red Led. */
                PINS_DRV_ClearPins(LED_GPIO, 1 << LED_RED);
            }
            else if (( mode== SBC_UJA_MODE_MC_STANDBY))
            {
                /* Turn off Green Led. */
                PINS_DRV_SetPins(LED_GPIO, 1 << LED_RED);
                /* Turn on Red Led. */
                PINS_DRV_ClearPins(LED_GPIO, 1 << LED_GREEN);
                /* Move MCU to sleep mode. */
               if((PINS_DRV_ReadPins(RXD_GPIO)& RXD_GPIO_MASK) >> RXD)
               {
                   /* Turn off timer if RDX is HIGH in sleep mode. WachDog is off
                   * in autonomous mode when stand by mode is set and RXD is high. */
                   LPIT_DRV_StopTimerChannels(INST_LPIT1, 1U << LPIT_CHANNEL);
                   /* Wait second in the StandBy mode to demonstrate WachDog
                    * is not needed. */
                   OSIF_TimeDelay(1000);
               }
               else
               {
                   /* Turn off Green Led. */
                   PINS_DRV_SetPins(LED_GPIO, 1 << LED_RED);
                   /* Turn on Red Led. */
                   PINS_DRV_SetPins(LED_GPIO, 1 << LED_GREEN);
               }
            }
            timerCount = 0;
        }
        timerCount++;
        /* Wait 200ms before new loop is started. */
        OSIF_TimeDelay(200);
    }

    /* Unused variable. It is used only for test reason. */
    (void )status;
}
