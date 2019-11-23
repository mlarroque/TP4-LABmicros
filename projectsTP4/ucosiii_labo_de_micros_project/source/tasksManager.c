/*
 * tasksManager.c
 *
 *  Created on: 22 nov. 2019
 *      Author: G5
 */

#include "hardware.h"
#include  <os.h>
#include "App.h"
#include <stdint.h>
#include "MK64F12.h"


// Task Start
#define TASKSTART_STK_SIZE 		512u
#define TASKSTART_PRIO 			2u
static OS_TCB TaskStartTCB;
static CPU_STK TaskStartStk[TASKSTART_STK_SIZE];


// Task 2 - APP
#define TASK2_STK_SIZE			256u
#define TASK2_STK_SIZE_LIMIT	(TASK2_STK_SIZE / 10u)
#define TASK2_PRIO              3u
static OS_TCB Task2TCB;
static CPU_STK Task2Stk[TASK2_STK_SIZE];

// Task 3 - CLOUD
#define TASK3_STK_SIZE			256u
#define TASK3_STK_SIZE_LIMIT	(TASK2_STK_SIZE / 10u)
#define TASK3_PRIO              3u
static OS_TCB Task3TCB;
static CPU_STK Task3Stk[TASK3_STK_SIZE];

//App queue
OS_Q AppQ;

//Cloud queue
OS_Q CloudQ;

static void TaskStart(void *p_arg);
static void Task2(void *p_arg);
static void Task3(void *p_arg);


void startTasks(void)
{
	OS_ERR err;

	#if (CPU_CFG_NAME_EN == DEF_ENABLED)
	    CPU_ERR  cpu_err;
	#endif



	 OSInit(&err);
	 #if (OS_CFG_SCHED_ROUND_ROBIN_EN > 0u)
		 // Enable task round robin.
		 OSSchedRoundRobinCfg((CPU_BOOLEAN)1, 0, &err);
	 #endif
	 OS_CPU_SysTickInit(SystemCoreClock / ((uint32_t)OSCfg_TickRate_Hz));

	 OSTaskCreate(&TaskStartTCB, "App Task Start", TaskStart, 0u, TASKSTART_PRIO,
			 	 &TaskStartStk[0u], (TASKSTART_STK_SIZE / 10u), TASKSTART_STK_SIZE,
				 0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP), &err);

	 OSStart(&err);

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

   // Create Task2
   OSTaskCreate(&Task2TCB, "APP", Task2, 0u, TASK2_PRIO, &Task2Stk[0u], TASK2_STK_SIZE_LIMIT, TASK2_STK_SIZE,
		   	   	   0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &os_err);

   // Create Task3
   OSTaskCreate(&Task3TCB, "Things Speak", Task3, 0u, TASK3_PRIO, &Task3Stk[0u], TASK3_STK_SIZE_LIMIT, TASK3_STK_SIZE,
   		   	   	  0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &os_err);

   while (1)
   {
	   OSTimeDlyHMSM(0u, 0u, 10u, 0u, OS_OPT_TIME_HMSM_STRICT, &os_err);
  }
}

static void Task2(void *p_arg) {
    (void)p_arg;
    OS_ERR os_err;
    OS_ERR os_errAUX;
    OS_MSG_SIZE msgSize;
    int * p2data = 0;

    OSQCreate((OS_Q *)&AppQ, "My App Queue", (OS_MSG_QTY  )100, (OS_ERR     *)&os_err);

   // hw_DisableInterrupts();
     App_Init((void *) &AppQ); //Program-specific setup
    //hw_EnableInterrupts();


    while (1) {
    	p2data = (int *) OSQPend(&AppQ, 0, (OS_OPT)OS_OPT_PEND_BLOCKING, &msgSize, 0, (OS_ERR *)&os_err);
    	os_errAUX = os_err;
    	App_Run(); // Program-specific loop
    }

}




static void Task3(void *p_arg) {
    (void)p_arg;
    OS_ERR os_err;
    OS_ERR os_errAUX;
    int * p2data = 0;
    OS_MSG_SIZE msgSize;
    //while (1) {

    //}
}




