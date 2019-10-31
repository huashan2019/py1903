/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : APP_lowpower.c  ---   低功耗
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/

#include "include.h"



void SysWakeUpInit_FromACC(void)
{
	SIM->SCGC |= SIM_SCGC_IRQ_MASK;
	SIM->PINSEL = (SIM->PINSEL & (~SIM_PINSEL_IRQPS_MASK)) | SIM_PINSEL_IRQPS(3);///3--PTI2 
	IRQ->SC = IRQ_SC_IRQPDD_MASK|IRQ_SC_IRQEDG_MASK|IRQ_SC_IRQPE_MASK|IRQ_SC_IRQIE_MASK;
	NVIC_EnableIRQ(IRQ_IRQn);
}
void SysWakeUpExit_FromACC(void)
{
	SIM->SCGC &= ~SIM_SCGC_IRQ_MASK;
	SIM->PINSEL = (SIM->PINSEL & (~SIM_PINSEL_IRQPS_MASK)) | SIM_PINSEL_IRQPS(0);///0--PTA5  default 
	NVIC_DisableIRQ(IRQ_IRQn);
}

void SysWakeUpInit_FromRTC(void)
{
	
}

/*******************************************************************************\
* Function    : Reset I/O.          											*
* Input       :                                                                 *
* Output      :                                                                 *
* description :ACC off mode,reduce power consumption to lowest,reset all I/O	*
\*-----------------------------------------------------------------------------*/
void ResetIO(void)
{
	
}
void SysEnterBackup(void)
{
	Bsp_SysTick_Close();
	SysRtcExit();
	SysUartExit(SCH_Uart0);
	SysUartExit(SCH_Uart1);
	SysUartExit(SCH_Uart2);
}
/**************************************************************************************
* FunctionName   : TASK_AccOff_Pro()
* Description    : 任务处理   ---     lowpower
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void TASK_AccOff_Pro(void)
{
	while(Get_SLEEP_Mode)
	{
		Printf("Enter lowerpowermode \n");
		FeedDog();
		SysEnterBackup();
		Clr_AccWakeUP_Flag;
		SysWakeUpInit_FromACC();
		SysWakeUpInit_FromRTC();
		while(!Get_AccWakeUP_Flag)
		{
			FeedDog();
			if(ACC_DET_LVOFF)
				PMC_SetMode(PMC,PmcModeStop3);
			Printf("Be WakeUp \n");
			if(ACC_DET_LVON)
				Set_AccWakeUP_Flag;
		}
		SysWakeUpExit_FromACC();
		Bsp_RTC_Init();
		Bsp_SysTick_Init();
		Print_Init();
		Clr_SLEEP_Mode;		
		Printf("Enter Normalmode \n");
	}
}




