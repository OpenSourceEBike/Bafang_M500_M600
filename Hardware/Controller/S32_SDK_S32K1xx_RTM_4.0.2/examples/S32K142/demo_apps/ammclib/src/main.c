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

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "freemaster.h"
#include "sdk_project_config.h"

/* Defines for message communication */
#define WELCOME_TXT     "Welcome to AMMCLib example project !\n"        \
                        " This application shows the use of AMMClib\n"  \
                        " To start the application select one of the\n" \
                        "two modes of operation:\n"                     \
                        " - 1 Basic mathematical functions.\n"          \
                        " - 2 ADC filter and PWM sine generation.\n"    \
                        "\n"

#define BASIC_TXT       "\nBasic mathematical functions selected.\n"    \
                        " Available operations:\n"                      \
                        "\n - Arithmetic: + - * / \n"                   \
                        "\n - Trigonometric:\n"                         \
                        "       sin(x)  cos(x)  tan(x); x in [-180,180]\n"\
                        "       asin(x) acos(x) atan(x); x in [-1,1]\n" \
                        "\n - Square root:    sqrt(x)\n"                \
                        "\n - Absolute value: abs(x)\n"                 \
                        "\n - Negate value:   - x\n"                    \
                        "\nExample use: value1 + value2\n"              \
                        "             sin value\n"                      \
                        "\nTo go back to the menu type exit.\n"         \
                        "\n"

#define ADC_PWM_TXT     "\nADC filter conversions and PWM generation selected.\n"       \
                        "\nPlease disconnect the active terminal and use\n"             \
                        " FreeMASTER to see the variables and plots.\n"                 \
                        "\nTo go back to the menu reset the board, stop FreeMASTER\n"   \
                        " and reconnect the console\n"

#define CLR_SCR         "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"

#define ERROR_TXT       "\n\nOption not available. Try again\n\n\n"

#define LPUART_TX_TIMEOUT 1000U
#define LPUART_RX_TIMEOUT 500U

/* Defines for various PI usage */
#define PI          (3.1415f)
#define TWO_PI      (2 * PI)
#define PI_DIV2     (PI / 2)
#define PI_DIV3     (PI / 3)
#define PI_DIV4     (PI / 4)
#define TWO_PI_DIV3 (TWO_PI / 3)

/* Macro that will convert a frequency to a sine argument */
#define SINE_PARAM(x, y)    (2 * PI * ((x) / (y)))
/* Macro that will scale the sine value to duty cycle */
#define SINE_TO_DUTYCYCLE(x) ((1UL + x) * (FTM_MAX_DUTY_CYCLE / 2))

/* Flag that signals if new data has been generated */
volatile bool newData;
/* Vector used to store sine samples for RGB colors */
volatile float RGB[3];
/* Vector used to store sine frequencies for RGB colors */
volatile float frq[3] = {250, 250, 250};
/* time conversion variable for sampled time */
float sample_time;

#define R (0U)
#define G (1U)
#define B (2U)

/* Variables used for processing FreeMASATER application commands */
FMSTR_APPCMD_CODE cmd;
FMSTR_APPCMD_PDATA cmdDataP;
FMSTR_SIZE cmdSize;

/* MA filter */
GDFLIB_FILTER_MA_T_FLT fltMA = GDFLIB_FILTER_MA_DEFAULT_FLT;
/* MA filter input */
volatile uint16_t adcRawValue;
/* MA Filter output */
volatile float out;

/**
 * @brief This function will convert angles in radians to [-pi, pi)
 * @param angleIn Angle to convert
 * @return The angle normalized to [-pi, pi)
 */
float Normalize_Angle(float angleIn)
{
    /*
     * Perform a normalization of the angle in degrees to
     * 2PI and then subtract PI to obtain a value in
     * [-PI, PI)
     */
    float angle = fmodf(angleIn, TWO_PI);
    return (angle > PI) ? (angle - TWO_PI) : (angle);
}

/**
 * @brief This function will generate a sine sample based on the
 *        input parameters
 * @param frequency Frequency in mHz. The value will be converted to
 *        a sine argument
 * @param amplitude The amplitude of the resulting sine signal
 * @param sample Sample number
 * @param phase Phase to be added to the sine argument
 * @return The value of the sine signal at the respective sample
 */
