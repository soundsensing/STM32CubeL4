/**
  ******************************************************************************
  * @file    PWR/PWR_STOP2/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L4xx PWR HAL API to enter
  *          and exit the STOP 2 mode.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_STOP2
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY         100
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SYSCLKConfig_STOP(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

#define QSPI_STANDBY_PORT GPIOB
#define QSPI_STANDBY_PIN GPIO_PIN_11
#define QSPI_STANDBY_PORT_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define CODEC_RESET_PORT GPIOC
#define CODEC_RESET_PIN GPIO_PIN_6
#define CODEC_RESET_PORT_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()

#define SDCARD_ENABLE_PORT GPIOD
#define SDCARD_ENABLE_PIN GPIO_PIN_0
#define SDCARD_ENABLE_PORT_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#include <stdbool.h>

#if 0
typedef enum _bq25070_current {
    BQ25070_CHARGE_OFF = 0,
    BQ25070_CHARGE_DEFAULT = -1,
    BQ25070_CHARGE_MIN = 4,
    BQ25070_CHARGE_500mA = 8,
    BQ25070_CHARGE_1A = 11,
    BQ25070_CHARGE_MAX = 11,
} bq25070_current;

bool bq25070_configure(bq25070_current current);

#define BG25070_CTRL_PORT GPIOG
#define BG25070_CTRL_PIN GPIO_PIN_6
bool
bq25070_configure(bq25070_current current)
{

    DWT_Delay_Init();

    int pulses = (int)current;
    if (current == BQ25070_CHARGE_DEFAULT) {
        pulses = 0;
    }
    const bool enable = (current != BQ25070_CHARGE_OFF);


    // Setup GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = BG25070_CTRL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BG25070_CTRL_PORT, &GPIO_InitStruct);


    // Turn off
    HAL_GPIO_WritePin(BG25070_CTRL_PORT, BG25070_CTRL_PIN, GPIO_PIN_SET);
    HAL_Delay(10);

    if (!enable) {
        return true;
    }

    // enable
    HAL_GPIO_WritePin(BG25070_CTRL_PORT, BG25070_CTRL_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);

    // pulse to configure charge current
    // Ech pulse should be 50-1000 useconds
    for (int i=0; i<pulses; i++) {
        HAL_GPIO_WritePin(BG25070_CTRL_PORT, BG25070_CTRL_PIN, GPIO_PIN_SET);
        DWT_Delay_us(100);
        HAL_GPIO_WritePin(BG25070_CTRL_PORT, BG25070_CTRL_PIN, GPIO_PIN_RESET);
        DWT_Delay_us(100);
    }

    return true;
}
#endif

#define BG96_BOOT_TIME (7000U)

#define MDM_RST_Pin GPIO_PIN_2
#define MDM_RST_GPIO_Port GPIOB
#define MDM_PWR_EN_Pin GPIO_PIN_3
#define MDM_PWR_EN_GPIO_Port GPIOD

#define MODEM_RST_GPIO_PORT     ((GPIO_TypeDef *)MDM_RST_GPIO_Port)    /* for DiscoL496: GPIOB      */
#define MODEM_RST_PIN           MDM_RST_Pin                            /* for DiscoL496: GPIO_PIN_2 */
#define MODEM_PWR_EN_GPIO_PORT  ((GPIO_TypeDef *)MDM_PWR_EN_GPIO_Port) /* for DiscoL496: GPIOD      */
#define MODEM_PWR_EN_PIN        MDM_PWR_EN_Pin                         /* for DiscoL496: GPIO_PIN_3 */

#define LED_BLUE_PIN GPIO_PIN_8
#define LED_GREEN_PIN GPIO_PIN_10
#define LED_RED_PIN GPIO_PIN_5
#define LED_PORT GPIOA
#define LED_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()

bool
statusled_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    HAL_GPIO_WritePin(LED_PORT, LED_RED_PIN, (r > 127) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN, (g > 127) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, LED_BLUE_PIN, (b > 127) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    return true;
}

void
modem_gpio_init()
{

  // PWRKEY, RST
  uint8_t normal_state = GPIO_PIN_SET;

  GPIO_InitTypeDef GPIO_InitStruct;

  HAL_GPIO_WritePin(MODEM_PWR_EN_GPIO_PORT, MODEM_PWR_EN_PIN, normal_state);
  HAL_GPIO_WritePin(MODEM_RST_GPIO_PORT, MODEM_RST_PIN, normal_state);

  // PWRKEY and RESET config
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  /* Init GPIOs - RESET pin */
  GPIO_InitStruct.Pin = MODEM_RST_PIN;
  HAL_GPIO_Init(MODEM_RST_GPIO_PORT, &GPIO_InitStruct);

  /* Init GPIOs - PWR_EN pin */
  GPIO_InitStruct.Pin = MODEM_PWR_EN_PIN;
  HAL_GPIO_Init(MODEM_PWR_EN_GPIO_PORT, &GPIO_InitStruct);

}

