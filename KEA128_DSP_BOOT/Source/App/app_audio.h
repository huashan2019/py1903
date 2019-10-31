///=========================
#ifndef __APP_AUDIO_H
#define __APP_AUDIO_H

extern SCH_U8 I2c_Aud_Buff[40];
/*typedef enum
{
	_150_HZ,
	_120_HZ,
	_100_HZ,
	_80_HZ,
	_70_HZ,
	_60_HZ,
	_50_HZ,
	_40_HZ
}eAudioBassCentFreq_t;
typedef enum
{
	_500_HZ=0,
	_1000_HZ,
	_1200_HZ,
	_2000_HZ
}eAudioMidCentFreq_t;
typedef enum
{
	_10000_HZ=0,
	_12500_HZ,
	_15000_HZ,
	_17500_HZ
}eAudioTrebCentFreq_t;*/
#define MIN_VOLUME            0
#define MAX_VOLUME            40
#define DEFAULT_VOLUME        20

#define DEFAULT_MIX_VOL    	    25
#define DEFAULT_BT_VOL		  	25
#define DEFAULT_G3_VOL		  	25

#define DEF_BASS_MID_TRE	   7
#define MAX_BASS_MID_TRE	   14
#define MAX_BASS               MAX_BASS_MID_TRE
#define DEFAULT_BASS           DEF_BASS_MID_TRE
#define MAX_MIDDLE             MAX_BASS_MID_TRE
#define DEFAULT_MIDDLE         DEF_BASS_MID_TRE
#define MAX_TREBLE             MAX_BASS_MID_TRE
#define DEFAULT_TREBLE         DEF_BASS_MID_TRE

#define	DEFAULT_BASS_F		_100_HZ
#define	DEFAULT_MID_F		_1000_HZ
#define	DEFAULT_TREB_F		_10000_HZ

#define	DEFAULT_BASS_Q		0
#define	DEFAULT_MID_Q		0
#define	DEFAULT_TREB_Q		0

#define DEF_SPEAKER	          9///7
#define MAX_SPEAKER	          18///14
#define MAX_FADE           	  MAX_SPEAKER
#define DEFAULT_FADE       	  DEF_SPEAKER
#define MAX_BALANCE        	  MAX_SPEAKER
#define DEFAULT_BALANCE    	  DEF_SPEAKER

#define MIN_SUBWOOFER         0///14///
#define MAX_SUBWOOFER         1///14///
#define DEFAULT_SUBWOOFER     0///7///
#define	DEFAULT_SUB_F		  0

#define MIN_LOUD              0
#define MAX_LOUD              1
#define DEFAULT_LOUD          0
#define	DEFAULT_LOUND_F		  0

typedef enum
{
	USER,
	FLAT,
	JAZZ,
	POP,
	CLASSIC,
	ROCK,
	NEWS,
	URBAN,
	RAVE,
	MOVE,
	TECHNO,
	MAX_EQ
}_EQ_MODE_;
#define MIN_EQ_MODE           USER
#define MAX_EQ_MODE           MAX_EQ
#define DEFAULT_EQ_MODE       FLAT///USER


typedef enum
{
	bass_index,
	middle_index,
	treble_index
}BassMidTrb_T;
typedef enum  
{
	AUDIO7719_TYPE,
	TEF663X_AUDIO_TYPE,
	MAX_AUDIO_TYPE
}_AudioSysType_t;
typedef enum
{
	Audio_Idle,	
	Audio_PowerOn,	
	Audio_Release,
	Audio_Init,
	Audio_Setting,
	Audio_PowerOff,	  
	Audio_Running
}_ASPSTATE;
typedef enum
{
	fader_index,
	balance_index
}FaderBalance_T;
typedef struct 
{
	_ASPSTATE AspState;
	SCH_BOOL AudioTypeCheck;
	_AudioSysType_t	AudioType;
	SCH_U8 AudioAddr;
	void (*pAudioInit)(void);
	void (*pAudio_UpdateChannel)(SCH_U8 channel);
	void (*pAudio_SetVolume)(SCH_U8 Lev);
	void (*pAudio_BassMidTrebAdjust)(BassMidTrb_T index,SCH_U8 lev);
	void (*pAudio_FaderBalanceAdjust)(FaderBalance_T index,SCH_U8 lev);
	void (*pAudio_SetLoudnessGain)(SCH_U8 index);
	void (*pAudio_SubwooferAdjust)(SCH_U8 AudPara);
	void (*pAudio_MixOnOff)(SCH_U8 AudPara);
}Audio_T;
extern Audio_T App_Audio;