float Generate_Sine_Signal(float frequency, float amplitude, uint32_t sample, float phase)
{
    float normalizedAngle;
    float sineArgInDegrees;

    /*
     * Perform a Multiply and Accumulate operation on the sine argument,
     * sample number and phase.
     * sineArgInDegrees = ((2 * pi * (f / fs) * sample) + phase)
     */
    sineArgInDegrees = MLIB_Mac_FLT(phase, SINE_PARAM(frequency/1000, sample_time), (float)sample);
    /* Convert the angle to radians */
    normalizedAngle = Normalize_Angle(sineArgInDegrees);
    /* Calculate the sample sine value */
    return amplitude * GFLIB_Sin(normalizedAngle, GFLIB_SIN_DEFAULT);
}

/**
 * @brief Function to configure the filter
 * @param factor The factor to control smoothing of filter
 */
void Setup_Lowpass_Filter(uint32_t factor)
{
    fltMA.fltAcc = 0.0;
    fltMA.fltLambda = (tFloat)(1.0 / factor);
    GDFLIB_FilterMAInit(&fltMA);
}

/**
 * @brief LPTMR Interrupt Service Routine.
 * This ISR will generate the sine signals for the RGB Leds.
 */
void LPTMR_ISR(void)
{
    /* Sample number */
    static uint32_t sample = 0UL;
    /* Clear LPTMR interrupt flag */
    LPTMR_DRV_ClearCompareFlag(INST_LPTMR1);
    /*
     * Generate one sine sample for each color. To create a gradient effect
     * each sine will have a phase difference of PI/3.
     */
    RGB[R] = Generate_Sine_Signal(frq[R], 1.0f, sample, 0);
    RGB[G] = Generate_Sine_Signal(frq[G], 1.0f, sample, PI_DIV2);
    RGB[B] = Generate_Sine_Signal(frq[B], 1.0f, sample, PI);
    /* Increment sample number */
    sample++;
    /* Signal that new data is ready */
    newData = true;
}

/**
 * @brief ADC Interrupt Service Routine.
 * This ISR will calculate the filter output value.
 */
void ADC_ISR(void)
{
    ADC_DRV_GetChanResult(INST_ADCONV1, 0U, (uint16_t *)&adcRawValue);
    out = GDFLIB_FilterMA_FLT((tFloat)adcRawValue, &fltMA);
}

/**
 * @brief Function that uses FreeMASTER and outputs PWM on LEDs
 */
void ADC_PWM_Section(void)
{
    /* Send instructions to terminal */
    LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)ADC_PWM_TXT, strlen(ADC_PWM_TXT), LPUART_TX_TIMEOUT);

    /* Initialize LPTMR as a periodic counter with 1 ms compare value */
    LPTMR_DRV_Init(INST_LPTMR1, &lpTmr1_config0, false);
    /* Calculate the sample time conversion unit */
    sample_time = 1000000 / ((float)lpTmr1_config0.compareValue);
    /* Install the corresponding ISR and enable the interrupt */
    INT_SYS_InstallHandler(LPTMR0_IRQn, &LPTMR_ISR, (isr_t *)NULL);
    INT_SYS_EnableIRQ(LPTMR0_IRQn);

    /* FTM state structure declaration */
    ftm_state_t ftmState;
    /* Initialize FTM in PWM mode */
    FTM_DRV_Init(INST_FLEXTIMER_PWM1, &flexTimer_pwm1_InitConfig, &ftmState);
    /* Initialize PWM channels */
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM1, &flexTimer_pwm1_PwmConfig);

    /* Clear newData flag */
    newData = false;
    /* Start LPTMR counter */
    LPTMR_DRV_StartCounter(INST_LPTMR1);

    /* Setup filter before ADC interrupt is enabled */
    Setup_Lowpass_Filter(1000);
    /* Configure the ADC and enable the interrupt */
    ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig0);
    ADC_DRV_ConfigChan(INST_ADCONV1, 0U, &adConv1_ChnConfig0);
    /* Install the corresponding ISR and enable the interrupt */
    INT_SYS_InstallHandler(ADC0_IRQn, &ADC_ISR, (isr_t *)NULL);
    INT_SYS_EnableIRQ(ADC0_IRQn);

    /* Give control of communication to FreeMASTER */
    INT_SYS_InstallHandler(LPUART1_RxTx_IRQn, &FMSTR_Isr, (isr_t *)NULL);
    /* Initialize FreeMASTER */
    FMSTR_Init();

    while(1)
    {
        /* When new data is ready, update FTM channels */
        if(newData)
        {
            FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM1, 0U, FTM_PWM_UPDATE_IN_DUTY_CYCLE, (uint16_t)SINE_TO_DUTYCYCLE(RGB[R]),0U, false);
            FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM1, 1U, FTM_PWM_UPDATE_IN_DUTY_CYCLE, (uint16_t)SINE_TO_DUTYCYCLE(RGB[G]),0U, false);
            FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM1, 2U, FTM_PWM_UPDATE_IN_DUTY_CYCLE, (uint16_t)SINE_TO_DUTYCYCLE(RGB[B]),0U, true);
            newData = false;
        }

        /* Process FreeMASTER application commands */
        cmd = FMSTR_GetAppCmd();
        if (cmd != FMSTR_APPCMDRESULT_NOCMD)
        {
            cmdDataP = FMSTR_GetAppCmdData(&cmdSize);
            INT_SYS_DisableIRQGlobal();
            switch (cmd)
            {
                case 0:     /* set_FilterMA_lambda command */
                    if(cmdSize / 4 == 1)
                    {
                        Setup_Lowpass_Filter(*(int32_t *)cmdDataP);
                        FMSTR_AppCmdAck(0);
                    }
                    else
                    {
                        FMSTR_AppCmdAck(1);
                    }
                    break;
                case 1:     /* set_RGB_frequency command */
                    if (cmdSize / 4 == 3)
                    {
                        frq[R] = ((int32_t *)cmdDataP)[R];
                        frq[G] = ((int32_t *)cmdDataP)[G];
                        frq[B] = ((int32_t *)cmdDataP)[B];
                        FMSTR_AppCmdAck(0);
                    }
                    else
                    {
                        FMSTR_AppCmdAck(1);
                    }
                    break;
                default:    /* Acknowledge the command with failure */
                    FMSTR_AppCmdAck(1);
                    break;
            }
            INT_SYS_EnableIRQGlobal();
        }
        FMSTR_Poll();
    }
}

