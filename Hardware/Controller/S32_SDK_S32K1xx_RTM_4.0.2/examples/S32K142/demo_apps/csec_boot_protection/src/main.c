/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */


/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"
#include "flash_driver.h"
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif

volatile int exit_code = 0;

#include <stdint.h>
#include <stdbool.h>

#include "csec_utils.h"

#define EVB

#ifdef EVB
    #define LED_PORT       PTD
    #define LED_OK         15U
    #define LED_ERROR      16U
#else
    #define LED_PORT       PTC
    #define LED_OK         0U
    #define LED_ERROR      1U
#endif

/* Set this macro-definition to 1 if you want to reset all the keys and partition flash
 * when running the program from RAM and write to flash in order to test the secure boot
 * when running the program from FLASH */
#define FLASH_MODIFY 1

/* Comment this line after the first run and uncomment it to initialize the flash. */
#define INIT_PHASE

#ifndef FLASH_TARGET

/* Flash configuration */
static const flash_user_config_t flash1_InitConfig0 = {
    .PFlashBase  = 0x00000000U,                     /* Base address of Program Flash block */
    .PFlashSize  = 0x00040000U,                     /* Size of Program Flash block         */
    .DFlashBase  = 0x10000000U,                     /* Base address of Data Flash block    */
    .EERAMBase   = 0x14000000U,                     /* Base address of FlexRAM block */
    /* If using callback, any code reachable from this function must not be placed in a Flash block targeted for a program/erase operation.*/
    .CallBack    = NULL_CALLBACK
};

bool initFlashForCsecOperation(void)
{
  flash_ssd_config_t flashSSDConfig;
  status_t status = STATUS_SUCCESS;

  if (FLASH_DRV_Init(&flash1_InitConfig0, &flashSSDConfig) != STATUS_SUCCESS)
    return 0;

  if (flashSSDConfig.EEESize == 0)
  {
    /* Flash partitioning for CSEc operation must only be ran with RAM configuration.
     * The first time when running the example on the board, or after a key erase,
     * this example should be ran from RAM, in order to enable CSEc operation. Please
     * refer to the documentation for more information. */
    uint32_t address;
    uint32_t size;

#if (FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD == 1u)
    uint8_t unsecure_key[FTFx_PHRASE_SIZE] = {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFEu, 0xFFu, 0xFFu, 0xFFu};
#else   /* FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD */
    uint8_t unsecure_key[FTFx_LONGWORD_SIZE] = {0xFEu, 0xFFu, 0xFFu, 0xFFu};
#endif  /* FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD */

    /* First, erase all Flash blocks to ensure the IFR region is blank
     * before partitioning FlexNVM and FlexRAM */
    status = FLASH_DRV_EraseAllBlock(&flashSSDConfig);
    if (status != STATUS_SUCCESS)
    {
      return 0;
    }
    /* Reprogram secure byte in Flash configuration field */
#if (FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD == 1u)
    address = 0x408u;
    size = FTFx_PHRASE_SIZE;
#else   /* FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD == 1u */
    address = 0x40Cu;
    size = FTFx_LONGWORD_SIZE;
#endif /* FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD */
    status = FLASH_DRV_Program(&flashSSDConfig, address, size, unsecure_key);
    if (status != STATUS_SUCCESS)
    {
      return 0;
    }

    status = FLASH_DRV_DEFlashPartition(&flashSSDConfig, 0x2, 0x4, 0x3, false, true);
    if (status != STATUS_SUCCESS)
    {
      return 0;
    }
  }
  return 1;
}
#endif /* FLASH_TARGET */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - __init_hardware()
 * - main()
 *   - PE_low_level_init()
 *     - Common_Init()
 *     - Peripherals_Init()
*/
int main(void)
{
  /* Write your code here */
  /* Initialize and configure clocks
   *  - see clock manager component for details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
            g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);

  /* Initialize pins
   *  - See PinSettings component for more info
   */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

  /* Initialize CSEc driver */
  CSEC_DRV_Init(&csecState);

#ifndef FLASH_TARGET
  bool retVal;
#if FLASH_MODIFY
  /* Erase all CSEc keys, if any */
  eraseKeys();

#ifdef INIT_PHASE
  /* Initialize Flash for CSEc operation */
  retVal = initFlashForCsecOperation();
  DEV_ASSERT(retVal);
#else /* INIT_PHASE */
  /* suppress unused variable warning */
  (void) retVal;

#endif

#else /* FLASH_MODIFY */
  status_t statusVal;
  uint8_t key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  uint8_t test_key[16] = {0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
		  	  	0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
  uint32_t u32BootSize = 0x00018000U;

  /* Load the MASTER_ECU key with a known value, which will be used as Authorization
   * key (a secret key known by the application in order to configure other user keys) */
  retVal = setAuthKey();
  DEV_ASSERT(retVal);

  /* load CSEC_BOOT_MAC_KEY */
  retVal = loadKey(CSEC_BOOT_MAC_KEY, key, 1, 0);
  DEV_ASSERT(retVal);

  /* enable serial secure boot */
  statusVal = CSEC_DRV_BootDefine(u32BootSize, CSEC_BOOT_SERIAL);
  DEV_ASSERT(statusVal == STATUS_SUCCESS);

  /* load a user key with the boot protected flag set to 1 */
  retVal = loadKey(CSEC_KEY_1, test_key, 1, 1);
  DEV_ASSERT(retVal);

#endif /* FLASH_MODIFY */

#else /* FLASH_TARGET */
  status_t statusVal;
  /* Output direction for the LEDs */
  PINS_DRV_SetPinsDirection(LED_PORT, (1 << LED_OK) | (1 << LED_ERROR));

  /* Set Output value of the LEDs */
  PINS_DRV_SetPins(LED_PORT, (1 << LED_ERROR) | (1 << LED_OK));

  uint8_t input_text[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  uint8_t output_text[16];
  /* this variable is used to modify the .text segment of the flash by having
   * a different assignment in two separate runs in order to show the secure
   * boot functionality */
  volatile int a = 0;
  /* suppress unused variable warning */
  (void)a;

  /* try to perform an encryption using the test key */
  statusVal = CSEC_DRV_EncryptECB(CSEC_KEY_1, input_text, 16, output_text, 10);
  if(statusVal == STATUS_SUCCESS) {
    PINS_DRV_ClearPins(LED_PORT, 1 << LED_OK);
  }
  else {
    PINS_DRV_ClearPins(LED_PORT, 1 << LED_ERROR);
  }
#if FLASH_MODIFY
  /* modified assignment */
  a = 2;
#else
  /* original assignment */
  a = 1;
#endif /* FLASH_MODIFY */
#endif /* FLASH_TARGET */

  /* Disable interrupt for CSEc */
  CSEC_DRV_Deinit();

  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
