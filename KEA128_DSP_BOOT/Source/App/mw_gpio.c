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
**  Function    : ACC_Detect
**  Author      : wenjunHu
**  Created on  : 20160909
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T AccDetect;
void ACC_Init(void)
{///===ACC
	GPIO_PinInit(GPIO_ACC_DECT, GPIO_PinInput);
	sch_memset(&AccDetect, 0x00, sizeof(AccDetect));
}
void ACC_Detect(void)
{
	AccDetect.IO_Status = ACC_DET_LEVEL;
	if(AccDetect.IO_Status != Get_ACC_Flag)
	{
		if(++AccDetect.IO_SamplingCounter >= T200MS_8)
		{
			AccDetect.IO_SamplingCounter = 0;
			if(AccDetect.IO_Status)
			{
				Set_ACC_Flag;
				Printf("ACC ON \n");
			}
			else
			{
				Clr_ACC_Flag;
				PostMessage(POWER_MODULE,EVT_POWER_OFF,POWEROFF_FROM_ACC);
				Printf("ACC OFF \n");
			}
		}	
	}
	else
	{
		AccDetect.IO_SamplingCounter = 0;
	}
}
/********************************************************************************
**  Function    : REV_Detect
**  Author      : 
**  Created on  : 20160909
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T RevDetect;
void REV_Init(void)
{///===
	GPIO_PinInit(GPIO_REV_DECT, GPIO_PinInput_InternalPullup);
	sch_memset(&RevDetect, 0x00, sizeof(RevDetect));
}
void REV_Detect(void)
{
	if(Get_SysPower_Flag==OFF)
	{
		Clr_REV_Flag;
		Clr_REV_Info;
		RevDetect.IO_SamplingCounter = 0;
		return;
	}	
	RevDetect.IO_Status = !REV_DET_LEVEL;
	if(RevDetect.IO_Status != Get_REV_Flag)
	{
		if(++RevDetect.IO_SamplingCounter >= T200MS_8)
		{
			RevDetect.IO_SamplingCounter = 0;
			if(RevDetect.IO_Status)
			{
				Set_REV_Flag;
				Printf("REV ON \n");
			}
			else
			{
				Clr_REV_Flag;
				Printf("REV OFF \n");
			}
			PostMessage(MMI_MODULE,UICC_TFT_AWAKE,1);
		}
	}
	else
	{
		RevDetect.IO_SamplingCounter = 0;				
	}
}
/********************************************************************************
**  Function    : 
**  Author      : 
**  Created on  : 20160909
**  Description :
**  Return      : 
********************************************************************************/
void CAM_Init(void)
{///===
}
/********************************************************************************
**  Function    : ILL_Detect
**  Author      : 
**  Created on  : 20170428
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T IllDetect;
void ILL_Init(void)
{///===
	GPIO_PinInit(GPIO_ILL_DECT, GPIO_PinInput_InternalPullup);
	sch_memset(&IllDetect, 0x00, sizeof(IllDetect));
}
void ILL_Detect(void)
{
	if(Get_ACC_Flag==OFF)
	{
		Clr_ILL_Flag;
		IllDetect.IO_SamplingCounter = 0;
		return;
	}	
	IllDetect.IO_Status = !ILL_DET_LEVEL;
	if(IllDetect.IO_Status != Get_ILL_Flag)
	{
		if(++IllDetect.IO_SamplingCounter >= T200MS_8)
		{
			IllDetect.IO_SamplingCounter = 0;
			if(IllDetect.IO_Status)
			{
				Set_ILL_Flag;
				Printf("ILL ON \n");
			}
			else
			{
				Clr_ILL_Flag;
				Printf("ILL OFF \n");
			}
			if(Get_CCFL_POWER_Flag)
				PostMessage(MMI_MODULE,UICC_TFT_AWAKE,1);

		}
	}
	else
	{
		IllDetect.IO_SamplingCounter = 0;
	}
}
/********************************************************************************
**  Function    : PARK_Detect
**  Author      : 
**  Created on  : 20160909
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T ParkDetect;
void PARK_Init(void)
{///===
	GPIO_PinInit(GPIO_PARK_DECT, GPIO_PinInput_InternalPullup);
	sch_memset(&ParkDetect, 0x00, sizeof(ParkDetect));
}
void PARK_Detect(void)
{
	if(!Get_OsStartOk)
	{
		Clr_PARK_Flag;
		ParkDetect.IO_SamplingCounter = 0;
		return;
	}	
	ParkDetect.IO_Status = PARK_DET_LEVEL;
	if(ParkDetect.IO_Status != Get_PARK_Flag)
	{
		if(++ParkDetect.IO_SamplingCounter >= T200MS_8)
		{
			ParkDetect.IO_SamplingCounter = 0;
			if(ParkDetect.IO_Status)
			{
				Set_PARK_Flag;
				Printf("PARK ON \n");
			}
			else
			{
				Clr_PARK_Flag;
				Printf("PARK OFF \n");
			}
			PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_PARK);
			PostMessage(MMI_MODULE,UICC_TFT_AWAKE,1);
		}	
	}
	else
	{
		ParkDetect.IO_SamplingCounter = 0;	
	}
}
/********************************************************************************
**  Function    : MIC
**  Author      : 
**  Created on  : 20170428
**  Description :
**  Return      : 
********************************************************************************/
void MIC_IO_Init(void)
{
	GPIO_PinInit(GPIO_MIC_DECT, GPIO_PinInput_InternalPullup);
}
/********************************************************************************
**  Function    : POWER_KEY_Detect  LED_Power_Ctl
**  Author      : 
**  Created on  : 20160909
**  Description :
**  Return      : 
********************************************************************************/
void POWER_KEY_Init(void)
{
	GPIO_PinInit(GPIO_POWER_KEY_DECT, GPIO_PinInput_InternalPullup);
	GPIO_PinInit(GPIO_POWER_LED_CTL, GPIO_PinOutput);
}
void LED_Power_Ctl(void)
{
	if(Get_ACC_Flag)
	{
		if(!Get_SysPower_Flag) 
		{
			TurnOn_POWER_LED;
		}
		else
		{
			TurnOff_POWER_LED;
		}
	}
	else
	{
		TurnOff_POWER_LED;
	}
}