/**
 * @brief Function to print float variables
 * @param out   String containing value to be printed
 * @param value Value to be printed
 */
void Print_Float(char *out, float value)
{
    uint16_t val;
    uint16_t frac;
    char *sign;

    /* get the sign of value */
    sign = (value < 0) ? "-" : "";
    /* get the absolute value */
    value = (value < 0) ? (-value) : (value);
    /* get integer part */
    val = (uint16_t)(value);
    /* get fractional part with precision of 4 */
    frac = (uint16_t)((value - val) * 10000);

    /* Print the value with a precision of 4 */
    sprintf(out, "%s%d.%04d\n", sign, val, frac);
}

/**
 * @brief Function interprets user input and gives the result of mathematical operation
 * @param str   String containing user expression
 * @param value The result of user expression
 * @return Expression command
 *      0 - User wants to go back to menu
 *      1 - Continue with mathematical expressions
 */
float Evaluate_Expression(char *str, tFloat *value)
{
    char *tok;
    *value = 0.0;

    /* Remove CRLF from the end of the string */
    if ((char)0x0D == str[strlen(str) - 1])
    {
        str[strlen(str) - 1] = 0;
    }
    if ((char)0x0A == str[strlen(str) - 1])
    {
        str[strlen(str) - 1] = 0;
    }

    /* Interpret expression */
    for(tok = strtok(str, " "); tok; tok = strtok(NULL, " "))
    {
        if(strcmp(tok, "+") == 0)
        {
            tok = strtok(NULL, " ");
            *value = MLIB_Add(*value, (tFloat)atof(tok));
        }
        else if(strcmp(tok, "-") == 0)
        {
            tok = strtok(NULL, " ");
            if ((int16_t)*value == 0)
            {
                *value = MLIB_Neg((tFloat)atof(tok));
            }
            else
            {
                *value = MLIB_Sub(*value, (tFloat)atof(tok));
            }
        }
        else if((strcmp(tok, "*") == 0))
        {
            tok = strtok(NULL, " ");
            *value = MLIB_Mul(*value, (tFloat)atof(tok));
        }
        else if((strcmp(tok, "/") == 0))
        {
            tok = strtok(NULL, " ");
            *value = MLIB_Div(*value, (tFloat)atof(tok));
        }
        else if((strcmp(tok, "sin") == 0))
        {
            tok = strtok(NULL, " ");
            *value = GFLIB_Sin((tFloat)((atof(tok) / 180.0f) * PI), GFLIB_SIN_DEFAULT);
        }
        else if((strcmp(tok, "cos") == 0))
        {
            tok = strtok(NULL, " ");
            *value = GFLIB_Cos((tFloat)((atof(tok) / 180.0f) * PI), GFLIB_COS_DEFAULT);
        }
        else if((strcmp(tok, "tan") == 0))
        {
            tok = strtok(NULL, " ");
            *value = GFLIB_Tan((tFloat)((atof(tok) / 180.0f) * PI), GFLIB_TAN_DEFAULT);
        }
        else if((strcmp(tok, "asin") == 0))
        {
            tok = strtok(NULL, " ");
            *value = ((GFLIB_Asin((tFloat)atof(tok), GFLIB_ASIN_DEFAULT)* 180.0f) / PI);
        }
        else if((strcmp(tok, "acos") == 0))
        {
            tok = strtok(NULL, " ");
            *value = ((GFLIB_Acos((tFloat)atof(tok) , GFLIB_ACOS_DEFAULT)* 180.0f) / PI);
        }
        else if((strcmp(tok, "atan") == 0))
        {
            tok = strtok(NULL, " ");
            *value = ((GFLIB_Atan((tFloat)atof(tok), GFLIB_ATAN_DEFAULT)* 180.0f) / PI);
        }
        else if((strcmp(tok, "sqrt") == 0))
        {
            tok = strtok(NULL, " ");
            *value = GFLIB_Sqrt((tFloat)atof(tok));
        }
        else if((strcmp(tok, "abs") == 0))
        {
            tok = strtok(NULL, " ");
            *value = MLIB_Abs_FLT((tFloat)atof(tok));
        }
        else if((strcmp(tok, "exit") == 0))
        {
            return 0;
        }
        else
        {
            *value = (float)atof(tok);
        }
    }
    return 1;
}

