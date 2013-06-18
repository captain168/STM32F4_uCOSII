#include "ucos_ii.h"
#include "bsp.h"
#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3
#define partstMAX_LEDS		4

static const Led_TypeDef xLEDs[ partstMAX_LEDS ] = { LED5, LED6, LED3, LED4 };

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];
static OS_STK task1_stk[TASK1_STK_SIZE];
static OS_STK task2_stk[TASK2_STK_SIZE];
static OS_STK task3_stk[TASK3_STK_SIZE];
static OS_STK task4_stk[TASK4_STK_SIZE];
static OS_STK task5_stk[TASK5_STK_SIZE];
static OS_STK task6_stk[TASK6_STK_SIZE];
static OS_STK task7_stk[TASK7_STK_SIZE];
static OS_STK task8_stk[TASK8_STK_SIZE];
void led_on(uint32_t n);
void led_off(uint32_t n);
static void startup_task(void *p_arg);
static void task1(void *p_arg);
static void task2(void *p_arg);
static void task3(void *p_arg);
static void task4(void *p_arg);
static void task5(void *p_arg);
static void task6(void *p_arg);
static void task7(void *p_arg);
static void task8(void *p_arg);
double a;
double b;
int main(void)
{
	BSP_IntDisAll();                                            /* Disable all interrupts until we are ready to accept them */
    RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC);

	OSInit();
    OSTaskCreate(startup_task,
                 (void *)0,
                 &startup_task_stk[STARTUP_TASK_STK_SIZE-1],
                 STARTUP_TASK_PRIO);
    OSStart();
    return 0;
}
void led_init(void)
{
	/* Initialise all four LEDs that are built onto the starter kit. */
	STM_EVAL_LEDInit( LED5 );
	STM_EVAL_LEDInit( LED6 );
	STM_EVAL_LEDInit( LED3 );
	STM_EVAL_LEDInit( LED4 );
}
void led_on(uint32_t n)
{
  STM_EVAL_LEDOn( xLEDs[ n ] );
}
void led_off(uint32_t n)
{
    STM_EVAL_LEDOff( xLEDs[ n ] );
}
static void startup_task(void *p_arg)
{
  	BSP_Init();
#if(OS_TASK_STAT_EN>0)
    OSStatInit(); /*DetermineCPUcapacity.*/
#endif
    /*TODO:createapplicationtaskshere*/
    OSTaskCreate(task1, (void *)0,
                 &task1_stk[TASK1_STK_SIZE-1], TASK1_PRIO);
    OSTaskCreate(task2, (void *)0,
                 &task2_stk[TASK2_STK_SIZE-1], TASK2_PRIO);
	OSTaskCreate(task3, (void *)0,
                 &task3_stk[TASK3_STK_SIZE-1], TASK3_PRIO);
    OSTaskCreate(task4, (void *)0,
                 &task4_stk[TASK4_STK_SIZE-1], TASK4_PRIO);

	OSTaskCreate(task5, (void *)0,
                 &task5_stk[TASK5_STK_SIZE-1], TASK5_PRIO);
    OSTaskCreate(task6, (void *)0,
                 &task6_stk[TASK6_STK_SIZE-1], TASK6_PRIO);
	OSTaskCreate(task7, (void *)0,
                 &task7_stk[TASK7_STK_SIZE-1], TASK7_PRIO);
    OSTaskCreate(task8, (void *)0,
                 &task8_stk[TASK8_STK_SIZE-1], TASK8_PRIO);
    OSTaskDel(OS_PRIO_SELF);
}
static void task1(void *p_arg)
{
    while(1)
    {
		a=cos(1);
		b=123.455*1233.3333;
        led_on(LED_0);
        OSTimeDly(200);
        led_off(LED_0);
        OSTimeDly(200);
    }
}
static void task2(void *p_arg)
{
    while(1)
    {
        led_on(LED_1);
        OSTimeDly(500);
        led_off(LED_1);
        OSTimeDly(500);
    }
}
static void task3(void *p_arg)
{
    while(1)
    {
        led_on(LED_2);
        OSTimeDly(100);
        led_off(LED_2);
        OSTimeDly(100);
    }
}
static void task4(void *p_arg)
{
    while(1)
    {
        led_on(LED_3);
        OSTimeDly(800);
        led_off(LED_3);
        OSTimeDly(800);
    }
}

static void task5(void *p_arg)
{
    while(1)
    {
        int a=1+2;
        OSTimeDly(800);
        a=3-1;
        OSTimeDly(800);
    }
}
static void task6(void *p_arg)
{
    while(1)
    {
        int a=4-2;
        OSTimeDly(800);
        a=8+1;
        OSTimeDly(800);
    }
}
int testarg(double a,double b,double c,double d,double e,double f,double h,double i)
{
  	return 0;
}
static void task7(void *p_arg)
{
    while(1)
    {
        double a=8*9.2;
		testarg(a,a+1,a,a, a,a,a+3,a);
        OSTimeDly(800);
        a=88*99;
        OSTimeDly(800);
    }
}
static void task8(void *p_arg)
{
    while(1)
    {
        double a=123/345;
        OSTimeDly(800);
        a=3444/2323;
        OSTimeDly(800);
    }
}