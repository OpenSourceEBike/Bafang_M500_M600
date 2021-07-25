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

/* Flash configuration */
static const flash_user_config_t flash1_InitConfig0 = {
    .PFlashBase  = 0x00000000U,                     /* Base address of Program Flash block */
    .PFlashSize  = 0x00040000U,                     /* Size of Program Flash block         */
    .DFlashBase  = 0x10000000U,                     /* Base address of Data Flash block    */
    .EERAMBase   = 0x14000000U,                     /* Base address of FlexRAM block */
    /* If using callback, any code reachable from this function must not be placed in a Flash block targeted for a program/erase operation.*/
    .CallBack    = NULL_CALLBACK
};

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

/* Set this macro-definition to 1 if you want to reset all the keys */
#define ERASE_ALL_KEYS	0

void initFlashForCsecOperation(void)
{
	flash_ssd_config_t flashSSDConfig;

	FLASH_DRV_Init(&flash1_InitConfig0, &flashSSDConfig);

	if (flashSSDConfig.EEESize == 0)
	{
#ifdef FLASH_TARGET
		/* Flash partitioning for CSEc operation must only be ran with RAM configuration.
		 * The first time when running the example on the board, or after a key erase,
		 * this example should be ran from RAM, in order to enable CSEc operation. Please
		 * refer to the documentation for more information. */
		PINS_DRV_ClearPins(LED_PORT, 1 << LED_OK);
#else
		uint32_t address;
		uint32_t size;
#if (FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD == 1u)
		uint8_t unsecure_key[FTFx_PHRASE_SIZE] = {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFEu, 0xFFu, 0xFFu, 0xFFu};
#else   /* FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD */
		uint8_t unsecure_key[FTFx_LONGWORD_SIZE] = {0xFEu, 0xFFu, 0xFFu, 0xFFu};
#endif  /* FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD */

		/* First, erase all Flash blocks to ensure the IFR region is blank
		 * before partitioning FlexNVM and FlexRAM */
		FLASH_DRV_EraseAllBlock(&flashSSDConfig);
		/* Reprogram secure byte in Flash configuration field */
#if (FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD == 1u)
		address = 0x408u;
		size = FTFx_PHRASE_SIZE;
#else   /* FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD == 1u */
		address = 0x40Cu;
		size = FTFx_LONGWORD_SIZE;
#endif /* FEATURE_FLS_HAS_PROGRAM_PHRASE_CMD */
		FLASH_DRV_Program(&flashSSDConfig, address, size, unsecure_key);

        FLASH_DRV_DEFlashPartition(&flashSSDConfig, 0x2, 0x4, 0x3, false, true);
#endif /* FLASH_TARGET */
	}
}

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
   * 	-	see clock manager component for details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
						g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);

  /* Initialize pins */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

  /* Turn off the leds */
  PINS_DRV_SetPins(LED_PORT, (1 << LED_ERROR) | (1 << LED_OK));

  bool keyLoaded;
  uint8_t key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

  /* Initialize CSEc driver */
  CSEC_DRV_Init(&csecState);

  /* Initialize Flash for CSEc operation */
  initFlashForCsecOperation();

  /* Load the MASTER_ECU key with a known value, which will be used as Authorization
   * key (a secret key known by the application in order to configure other user keys) */
  setAuthKey();

  /* Load the selected key */
  /* First load => counter == 1 */
  keyLoaded = loadKey(CSEC_KEY_1, key, 1);

  if (keyLoaded)
  {
      /* Test an encryption using the loaded key.
       *
       * key        = 000102030405060708090a0b0c0d0e0f
       * plaintext  = 00112233445566778899aabbccddeeff
       * ciphertext = 69c4e0d86a7b0430d8cdb78070b4c55a
       *
       * The values are extracted from the SHE Spec 1.1 test vectors.
       */
      uint8_t i;

      status_t stat;
      bool encryptionOk = true;
      uint8_t cipherText[16];
      uint8_t plainText[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
      uint8_t expectedCipherText[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04,
        0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};

      stat = CSEC_DRV_EncryptECB(CSEC_KEY_1, plainText, 16U, cipherText, 1U);
      if (stat == STATUS_SUCCESS)
      {
          /* Check if the cipher text is the one expected */
          for (i = 0; i < 16; i++)
          {
              if (cipherText[i] != expectedCipherText[i])
              {
                  encryptionOk = false;
                  break;
              }
          }
      }

      if (encryptionOk)
      {
          PINS_DRV_ClearPins(LED_PORT, 1 << LED_OK);
      }
  }
  else
  {
      PINS_DRV_ClearPins(LED_PORT, 1 << LED_ERROR);
  }


#if ERASE_ALL_KEYS
  if (eraseKeys())
  {
      PINS_DRV_ClearPins(LED_PORT, 1 << LED_OK);
      PINS_DRV_ClearPins(LED_PORT, 1 << LED_ERROR);
  }
#endif


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

