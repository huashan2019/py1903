/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_power.c  ---   系统开关机管理
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

Sys_Power_T SysPower;

#define HARDWARE_DET_AD     	ADC_CHANNEL_AD7
void Hardware_Ver_Pro(void)
{
	if(Sys.Hardware_Ver >= 0xF00)
	{
		Sys.Dsp_Hardware_Mode = 1;///1708布线模式
	}
	else
	{
		Sys.Dsp_Hardware_Mode = 0;///正常布线模式
	}
}

void SystemPowerUp(void)
{
	ClearAllModeMessage();
	GPIOInit();
	AudioMute(HARDON);
	TurnOn_DIGITAL_MUTE;
	TurnOff_AUDIO_SWITCH;/*wink channel*/
	DSP_RESET_HOLD;
	BT_RESET_HOLD;
	AD1938_RESET_HOLD;
	AD1978_RESET_HOLD;
	SYS_Power_Ctl(ON);
	ACC_EN_Ctl(ON);
	TurnOn_PRE_AMP;
	///TurnOn_REM_EN;
}
void PowerOnSystemModule(void)
{
	AMP_TURN_ON();
	Bsp_UART_Init();
	Bsp_ADC_Init();
	DSP_RESET_RELEASE;
	BT_RESET_RELEASE;
	AD1938_RESET_RELEASE;
	AD1978_RESET_RELEASE;
}

void PowerOffSystemModule(void)
{
	AudioMute(HARDON);
	TurnOn_DIGITAL_MUTE;
	///App_Dsp.DspPwrState = DSP_CLOSE;
}
void EnterPowerOff(void)
{
	AudioMute(HARDON);
	TurnOn_DIGITAL_MUTE;
	TurnOff_AUDIO_SWITCH;
	AMP_TURN_OFF();
	SYS_Power_Ctl(OFF);
	TurnOff_PRE_AMP;
	ACC_EN_Ctl(OFF);
	DSP_RESET_HOLD;
	BT_RESET_HOLD;
	AD1938_RESET_HOLD;
	AD1978_RESET_HOLD;
	Clr_AppStartOk;
	DSP_OFF_FLAG = Get_DSP_OFF_Flag;
}

void PowerMessage(void)
{
	MESSAGE pMsg;
	if(FALSE==GetMessage(POWER_MODULE,&pMsg))
		return;
	switch(pMsg.ID)
	{
		case EVT_POWER_ON:
			SysPower.bk_PowerOffReason=SysPower.PowerOffReason;
			SysPower.PowerOffReason=POWER_ON;
			SysPower.nPowerState=POWER_ON_DELAY;
			SysPower.Power_Timer=0;
			AudioMute(HARDON);
			TurnOn_DIGITAL_MUTE;
			///CCFL_Power_Ctl(OFF);
			ClearAllModeMessage();
			break;
		case EVT_POWER_OFF:
			SysPower.PowerOffReason = (POWEROFFREASON)pMsg.prm;
			if(SysPower.PowerOffReason==POWEROFF_FROM_START)
			{
				if(SysPower.nPowerState>=POWER_CLOSE_SCREEN)
					break;
				SysPower.nPowerState=POWER_ACCOFF;
			}
			else if(SysPower.PowerOffReason==POWEROFF_FROM_VOLTAGE)
			{
				if(SysPower.nPowerState>=POWER_CLOSE_SCREEN)
					break;
				SysPower.nPowerState=POWER_CLOSE_SCREEN;
			}
			SysPower.Power_Timer = 0;
			PowerOffSystemModule();
			///PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_POWER);
			break;
		default:break;
	}
}

