STM32F4_uCOSII
==============
说明：将V2.92移植到STM32F107ZET6和STM32F107VCT6.
    关于uCOS的移植，其实要说难也难，要说简单也简单。简单的是现在官方都已经移植好了，难的是自己写那个汇编的移植代码。废话不多说，直接上移植步骤和注意事项。
    第一步：到官网下载最新版本的uCOS-II V2.92，在uCOS-II的文件夹下有三个这样的文件主要有这两个移植的文件,在DOC文件夹下主要是移植的说明和性能的测试结果，
其中uCOS-II-RefMan和uCOS-II-CfgMan比较有参考价值，第一个文件对每个函数进行了说明，重要的是给出了大部分函数的应用实例。第二个就是我们移植好后的配置文件了。还有一个重要的文件就是很重要的文件就是AN-1018，这个是官方的移植说明（针对的Cortex-M3处理器）。
 
   第二步：Source文件夹下面都是源代码，不用管他们，Ports文件夹下面就是我们移植的时候需要弄的文件了，有以下几个文件，
 
os_cpu.h,os_cpu_c.c,os_cpu_a.asm这三个文件在移植过程中最重要。
os_cpu.h：定义数据类型，处理器相关代码和声明函数原型。
#ifndef  OS_CPU_H
#define  OS_CPU_H
#ifdef   OS_CPU_GLOBALS
#define  OS_CPU_EXT
#else
#define  OS_CPU_EXT  extern
#endif
#ifndef  OS_CPU_EXCEPT_STK_SIZE
#define  OS_CPU_EXCEPT_STK_SIZE    128u   //默认的异常堆栈大小
#endif
typedef unsigned char  BOOLEAN;           //数据类型的说明
typedef unsigned char  INT8U;                   
typedef signed   char  INT8S;                  
typedef unsigned short INT16U;                 
typedef signed   short INT16S;                  
typedef unsigned int   INT32U;                  
typedef signed   int   INT32S;                  
typedef float          FP32;                    
typedef double         FP64;                    
typedef unsigned int   OS_STK;                  
typedef unsigned int   OS_CPU_SR;
临界代码
     和其它内核一样，μC/OS-Ⅱ为了处理临界段代码需要关中断，处理完毕后再开中断。这使得μC/OS-Ⅱ能够避免同时有其它任务或中断服务进入临界段代码。关中断的时间是实时内核开发商应提供的最重要的指标之一，因为这个指标影响用户系统对实时事件的响应性。μC/OS-Ⅱ努力使关中断时间降至最短，但就使用μC/OS-Ⅱ而言，关中断的时间很大程度上取决于微处理器的架构以及编译器所生成的代码质量。
    μC/OS-Ⅱ中的这两个宏调用分别是：OS_ENTER_CRITICAL()和 OS_EXIT_CRI TICAL()。因为这两个宏的定义取决于所用的微处理器，故在文件 OS_CPU.H 中可以找到相应宏定义。每种微处理器都有自己的os_cpu.h，
#define  OS_CRITICAL_METHOD   3u  //临界代码的处理方法，有3种，一般都选三
#if OS_CRITICAL_METHOD == 3u
#define  OS_ENTER_CRITICAL() {cpu_sr = OS_CPU_SR_Save();} //进入临界代码
#define  OS_EXIT_CRITICAL()  {OS_CPU_SR_Restore(cpu_sr);} //推出临界代码
#endif
开关中断的具体实现，在os_cpu_a.asm文件中，下面会讲到。
#define  OS_STK_GROWTH        1u          //中断的生长方向，从高地址到低地址
#define  OS_TASK_SW()         OSCtxSw()   //用户级的任务切换。
//申明外部函数定义的申一个额外的异常堆栈空间，
OS_CPU_EXT  OS_STK   OS_CPU_ExceptStk[OS_CPU_EXCEPT_STK_SIZE];
OS_CPU_EXT  OS_STK  *OS_CPU_ExceptStkBase;
void       OSCtxSw(void);                   //用户任务的任务切换          
void       OSIntCtxSw(void);                //中断任务的任务切换
void       OSStartHighRdy(void); //在操作系统第一次启动的时候调用的任务切换
void       OS_CPU_PendSVHandler(void); //用户任务和中断任务的在可悬起系统调用中断里面实现任务的切换。                                             
void       OS_CPU_SysTickHandler(void);//这个函数是时钟节拍中断函数。
void       OS_CPU_SysTickInit(INT32U  cnts);//这个是滴答定时器的初始化，
第三步：OS_CPU_C.C
这个文件里面有9个钩子函数和一个堆栈初始化函数。
钩子函数：钩子函数说白就是为了扩展函数的功能。别的没什么，用得到就往里面加入自己设计的代码，不需要就不用设置。
堆栈的初始化函数需要根据不同的处理器进行设置。关于堆栈函数的处理首先要明白几点，
xPSR, PC, LR, R12, R0-R3 这几个寄存器在中断的时候实现自动的保存
剩下的几个寄存器需要手动保存。
uCOS-II初始化的时，是如何在任务切换的时候，实现将变量存入到自己的任务堆栈里面
关键就在每个任务初始化堆栈的时候，将堆栈指针R13指向相应的任务地址，这样初始化以后进行任务切换的时候，就能将变量存入好自己的任务堆栈。
*(stk)    = (INT32U)0x01000000uL;        
*(--stk)  = (INT32U)task;                
*(--stk)  = (INT32U)OS_TaskReturn;       
*(--stk)  = (INT32U)0x12121212uL;        
*(--stk)  = (INT32U)0x03030303uL;        
*(--stk)  = (INT32U)0x02020202uL;        
*(--stk)  = (INT32U)0x01010101uL;        
*(--stk)  = (INT32U)p_arg;               
*(--stk)  = (INT32U)0x11111111uL;        
*(--stk)  = (INT32U)0x10101010uL;        
*(--stk)  = (INT32U)0x09090909uL;        
*(--stk)  = (INT32U)0x08080808uL;        
*(--stk)  = (INT32U)0x07070707uL;        
*(--stk)  = (INT32U)0x06060606uL;        
*(--stk)  = (INT32U)0x05050505uL;        
*(--stk)  = (INT32U)0x04040404uL;        
return (stk);                          //返回任务的栈顶
第四步：是移植中最重要的部分，也就是汇编底层函数的编写。
主要有一下这几个函数
（一）开中断和关中断函数的实现。
OS_CPU_SR_Save           ;用于实现关中断的汇编指令
    MRS     R0, PRIMASK ;读取PRIMASK到R0，R0是返回值
    CPSID   I           ;PRIMASK=1，关中断（NMI和硬fault可以响应）
    BX      LR   