/********************************************************************************
**  Function    : LED_KEY_Ctl
**  Author      : 
**  Created on  : 20160909
**  Description :
**  Return      : 
********************************************************************************/
void LED_Key_Init(void)
{///===
	GPIO_PinInit(GPIO_KEY_LED_CTL, GPIO_PinOutput);
}
void LED_Key_Ctl(void)
{
	if(!Get_SysPower_Flag)
	{
		TurnOff_KEY_LED;
	}
	else
	{
		if(Get_ILL_Flag)
		{
			TurnOn_KEY_LED;
		}
		else
		{
			TurnOff_KEY_LED;
		}
	}
}
/********************************************************************************
**  Function    : ARM_Ctl
**  Author      : 
**  Created on  : 20170427
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T NaviGateDetect;
void ARM_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_ARM_RESET_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_ARM_REV_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_SYS_POWER_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_ARM_WORK_DECT, GPIO_PinInput_InternalPullup);
	GPIO_PinInit(GPIO_NAVI_GATE_DECT, GPIO_PinInput_InternalPullup);
	sch_memset(&NaviGateDetect, 0x00, sizeof(NaviGateDetect));
}
void NAVI_GATE_Detect(void)
{
	if(!Get_OsStartOk)
	{
		Clr_NAVI_GATE_Flag;
		NaviGateDetect.IO_SamplingCounter = 0;
		return;
	}
	NaviGateDetect.IO_Status = NAVI_GATE_LEVEL;
	if(NaviGateDetect.IO_Status != Get_NAVI_GATE_Flag)
	{
		if(++NaviGateDetect.IO_SamplingCounter >= T16MS_8)
		{
			NaviGateDetect.IO_SamplingCounter = 0;
			if(NaviGateDetect.IO_Status)
			{
				Set_NAVI_GATE_Flag;
				Printf("NAVI_GATE ON \n");
			}
			else
			{
				Clr_NAVI_GATE_Flag;
				Printf("NAVI_GATE OFF \n");
			}
		}	
	}
	else
	{
		NaviGateDetect.IO_SamplingCounter = 0;
	}
}
IO_DET_T ARM_WORKGateDetect;
void ARM_WORK_GATE_Detect(void)
{
	if(Get_SysPower_Flag==OFF)
	{
		Clr_ARM_WORK_GATE_Flag;
		ARM_WORKGateDetect.IO_SamplingCounter = 0;
		return;
	}
	ARM_WORKGateDetect.IO_Status = !ARM_WORK_LEVEL;
	if(ARM_WORKGateDetect.IO_Status != Get_ARM_WORK_GATE_Flag)
	{
		if(++ARM_WORKGateDetect.IO_SamplingCounter >= T48MS_8)
		{
			ARM_WORKGateDetect.IO_SamplingCounter = 0;
			if(ARM_WORKGateDetect.IO_Status)
			{
				Set_ARM_WORK_GATE_Flag;
				Printf("ARM_WORK_GATE ON \n");
			}
			else
			{
				Clr_ARM_WORK_GATE_Flag;
				Printf("ARM_WORK_GATE OFF \n");				
			}
		}	
	}
	else
	{
		ARM_WORKGateDetect.IO_SamplingCounter = 0;
	}
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
/********************************************************************************
**  Function	: SYS_POWER
**  Author		: 
**  Created on	: 20161009
**  Description	:
**  Return		: 
********************************************************************************/
void SYS_PWR_Init(void)
{
	///GPIO_PinInit(GPIO_SYS_3V3_CTL, GPIO_PinOutput);
	///GPIO_PinInit(GPIO_SYS_12V_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_SYS_POWER_CTL, GPIO_PinOutput);
}
/********************************************************************************
**  Function	: GPS_Ctl
**  Author		: 
**  Created on	: 20170427
**  Description	:
**  Return		: 
********************************************************************************/
void GPS_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_GPS_POWER_CTL, GPIO_PinOutput);
}
/********************************************************************************
**  Function	: USB_Ctl
**  Author		: 
**  Created on	: 20170427
**  Description	:
**  Return		: 
********************************************************************************/
void USB_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_USB_OC_DECT, GPIO_PinInput_InternalPullup);
	GPIO_PinInit(GPIO_USB_EN_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_USB1_OC_DECT, GPIO_PinInput_InternalPullup);
	GPIO_PinInit(GPIO_USB1_EN_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_USB_POWER_CTL, GPIO_PinOutput);
}

