/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_audio.c  ---   声音
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

Audio_T App_Audio;
_ASP_PRM_ AspPrm;
_Audio_Breaks_    AudioBreaks;
SCH_U8 I2c_Aud_Buff[40];
const SCH_U8 AudioChipAddr[MAX_AUDIO_TYPE]=
{
	0x88,
	0xC6
};

const pfunction AudioDllLoader[MAX_AUDIO_TYPE]=
{
	SCH_NULL,
	Audio663X_DLL_Loader
};
///=============================================================================================DEFAULT===================
const pU8 SettingVar_Table[Max_ASP_Size]=
{
	&AspPrm.Bass_F,
	&AspPrm.Bass_Q,
	&AspPrm.Mid_F,
	&AspPrm.Mid_Q,
	&AspPrm.Treb_F,
	&AspPrm.Treb_Q,
	&AspPrm.Sub_F,
	&AspPrm.Lound_F,
	&AspPrm.EQ,
	&AspPrm.BassLevel,
	&AspPrm.MidLevel,
	&AspPrm.TrebLevel,
	&AspPrm.SubLevel,
	&AspPrm.LoundLevel,
	&AspPrm.FadLevel,
	&AspPrm.BalLevel,
	&AspPrm.VolLevel,
	&AspPrm.MixLevel,
	&AspPrm.MixEn
};
const SCH_U8 Var_Range_Table[Max_ASP_Size]=
{
	DEFAULT_BASS_F,
	DEFAULT_BASS_Q,
	DEFAULT_MID_F,
	DEFAULT_MID_Q,
	DEFAULT_TREB_F,
	DEFAULT_TREB_Q,
	DEFAULT_SUB_F,
	DEFAULT_LOUND_F,

	DEFAULT_EQ_MODE,
	DEFAULT_BASS,
	DEFAULT_MIDDLE,
	DEFAULT_TREBLE,	
	DEFAULT_SUBWOOFER,
	DEFAULT_LOUD,
	DEFAULT_FADE,		
	DEFAULT_BALANCE,
	DEFAULT_VOLUME,
	DEFAULT_MIX_VOL,
	OFF           
};
const U8pfunction FunCall[Max_ASP_Size]=
{
	SetASPBASS_F, 
	SetASPBass_Q,
	SetASPMiddle_F,
	SetASPMiddle_Q,
	SetASPTRE_F,
	SetASPTRE_Q,
	SetASPSUB_F,
	SetASPLOUND_F,		
			
	SetASP_EQ,
	SetASP_MBASE,
	SetASP_MIDDLE,
	SetASP_MTREBLE,	
	SetASP_SUB,
	SetASP_MLOUD,  
	SetASP_FAD, 
	SetASP_BAL,
	SetASP_MVOL,
	SetASPMIX_Level,
	SetASPMIX_ON_OFF,
};
void DefaultSetting(void)
{
	SCH_U8 index;
	for(index=0;index< Max_ASP_Size;index++)
		*SettingVar_Table[index]=Var_Range_Table[index];
	nNavi_Level = 20;
	nBt_Level = 20;
}
void Call_Funs(SCH_U8 index,SCH_U8 para)
{
	if(index>=Max_ASP_Size) 
		return;
	(*(*FunCall[index]))(para);
}
void ASP_Setting_AGain(void)
{
	SCH_U8 i;
	pU8	pBuf=(pU8)&AspPrm;
	for(i=Stat_BASS_F;i<Max_ASP_Size;i++)
	{
		Call_Funs(i, pBuf[i]); 
	}
}
void Aud_DefMode(void)
{
	SetASP_FAD(DEFAULT_FADE);
	SetASP_BAL(DEFAULT_BALANCE);
	SetASP_MLOUD(DEFAULT_LOUD);
	SetASP_SUB(DEFAULT_SUBWOOFER);
	SetASP_EQ(DEFAULT_EQ_MODE);
}
///=============================================================================================DEFAULT END===================