typedef enum
{
	Stat_BASS_F, 
	Stat_BASS_Q, 
	Stat_MID_F,
	Stat_MID_Q,
	Stat_TRE_F,
	Stat_TRE_Q,
	Stat_SUB_F,
	Stat_LOUND_F,
	Stat_EQ,
	Stat_BASE_LEVEL,
	Stat_MID_LEVEL,
	Stat_TRE_LEVEL,
	Stat_SUB_LEVEL,
	Stat_LOUD_LEVEL, 
	Stat_FAD_LEVEL,
	Stat_BAL_LEVEL,
	Stat_VOL_LEVEL,
	Stat_MIX_LEVEL,
	Stat_MIX_ONOFF,
	Max_ASP_Size
} ASPfunction;
typedef struct{ 
	SCH_U8 Bass_F;
	SCH_U8 Bass_Q;
	SCH_U8 Mid_F;
	SCH_U8 Mid_Q;
	SCH_U8 Treb_F;
	SCH_U8 Treb_Q;
	SCH_U8 Sub_F;
	SCH_U8 Lound_F;
	_EQ_MODE_ EQ;
	SCH_U8 BassLevel;
	SCH_U8 MidLevel;
	SCH_U8 TrebLevel;
	SCH_U8 SubLevel;
	SCH_U8 LoundLevel;
	SCH_U8 FadLevel;
	SCH_U8 BalLevel;
	SCH_U8 VolLevel;
	SCH_U8 MixLevel;
	SCH_U8 MixEn;	
	SCH_U8 BtLevel;
	SCH_U8 NaviLevel;
}_ASP_PRM_;
extern _ASP_PRM_ AspPrm;

#define nBASS_F    			AspPrm.Bass_F
#define nBASS_Q   	        AspPrm.Bass_Q
#define nMiddle_F           AspPrm.Mid_F
#define nMiddle_Q           AspPrm.Mid_Q
#define nTREBLE_F     		AspPrm.Treb_F
#define nTREBLE_Q           AspPrm.Treb_Q
#define nSub_F      		AspPrm.Sub_F
#define nLound_F      		AspPrm.Lound_F

#define nEQ_Mode    	    AspPrm.EQ
#define nBass_Level         AspPrm.BassLevel
#define nMiddle_Level       AspPrm.MidLevel
#define nTreble_Level      	AspPrm.TrebLevel
#define nSubLevel		    AspPrm.SubLevel
#define nLoud_Level  	    AspPrm.LoundLevel
#define nFade_Level        	AspPrm.FadLevel
#define nBalance_Level      AspPrm.BalLevel
#define nVolume             AspPrm.VolLevel
#define nMix_Level         	AspPrm.MixLevel
#define nMix_En         	AspPrm.MixEn
#define nBt_Level		    AspPrm.BtLevel
#define nNavi_Level		    AspPrm.NaviLevel








typedef enum
{
	SLOW_DOWN,	
	SLOW_UP,	
	DIRE_DOWN,
	DIRE_UP
}_ADJUST_FUN;



#define ASP_POWER_ON()    App_Audio.AspState=Audio_PowerOn
#define ASP_POWER_OFF()   App_Audio.AspState=Audio_PowerOff
///=========================================================================================================AudioBreak==================
typedef enum
{
	AudioBreaks_IDLE,
	AudioBreaks_Priority,
	AudioBreaks_AudioChg,
	AudioBreaks_ReleaseMute,
	AudioBreaks_MixVolume,
	AudioBreaks_Waitting,
	AudioBreaks_AudioBack,
	AudioBreaks_End
}AudioBreaks_STATE;

typedef struct{
	AudioBreaks_STATE     AudioBreaksState;
	UCharField            Audio_Break_Flag;
	SCH_U16               AudioMuteTimer;
}_Audio_Breaks_; 
extern	_Audio_Breaks_    AudioBreaks;
#define F_Navi_handle     AudioBreaks.Audio_Break_Flag.field.B0
#define F_BT_handle       AudioBreaks.Audio_Break_Flag.field.B1
///===========================================================================================================================

extern void DefaultSetting(void);

extern void SetASPBASS_F( SCH_U8 AudPara  ); 			 
extern void SetASPBass_Q(SCH_U8 AudPara);
extern void SetASPMiddle_F( SCH_U8 AudPara  );
extern void SetASPMiddle_Q(SCH_U8 AudPara);
extern void SetASPTRE_F( SCH_U8 AudPara   );
extern void SetASPTRE_Q(SCH_U8 AudPara);
extern void SetASPSUB_F(SCH_U8 AudPara);
extern void SetASPLOUND_F(SCH_U8 AudPara);
extern void SetASP_EQ(SCH_U8 AudPara);
extern void SetASP_MBASE(SCH_U8 AudPara);
extern void SetASP_MIDDLE(SCH_U8 AudPara);
extern void SetASP_MTREBLE(SCH_U8 AudPara);
extern void SetASP_SUB(SCH_U8 AudPara);
extern void SetASP_MLOUD(SCH_U8 AudPara);
extern void SetASP_FAD(SCH_U8 AudPara);
extern void SetASP_BAL(SCH_U8 AudPara);
extern void SetASP_MVOL(SCH_U8 AudPara);
extern void SetASPMIX_Level(SCH_U8 AudPara);
extern void SetASPMIX_ON_OFF(SCH_U8 AudPara);
extern void AUD_UpdateSource(void);
extern void Aud_DefMode(void);
extern void Adj_Volume_Up_Down(SCH_U8 up_down);
extern void Adj_Volume(void);
extern void AdjustVolumeStepper(_ADJUST_FUN ad_fun);
extern void TASK_Audio_Pro(void);

#endif
