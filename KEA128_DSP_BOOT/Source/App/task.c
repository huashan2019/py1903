/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : task.c
**  Description : This file is the SYS.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"



///===================================================
void Task_1ms_Pro(void)
{
	///TASK_Voltage_Det();
}

void Task_2ms_Pro(void)
{
	
}

void Task_4ms_Pro(void)
{
	///TASK_Voltage_Det();
	///Tw8816CtrlProc();
}

void Task_8ms_Pro(void)
{
	Iap.IAP_ResendTime++;
	Iap.IAP_DelayTime++;
	TASK_Arm_Pro();
	///TASK_Auto_Pro();
	///TASK_Can_Pro();
	///TASK_Power_Pro();
	///TASK_Audio_Pro();
	///TASK_Radio_Pro();
	///TASK_Key_Pro();
	///ACC_Detect();
	///REV_Detect();
	///ILL_Detect();
	///PARK_Detect();
	///NAVI_GATE_Detect();
}

void Task_16ms_Pro(void)
{
	///TASK_CCFL_Pro();
	///TASK_Mmi_Pro();
	///TASK_Main_Pro();
}

void Task_100ms_Pro(void)
{
	///GetRTCClock();
	if(Get_IAP_Mode)
		IAP_Pro();
}
///=============================================================================================
void SCH_Task_Execute (void)
{
	/*if(F_1ms_Val)
	{
		F_1ms_Clr;
		Task_1ms_Pro();
	}
	if(F_2ms_Val)
	{
		F_2ms_Clr;
		Task_2ms_Pro();
	}*/
	
	if(F_4ms_Val)
	{
		F_4ms_Clr;
		Task_4ms_Pro();
	}
	if(F_8ms_Val)
	{
		F_8ms_Clr;
		Task_8ms_Pro();
	}
	if(F_16ms_Val)
	{
		F_16ms_Clr;
		Task_16ms_Pro();
	}
	if(F_100ms_Val)
	{
		SCH_U16 i;
		F_100ms_Clr;
		
		//Arm_ACK(COMM_ACK);
		Set_MUTE_Flag;

		Task_100ms_Pro();
	}
}