OS_CPU_SR_Restore       ;用于实现开中断的汇编指令
    MSR     PRIMASK, R0 ;读取R0到PRIMASK中，R0为参数
    BX      LR
 
（二）函数 void OSStartHighRdy(void)
OSStartHighRdy()由OSStart()调用，用来启动最高优先级任务，当然任务必须在OSStart()前已被创建
PendSV中断的优先级应该为最低优先级，原因在<<ARM Cortex-M3权威指南>>的7.6节已有说明
.PSP设置为0，是告诉具体的任务切换程序（OS_CPU_PendSVHandler()），这是第一次任务切换。做过切换后PSP就不会为0了，后面会看到。
往中断控制及状态寄存器ICSR(0xE000ED04)第28位写1即可产生PendSV中断。这个<<ARM Cortex-M3权威指南>>8.4.5 其它异常的配置寄存器有说明
OSStartHighRdy                                ;设置PendSV中断的优先级
    LDR     R0, =NVIC_SYSPRI14                ;Set the PendSV exception priority
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]                          ;*(uint8_t *)NVIC_SYSPRI14 = NVIC_PENDSV_PRI
    MOVS    R0, #0           ;初始化进程堆栈指针  Set the PSP to 0 for initial context switch call
    MSR     PSP, R0          ;初始化PSP为0        初始化上下文切换调用
    LDR     R0, =OSRunning   ; OSRunning = TRUE
    MOVS    R1, #1           ;设置OSRunning = TRUE
    STRB    R1, [R0]
    LDR     R0, =NVIC_INT_CTRL    ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET   ;触发PendSV中断
    STR     R1, [R0]              ;*(uint32_t *)NVIC_INT_CTRL = NVIC_PENDSVSET
    CPSIE   I                     ; Enable interrupts at processor level开启中断
OSStartHang
    B       OSStartHang           ; Should never get here 死循环 which（1）；
(三) 用户任务切换函数和中断任务的切换函数。
     官方的移植代码里面，这两个函数的处理函数是一样的，而关于这个问题的讨论，网上也有很多，我查了下官方关于在Cortex-M3各个厂家处理器上的移植，都是一样的，就连现在最新推出的M4系列（飞思卡尔 K60），他们也是这么做，所以我决定也这么干吧。
     这两个函数就做了一件事，就是触发PendSV中断。如果没有比PendSV优先级高的中断触发，那么用户的切换任务和中断的切换任务就会得到及时的执行。
OSIntCtxSw
    LDR     R0, =NVIC_INT_CTRL       ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR
OSCtxSw
    LDR     R0, =NVIC_INT_CTRL       ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]                 ;*(uint32_t *)NVIC_INT_CTRL = NVIC_PENDSVSET
    BX      LR   
（四）这个函数是真正实现任务切换的函数，理解这个函数很重要。
  首先我们要明白一点，那就是这个函数说白了就是一个中断函数，而它不同于一般的中断函数，在KEIL里面我们写好C的程序之后，已经将中断的入栈和出栈的工作做好了，在这里我们要自己写入栈和出栈的汇编指令，而且这里的关键就在，中断函数出栈的时候恢复的堆栈指针是指向别的任务的，理解了这个，下面的函数就很好的理解了。
