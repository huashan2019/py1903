///=========================
#ifndef __APP_RADIO_H
#define __APP_RADIO_H

typedef enum  
{
	NXPTEF6851_TUNER,
	TEF663X_TUNER_TYPE,
	MAX_TUNER_TYPE
}_RadioSysType_t;
typedef enum  
{
	RADIO_INIT_IDLE,	//0
	RADIO_POWER_EN,
	RADIO_RESET_WAIT,
	RADIO_RESET_RELEASE,
	RADIO_TYPE_CHECK,
	RADIO_POWER_ON,
	RADIO_POWER_INIT,
	RADIO_NORMAL,
	RADIO_CLOSE,
	RADIO_POWEROFF
} RadioPowerState_t;
#define RadioStartOn()      App_Radio.RadioPwrState=RADIO_POWER_EN
#define RadioReinit()       App_Radio.RadioPwrState=RADIO_POWER_INIT
#define RadioPowerOff()     App_Radio.RadioPwrState=RADIO_POWEROFF
#define RadioClose()        App_Radio.RadioPwrState=RADIO_CLOSE

///===========================================================================================
#define Region_USA         0
#define Region_LATIN       1////拉美
#define Region_EU          2///欧洲
#define Region_OIRT        3///东欧
#define Region_JAPAN       4///日本
#define Region_CHN         5///中国
#define MAX_REGION         6
#define RADIO_DEFAULT_REGION	Region_CHN  

#define DX          0
#define LOC         1

#define BAND_FM1                0
#define BAND_FM2                1
#define BAND_FM3                2
#define BAND_AM1                3
#define BAND_AM2                4
#define FM_BAND_NUM          3
#define AM_BAND_NUM          2
#define MAX_BAND_NUM         (FM_BAND_NUM+AM_BAND_NUM)      
#define MAX_PRESET_NUM		 6
///==================================================================================
#define BAD_STOP_LEVEL		    1
#define BAD_STOP_USN			2
#define BAD_STOP_WAM			3
#define ALL_QUAL_Good			4
#define BAD_OFFSET				5
#define BAD_STOP_SNR			6
 
#define NO_IFC_Result_Availble		0
#define BAD_IFC					    1
#define GOOD_IFC					2

#define RadioMuteOff        0
#define RadioMuteOn         1
 //Tuning Mode
#define GEN_TUNING_MODE_STANDARD	0x00
#define GEN_TUNING_MODE_PRESET		0x01
#define GEN_TUNING_MODE_SEARCH		0x02
#define GEN_TUNING_MODE_AFUPDATE	0x03
#define GEN_TUNING_MODE_JUMP		0x04
#define GEN_TUNING_MODE_CHECK		0x05
#define GEN_TUNING_MODE_MIRROR	    0x06
#define GEN_TUNING_MODE_END		    0x07

extern SCH_U16 GetFreqRang(SCH_U8 flag);
#define MIN_FREQ()	   GetFreqRang(0)
#define MAX_FREQ()	   GetFreqRang(1)
///===============================================================================================
typedef enum
{
	EVT_TUN_NONE,
	EVT_TUN_REQUEST,
	EVT_TUN_BAND,   /*change band*/
	EVT_TUN_SEEK,  /*EVT in tuner*/ /** prm=0: dowm ;prm>0: up**/
	EVT_TUN_MANUAL,  			/** prm=0: dowm ;prm>0: up**/
	EVT_TUN_AS,      			/** prm=0: dowm ;prm>0: up**/
	EVT_TUN_SCAN,           		/** prm=0: dowm ;prm>0: up**/
	EVT_TUN_DX_LOC,				/** DX mode**/
	EVT_TUN_SAVE,           		/** Save the station number into memory*/
	EVT_TUN_LISTEN,
	EVT_TUN_PLAY_UP_DOWN,
	EVT_TUN_PS_LISTEN,
	EVT_TUN_ON,             		/** turn on the tuner**/
	EVT_TUN_OFF,            		/** turn off the turner**/
	EVT_TUN_SEEK_BREAK,            		/** turn seek break**/
	EVT_TUN_FREQ,
	EVT_TUN_AREA,
	EVT_TUN_BANDSCAN,
	EVT_MUTE_ON_OFF,
	EVT_MUTE_ON,
	EVT_MUTE_OFF,
	EVT_EON,
	//-----------ADD by hsj 08.12.06
	EVT_RDS_AF,
	EVT_RDS_TA,
	EVT_RDS_PTY_UP,
	EVT_RDS_PTY_DOWN,
	EVT_RDS_PTY,
	EVT_RDS_REG,
	EVT_RDS_EON
}TUNER_EVT;
///===============================================================================================
typedef enum  {
    Seek_Idle,	      
    Seek_Prest ,    
    Seek_Request, 
    Seek_Search_Freqency, 
    Seek_Wait_Qual_Data_Available,
    Seek_Read_Quality,
    Seek_Wait_IFC_Available,
    Seek_Wait_RDS,
    Seek_Wait_Traffic,	
    Seek_Wait_PTY,
    Seek_Wait_PI,		   	
    Seek_Check_Station,
    Seek_Success_End
}SeekProcState_t;
#define Seek_NONE   0
#define Seek_DW     1
#define Seek_UP     2
typedef struct
{
	SCH_U8 Seeking   :2;
	SCH_U8 ASing     :1;	
	SCH_U8 Scaning   :1;	
	SCH_U8 LocalDx   :1;
	SCH_U8 PSing     :1;	
	SCH_U8 Stereo    :1;
	SCH_U8 mute      :1;
}_ICON_FLAG;
typedef union
{
	SCH_U8 byte;
	_ICON_FLAG field;
}ICON_FLAG;
#define MAX_AS_STATION		60
typedef struct {
	SCH_U16 freq;
	SCH_U8	SMeter;
}_AS_RADIO_INFO_;
extern _AS_RADIO_INFO_  AsRadioInfo[MAX_AS_STATION];
typedef struct Radio_Info{
	SeekProcState_t  SeekProcState;
	SCH_U8 region;
	SCH_U8 band;      
	SCH_U8 AS_Cnt; 
	SCH_U16 SMeter;
	SCH_U8 Usn;
	SCH_U8 Wam;
	SCH_U16 def_Smeter;                        
	ICON_FLAG icon;
	UCharField rds_icon;
	SCH_U8 Preset[MAX_BAND_NUM]; 
	SCH_U16 freq[MAX_BAND_NUM]; 
	SCH_U16 Seek_Start_Fre;
	_AS_RADIO_INFO_ memfreq[MAX_BAND_NUM][MAX_PRESET_NUM];
	SCH_U16 ScanDelayTimer;
	SCH_U16 StereoDispTimer;
	SCH_U16 SeekWaitTimer;
} _Radio_Info_;
extern _Radio_Info_ radioinfo;