/**
 * @brief Function that communicates with the user
 * Function takes input from the user using LPUART
 * and outputs the result of implemented mathematical
 * operations.
 */
void Basic_Section(void)
{
    char buffer[1024] = {0};
    char out[1024] = {0};
    float value = 0;
    int status = 0;
    int buffer_l = 0;

    /* Send instructions to terminal */
    LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)BASIC_TXT, strlen(BASIC_TXT), LPUART_TX_TIMEOUT);

    /* Infinite loop */
    while(true)
    {
        /* Wait for user to send expression */
        while(true)
        {
            LPUART_DRV_ReceiveDataBlocking(INST_LPUART1, (uint8_t *)buffer, 1024, LPUART_RX_TIMEOUT);
            if (strlen(buffer) != 0)
            {
                break;
            }
        }
        buffer_l = strlen(buffer);
        /* Evaluate user input */
        status = Evaluate_Expression(buffer, &value);
        if (status == 1)
        {
            Print_Float(out, value);
            /* Send result to terminal */
            LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)out, strlen(out), LPUART_TX_TIMEOUT);
            /* Clear output and input buffers */
            memset(buffer, 0 , buffer_l);
            memset(out, 0 ,strlen(out));
            value = 0;
        }
        else
        {/* Return to menu */
            return;
        }
    }
}

int main(void)
{
  status_t error;
  /* Configure clocks for PORT */
  error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
  DEV_ASSERT(error == STATUS_SUCCESS);
  /* Set pins as GPIO */
  error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
  DEV_ASSERT(error == STATUS_SUCCESS);

  /*
   * Initialize LPUART communication
   *  -   See lpuart component for more info
   */
  LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);

  char buffer[1024] = {0};

  /* Infinite menu loop */
  while(true)
  {
    /* Print menu */
    LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)WELCOME_TXT, strlen(WELCOME_TXT), LPUART_TX_TIMEOUT);

    /* Receive option */
    while(true)
    {
      LPUART_DRV_ReceiveDataBlocking(INST_LPUART1, (uint8_t *)buffer, 1024, LPUART_RX_TIMEOUT);
      if (strlen(buffer) != 0)
      {
        break;
      }
    }

    if(strlen(buffer) > 2)
    {
      /* Command received is to long */
      LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)CLR_SCR, strlen(CLR_SCR), LPUART_TX_TIMEOUT);
      LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)ERROR_TXT, strlen(ERROR_TXT), LPUART_TX_TIMEOUT);
    }
    else
    {
      switch(buffer[0])
      {
        case '1':   /* Basic mathematical functions */
          Basic_Section();
          LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)CLR_SCR, strlen(CLR_SCR), LPUART_TX_TIMEOUT);
          break;
        case '2':   /* ADC and PWM LED */
          ADC_PWM_Section();
          LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)CLR_SCR, strlen(CLR_SCR), LPUART_TX_TIMEOUT);
          break;
        default:    /* Unavailable */
          LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)CLR_SCR, strlen(CLR_SCR), LPUART_TX_TIMEOUT);
          LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)ERROR_TXT, strlen(ERROR_TXT), LPUART_TX_TIMEOUT);
          break;
      }
    }
    /* clear input buffer */
    memset(buffer, 0, strlen(buffer));
  }
}