///=============================================================================================BassMidTrb===================
SCH_U8 UserEqData[3]={DEFAULT_BASS,DEFAULT_MIDDLE,DEFAULT_TREBLE};
void AUD_BassMidTrbUpdate(BassMidTrb_T basetrb,SCH_U8 startbass,SCH_U8 stopbass)
{
	while(1)
	{	        
		if(startbass>stopbass)
			--startbass;       
		else if(startbass<stopbass)
			++startbass;
		if(App_Audio.pAudio_BassMidTrebAdjust)
			App_Audio.pAudio_BassMidTrebAdjust(basetrb,startbass);  
		if(startbass==stopbass)
			break;	
	}
}
void SetASPBASS_F(SCH_U8 AudPara)
{
	AspPrm.Bass_F=AudPara;
	AUD_BassMidTrbUpdate(bass_index,AspPrm.BassLevel,AspPrm.BassLevel);
}
void SetASPBass_Q(SCH_U8 AudPara)
{
	AspPrm.Bass_Q=AudPara;
	AUD_BassMidTrbUpdate(bass_index,AspPrm.BassLevel,AspPrm.BassLevel);
}
void SetASPMiddle_F(SCH_U8 AudPara)
{
	AspPrm.Mid_F=AudPara;
	AUD_BassMidTrbUpdate(middle_index,AspPrm.MidLevel,AspPrm.MidLevel);
}
void SetASPMiddle_Q(SCH_U8 AudPara)
{
	AspPrm.Mid_Q=AudPara;
	AUD_BassMidTrbUpdate(middle_index,AspPrm.MidLevel,AspPrm.MidLevel);
}
void SetASPTRE_F(SCH_U8 AudPara)
{
	AspPrm.Treb_F=AudPara;
	AUD_BassMidTrbUpdate(treble_index,AspPrm.TrebLevel,AspPrm.TrebLevel);
}
void SetASPTRE_Q(SCH_U8 AudPara)
{
	AspPrm.Treb_Q=AudPara;
	AUD_BassMidTrbUpdate(treble_index,AspPrm.TrebLevel,AspPrm.TrebLevel);
}
void SetASP_MBASE(SCH_U8 AudPara)
{
	SCH_U8 baselevel; 		
	baselevel=nBass_Level;
	nBass_Level =AudPara; 
	UserEqData[bass_index]=nBass_Level;
	AUD_BassMidTrbUpdate(bass_index,baselevel,AspPrm.BassLevel);
}
void SetASP_MIDDLE(SCH_U8 AudPara)
{
	SCH_U8 midlevel; 		
	midlevel=nMiddle_Level;
	nMiddle_Level = AudPara;
	UserEqData[middle_index]=nMiddle_Level;
	AUD_BassMidTrbUpdate(middle_index,midlevel,AspPrm.MidLevel);
}
void SetASP_MTREBLE(SCH_U8 AudPara)
{
	SCH_U8 trelevel; 		
	trelevel=nTreble_Level;
	nTreble_Level = AudPara; 
	UserEqData[treble_index]=nTreble_Level;
	AUD_BassMidTrbUpdate(treble_index,trelevel,AspPrm.TrebLevel);
}
///================================================================================================BassMidTrb END================

