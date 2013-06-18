/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : EHS
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            BSP CONFIGURATION
*********************************************************************************************************
*/

#define  BSP_CFG_LED_SPI2_EN                  DEF_ENABLED       /* Enable/disable LEDs on SPI port.                     */
#define  BSP_CFG_LED_PIOC_EN                  DEF_ENABLED       /* Enable/disable PIOC LEDs.                            */


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO                              4
#define  APP_CFG_TASK_1_PRIO                              6
#define  APP_CFG_TASK_2_PRIO                              7
#define  OS_TASK_TMR_PRIO                       (OS_LOWEST_PRIO - 2)

/*taskpriority*/
#define	STARTUP_TASK_PRIO 4
#define	TASK1_PRIO 5
#define	TASK2_PRIO 6
#define	TASK3_PRIO 7
#define	TASK4_PRIO 8
#define	TASK5_PRIO 9
#define	TASK6_PRIO 10
#define	TASK7_PRIO 11
#define	TASK8_PRIO 12


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                         128

#define  APP_CFG_TASK_LED_STK_SIZE                       128

#define  BUFF_SIZE                                      1000

/*taskstacksize*/
#define	STARTUP_TASK_STK_SIZE 80
#define	TASK1_STK_SIZE 256
#define	TASK2_STK_SIZE 256
#define	TASK3_STK_SIZE 256
#define	TASK4_STK_SIZE 256
#define	TASK5_STK_SIZE 256
#define	TASK6_STK_SIZE 256
#define	TASK7_STK_SIZE 256
#define	TASK8_STK_SIZE 256
/*
*********************************************************************************************************
*                                        uC/LIB CONFIGURATION
*********************************************************************************************************
*/

#define  LIB_MEM_CFG_OPTIMIZE_ASM_EN      DEF_ENABLED
#define  LIB_MEM_CFG_ARG_CHK_EXT_EN       DEF_ENABLED
#define  LIB_MEM_CFG_ALLOC_EN             DEF_DISABLED
#define  LIB_MEM_CFG_POOL_NBR                  10
#define  LIB_MEM_CFG_HEAP_SIZE              28000L


/*
*********************************************************************************************************
*                                    BSP CONFIGURATION: RS-232
*********************************************************************************************************
*/

#define  BSP_SER_COMM_EN                  DEF_DISABLED
#define  BSP_CFG_SER_COMM_SEL             BSP_SER_COMM_UART_02
#define  BSP_CFG_TS_TMR_SEL                    2

/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#define  TRACE_LEVEL_OFF                       0
#define  TRACE_LEVEL_INFO                      1
#define  TRACE_LEVEL_DEBUG                     2

#define  APP_TRACE_LEVEL                TRACE_LEVEL_INFO
#define  APP_TRACE                      BSP_Ser_Printf

#define  APP_TRACE_INFO(x)            ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DEBUG(x)           ((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE x) : (void)0)



#endif
