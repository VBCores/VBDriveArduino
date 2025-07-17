#include "vbdrive_arduino.h"

using namespace VBDrive;

void CanFD::init() {
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_FDCAN_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void CanFD::default_start() {
    FDCAN_FilterTypeDef sFilterConfig;  
    sFilterConfig.IdType = FDCAN_EXTENDED_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x0;
    sFilterConfig.FilterID2 = 0x1FFFFFFF;
    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK){ Error_Handler(); }  
    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK){ Error_Handler(); }
    if (HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan1, 5, 0) != HAL_OK){ Error_Handler(); }
    if (HAL_FDCAN_EnableTxDelayCompensation(&hfdcan1) != HAL_OK){ Error_Handler(); }
    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK){ Error_Handler(); }
}

void CanFD::write_default_params() {
    hfdcan1.Instance = FDCAN1; 
    hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV2; 
    hfdcan1.Init.FrameFormat = FDCAN_FRAME_FD_BRS; 
    hfdcan1.Init.Mode = FDCAN_MODE_NORMAL; 
    hfdcan1.Init.AutoRetransmission = ENABLE; 
    hfdcan1.Init.TransmitPause = ENABLE; 
    hfdcan1.Init.ProtocolException = DISABLE; 
    hfdcan1.Init.NominalPrescaler = 1; 
    hfdcan1.Init.NominalSyncJumpWidth = 24; 
    hfdcan1.Init.NominalTimeSeg1 = 55; 
    hfdcan1.Init.NominalTimeSeg2 = 24; 
    hfdcan1.Init.DataPrescaler = 1; 
    hfdcan1.Init.DataSyncJumpWidth = 4; 
    hfdcan1.Init.DataTimeSeg1 = 5; 
    hfdcan1.Init.DataTimeSeg2 = 4; 
    hfdcan1.Init.StdFiltersNbr = 0; 
    hfdcan1.Init.ExtFiltersNbr = 1; 
    hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
}

void CanFD::write_default_params_classic() {
    hfdcan1.Instance = FDCAN1;
    hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
    hfdcan1.Init.AutoRetransmission = ENABLE;
    hfdcan1.Init.TransmitPause = ENABLE;
    hfdcan1.Init.ProtocolException = DISABLE;
    hfdcan1.Init.NominalPrescaler = 4;
    hfdcan1.Init.NominalSyncJumpWidth = 75;
    hfdcan1.Init.NominalTimeSeg1 = 29;
    hfdcan1.Init.NominalTimeSeg2 = 10;
    hfdcan1.Init.DataPrescaler = 4;
    hfdcan1.Init.DataSyncJumpWidth = 75;
    hfdcan1.Init.DataTimeSeg1 = 29;
    hfdcan1.Init.DataTimeSeg2 = 10;
    hfdcan1.Init.StdFiltersNbr = 0;
    hfdcan1.Init.ExtFiltersNbr = 1;
    hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
}

void CanFD::apply_config() {
    if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK) { Error_Handler(); }
}

void SystemClock_Config() {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

    /// Configure the main internal regulator output voltage 

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
    /// Initializes the CPU, AHB and APB busses clocks 

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

    /// Initializes the CPU, AHB and APB busses clocks 

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) { Error_Handler(); }

    /// Initializes the peripherals clocks 

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_ADC12 | RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
    PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { Error_Handler(); }
}