///================================================================================================EQ================
const SCH_U8 EQTab[3][MAX_EQ]=
{
/*
0--user
1-flat,轻音乐
2-jazz,爵士
3-pop,流行
4-classical,古典
5-rock，摇滚
6-news，新闻
7-urban，城市
8-rave，舞会
9-movie，电影
10-techno，电子乐
*/
	7, 7,13, 9,12,14, 6,10,10, 9,12,//bass 对应UI值 7-0db
	7, 7,10,10, 7, 6, 5, 9,10, 9, 9,//Middle 对应UI值
	7, 7,11,13,11,13, 5,12,13, 5,10,//treble 对应UI值
};
void SetASP_EQ(SCH_U8 AudPara)
{
	SCH_U8 ntemp;
	if(AudPara)
	{
		ntemp=nBass_Level;
		nBass_Level=EQTab[0][AudPara];
		AUD_BassMidTrbUpdate(bass_index,ntemp,nBass_Level);
		ntemp=nMiddle_Level;
		nMiddle_Level=EQTab[1][AudPara];
		AUD_BassMidTrbUpdate(middle_index,ntemp,nMiddle_Level);
		ntemp=nTreble_Level;
		nTreble_Level=EQTab[2][AudPara];
		AUD_BassMidTrbUpdate(treble_index,ntemp,nTreble_Level);
	}
	else
	{
		ntemp=nBass_Level;
		nBass_Level=UserEqData[bass_index];
		AUD_BassMidTrbUpdate(bass_index,ntemp,nBass_Level);
		ntemp=nMiddle_Level;
		nMiddle_Level=UserEqData[middle_index];
		AUD_BassMidTrbUpdate(middle_index,ntemp,nMiddle_Level);
		ntemp=nTreble_Level;
		nTreble_Level=UserEqData[treble_index];
		AUD_BassMidTrbUpdate(treble_index,ntemp,nTreble_Level);
	}
	nEQ_Mode=(_EQ_MODE_)AudPara;   
}
///================================================================================================EQ END================

///================================================================================================Sub================
void AUD_SubUpdate(SCH_U8 startsub,SCH_U8 targetsub)
{ 
	while(1)
	{	        
		if(startsub>targetsub)
			--startsub;       
		else if(startsub<targetsub)
			++startsub;
		if(App_Audio.pAudio_SubwooferAdjust)
			App_Audio.pAudio_SubwooferAdjust(startsub);
		if(startsub==targetsub)
			break;	
	}
}
void SetASPSUB_F(SCH_U8 AudPara)
{
	AspPrm.Sub_F = AudPara;
	AUD_SubUpdate(AspPrm.SubLevel,AspPrm.SubLevel);
}
void SetASP_SUB(SCH_U8 AudPara)
{
	SCH_U8 sublevel;
	sublevel = nSubLevel;
	nSubLevel = AudPara;
	AUD_SubUpdate(sublevel,nSubLevel);
}
///================================================================================================Sub END================

///================================================================================================Lound================
void AUD_LoundUpdate(SCH_U8 startLound,SCH_U8 targetLound)
{ 
	while(1)
	{	        
		if(startLound>targetLound)
			--startLound;       
		else if(startLound<targetLound)
			++startLound;
		if(App_Audio.pAudio_SetLoudnessGain)
			App_Audio.pAudio_SetLoudnessGain(startLound);
		if(startLound==targetLound)
			break;	
	}
}
void SetASPLOUND_F(SCH_U8 AudPara)
{
	AspPrm.Lound_F= AudPara;
	AUD_LoundUpdate(AspPrm.LoundLevel,AspPrm.LoundLevel);
}
void SetASP_MLOUD(SCH_U8 AudPara)
{
	SCH_U8 loudlevel;
	loudlevel = nLoud_Level;
	nLoud_Level = AudPara;
	AUD_LoundUpdate(loudlevel,nLoud_Level);
}
///================================================================================================Lound END================

///================================================================================================FAD BAL================
void SetASP_FAD(SCH_U8 AudPara)
{
	SCH_U8 startFAD,targetFAD;    
	startFAD=nFade_Level ;
	targetFAD=AudPara;
	while(1)
	{	 
		if(startFAD>targetFAD)
			--startFAD;       
		else  if(startFAD<targetFAD) 
			++startFAD;
		nFade_Level =startFAD;
        if(App_Audio.pAudio_FaderBalanceAdjust)
			App_Audio.pAudio_FaderBalanceAdjust(fader_index,nFade_Level);
		if(startFAD==targetFAD)
			break;
	}	
}
void SetASP_BAL(SCH_U8 AudPara)
{
	SCH_U8 startBAL,targetBAL;    
	startBAL=nBalance_Level ;
	targetBAL=AudPara;
	while(1)
	{	 
		if(startBAL>targetBAL)
			--startBAL;       
		else  if(startBAL<targetBAL) 
			++startBAL;
		nBalance_Level =startBAL;
        if(App_Audio.pAudio_FaderBalanceAdjust)
			App_Audio.pAudio_FaderBalanceAdjust(balance_index,nBalance_Level);
		if(startBAL==targetBAL)
			break;
	}	
}
///================================================================================================FAD BAL END================

