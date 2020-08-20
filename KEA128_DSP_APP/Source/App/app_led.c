/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_led.c  ---   led灯控制
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.01
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
/********************************************************************************
**  Function	: LED_Ctl
**  Author		: 
**  Created on	: 20180723
**  Description	:
**  Return		: 
********************************************************************************/
void LED_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_LED_NORMAL_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_LED_FAIL_CTL, GPIO_PinOutput);
}
void LED_Normal_Ctl(SCH_BOOL OnOff)
{
	if(OnOff == ON)
	{
		TurnOn_NORMAL_LED;
	}
	else
	{
		TurnOff_NORMAL_LED;
	}
}

void LED_Fail_Ctl(SCH_BOOL OnOff)
{
	if(OnOff == ON)
	{
		TurnOn_FAIL_LED;
	}
	else
	{
		TurnOff_FAIL_LED;
	}
}

/********************************************************************************
**  Function	: TASK_LED_pro
**  Author		: huwenjun
**  Created on	: 20180723
**  Description	:100ms执行一次
**  Return		: 
********************************************************************************/
void TASK_LED_pro(void)
{
	if(AMP_OFF_FLAG ||  AMP1_OFF_FLAG) return;
	if(Get_SysPower_Flag)
	{
		LED_Normal_Ctl(ON);
	}
	else
	{
		LED_Normal_Ctl(OFF);
	}
}






