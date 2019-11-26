/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   G5
 ******************************************************************************/

#include "hardware.h"
#include "tasksManager.h"


int main(void) {

	hw_Init();
	//Disable_Interrupts();
	startTasks();
	//Enable_Interrupts();
}
