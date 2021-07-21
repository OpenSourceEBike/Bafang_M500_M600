/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "config.h"
#include "display_kunteng.h"
#include "CAN-Registers.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CAN_HandleTypeDef hcan;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
char UART_TX_Buffer[500];
char UART_RX_Buffer[100];
uint8_t i=0; //counter for loops
uint8_t j=0; //counter for autozero at startup
uint8_t k=0; //counter for CAN_TX
uint8_t l=0; //delay-counter
uint8_t ODOByte6=95; //delay-counter
uint8_t UART_RX_Flag=0;
uint8_t UART_TX_Flag=1;
uint8_t CAN_RX_Flag=0;
uint8_t CAN_TX_Flag=0;
uint8_t Timer3_Flag=0;
uint8_t ADC_Flag=0;
uint8_t DIS_Flag=0;
uint8_t UART_Tx_lenght=0;
uint8_t UART_Tx_async_flag=0;
uint8_t UART_Tx_counter=0;
int16_t i16_Gauge_Voltage=0;
int16_t i16_Gauge_Voltage_Offset=0;
int16_t i16_Pedal_Torque=0;
int32_t i32_Pedal_Torque_cumulated=0;
int16_t i16_Current_Target=0;
uint16_t ui16_slow_loop_counter=0;
uint16_t timeCounter=0;
volatile uint16_t adcData[3]; //Buffer for ADC1 Input

int32_t ExtID[500][2];
int16_t ExtID_counter=0;
MotorState_t MS; //struct for motor state

uint8_t               CAN_Buffer[11][8];
uint8_t               Buffer_Counter=0;

CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CAN_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);

void Send_CAN_Request(uint8_t command);
void Send_CAN_Command(uint32_t function, uint16_t value);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
#if (DISPLAY_TYPE == DISPLAY_TYPE_KUNTENG)
  kunteng_init();