///=======================================================================================================VOL================
void AUD_VolUpdate(SCH_U8 startVol,SCH_U8 targetVol)
{ 
	while(1)
	{	        
		if(startVol>targetVol)
			--startVol;       
		else if(startVol<targetVol)
			++startVol;
		if(App_Audio.pAudio_SetVolume)
			App_Audio.pAudio_SetVolume(startVol);
		if(startVol==targetVol)
			break;	
	}
}
void SetASP_MVOL_Dire(SCH_U8 AudPara)
{
	if(App_Audio.pAudio_SetVolume)
		App_Audio.pAudio_SetVolume(AudPara);
}
void SetASP_MVOL(SCH_U8 AudPara)
{
	static SCH_U8 nCurrentVol;
	AUD_VolUpdate(nCurrentVol,AudPara);
	nCurrentVol = AudPara;
}
void Adj_Volume(void)
{
	if(Get_BT_GATE_Flag)
	{
		SetASP_MVOL(nBt_Level);
	}
	else if(Get_NAVI_GATE_Flag)
	{
		SetASP_MVOL(nNavi_Level);
	}
	else
	{
		SetASP_MVOL(nVolume);
	}
}
void AdjustVolumeStepper(_ADJUST_FUN ad_fun)
{
	if(ad_fun==SLOW_DOWN)///slow down
	{
		SetASP_MVOL(0);             
	}
	else if(ad_fun==SLOW_UP)///slow up
	{
		Adj_Volume();            
	}
	else if(ad_fun==DIRE_DOWN)///direct down
	{
		SetASP_MVOL_Dire(0);              
	}
	else if(ad_fun==DIRE_UP)///direct up
	{
		SetASP_MVOL_Dire(nVolume);              
	}
}
void Adj_Volume_Up_Down(SCH_U8 up_down)
{
	SCH_U8 index = 0;
	#if 1
	if(Get_BT_GATE_Flag && Get_BT_MUTE)
	{
		index = 1;
		Clr_BT_MUTE;

	}
	else if(Get_NAVI_GATE_Flag && Get_NAV_MUTE)
	{
		index = 1;
		Clr_NAV_MUTE;
	}
	else if(!Get_NAVI_GATE_Flag && !Get_BT_GATE_Flag && Get_MED_MUTE)
	{
		index = 1;
		Clr_MED_MUTE;
	}
	if(Get_SYS_MUTE) 
	{
		index = 1;
		Clr_SYS_MUTE;
	}
	Mute_State=MUTE_UNMUTE_STATE;
	#else
	if(Get_SYS_MUTE) 
	{
		if(Get_BT_GATE_Flag)
		{
			index = 1;
			Clr_BT_MUTE;
			Get_USER_ADJVOL_Flag = 0x02;
		}
		else if(Get_NAVI_GATE_Flag)
		{
			index = 1;
			Clr_NAV_MUTE;
			Get_USER_ADJVOL_Flag = 0x01;
		}
		else
		{
			index = 1;
			Clr_MED_MUTE;
			Get_USER_ADJVOL_Flag = 0x00;
		}
		Clr_SYS_MUTE;
		Mute_State=MUTE_UNMUTE_STATE;
	}
	#endif
	if(up_down==UP && !index)
	{
		if(Get_BT_GATE_Flag)
		{
			index = 1;
			nBt_Level = GetMin(nBt_Level+1,MAX_VOLUME);
			Get_USER_ADJVOL_Flag = 0x02;
		}
		else if(Get_NAVI_GATE_Flag)
		{
			index = 1;
			nNavi_Level = GetMin(nNavi_Level+1,MAX_VOLUME);
			Get_USER_ADJVOL_Flag = 0x01;
		}
		else
		{
			index = 1;
			nVolume = GetMin(nVolume+1,MAX_VOLUME);
			Get_USER_ADJVOL_Flag = 0x00;
		}
		Adj_Volume();		
	}
	else if(up_down==DOWN && !index)
	{
		if((Get_BT_GATE_Flag)&&(nBt_Level>MIN_VOLUME))
		{
			index = 1;
			nBt_Level--;
			Adj_Volume();
			Get_USER_ADJVOL_Flag = 0x02;
		}
		else if((Get_NAVI_GATE_Flag)&&(nNavi_Level>MIN_VOLUME))
		{
			index = 1;
			nNavi_Level--;
			Adj_Volume();
			Get_USER_ADJVOL_Flag = 0x01;
		}
		else if(nVolume>MIN_VOLUME)
		{
			index = 1;
			nVolume--;
			Adj_Volume();
			Get_USER_ADJVOL_Flag = 0x00;
		}
		else
		{
			index = 1;
			//Set_SYS_MUTE;
			//AudioMute(HARDON);	
			if(Get_BT_GATE_Flag)
			{
				Get_USER_ADJVOL_Flag = 0x02;
				//Set_BT_MUTE;
			}
			else if(Get_NAVI_GATE_Flag) 
			{
				Get_USER_ADJVOL_Flag = 0x01;
				//Set_NAV_MUTE;
			}
			else
			{
				Get_USER_ADJVOL_Flag = 0x00;
				//Set_MED_MUTE;
			}
			
		}
	}
	if(index)
		PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_VOLUME);
}
///=======================================================================================================VOL END================

