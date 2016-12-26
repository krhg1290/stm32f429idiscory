/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "timers.h "
#include "test_program.h"
#include <stddef.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Background_Task_PRIO    ( tskIDLE_PRIORITY  + 10 )
#define Background_Task_STACK   ( 512 )

#define LED_Task_PRIO    ( tskIDLE_PRIORITY  + 10 )
#define LED_Task_STACK   ( 512 )

#define Demo_Task_PRIO          ( tskIDLE_PRIORITY  + 9 )
#define Demo_Task_STACK         ( 3048 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
xTaskHandle                   Task_Handle;
xTaskHandle                   Demo_Handle;
xTaskHandle                   LED_Handle;
xTimerHandle                  TouchScreenTimer;

uint32_t demo_mode = 0;

extern WM_HWIN  ALARM_hWin;

/* Private function prototypes -----------------------------------------------*/
static void Background_Task(void * pvParameters);
static void Demo_Task(void * pvParameters);
static void vTimerCallback( xTimerHandle pxTimer );
static void LED_Task(void * pvParameters);
extern void DEMO_Starup(void);
extern void DEMO_MainMenu(void);
extern void ALARM_BackgroundProcess (void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */ 
int main(void)
{ 
  
  /* Setup SysTick Timer for 1 msec interrupts.*/
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }

  
  
  /* Create background task */
  xTaskCreate(Background_Task,
              (signed char const*)"BK_GND",
              Background_Task_STACK,
              NULL,
              Background_Task_PRIO,
              &Task_Handle);


  /* Start the FreeRTOS scheduler */
  vTaskStartScheduler();
}


/**
  * @brief  Background task
  * @param  pvParameters not used
  * @retval None
  */
static void Background_Task(void * pvParameters)
{
    
  /* Initialize the BSP layer */
  LowLevel_Init();

  xTaskCreate(LED_Task,
              (signed char const*)"LED_DEMO",
              LED_Task_STACK,
              NULL,
              LED_Task_PRIO,
              &LED_Handle);

  
  /* Run the background task */
  
  while (1)
  {

    /* Handle USB Host background process */
    USBH_USR_BackgroundProcess();
    
    /* Handle ALARM background process */
    ALARM_BackgroundProcess();
    /* This task is handled periodically, each 10 mS */
    vTaskDelay(10);
  }
}

static void LED_Task(void * pvParameters){
  
  uint32_t ticks = 0;
  
  
  while (1)
  {
    if(ticks++ > 10)
    {
      ticks = 0;
      STM_EVAL_LEDToggle(LED3);
    }
    
    vTaskDelay(10);
    
  }
  
  
  
}


/**
  * @brief  Demonstration task
  * @param  pvParameters not used
  * @retval None
  */
static void Demo_Task(void * pvParameters)
{  
  /* Change Skin */
  PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
  RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
  SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
  SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
  SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
  BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
  
  /* Setup layer configuration during startup */
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_SelectLayer(1);
  GUI_Clear();
  GUI_SetBkColor(GUI_TRANSPARENT); 
  GUI_SelectLayer(0);

  /* Run the Startup frame */
  DEMO_Starup();
  
  /* Show the main menu */
  DEMO_MainMenu();
}


/**
  * @brief  Timer callback
  * @param  pxTimer 
  * @retval None
  */
static void vTimerCallback( xTimerHandle pxTimer )
{
   BSP_Pointer_Update();
}

/**
  * @brief  Error callback function
  * @param  None
  * @retval None
  */
void vApplicationMallocFailedHook( void )
{
  while (1)
  {}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