附上一幅任务出栈和入栈的图
//                 |     ....        |
;//                 |-----------------|
;//                 |     ....        |
;//                 |-----------------|
;//                 |     ....        |
;//                 |-----------------|       |---- 任务切换时PSP
;//     Low Memory  |     ....        |       |
;//                 |-----------------|       |    |---------------|      |----------------|
;//        ^        |       R4        |  <----|----|--OSTCBStkPtr  |<-----|   (OS_TCB *)   |
;//        ^        |-----------------|            |---------------|      |----------------|
;//        ^        |       R5        |            |               |         OSTCBHighRdy
;//        |        |-----------------|            |---------------|
;//        |        |       R6        |            |               |
;//        |        |-----------------|            |---------------|
;//        |        |       R7        |            |               |
;//        |        |-----------------|            |---------------|
;//        |        |       R8        |                 Task's
;//        |        |-----------------|                 OS_TCB
;//        |        |       R9        |
;//        |        |-----------------|
;//        |        |      R10        |
;//      Stack      |-----------------|
;//      Growth     |      R11        |
;//       = 1       |-----------------|
;//        |        |    R0 = p_arg   |  <-------- 异常时的PSP (向上生长的满栈)
;//        |        |-----------------|
;//        |        |       R1        |
;//        |        |-----------------|
;//        |        |       R2        |
;//        |        |-----------------|
;//        |        |       R3        |
;//        |        |-----------------|
;//        |        |       R12       |
;//        |        |-----------------|
;//        |        |       LR        |
;//        |        |-----------------|
;//        |        |    PC = task    |
;//        |        |-----------------|
;//        |        |      xPSR       |
;//    High Memory  |-----------------|
这个里面有个PSP进程堆栈指针，关于这个指针和主堆栈指针的区别需要好好的看看权威指南
OS_CPU_PendSVHandler   ;xPSR, PC, LR, R12, R0-R3已自动保存
    CPSID   I         ;任务切换期间需要关中断 Prevent interruption during context switch
    MRS     R0, PSP   ;R0 = PSP PSP is process stack pointer 线程堆栈指针
    CBZ     R0, OS_CPU_PendSVHandler_nosave    ;如果PSP==0跳转到OS_CPU_PendSVHandler_nosave去执行 在多任务的初始化时PSP被初始化为0  Skip register save the first time
                              ;若果PSP如果是0,标示任务没有运行过,那么不需要压栈
    SUBS    R0, R0, #0x20    ;R0 -= 0x20 保存R4-R11到任务堆栈 共32个字节            
    STM     R0, {R4-R11}     ;压栈R4-R11, 其他8个寄存器是在异常时自动压栈的
    LDR     R1, =OSTCBCur    ;获取OSTCBCur->OSTCBStkPtr                              
    LDR     R1, [R1]         ;R1 = *R1 (R1 = OSTCBCur)
    STR     R0, [R1]         ;*R1 = R0 (*OSTCBCur = SP) R0 is SP of process being switched out
                             ;将当前任务的堆栈保存到自己的任务控制块
                             ;OSTCBCur->OSTCBStkPtr = PSP
                             ;程序运行此位置,已经保存了当前任务的context了
                             ; At this point, entire context of process has been saved
OS_CPU_PendSVHandler_nosave
    PUSH    {R14}            ; Save LR exc_return value 保存R14，因为后面要调用函数 
    LDR     R0, =OSTaskSwHook  ; OSTaskSwHook();           R0 = &OSTaskSwHook
    BLX     R0               ; 调用OSTaskSwHook()
    POP     {R14}            ; 恢复R14
    LDR     R0, =OSPrioCur   ; OSPrioCur = OSPrioHighRdy; R0 = &OSPrioCur
    LDR     R1, =OSPrioHighRdy  ; R1 = &OSPrioHighRdy
    LDRB    R2, [R1]          ; R2 = *R1 (R2 = OSPrioHighRdy)
    STRB    R2, [R0]          ; *R0 = R2 (OSPrioCur = OSPrioHighRdy)
    LDR     R0, =OSTCBCur     ; OSTCBCur  = OSTCBHighRdy;;R0 = &OSTCBCur
    LDR     R1, =OSTCBHighRdy  ; R1 = &OSTCBHighRdy
    LDR     R2, [R1]           ; R2 = *R1 (R2 = OSTCBHighRdy)
    STR     R2, [R0]           ; *R0 = R2 (OSTCBCur = OSTCBHighRdy) 此时 [R2] = 新任务的PSP
    LDR     R0, [R2]          ;R0 = *R2 (R0 = OSTCBHighRdy), 此时R0是新任务的SP R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;
    LDM     R0, {R4-R11}      ; 从任务堆栈SP恢复R4-R11 Restore r4-11 from new process stack
    ADDS    R0, R0, #0x20     ; 调整PSP  R0 += 0x20
    MSR     PSP, R0           ; Load PSP with new process SP  PSP = R0，    用新任务的SP加载PSP
    ORR     LR, LR, #0x04     ; Ensure exception return uses process stack  确保LR位2为1，返回后使用进程堆栈PSP
    CPSIE   I                 ;开中断
    BX      LR                ; Exception return will restore remaining context  中断返回
    END
