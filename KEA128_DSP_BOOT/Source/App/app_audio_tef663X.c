/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_audio_tef663X.c  ---  
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

#define  INPUT_ARM	    		INPUT_SOURCE_IIS1
#define  INPUT_COMMON	    	INPUT_SOURCE_AIN0  

const eAudio_InputSource Audio_Path_Table[NUM_OF_SOURCE]=
{
	INPUT_SOURCE_RADIO,//SOURCE_TUNER
	INPUT_SOURCE_MAX,//SOURCE_DVD=0,
	INPUT_ARM,//SOURCE_USB,
	INPUT_ARM,//SOURCE_SD,
	INPUT_ARM,//SOURCE_BT,
	INPUT_SOURCE_MAX,//SOURCE_AUX,
	INPUT_SOURCE_MAX,//SOURCE_FRONT_AUX,
	INPUT_SOURCE_MAX,//SOURCE_CAMERA,
	INPUT_SOURCE_MAX,//SOURCE_DTV,
	INPUT_SOURCE_MAX,//SOURCE_DVR,
	INPUT_SOURCE_MAX,//SOURCE_IPOD,
	INPUT_SOURCE_MAX,//SOURCE_3G,
	INPUT_SOURCE_MAX,//SOURCE_HDMI,
};

#define F_TUNER_GAIN	5
#define F_ARM_GAIN	5
#define F_AUX_GAIN	5
#define F_BT_GAIN		3
#define F_A2DP_GAIN	5	
#define F_HDMI_GAIN	3
const S8 Channel_inGain[NUM_OF_SOURCE]=
{
	F_TUNER_GAIN,//SOURCE_TUNER
	0,//SOURCE_DVD,
	F_ARM_GAIN,//SOURCE_USB,
	F_ARM_GAIN,//SOURCE_SD,
	F_BT_GAIN,//SOURCE_BT,
	F_AUX_GAIN,//SOURCE_AUX,
	0,//SOURCE_FRONT_AUX,
	0,//SOURCE_CAMERA,
	0,//SOURCE_DTV,
	0,//SOURCE_DVR,
	0,//SOURCE_IPOD,
	0,//SOURCE_3G,
	F_HDMI_GAIN,//SOURCE_HDMI,
};


void AUD663x_UpdateChannel(u8 channel)
{
 	if(Audio_Path_Table[channel]==INPUT_SOURCE_MAX)
		return;
 	AudioSource=(SOURCE)channel;//////////////for debu
	if(channel<dim(Audio_Path_Table))
	{
		Audio_SetPrimaryInputSource(Audio_Path_Table[channel]);
	}
}


void Audio663x_SetVolume(SCH_U8 vol)
{
	Audio_SetVolume(PrimaryStr, nVolume);
}


const S8  BassMidTreGainTab[]=
{
	-12,-10,-8,-6,-4,-2,
	0,
	2,4,6,8,10,12
};

void Aud663x_BassMidTrebAdjust(BassMidTrb_T index,SCH_U8 lev)
{
	if(lev>=dim(BassMidTreGainTab))
		return;
	if(index==treble_index)  //treble
	{
		nTreble_Level = lev;
		Audio_SetTreb(PrimaryStr, BassMidTreGainTab[lev]);
	}
	else if(index==middle_index)
	{
		nMiddle_Level = lev;
		Audio_SetMid(PrimaryStr, BassMidTreGainTab[lev]);
	}
	else
	{
		nBass_Level = lev;
		Audio_SetBass(PrimaryStr, BassMidTreGainTab[lev]);
	}
}
const u8 Speaker_GainTab[]={

	0,3,6,10,15,20,73
};
void Aud663x_FaderBalanceAdjust(FaderBalance_T index,SCH_U8 lev)
{
	if(index == fader_index)
	{
		if(lev>MAX_FADE)
			return;
		nFade_Level = lev;
		if(lev >= DEFAULT_FADE)
			Audio_DrvFaderSet(Front,table_dB2Lin[Speaker_GainTab[lev-DEFAULT_FADE]]);
		else
			Audio_DrvFaderSet(Rear,table_dB2Lin[Speaker_GainTab[DEFAULT_FADE-lev]]);
	}
	else if(index == balance_index)
	{
		if(lev>MAX_BALANCE)
			return;		
		nBalance_Level = lev;
		if(lev >= DEFAULT_BALANCE)
			Audio_DrvBalanceSet(PrimaryStr,Left,table_dB2Lin[Speaker_GainTab[lev-DEFAULT_BALANCE]]);
		else
			Audio_DrvBalanceSet(PrimaryStr,Right,table_dB2Lin[Speaker_GainTab[DEFAULT_BALANCE-lev]]);   
	}
}

void Audio7719_SetLoudnessGain(SCH_U8 index)
{
	Audio_SetLoudness(index?ON:OFF);
}
void Audio7719_SubwooferAdjust(SCH_U8 index)
{
	
}

void Audio663X_DLL_Loader(void)
{
	Printf("AudioType --- TEF663X \n");
	App_Audio.pAudioInit=NULL;
	App_Audio.pAudio_UpdateChannel=AUD663x_UpdateChannel;
	App_Audio.pAudio_SetVolume=Audio663x_SetVolume;
	App_Audio.pAudio_BassMidTrebAdjust=Aud663x_BassMidTrebAdjust;
	App_Audio.pAudio_FaderBalanceAdjust=Aud663x_FaderBalanceAdjust;
	App_Audio.pAudio_SetLoudnessGain=Audio7719_SetLoudnessGain;
	App_Audio.pAudio_SubwooferAdjust=Audio7719_SubwooferAdjust;
	App_Audio.pAudio_MixOnOff=NULL;
}