#endif
  /* Run the ADC calibration */
  if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  MX_TIM3_Init();

  // Start Timer 3

  if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
       {
         /* Counter Enable Error */
         Error_Handler();
       }
  // Start ADC1 with DMA
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcData, 3);

  /* USER CODE BEGIN 2 */

  UART_RX_Buffer[0]=1;



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



	  if(UART_RX_Flag){
		  UART_RX_Flag=0;
		  HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);

		  if(UART_TX_Flag){
			 UART_TX_Flag=0;

			 UART_Tx_lenght=sprintf(UART_TX_Buffer, "Empfangenes UART Byte %d, %d, %d, %d\r\n",UART_RX_Buffer[0],UART_RX_Buffer[1], UART_RX_Buffer[2], UART_RX_Buffer[3]);

		  	  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&UART_TX_Buffer, UART_Tx_lenght);



		  }






	  } //end uart rx flag



	  //Timer 3 running with 1kHz ISR frequency
	  if(Timer3_Flag){
		  timeCounter++;
		  Timer3_Flag=0;
		  ui16_slow_loop_counter++;



		  if (ui16_slow_loop_counter>100 && UART_RX_Buffer[1]){

			  ui16_slow_loop_counter=0;
			  switch (k){

			  case 0:
				  Send_CAN_Command(CON_WHEEL_CIRCUMFERENCE, 0);
				  Send_CAN_Command(CON_SPEED2, 0);
				  k++;
				  break;

			  case 1:
				  Send_CAN_Command(CON_SPEED2,0);
				  k++;
				  break;

			  case 2:
				  Send_CAN_Command(CON_ALIVE, 0);
				  Send_CAN_Command(CON_SPEED2, 0);
				  k++;
				  break;

			  case 3:
				  Send_CAN_Command(CON_SPEED1,0);
				  Send_CAN_Command(CON_SPEED2, 0);
				  k++;
				  break;

			  case 4:
				  Send_CAN_Command(CON_ODO,0);
				  Send_CAN_Command(CON_SPEED2, 0);
				  k++;
				  break;

			  case 5:
				  Send_CAN_Command(CON_SPEED2, 0);
				  k++;
				  break;

			  case 6:
				  Send_CAN_Command(CON_UNKOWN_1,0);
				  Send_CAN_Command(CON_SPEED2, 0);
				  k++;
				  break;
			  case 7:
				  Send_CAN_Command(CON_ALIVE, 0);
				  Send_CAN_Command(CON_SPEED2, 0);
				  k++;
				  break;

			  case 8:
				  Send_CAN_Command(CON_SPEED1, 0);
				  Send_CAN_Command(CON_SPEED2, 0);
				  k++;
				  break;

			  case 9:
				  Send_CAN_Command(CON_SPEED2,0);
				  k=0;
				  HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
				  break;
			  }


		  }//end slow loop
	  }// end timer 1 kHz loop



	  if(CAN_RX_Flag){


		  //UART_Tx_async_flag=1;

		  if(RxHeader.ExtId == DIS_ASSIST){
			  DIS_Flag=1;

		  }

			  if( UART_TX_Flag && UART_RX_Buffer[0] && !UART_RX_Buffer[2]){//print out received CAN message

				  UART_Tx_lenght=sprintf(UART_TX_Buffer, "%lu, %lu, %lu, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n",
						  	  	  	  RxHeader.StdId,
									  RxHeader.ExtId,
									  RxHeader.IDE,
									  (uint16_t)RxHeader.RTR,
									  (uint16_t)RxHeader.DLC,
						  			  RxData[0],
									  RxData[1],
									  RxData[2],
									  RxData[3],
									  RxData[4],
									  RxData[5],
									  RxData[6],
									  RxData[7]


				  			  			  );


				  			  			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&UART_TX_Buffer, UART_Tx_lenght);
				  			  			UART_TX_Flag=0;

			  }

			  else if(UART_TX_Flag && !UART_RX_Buffer[0] && UART_RX_Buffer[2]){//print out Array of ExtID

				  for(int l=0; l<500; l++){
					  UART_TX_Flag=0;
					  UART_Tx_lenght=sprintf(UART_TX_Buffer, "%d, %d, %lu\r\n", l, (uint16_t)ExtID[l][0], ExtID[l][1]);
					  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&UART_TX_Buffer, UART_Tx_lenght);
					  while(!UART_TX_Flag);
				  }

			  }



			  CAN_RX_Flag=0;

		  }//End CAN Rx


	  }


    /* USER CODE END WHILE */



    /* USER CODE BEGIN 3 */
  } //end while loop
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 9;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_4TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;


  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  /* USER CODE BEGIN CAN_Init 2 */
  /*##-2- Configure the CAN Filter ###########################################*/
  	CAN_FilterTypeDef  sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
    {
      /* Filter configuration Error */
      Error_Handler();
    }

    /*##-3- Start the CAN peripheral ###########################################*/
    if (HAL_CAN_Start(&hcan) != HAL_OK)
    {
      /* Start Error */
      Error_Handler();
    }

    /*##-4- Activate CAN RX notification #######################################*/
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
      /* Notification Error */
      Error_Handler();
    }

    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
    {
      /* Notification Error */
      Error_Handler();
    }

    /*##-5- Configure Transmission process #####################################*/
    TxHeader.StdId = 0x00;
    TxHeader.ExtId = 0x04;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;
  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 512;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 139;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 256000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
#if (DISPLAY_TYPE == DISPLAY_TYPE_DEBUG)
  if (HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART_RX_Buffer, 4) != HAL_OK)
   {
	   Error_Handler();
   }

#endif
  /* USER CODE END USART1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
	  /* DMA controller clock enable */
	  __HAL_RCC_DMA1_CLK_ENABLE();

	  /* DMA interrupt init */
	  /* DMA1_Channel1_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	  /* DMA1_Channel4_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 1, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	  /* DMA1_Channel5_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 1, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

	  HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
	  HAL_NVIC_EnableIRQ(USART1_IRQn);


}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Onboard_LED_Pin */
  GPIO_InitStruct.Pin = Onboard_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Onboard_LED_GPIO_Port, &GPIO_InitStruct);

}

static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/* USER CODE BEGIN 4 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

	UART_TX_Flag=1;
	//HAL_UART_IRQHandler(&huart1);// reset huart1

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UART_RX_Flag=1;


}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
  /* Get RX message */
	//HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
  if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }

  if(UART_RX_Buffer[2]){ //just collect ExtIds in Array
	  if(!CAN_RX_Flag){
	  ExtID[ExtID_counter][1]=RxHeader.ExtId;
	  ExtID[ExtID_counter][0]=timeCounter;
	  timeCounter=0;
	  HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
	  if (ExtID_counter<495){
		  ExtID_counter++;
  	  	  }
	  else {
		  ExtID_counter=0;
		  CAN_RX_Flag=1;
  	  	  }
	  }
  }
  else CAN_RX_Flag=1;


}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *CanHandle)
{


	CAN_TX_Flag=1;

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim3) {
		Timer3_Flag=1;


	}
}