void
modem_power_on()
{

  HAL_GPIO_WritePin(MODEM_PWR_EN_GPIO_PORT, MODEM_PWR_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(600U);
  HAL_GPIO_WritePin(MODEM_PWR_EN_GPIO_PORT, MODEM_PWR_EN_PIN, GPIO_PIN_SET);
  HAL_Delay(BG96_BOOT_TIME);
}

void 
modem_power_off()
{

  HAL_GPIO_WritePin(MODEM_PWR_EN_GPIO_PORT, MODEM_PWR_EN_PIN, GPIO_PIN_RESET);
  // 650-1500 ms according to spec. 3.6.2.1. Turn off Module through PWRKEY
  HAL_Delay(650U);
  //HAL_Delay(1500U);
  HAL_GPIO_WritePin(MODEM_PWR_EN_GPIO_PORT, MODEM_PWR_EN_PIN, GPIO_PIN_SET);
  // wait for turn off
  HAL_Delay(5000U);
}


void
modem_power_reset()
{
  HAL_GPIO_WritePin(MODEM_PWR_EN_GPIO_PORT, MODEM_PWR_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(3000U);
  HAL_GPIO_WritePin(MODEM_PWR_EN_GPIO_PORT, MODEM_PWR_EN_PIN, GPIO_PIN_SET);
}

bool led_state = false;




int main(void)
{

  GPIO_InitTypeDef  GPIO_InitStruct = {0,};

  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */

  HAL_Init();


  /* Configure the system clock to 80 MHz */
  SystemClock_Config();

  //HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_NOCLOCK, RCC_MCODIV_1);

#if 1

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Ensure that MSI is wake-up system clock */ 
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);


  // Disable battery charging
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  // Set QSPI flash into Standby
#if 1
  QSPI_STANDBY_PORT_CLK_ENABLE();
  GPIO_InitStruct.Pin = QSPI_STANDBY_PIN;
  HAL_GPIO_Init(QSPI_STANDBY_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(QSPI_STANDBY_PORT, QSPI_STANDBY_PIN, GPIO_PIN_SET);
#endif

#if 1
  // Disable SDCard
  SDCARD_ENABLE_PORT_CLK_ENABLE();
  GPIO_InitStruct.Pin = SDCARD_ENABLE_PIN;
  HAL_GPIO_Init(SDCARD_ENABLE_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(SDCARD_ENABLE_PORT, SDCARD_ENABLE_PIN, GPIO_PIN_RESET);
#endif

#if 1
  // Turn audio codec off
  CODEC_RESET_PORT_CLK_ENABLE();
  GPIO_InitStruct.Pin = CODEC_RESET_PIN;
  HAL_GPIO_Init(CODEC_RESET_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(CODEC_RESET_PORT, CODEC_RESET_PIN, GPIO_PIN_RESET);
#endif

#endif

  modem_gpio_init();


    LED_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = LED_RED_PIN|LED_GREEN_PIN|LED_BLUE_PIN;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);


  statusled_set_rgb(0, 0, 0);
	HAL_Delay(1000);
  statusled_set_rgb(255, 0, 0);
	HAL_Delay(1000);
  statusled_set_rgb(0, 0, 0);


#if 1

  statusled_set_rgb(255, 255, 255);
    //modem_power_on();
    modem_power_reset();

	HAL_Delay(10000);
    modem_power_off();
  statusled_set_rgb(0, 0, 0);

#endif


	HAL_Delay(10000);

  while (1)
  {

#if 0
      /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();        
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
#endif

#if 0
  /* Set all GPIO in analog state to reduce power consumption,                */
  /*   except GPIOC to keep user button interrupt enabled                     */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.           */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  //HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  //HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);    
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  //__HAL_RCC_GPIOA_CLK_DISABLE();
  //__HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();  
  __HAL_RCC_GPIOG_CLK_DISABLE();  
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();

#endif


   // disable SysTick while sleeping
   HAL_SuspendTick();

    /* Enter STOP 2 mode */
   HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
    
   /* ... STOP 2 mode ... */    

    /* Re-configure the system clock to 80 MHz based on MSI, enable and
       select PLL as system clock source (PLL is disabled in STOP mode) */
    SYSCLKConfig_STOP();

    HAL_ResumeTick();
  }

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


/**
  * @brief  Configures system clock after wake-up from STOP: enable MSI, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
static void SYSCLKConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t pFLatency = 0;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Get the Oscillators configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType     = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource  = RCC_SYSCLKSOURCE_PLLCLK;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Suspend tick */
  HAL_SuspendTick();
  
  /* Turn LED2 */
  BSP_LED_On(LED3);
  while (1)
  {
  }
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();

  if (TimingDelay != 0)
  {
    TimingDelay--;
  }
  else
  {
    /* Toggle LED1 */
	BSP_LED_Init(LED1);
    BSP_LED_Toggle(LED1);
    TimingDelay = LED_TOGGLE_DELAY;
  }
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USER_BUTTON_PIN)
  {
#if 1
	BSP_LED_Init(LED2);
	BSP_LED_Toggle(LED2);
#endif
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