#define radio_region          radioinfo.region
#define radio_band            radioinfo.band
#define radio_freq            radioinfo.freq[radio_band]
#define radio_Smeter          radioinfo.SMeter
#define radio_start_freq      radioinfo.Seek_Start_Fre  
#define radio_memfreq         radioinfo.memfreq
#define radio_curpreset       radioinfo.Preset[radio_band]
#define radio_AS_Cnt          radioinfo.AS_Cnt
#define radio_ScanDelayTimer  radioinfo.ScanDelayTimer

#define F_TunerDispIcon   radioinfo.icon.byte
#define F_TunerSeekIcon   radioinfo.icon.field.Seeking
#define F_TunerASIcon     radioinfo.icon.field.ASing
#define F_TunerScanIcon   radioinfo.icon.field.Scaning
#define F_LocalDx         radioinfo.icon.field.LocalDx
#define F_TunerPSIcon     radioinfo.icon.field.PSing
#define F_TunerStereo     radioinfo.icon.field.Stereo
#define F_TunerMute       radioinfo.icon.field.mute

#define F_RdsDispIcon     radioinfo.rds_icon.byte
#define F_RDS_FUN         radioinfo.rds_icon.field.B0
#define F_TunerAF         radioinfo.rds_icon.field.B1
#define F_TunerTA         radioinfo.rds_icon.field.B2
#define F_TunerEON        radioinfo.rds_icon.field.B3

typedef struct 
{
	SCH_U8 nRadioInitTimer;
	SCH_U8 TunerAddr;
	SCH_BOOL TunerTypeCheck;
	_RadioSysType_t TunerType;
	RadioPowerState_t RadioPwrState;
	SCH_U8 FmLoc_Level;
	SCH_U8 FmDx_Level;
	SCH_U8 AmLoc_Level;
	SCH_U8 AmDx_Level;
	void (*pTunerRegisterInit)(SCH_U8 band, SCH_U8  tuning_mode);
	void (*pTuner_BandChange)( void);
	void (* pTunerSetFrequency)(SCH_U16 frequency, SCH_U8 band,SCH_U8  tuning_mode);
	void (*pMute)(SCH_BOOL mute);
	SCH_U8 (*pTunerReadStatus)(void);
	SCH_U8 (*pGetSmeterIndicator)(void);
	SCH_U8 (*pGetStereoIndicator)(void);
	SCH_U8 (*pTunerCheckGoodIFC)(SCH_U8 measure_time);
	SCH_U8 (*pTuner_QUAL_DATA_AVAILABLE)(void);
	SCH_U8 (*pTunerCheckGoodQuality)(void);
	SCH_BOOL (*pTunerPowerOn)(void);
}Radio_T;
extern Radio_T App_Radio;
///=====================================================================================================================



extern void RadioInit(void);
extern void TASK_Radio_Pro(void);

#endif