// regular ADC callback
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	ADC_Flag=1;

}

void Send_CAN_Request(uint8_t command){

	TxHeader.StdId=0;
	TxHeader.DLC=2;
	TxData[0] = 0;
	TxData[1] = command;

	//HAL_GPIO_TogglePin(Onboard_LED_GPIO_Port, Onboard_LED_Pin);
	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	   	{
    	  Error_Handler();
    	}


}

void Send_CAN_Command(uint32_t function, uint16_t value){
	TxHeader.StdId=0;

	TxHeader.IDE=4;
	TxHeader.DLC=8;


	switch (function) {

	case CON_SPEED1: //Geschwindigkeit in Byte 3. ist das nur das LSB?!

		TxHeader.ExtId=function;
		TxData[0] = 0;
		TxData[1] = 0;
		TxData[2] = 0;
		TxData[3] = value & 0xFF;
		TxData[4] = value>>8;
		TxData[5] = 0;
		TxData[6] = 0;
		TxData[7] = 0;

		break;

	case CON_SPEED2: //Geschwindigkeit in Byte 3. ist das nur das LSB?!

		TxHeader.ExtId=function;
		TxData[0] = 1;
		TxData[1] = 0;
		TxData[2] = 0;
		TxData[3] = value & 0xFF;
		TxData[4] = value>>8;
		TxData[5] = 0;
		TxData[6] = 0;
		TxData[7] = 0;

		break;

	case CON_UNKOWN_1: //Batteriestand in Byte 6?!
		//230, 0, 230, 1, 146, 8, 160, 0
		//230, 0, 230, 1, 146, 8, 160, 0
		//0, 419300847, 4, 0, 8, 50, 0, 204, 1, 146, 8, 64, 0
		TxHeader.ExtId=function;
		TxData[0] = 50;
		TxData[1] = 0;
		TxData[2] = 204;
		TxData[3] = 1;
		TxData[4] = 146;
		TxData[5] = 8;
		TxData[6] = 64;
		TxData[7] = 0;

		break;

		//128, 0, 64, 0, 1, 0, 108, 0

	case CON_ALIVE:
		TxHeader.ExtId=function;
		TxData[0] = 128;
		TxData[1] = 0;
		TxData[2] = 64;
		TxData[3] = 0;
		TxData[4] = 1;
		TxData[5] = 0;
		TxData[6] = 108;
		TxData[7] = 0;

		break;

		//13, 0, 0, 0, 0, 0, 64, 0

	case 13:

		TxData[1] = 0;
		TxData[2] = 0;
		TxData[3] = 33;
		TxData[4] = 1;
		TxData[5] = 0;
		TxData[6] = 64;
		TxData[7] = 0;

		break;
		//68, 23, 0, 42, 0, 0, 0, 0: Byte 0 ist LowByte von ODO Byte 1 ist HiByte von ODO, Byte 3 ist LowByte von Trip, ODO und Trip in 0,1km

	case CON_ODO:

		TxHeader.ExtId=function;
		TxData[0] = 70;
		TxData[1] = 23;
		TxData[2] = 0;
		TxData[3] = 44;
		TxData[4] = 0;
		TxData[5] = 0;
		TxData[6] = ODOByte6;
		TxData[7] = 0;

		ODOByte6--;
		break;

		//9, 0, 0, 9, 1, 0, 0, 0 Byte3 und 4 Radumfang?!

	case CON_WHEEL_CIRCUMFERENCE:

		TxHeader.ExtId=function;
		TxData[0] = 9;
		TxData[1] = 0;
		TxData[2] = 0;
		TxData[3] = 9;
		TxData[4] = 1;
		TxData[5] = 0;
		TxData[6] = 0;
		TxData[7] = 0;

		break;

		//1, 0, 1, 0, 13, 0, 50, 178
	case 1:

		TxData[1] = 0;
		TxData[2] = 1;
		TxData[3] = 0;
		TxData[4] = 13;
		TxData[5] = 0;
		TxData[6] = 50;
		TxData[7] = 178;

		break;

	}

	// Start the Transmission process, zwei mal senden wie im Beispiel https://github.com/jliegner/bionxdrive
	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	  {
	   // Transmission request Error
	   Error_Handler();
	  }

}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
