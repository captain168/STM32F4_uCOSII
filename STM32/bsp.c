#define  BSP_MODULE
#include <bsp.h>

void LED_Init(void)
{
	/* Initialise all four LEDs that are built onto the starter kit. */
	STM_EVAL_LEDInit( LED5 );
	STM_EVAL_LEDInit( LED6 );
	STM_EVAL_LEDInit( LED3 );
	STM_EVAL_LEDInit( LED4 );
}



void BSP_Init(void)
{
	BSP_IntInit();
    LED_Init();                                             /* Initialize the I/Os for the LED      controls.       */

#ifdef TRACE_EN                                                 /* See project / compiler preprocessor options.         */
    DBGMCU_CR |=  DBGMCU_CR_TRACE_IOEN_MASK;                    /* Enable tracing (see Note #2).                        */
    DBGMCU_CR &= ~DBGMCU_CR_TRACE_MODE_MASK;                    /* Clr trace mode sel bits.                             */
    DBGMCU_CR |=  DBGMCU_CR_TRACE_MODE_SYNC_04;                 /* Cfg trace mode to synch 4-bit.                       */
#endif
}
/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}


/*
*********************************************************************************************************
*                                            BSP_StatusRd()
*
* Description : Get the current status of a status input
*
* Argument(s) : id    is the status you want to get.
*
* Return(s)   : DEF_ON    if the status is asserted
*               DEF_OFF   if the status is negated
*
* Caller(s)   : application
*
* Note(s)     : none.
*********************************************************************************************************
*/


CPU_BOOLEAN  BSP_StatusRd (CPU_INT08U  id)
{
    CPU_BOOLEAN  bit_val;


    switch (id) {
        case 1:
             bit_val = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
             return (bit_val);

        default:
             return ((CPU_BOOLEAN)DEF_OFF);
    }
}



/*
*********************************************************************************************************
*********************************************************************************************************
*                                           OS PROBE FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           OSProbe_TmrInit()
*
* Description : Select & initialize a timer for use with the uC/Probe Plug-In for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : OSProbe_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN          == 1))
void  OSProbe_TmrInit (void)
{
}
#endif


/*
*********************************************************************************************************
*                                            OSProbe_TmrRd()
*
* Description : Read the current counts of a free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 32-bit timer counts.
*
* Caller(s)   : OSProbe_TimeGetCycles().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN          == 1))
CPU_INT32U  OSProbe_TmrRd (void)
{
    return ((CPU_INT32U)DWT_CYCCNT);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time
*                       but MUST be less than the maximum measured time; otherwise, timer resolution
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  cpu_clk_freq_hz;
      
      
    DEM_CR         |= (CPU_INT32U)DEM_CR_TRCENA;                /* Enable Cortex-M3's DWT CYCCNT reg.                   */
    DWT_CYCCNT      = (CPU_INT32U)0u;
    DWT_CR         |= (CPU_INT32U)DWT_CR_CYCCNTENA;

    cpu_clk_freq_hz = BSP_CPU_ClkFreq();    
    CPU_TS_TmrFreqSet(cpu_clk_freq_hz);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer period            Timer's period in some units of
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same
*                                                                   units of (fractional) seconds
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less
*                           than the maximum measured time; otherwise, timer resolution inadequate to
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return ((CPU_TS_TMR)DWT_CYCCNT);
}
#endif