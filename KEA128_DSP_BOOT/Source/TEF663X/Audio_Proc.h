/***********************************************************************//**
 * @file		audio.h
 * @purpose		This example used to test NXP HERO function
 * @version		0.1
 * @date		1. Oct. 2010
 * @author		NXP BU Automotive Car Entertainment Team & VITEC ELECTRONICS(SHENZHEN) AE Team
 *---------------------------------------------------------------------
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors and VITEC Electronics assume no responsibility or liability for the
 * use of the software, convey no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors and VITEC Electronics
 * reserve the right to make changes in the software without
 * notification. NXP Semiconductors and VITEC Electronics also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/
#ifndef AUDIO_PROC_H
#define AUDIO_PROC_H

 #define AUDIO_PROCESS_TIMER	2000	//timeout 


 /*audio set */
typedef enum{
	AudioMode_Idel,
	AudioMode_Volume,
	AudioMode_Bass,
	AudioMode_Mid,
	AudioMode_Treble,
	AudioMode_Balance,
	AudioMode_Fader,
	AudioMode_Beep,
	AudioMode_Loudness,
	AudioMode_MixMic,
	AudioMode_Fade,
	AudioMode_UserEQBand1,
	AudioMode_UserEQBand2,
	AudioMode_UserEQBand3,
	AudioMode_UserEQBand4,
	AudioMode_UserEQBand5,
	AUDIO_MODE_LAST
}AUDIO_MODE;

extern AUDIO_MODE AudioMode;

/*usecase*/
#define HERO_USECASE  1


/*global variable*/
extern S8 TrebleGain;
extern S8 BassGain;
extern S8 MidGain;
extern S8 gBanlanceLevel;
extern S8 gFaderLevel;
extern char F_BeepEnable;
extern char F_MicEnable;
extern char F_FadeEnable;

extern U8 SoftMuteState;		//TRUE  = mute,
extern U8 Hero_GSABuffer[9];
extern S8 MainVolume_Lev;
extern U8 EQMode;
extern U8 Hero_LoudnessOn;

 /*Key define*/ 
#define AUDIO_MUTEKEY SK6


/* function ext*/
extern  void Audio_Set(void);
extern void Audio_Proc(void);
extern void Beep_Proc(void);
extern void Audio_GSAProc(void);
extern void VolumeProcInit(void);

extern void Audio_SetPrimaryInputSource(eAudio_InputSource InputSource);
#endif

