/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_mmi.c  ---   √¸¡Ó¥¶¿Ì
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

UICC_KEY_CODE keycode;
SCH_U8 keyvalue,keysource;



static void MainPowerCtl(void)
{
	SCH_U8 Evt_ID;
	Evt_ID=keycode;
	if(!Get_SysPower_Flag)
	{
		if(Evt_ID==UICC_POWER_ON||Evt_ID==UICC_POWER)
		{
			keycode=NO_KEY;	
			PostMessage(POWER_MODULE,EVT_POWER_ON,NONE);
			return;
		}
	}
	else
	{
		if(Evt_ID==UICC_POWER_OFF||Evt_ID==UICC_POWER)
		{
			keycode=NO_KEY;	
			PostMessage(POWER_MODULE,EVT_POWER_OFF,POWEROFF_FROM_MANNED);
		}	
		else if(Evt_ID==UICC_POWER_ON)	
		{
			keycode=NO_KEY;
			PostMessage(MMI_MODULE,UICC_MUTE,0);
		} 	
		else if(Evt_ID==UICC_TFT_AWAKE)
		{
			SetCCFL_DelayTime(keysource);
			keycode=NO_KEY;
		}
	}
}

void Tuner_Mode_Control(void)
{
	if(AudioSource!=SOURCE_TUNER)
		return;
	switch(keycode)
	{
		case UICC_CH_UP:
			PostMessage(TUNER_MODULE, EVT_TUN_PLAY_UP_DOWN,1);
			keycode=NO_KEY;
			break;
		case UICC_CH_DOWN:
			PostMessage(TUNER_MODULE, EVT_TUN_PLAY_UP_DOWN,0);
			keycode=NO_KEY;
			break;
		case UICC_FASTF:
			PostMessage(TUNER_MODULE, EVT_TUN_SEEK,1);
			///PostMessage(TUNER_MODULE, EVT_TUN_MANUAL,1);
			keycode=NO_KEY;
			break;
		case UICC_FASTR:
			PostMessage(TUNER_MODULE, EVT_TUN_SEEK,0);
			///PostMessage(TUNER_MODULE, EVT_TUN_MANUAL,0); 
			keycode=NO_KEY;	
			break;		
		case UICC_SKIPF:
			PostMessage(TUNER_MODULE, EVT_TUN_PLAY_UP_DOWN,1);
			keycode=NO_KEY;	
			break;
		case UICC_SKIPB:
			PostMessage(TUNER_MODULE, EVT_TUN_PLAY_UP_DOWN,0);
		    keycode=NO_KEY;
			break;
		case UICC_LOC_DX:
			PostMessage(TUNER_MODULE, EVT_TUN_DX_LOC,keyvalue);
	 	    keycode=NO_KEY;
			break;
		case UICC_TUNER:
			if(FrontSource==SOURCE_TUNER)
			{	
				PostMessage(TUNER_MODULE, EVT_TUN_BAND,0xff);  
				keycode=NO_KEY;
			}
			break;
		case UICC_SCAN:
			PostMessage(TUNER_MODULE, EVT_TUN_SCAN,0); 
	    	keycode=NO_KEY;
			break;
		case UICC_ASPS:
			PostMessage(TUNER_MODULE, EVT_TUN_AS,0); 
		    keycode=NO_KEY;
			break;
		default:break;
	}
}

void Arm_Key_Send(void)
{
	if(keycode!=NO_KEY)
		PostMessage(ARM_MODULE,M2A_UICC_CMD,SCH_WORD(keycode,M2A_KEY_SUB));
}

void TASK_Mmi_Pro(void)
{
	MESSAGE pMsg;
	if(FALSE==GetMessage(MMI_MODULE,&pMsg))
		return;
	keycode=(UICC_KEY_CODE)pMsg.ID;
	if(keycode==NO_KEY)
		return;
	keysource = LSB(pMsg.prm);
	keyvalue = MSB(pMsg.prm);
	MainPowerCtl();
	Tuner_Mode_Control();
	switch(keycode)
	{
		case UICC_MUTE:
			if(Get_SYS_MUTE || Get_NAV_MUTE || Get_BT_MUTE || Get_MED_MUTE)
			{
				Clr_SYS_MUTE;
				Clr_BT_MUTE;
				Clr_NAV_MUTE;
				Clr_MED_MUTE;
			}
			else
			{
				Get_SYS_MUTE=!Get_SYS_MUTE; 
			}
			Mute_State=MUTE_UNMUTE_STATE;
			PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_VOLUME);
			keycode=NO_KEY;
			break;
		case UICC_VOLUME_DOWN:
			Adj_Volume_Up_Down(DOWN);
			keycode=NO_KEY;
			break;
		case UICC_VOLUME_UP:
			Adj_Volume_Up_Down(UP);
			keycode=NO_KEY;
			break;
		case UICC_SCREEN:
			if(Get_CCFL_POWER_Flag)
			{
				PostMessage(MMI_MODULE,UICC_SCREEN_OPEN,0);
			}
			else
			{
				PostMessage(MMI_MODULE,UICC_SCREEN_CLOSE,0);
			}
			keycode=NO_KEY;
			break;
		case UICC_SCREEN_OPEN:
			PostMessage(MMI_MODULE,UICC_TFT_AWAKE,0);
			CCFL_Power_Ctl(ON);
			PostMessage(ARM_MODULE,M2A_SYS_CMD,M2A_SCREEN);
			keycode=NO_KEY;
			break;
		case UICC_SCREEN_CLOSE:
			CCFL_Power_Ctl(OFF);
			PostMessage(ARM_MODULE,M2A_SYS_CMD,M2A_SCREEN);
			keycode=NO_KEY;
			break;
		default:break;
	}	
	Arm_Key_Send();
}