///=======================================================================================================MIX================
void SetASPMIX_Level(SCH_U8 AudPara)
{
	
}
void SetASPMIX_ON_OFF(SCH_U8 AudPara)
{
	
}
///=======================================================================================================MIX END================

///======================================================================================================AudioTypeCheck=================
SCH_BOOL AudioTypeCheck(void)
{
	SCH_U8 i;
	App_Audio.AudioType=MAX_AUDIO_TYPE;
	for(i=0;i<MAX_AUDIO_TYPE;i++)
	{
		App_Audio.AudioAddr = AudioChipAddr[i];
		if(I2C0_IsFindAddr(App_Audio.AudioAddr))
		{
			Printf("AudioTypeCheck OK --- %d \n",i);
			App_Audio.AudioTypeCheck=TRUE;
			App_Audio.AudioType=(_AudioSysType_t)i;
			break;
		}
	}
	if(App_Audio.AudioType==MAX_AUDIO_TYPE)
	{
		Printf("AudioTypeCheck fail \n");
		return FALSE;
	}
	if(AudioDllLoader[App_Audio.AudioType])
		(*AudioDllLoader[App_Audio.AudioType])();
	return TRUE;
}
///======================================================================================================AudioTypeCheck END=================

void AudioInit(void)
{
	if(App_Audio.pAudioInit)
		App_Audio.pAudioInit();
}


///=======================================================================================================Channel================
void AUD_UpdateChannel(SCH_U8 channel)
{
	if(App_Audio.pAudio_UpdateChannel)
		App_Audio.pAudio_UpdateChannel(channel);
} 
void AUD_UpdateSource(void)
{	
	AUD_UpdateChannel(AudioSource);
}
///=======================================================================================================Channel END===============

