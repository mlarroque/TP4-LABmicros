/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"
/*
#include  <os.h>


// Task Start
#define TASKSTART_STK_SIZE 		512u
#define TASKSTART_PRIO 			2u
static OS_TCB TaskStartTCB;
static CPU_STK TaskStartStk[TASKSTART_STK_SIZE];

//App queue
OS_Q AppQ;


// Task 2
#define TASK2_STK_SIZE			256u
#define TASK2_STK_SIZE_LIMIT	(TASK2_STK_SIZE / 10u)
#define TASK2_PRIO              3u
static OS_TCB Task2TCB;
static CPU_STK Task2Stk[TASK2_STK_SIZE];

// Task 3
#define TASK3_STK_SIZE			256u
#define TASK3_STK_SIZE_LIMIT	(TASK2_STK_SIZE / 10u)
#define TASK3_PRIO              3u
static OS_TCB Task3TCB;
static CPU_STK Task3Stk[TASK3_STK_SIZE];



void App_Init ();
void App_Run (void);

// Example semaphore
static OS_SEM semTest;

static void Task2(void *p_arg) {
    (void)p_arg;
    OS_ERR os_err;

    while (1) {
        OSSemPost(&semTest, OS_OPT_POST_1, &os_err);
        OSTimeDlyHMSM(0u, 0u, 0u, 500u, OS_OPT_TIME_HMSM_STRICT, &os_err);
        //LED_R_TOGGLE();
    }
}

static void Task3(void *p_arg) {
    (void)p_arg;
    OS_ERR os_err;
    OS_ERR os_errAUX;
    int * p2data = 0;
    OS_MSG_SIZE msgSize;
    while (1) {
        OSSemPend(&semTest, 0, OS_OPT_PEND_BLOCKING, 0, &os_err);
        //OSTimeDly(1000u, OS_OPT_TIME_DLY, &os_err);
        p2data = (int *) OSQPend(&AppQ,
        						//(OS_MSG_SIZE * )  p2msgSize,
                              0,
                        (OS_OPT)        OS_OPT_PEND_BLOCKING,
                         &msgSize,
                        							0,
                        (OS_ERR *)       &os_err);
        os_errAUX = os_err;
        if(p2data != 0)
        {
        	if(p2data[0] == 0)
        	{
        		LED_G_TOGGLE();
        	    LED_B_TOGGLE();
        	 }
        	 else if(p2data[0] == 1)
        	 {
        		 LED_B_TOGGLE();
        		 LED_R_TOGGLE();
        	  }
        	  else if(p2data[0] == 2)
        	   {
        		  LED_R_TOGGLE();
        		  LED_G_TOGGLE();
        	   }
        }
    }
}


static void TaskStart(void *p_arg) {
    (void)p_arg;
    OS_ERR os_err;

    // Initialize the uC/CPU Services.
    CPU_Init();

#if OS_CFG_STAT_TASK_EN > 0u
    // (optional) Compute CPU capacity with no task running
    OSStatTaskCPUUsageInit(&os_err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif


    OSQCreate((OS_Q      *)&AppQ,
                 "My App Queue",
                     (OS_MSG_QTY  )100,
                     (OS_ERR     *)&os_err);

    //Create semaphore
    OSSemCreate(&semTest, "Sem Test", 0u, &os_err);

    // Create Task2
    OSTaskCreate(&Task2TCB, 			//tcb
                 "Task 2",				//name
                  Task2,				//func
                  0u,					//arg
                  TASK2_PRIO,			//prio
                 &Task2Stk[0u],			//stack
                  TASK2_STK_SIZE_LIMIT,	//stack limit
                  TASK2_STK_SIZE,		//stack size
                  0u,
                  0u,
                  0u,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);
    OSTaskCreate(&Task3TCB, 			//tcb
                     "Task 3",				//name
                      Task3,				//func
                      0u,					//arg
                      TASK3_PRIO,			//prio
                     &Task3Stk[0u],			//stack
                      TASK3_STK_SIZE_LIMIT,	//stack limit
                      TASK3_STK_SIZE,		//stack size
                      0u,
                      0u,
                      0u,
                     (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                     &os_err);

    data = 0;
    //LED_G_TOGGLE();
    while (1) {
        OSTimeDlyHMSM(0u, 0u, 1u, 0u, OS_OPT_TIME_HMSM_STRICT, &os_err);
        //OS_ERR os_errDEBUG = os_err;
        OSQPost(&AppQ, (void *)&data, 4, OS_OPT_POST_ALL, &os_err);
        //LED_G_TOGGLE();
        data++;
        if(data == 3)
        {
        	data = 0;
        }
    }
}

int main(void) {
    OS_ERR err;

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR  cpu_err;
#endif

    hw_Init();

    // RGB LED
    SIM->SCGC5 |= (SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK);
    LED_B_PORT->PCR[LED_B_PIN] = PORT_PCR_MUX(1);
    LED_G_PORT->PCR[LED_G_PIN] = PORT_PCR_MUX(1);
    LED_R_PORT->PCR[LED_R_PIN] = PORT_PCR_MUX(1);
    LED_B_GPIO->PDDR |= (1 << LED_B_PIN);
    LED_G_GPIO->PDDR |= (1 << LED_G_PIN);
    LED_R_GPIO->PDDR |= (1 << LED_R_PIN);
    LED_B_GPIO->PSOR |= (1 << LED_B_PIN);
    LED_G_GPIO->PSOR |= (1 << LED_G_PIN);
    LED_R_GPIO->PSOR |= (1 << LED_R_PIN);

    OSInit(&err);
 #if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
	 // Enable task round robin.
	 OSSchedRoundRobinCfg((CPU_BOOLEAN)1, 0, &err);
 #endif
    OS_CPU_SysTickInit(SystemCoreClock / (uint32_t)OSCfg_TickRate_Hz);

    OSTaskCreate(&TaskStartTCB,
                 "App Task Start",
                  TaskStart,
                  0u,
                  TASKSTART_PRIO,
                 &TaskStartStk[0u],
                 (TASKSTART_STK_SIZE / 10u),
                  TASKSTART_STK_SIZE,
                  0u,
                  0u,
                  0u,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 &err);

    OSStart(&err);

	//Should Never Get Here
    while (1) {

    }
}*/


int main (void)
{
	hw_Init();
    hw_DisableInterrupts();
    App_Init(); /* Program-specific setup */
    hw_EnableInterrupts();

    __FOREVER__
	App_Run(); /* Program-specific loop  */
}
