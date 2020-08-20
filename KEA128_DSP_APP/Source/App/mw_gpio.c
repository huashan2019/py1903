/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_gpio.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.08.08
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"



GPIO_Type *GPIO_Arry[]={GPIOA,GPIOB,GPIOC};
SCH_BOOL GPIO_ReadInputPin(GPIO_PinType GPIO_Pin)
{
	return (GPIO_Arry[GPIO_Pin/32]->PDIR & (SCH_U32)(1<<(GPIO_Pin%32))) ? HIGH_LEVEL : LOW_LEVEL;
}
/********************************************************************************
**  Function    : DSP_Test_Detect
**  Author      : wenjunHu
**  Created on  : 20180724
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T DSP_TEST_Detect;
void DSP_TEST_Init(void)
{///===
	GPIO_PinInit(GPIO_DSP_TEST_DECT, GPIO_PinInput_InternalPullup);
	sch_memset(&DSP_TEST_Detect, 0x00, sizeof(DSP_TEST_Detect));
}
SCH_BOOL DSP_OFF_FLAG = 0;
void DSP_Test_Detect(void)
{
	SCH_U8 pData;

	if(SysPower.nPowerState != POWER_NORMAL_RUN)
		return;
	DSP_TEST_Detect.IO_Status = DSP_TEST_DET_LVON;
	if(DSP_TEST_Detect.IO_Status != DSP_OFF_FLAG)
	{
		if(++DSP_TEST_Detect.IO_SamplingCounter >= T200MS_8)
		{
			DSP_TEST_Detect.IO_SamplingCounter = 0;
			if(DSP_TEST_Detect.IO_Status)
			{
				Set_DSP_OFF_Flag;
				DSP_OFF_FLAG = 1;
				pData = 1;/*turn off*/
				Dsp_OFF();
			}
			else
			{
				Clr_DSP_OFF_Flag;
				DSP_OFF_FLAG = 0;
				pData = 2;/*turn on*/
				Dsp_ON();
			}
			PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData,0x0E));
			PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0xFE,0x00));///Ë¢ÐÂ
		}	
	}
	else
	{
		DSP_TEST_Detect.IO_SamplingCounter = 0;
	}
}


/********************************************************************************
**  Function    : AMP_Detect
**  Author      : wenjunHu
**  Created on  : 20180724
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T AMP_Detect;
IO_DET_T AMP1_Detect;

void AMP_Init(void)
{///===
	GPIO_PinInit(GPIO_AMP_DECT, GPIO_PinInput_InternalPullup);
	sch_memset(&AMP_Detect, 0x00, sizeof(AMP_Detect));
	
	GPIO_PinInit(GPIO_AMP1_DECT, GPIO_PinInput_InternalPullup);
	sch_memset(&AMP1_Detect, 0x00, sizeof(AMP1_Detect));
}
SCH_BOOL AMP_OFF_FLAG = 0;
SCH_BOOL AMP1_OFF_FLAG = 0;
extern void LED_Fail_Ctl(SCH_BOOL OnOff);

void AMP_Diag_Detect(void)
{
	if(SysPower.nPowerState != POWER_NORMAL_RUN)
		return;
	AMP_Detect.IO_Status = AMP_DET_LVON;
	AMP1_Detect.IO_Status = AMP1_DET_LVON;
	if(AMP_Detect.IO_Status != AMP_OFF_FLAG || AMP1_Detect.IO_Status != AMP1_OFF_FLAG)
	{
		if(++AMP_Detect.IO_SamplingCounter >= T200MS_8)
		{
			AMP_Detect.IO_SamplingCounter = 0;
			if(AMP_Detect.IO_Status || AMP1_Detect.IO_Status)
			{
				if(AMP_Detect.IO_Status)
					AMP_OFF_FLAG = 1;
				if(AMP1_Detect.IO_Status)
					AMP1_OFF_FLAG = 1;
				//Dsp_OFF();
				LED_Fail_Ctl(ON);
				AMP_TURN_OFF();
				AudioMute(HARDON);
			}
			else
			{
				AMP_OFF_FLAG = 0;
				AMP1_OFF_FLAG = 0;
				Dsp_ON();
				LED_Fail_Ctl(OFF);
				AMP_TURN_ON();
				AudioMute(HARDOFF);
			}
		}	
	}
	else
	{
		AMP_Detect.IO_SamplingCounter = 0;
	}
}


/********************************************************************************
**  Function	: SYS_POWER
**  Author		: 
**  Created on	: 20161009
**  Description	:
**  Return		: 
********************************************************************************/
void PWR_IO_Init(void)
{
	///GPIO_PinInit(GPIO_SYS_3V3_CTL, GPIO_PinOutput);
	///GPIO_PinInit(GPIO_SYS_12V_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_SYS_POWER_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_ACC_EN_CTL,    GPIO_PinOutput);
	GPIO_PinInit(GPIO_PRE_AMP_CTL,	 GPIO_PinOutput);
}
void SYS_Power_Ctl(SCH_BOOL OnOff)
{
	if(OnOff == ON)
	{
		TurnOn_SYS_POWER;
		Set_SysPower_Flag;
	}
	else
	{
		TurnOff_SYS_POWER;
		Clr_SysPower_Flag;
	}
}
void ACC_EN_Ctl(SCH_BOOL OnOff)
{
	if(OnOff == ON)
	{
		TurnOn_ACC_EN;
	}
	else
	{
		TurnOff_ACC_EN;
	}
}
/********************************************************************************
**  Function	: AMP
**  Author		: 
**  Created on	: 20160909
**  Description	:
**  Return		: 
********************************************************************************/
void AMP_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_AMP_CTL,   GPIO_PinOutput);
	GPIO_PinInit(GPIO_AMP_DECT,  GPIO_PinInput_InternalPullup);
}
/********************************************************************************
**  Function	: BT_IO_Ctl
**  Author		: 
**  Created on	: 20171220
**  Description	:
**  Return		: 
********************************************************************************/
void BT_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_BT_RESET_CTL, GPIO_PinOutput);
}

/********************************************************************************
**  Function	: GPIOInit
**  Author		: wenjunHu
**  Created on	: 20160623
**  Description	: 
**  Return		: NULL
********************************************************************************/
void GPIOInit(void)
{
	DSP_IO_Init();
	BT_IO_Init();
	DSP_TEST_Init();
	AMP_Init();
	Start_IO_Init();
	PWR_IO_Init();
	AMP_IO_Init();
	MUTE_IO_Init();
	LED_IO_Init();
	AD1938_IO_Init();
	Eprom_IO_Init();
}