///=======================================================================================================================
void AudioProcedure(void)	
{
	if(!Get_ACC_Flag)
	{
		App_Audio.AspState = Audio_Idle;
		AudioSource = FrontSource;
		return;
	}
	switch(App_Audio.AspState)
	{
		case Audio_Idle:
			break;	
		case Audio_PowerOn:
			if(App_Radio.RadioPwrState == RADIO_NORMAL)
			{
				I2C0Init();
				App_Audio.AspState=Audio_Release;
			}
			break;			
		case Audio_Release:
			if(AudioTypeCheck())
			{
				App_Audio.AspState=Audio_Init;
			}
			break;
		case Audio_Init:
			AudioInit();
			App_Audio.AspState=Audio_Setting;
			break;
		case Audio_Setting:
			ASP_Setting_AGain();
			AUD_UpdateSource();
			///AudioMute(SOFTON);
			App_Audio.AspState=Audio_Running;
			break;
		case Audio_PowerOff:
			App_Audio.AspState=Audio_Idle;
			break;				
		case Audio_Running:
			break;
		default:break;
	}
}
///======================================================自动插播处理流程================================AudioBreak==============
void AudioInterCut_Pro(void)
{
	if(!Get_SysPower_Flag)
	{
		AudioBreaks.AudioBreaksState = AudioBreaks_IDLE;
		Clr_NAVI_GATE_Flag;
		Clr_BT_GATE_Flag;
		return;
	}
	if(AudioBreaks.AudioMuteTimer)
	{
		AudioBreaks.AudioMuteTimer--;
		return;
	}
	switch(AudioBreaks.AudioBreaksState)
	{
		case AudioBreaks_IDLE:
			if(!Get_SYS_MUTE && (Get_NAVI_GATE_Flag/* && !Get_NAV_MUTE*/ ||Get_BT_GATE_Flag/* && !Get_BT_MUTE*/))
			{
				AudioBreaks.AudioBreaksState=AudioBreaks_Priority;
			}
			break;
		case AudioBreaks_Priority:
			AudioMute(SOFTON);
			AudioMute(HARDON);
			if(Get_BT_GATE_Flag)
     		{
				AudioBreaks.AudioMuteTimer=300;
				F_BT_handle = 1;
     		}
     		else if(Get_NAVI_GATE_Flag)
     		{
				AudioBreaks.AudioMuteTimer=3;
				F_Navi_handle = 1;
     		}
			AudioBreaks.AudioBreaksState=AudioBreaks_AudioChg;
			break;
		case AudioBreaks_AudioChg:
			if(F_BT_handle)
			{
			 	AUD_UpdateChannel(SOURCE_BT);
			 	///SetASP_MVOL(nBt_Level);
	     		AudioBreaks.AudioMuteTimer=5;
			}
			else if(F_Navi_handle)
			{
			 	AUD_UpdateChannel(SOURCE_NAVI);
			 	///SetASP_MVOL(nNavi_Level);
	     		AudioBreaks.AudioMuteTimer=2;
			}
			AudioBreaks.AudioBreaksState=AudioBreaks_ReleaseMute;
			break;
		case AudioBreaks_ReleaseMute:
			Get_SYS_MUTE=0;
			Mute_State=MUTE_UNMUTE_STATE;
			AudioBreaks.AudioBreaksState=AudioBreaks_Waitting;
			PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_SETUP);
			break;
		case AudioBreaks_Waitting:
			if(Get_BT_GATE_Flag==0&&F_BT_handle)
			{
				AudioMute(SOFTON);
				AudioMute(HARDON);
				AudioBreaks.AudioMuteTimer=5;
				F_BT_handle=0;
				AudioBreaks.AudioBreaksState=AudioBreaks_AudioBack;
			}
			else if(Get_NAVI_GATE_Flag==0&&F_Navi_handle)
			{
				AudioMute(SOFTON);
				AudioMute(HARDON);
				AudioBreaks.AudioMuteTimer=5;
				F_Navi_handle=0;
				AudioBreaks.AudioBreaksState=AudioBreaks_AudioBack;
			}
			break;
		case AudioBreaks_AudioBack:
		 	AUD_UpdateChannel(FrontSource);
		 	///SetASP_MVOL(nVolume);
			AudioBreaks.AudioMuteTimer=5;
			AudioBreaks.AudioBreaksState=AudioBreaks_End;
			break;
		case AudioBreaks_End:
			Mute_State=MUTE_UNMUTE_STATE;
			AudioBreaks.AudioBreaksState=AudioBreaks_IDLE;
			break;
		default:break;
	}
} 
///======================================================自动插播处理流程================================AudioBreak END==============
void TASK_Audio_Pro(void)
{
	AudioProcedure();
	AudioInterCut_Pro();
}



