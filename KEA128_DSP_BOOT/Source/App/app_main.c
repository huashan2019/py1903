/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_main.c  ---   音视频源
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

Channel_T SysChannel;



void SourceChannel_Pro(void)
{
	if(SysChannel.Channel_Timer)
		SysChannel.Channel_Timer--;
	if(SysChannel.Channel_Timer)
		return;
	switch(SysChannel.nChannelState)
	{
		case CHANNEL_IDLE:
			break;
		case CHANNEL_FRONT_SOURCE_CHG:
			AudioMute(SOFTON);
			AudioMute(HARDON);
			AudioSource = FrontSource;
			AUD_UpdateSource();
			SysChannel.nChannelState = CHANNEL_FRONT_SOURCE_CHG_END;
			break;
		case CHANNEL_FRONT_SOURCE_CHG_END:
			Clr_SYS_MUTE;
			if(/*Get_NAVI_GATE_Flag && Get_NAV_MUTE || Get_BT_GATE_Flag && Get_BT_MUTE || */!Get_NAVI_GATE_Flag && !Get_BT_GATE_Flag && Get_MED_MUTE)
			{
				Clr_MED_MUTE;
			}
			Mute_State=MUTE_UNMUTE_STATE;
			PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_SETUP);
			SysChannel.nChannelState = CHANNEL_IDLE;
			break;
		default:
			break;
	}
}

void MainMesssage(void)
{
	MESSAGE pMsg;
	if(FALSE==GetMessage(MAIN_MODULE,&pMsg))
		return;
	switch(pMsg.ID)
	{
		case EVT_SRC_POWER_ON:
			SysChannel.nChannelState = CHANNEL_FRONT_SOURCE_CHG;
			SysChannel.Channel_Timer = GetMax(SysChannel.Channel_Timer,pMsg.prm);;///开机delay 18S出声
			break;
		case EVT_MAIN_FRONT_SOURCE_SET:
			SysChannel.ChannelSource = (SOURCE)pMsg.prm;
			if(FrontSource == SysChannel.ChannelSource)
			{
				SysChannel.nChannelState = CHANNEL_FRONT_SOURCE_CHG_END;
			}
			else
			{
				SysChannel.nChannelState = CHANNEL_FRONT_SOURCE_CHG;
			}
			FrontSource = SysChannel.ChannelSource;
			SysChannel.Channel_Timer = T32MS_16;
			PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
			PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_REQUEST);
			break;
		default:
			break;
	}
}
void TASK_Main_Pro(void)
{
	MainMesssage();
	SourceChannel_Pro();
}