void TASK_Power_Pro(void)
{
	PowerMessage();
	SysPower.Power_Timer++;
	switch(SysPower.nPowerState)
	{
		case POWER_ON_START:
			if(SysPower.Power_Timer>=T40MS_8)
			{
				if(Get_START_Flag&&!Get_VolErr_Flag)
				{
					PostMessage(POWER_MODULE,EVT_POWER_ON,0);  
				}
				else
				{
					if(Get_VolErr_Flag)
						SysPower.PowerOffReason=POWEROFF_FROM_VOLTAGE;
					SysPower.nPowerState=POWER_OFF_DELAY;
					SysPower.Power_Timer=0;  
				}
			}
			break;
		case POWER_ON_DELAY:
			SysPower.nPowerState=POWER_ARM_RESET;
			SysPower.Power_Timer=0;             			
			SystemPowerUp();
			break;
		case POWER_ARM_RESET:
			if(SysPower.Power_Timer>=T96MS_8)
			{
				DSP_RESET_HOLD;
				BT_RESET_HOLD;
				AD1938_RESET_HOLD;
				AD1978_RESET_HOLD;
				SysPower.nPowerState = POWER_VAR_RECOVER;
				SysPower.Power_Timer=0;
			}
			break;
		case POWER_VAR_RECOVER:
			if(SysPower.Power_Timer>=T480MS_8)
			{
				SysPower.Power_Timer=0;
				SysPower.nPowerState=POWER_TFT_POWER_ON;			       
				PowerOnSystemModule();
			}
			break;
		case POWER_TFT_POWER_ON:
			if(SysPower.Power_Timer>=T96MS_8)
			{
			
				SysPower.Power_Timer=0;				
				App_Dsp.DspPwrState = DSP_POWER_EN;
				SysPower.PowerOffReason=POWER_ON;/**/
				SysPower.nPowerState=POWER_SECURITY_CODE;
				ADC_GetData(HARDWARE_DET_AD,&Sys.Hardware_Ver);
				Hardware_Ver_Pro();
			}
			break;
		case POWER_SECURITY_CODE:
			if(App_Dsp.DspPwrState == DSP_NORMAL&&SysPower.Power_Timer>=T4S_8)
			{
				AudioMute(HARDOFF);
				TurnOff_DIGITAL_MUTE;
				Set_AppStartOk;
				//UartTxData(SCH_Uart_BT,BT_NAME_SET,sizeof(BT_NAME_SET));
				
				SysPower.nPowerState=POWER_NORMAL_RUN;
				Printf("PowerState = POWER_NORMAL_RUN \n");
			}
			break;
		case POWER_NORMAL_RUN:
			break;
		case POWER_CLOSE_SCREEN:
			TurnOff_REM_EN;
			if(SysPower.Power_Timer>=T480MS_8)
			{
				EnterPowerOff();
				SysPower.Power_Timer=0;
				SysPower.nPowerState=POWER_OFF_DELAY;
			}
			break;
		case POWER_OFF_DELAY:
			if(Get_START_Flag)
				SysPower.nPowerState=POWER_SYSTEM_STANDBY;
			else
				SysPower.nPowerState=POWER_ACCOFF;
			SysPower.Power_Timer=0;
			break;
		case POWER_SYSTEM_STANDBY:
			if(SysPower.Power_Timer >= T1S_8)
			{
				if(Get_SLEEP_Mode)
				{
				
				}
				else if(Get_START_Flag&&!Get_VolErr_Flag)
				{
					PostMessage(POWER_MODULE,EVT_POWER_ON,0);  
				}
				else if(!Get_START_Flag&&!Get_VolErr_Flag)
				{
					SysPower.nPowerState=POWER_ACCOFF;
					SysPower.Power_Timer = 0;
				}
			}
			break;
		case POWER_ACCOFF:
			if(SysPower.Power_Timer >= T3S_8)
			{
				TurnOff_REM_EN;
			}
			if(SysPower.Power_Timer >= T5S_8)
			{            
				EnterPowerOff();
				SysPower.nPowerState=POWER_ACCOFF2;
				SysPower.Power_Timer=0;
			}
			else if(Get_START_Flag)////
			{
				///TurnOn_REM_EN;
				///SysPower.nPowerState=POWER_TFT_POWER_ON;
				if(Get_AppStartOk)
					SysPower.nPowerState=POWER_SECURITY_CODE;
				else
					SysPower.nPowerState=POWER_ON_DELAY;
				SysPower.Power_Timer=0;
			}
			break;
		case POWER_ACCOFF2:
			if(SysPower.Power_Timer >= T240MS_8)
			{
				SysPower.Power_Timer=0;
				if(!Get_START_Flag)
				{
					Set_SLEEP_Mode;
				}			       
				SysPower.nPowerState=POWER_SYSTEM_STANDBY;
				Printf("PowerState = POWER_SYSTEM_STANDBY \n");
			}
			break;			
		case POWER_ARM_ERR_RESET:
			if(SysPower.Power_Timer>=T1S_8)
			{
				SysPower.Power_Timer=0;
				SysPower.nPowerState=POWER_ON_START;
			}
			break;
		default:break;
	}
}