/********************************************************************************
**  Function	: 4G_Ctl
**  Author		: 
**  Created on	: 20170427
**  Description	:
**  Return		: 
********************************************************************************/
void G4_G3_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_G4_POWER_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_G4_RESET_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_G3_WAKEUP_DECT, GPIO_PinInput_InternalPullup);
}

/********************************************************************************
**  Function	: TUNER_Reset_Ctl
**  Author		: 
**  Created on	: 20160924
**  Description	:
**  Return		: 
********************************************************************************/
void TUNER_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_TUNER_RESET_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_TUNER_POWER_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_ANT_CTL, GPIO_PinOutput);
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
	GPIO_PinInit(GPIO_AMP_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_AMP_BEEP, GPIO_PinOutput);
	GPIO_PinInit(GPIO_AMP_DECT, GPIO_PinInput_InternalPullup);
}
/********************************************************************************
**  Function	: MUTE_Ctl
**  Author		: 
**  Created on	: 20160909
**  Description	:
**  Return		: 
********************************************************************************/
void MUTE_Init(void)
{///===
	///GPIO_PinInit(GPIO_MUTE_CTL, GPIO_PinOutput);
}
void MUTE_Ctl(SCH_BOOL OnOff)
{
	if(OnOff == ON)
	{
		///TurnOn_MUTE;
		if(!Get_MUTE_Flag)
			AmpMute(ON);
		Set_MUTE_Flag;
	}
	else
	{
		///TurnOff_MUTE;
		if(Get_MUTE_Flag)
			AmpMute(OFF);
		Clr_MUTE_Flag;
	}
}
/********************************************************************************
**  Function	: BT_Ctl
**  Author		: 
**  Created on	: 20171220
**  Description	:
**  Return		: 
********************************************************************************/
void BT_Init(void)
{///===
	GPIO_PinInit(GPIO_BT_RESET_CTL, GPIO_PinOutput);
}

/********************************************************************************
**  Function	: DSP_Ctl
**  Author		: 
**  Created on	: 20171220
**  Description	:
**  Return		: 
********************************************************************************/
void DSP_Init(void)
{///===
	GPIO_PinInit(GPIO_DSP_RESET_CTL,    GPIO_PinOutput);
	GPIO_PinInit(GPIO_AD1938_RESET_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_AUDIO_DECT, GPIO_PinInput_InternalPullup);
	GPIO_PinInit(GPIO_SPI_SS_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_REM_EN_CTL, GPIO_PinInput_InternalPullup);
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
	///ARM_IO_Init();
	DSP_Init();
	BT_Init();
	ACC_Init();
	///REV_Init();
	///CAM_Init();
	///ILL_Init();
	///PARK_Init();
	///MIC_IO_Init();
	///POWER_KEY_Init();
	///LED_Key_Init();
	SYS_PWR_Init();
	///CCFL_Power_Init();
	///TUNER_IO_Init();
	///GPS_IO_Init();
	///G4_G3_IO_Init();
	///USB_IO_Init();
	AMP_IO_Init();
	///MUTE_Init();
}



