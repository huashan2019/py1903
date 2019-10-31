#ifndef HERO_AUDIO_DRV
#define HERO_AUDIO_DRV

/*I2c writ memory type define*/
 typedef enum
{
	I2CDRV_EP_MEM,          /*< Extended P-memory (also known as patchmemory or extension RAM) */
	I2CDRV_X_MEM,           /*< X-memory */
	I2CDRV_Y_MEM,            /*< Y-memory */
	I2CDRV_INVALID       
} I2CDrv_MemType_t;

typedef struct{
	I2CDrv_MemType_t type;
	U16 addr;
	S32 data;
}sMemAddr_Data;

#define TEF663x_SlaveAddr 0xC6


/*audio stream define*/
typedef enum Audio_Stream
{
	PrimaryStr     = 1,
	SecondaryStr   = 2,
	NavigationStr  = 3,
	PhoneStr       = 4
} Audio_Stream_t;
/*audio channel define*/
typedef enum Audio_Channel
{
	Mono        = 1,
	Front       = 2,
	Rear        = 3,
	Left        = 4,
	Right       = 5,
	FrontLeft   = 6,
	FrontRight  = 7,
	RearLeft    = 8,
	RearRight   = 9,
	Center      = 10,
	SubWoofer   = 11,
	SubCenter   = 12,
	Navigation  = 13,
	Phone       = 14,
	Chime       = 15,
	PDC         = 16,
	Secondary   = 17    /* Used for Sink Select Connect */
} eAudioChannel;


	  
/*Audio Feature define*/
typedef enum Audio_Feature
{   
	Balance                = 1,
	ChannelGain            = 2,
	ChannelMute            = 3,
	ChimeGenerator         = 4,
	ClickClack             = 5,
	ClipFrontRear          = 6,   /* only used for input pin connection */
	ClipSub                = 7,   /* only used for input pin connection */
	ClipCenter             = 8,   /* only used for input pin connection */
	ClipRearSecondary      = 9,   /* only used for input pin connection */
	Compressor             = 10,
	DC                     = 11,
	DeEmphasis             = 12,
	Delay                  = 13,
	Fader                  = 14,
	FastMute               = 15,  /* only used for input pin connection */
	GEQ                    = 16,
	GPAF1a                 = 17,
	GPAF1b                 = 18,
	GPAF2                  = 19,
	GPAF3a                 = 20,
	GPAF3b                 = 21,
	GPAF4                  = 22,
	GPAF5                  = 23,
	HDBlend                = 24,  /* only used for input pin connection */
	Limiter                = 25,
	Loudness               = 26,
	MakeSubCenter          = 27,
	Noise                  = 28,
	ParkingDistanceControl = 29,
	PEQ                    = 30,
	Sine                   = 31,
	SoftAudioMute          = 32,
	SourceScaling          = 33,
	SRC1                   = 34,
	SRC2                   = 35,
	SRC3                   = 36,
	SuperPosition          = 37,
	Tone                   = 38,
	Volume                 = 39,
	VoiceFilter            = 40, /* RESERVED */
	SecondarySuperposition = 41,
	SecondaryVolume        = 42,
	FrontDAC               = 43, /* For sink selection */
	RearDAC                = 44, /* For sink selection */
	I2SOUT0                = 45, /* For sink selection */
	I2SOUT1                = 46, /* For sink selection */
	I2SOUT2                = 47, /* RESERVED */
	I2SOUT3                = 48, /* RESERVED */
	NavigationFilter       = 49,
	PhoneFilter            = 50,
	PolyChime         =51
} eAudio_Feature_t;
//superposition feature   

/*define Superposition channel*/
typedef enum {
	eCHANNEL_PRIMARY = 0,
	eCHANNEL_SECONDARY,
	eCHANNEL_NAVI,
	eCHANNEL_PHONE,
	eCHANNEL_FRONT,
	eCHANNEL_REAR,
	eCHANNEL_CENTER,
	eCHANNEL_SUBWOOFER,
	eCHANNEL_EXTERNAL,
	eCHANNEL_CLICK_CLACK,
	eCHANNEL_POLY_CHIME,
	eCHANNEL_MAX,
} eAudio_SupperPositionCh_t;



/*DAC ID Define*/
#define PERIOHERALID_RDAC 0x32
#define PERIOHERALID_FDAC 0x33
#define DAC_ON 0x00
#define DAC_OFF 0x01

/*Audio ClickClack Feature*/
/*define control mode*/
typedef enum
{
	IO_FLAG=0,//use io control
	I2C_CONTROL //use i2c command control
}eAudio_ClickClackControlMode_t;

/*define sound type*/
typedef enum
{
	CLICK=0,  //sound type
	CLACK      //sound type
}eAudio_ClickClackSound_t;

/*define RAM or Rom */
typedef enum
{
	USE_RAM=0, //wave table in ram
	USE_ROM     //wave table in rom
}eAudio_ClickClackWavTable_t;

/*Audio PolyChime Feature*/
/*define sine generator parameter*/
typedef struct
{
	U32 iFreq;//sine freqency
	U32 iVolume;//sine generator volume
}sAudioPolyChimeSinePara_t;

/*define sineLFO   parameter*/
typedef struct
{
	U32 iFreq;//sineLFO freqency
	U32 iVolume;//sineLFO generator amplitude 
	U32 iOffset;
}sAudioPolyChimeSineLFOPara_t;
/*define misc parameter,e.g:mute decay,repetitions,repeat time*/
typedef struct
{
	U32 iDecay;//mute decay 
	U32 iRepetitions;//repetition
	U32 iRepeatTime;//repeat time
}sAudioPolyChimeMiscPara_t;

/*enveloppe parameter*/
/*
                  <attack>|<  sustain    > <decay>
                                ------------- 
                              /                        \ 
                            /                           \
                          /                              \
                        /                                 \
<delay time>  /                                      \
___________       /                                         \_______            */
typedef struct
{ 
	U32 iDuration; //Enveloppe duaration
	U32 iDecay;	//eveloppe decay
	U32 iVolume;//eveloppe volume
	U32 iSustain;//sustain time
	U32 iAttackTime;//attck time
}sAudioPolyChimeEnvPara_t;

/*define Sine generator*/
typedef enum
{
	SINE1=0,
	SINE2,
	SINE3,
	SINE4,
	SINE5,
	SINE6,
	SINE_COUNT
}eAudioPolyChimeSine_t;

/*define envloppe */
typedef enum
{
	ENV1=0,
	ENV2,
	ENV3,
	ENV_COUNT
}eAudioPolyChimeEnv_t;

/*polyChime Parameter;*/
typedef struct
{
	sAudioPolyChimeSinePara_t SinePara[SINE_COUNT];//sine generator parameter
	sAudioPolyChimeSineLFOPara_t SineLFOPara;//sinelfo parameter
	sAudioPolyChimeMiscPara_t  MiscPara; //misc parameter
	sAudioPolyChimeEnvPara_t   EnvloppePara[ENV_COUNT];//envloppe paramter
}sAudioPolyChimeParameter_t;

/*Audio tone control feature*/
/*bass center freqency define*/
typedef enum
{
	_60_HZ=0,
	_80_HZ,
	_100_HZ,
	_200_HZ
}eAudioBassCentFreq_t;
/*mid  freqency define*/
typedef enum
{
	_500_HZ=0,
	_800_HZ,
	_1000_HZ,
	_1500_HZ
}eAudioMidCentFreq_t;
/*treb freqency define*/
typedef enum
{
	_10000_HZ=0,
	_12500_HZ,
	_15000_HZ,
	_17500_HZ
}eAudioTrebCentFreq_t;

/*Audio PEQ Feature*/
typedef enum{
	BAND1=0,
	BAND2,
	BAND3,
	BAND4,
	BAND5,
	BAND6,
	BAND7,
	PEQ_BAND_MAX,
}ePeqBand;

/*Audio GEQ Feature*/
typedef struct
{
	U16 Fs;
	double Fc;
	double Q;
	double Gain;
}sGeq_ParaDef;

typedef enum
{
	Bar1=0x1A,
	Bar2,
	Bar3,
	Bar4,
	Bar5,
	Bar6,
	Bar7,
	Bar8,
	Bar9,
	Total=0xF0
}eHero_GSABarIDDef;

/*Audio common define*/
#define DOUBLE_PI		3.141592

#define HERO_XDATA 0
#define HERO_YDATA 1
#define HERO_2YDATA	2
//Note that now all the table are calcute on freq 44100Hz
#define TEF663x_ADSP_SAMPLEFREQ SAMP_FREQ_44KHZ    //Hz
#define SAMP_FREQ_44KHZ                 (44100)
#define SAMP_FREQ_48KHZ                 (48000)
///#define ENABLE     1
///#define DISABLE   0
#define   AUDIO_ON        1
#define   AUDIO_OFF       0

#define USER_DEFINE 0  //User define parameter
#define PRESET           1  //Preset parameter

/*Audio CompressExpander function define*/
typedef enum
{
	CompCurve_High=0,
	CompCurve_Mid,
	CompCurve_Low,
	CompCurve_User
}eAudioComp_Curve;

/*Audio Limiter/Clip Detect function mask define*/
#define LimiterFlag_Mask_SCD0      0x01
#define LimiterFlag_Mask_SCD1      0x02
#define LimiterFlag_Mask_SCD2      0x04
#define LimiterFlag_Mask_SCD3      0x08
#define LimiterFlag_Mask_GPIO0    (1<<9)
#define LimiterFlag_Mask_GPIO1    (1<<10)
#define LimiterFlag_Mask_GPIO2   (1<<11)
#define LimiterFlag_Mask_GPIO3    (1<<12)
#define LimiterFlag_Mask_GPIO4    (1<<13)
#define LimiterFlag_Mask_GPIO5    (1<<14)
#define LimiterFlag_Mask_GPIO6    (1<<15)
#define LimiterFlag_Mask_GPIO7    (1<<16)
#define LimiterFlag_Mask_GPIO8    (1<<17)
/*software clip detector define*/
typedef enum
{
	SCD0=0,
	SCD1,
	SCD2,
	SCD3
}eSoftwareClipDetect;

/*Audio Input define*/
typedef enum{
	INPUT_SOURCE_RADIO= 0,	
	INPUT_SOURCE_AIN0=0x08,
	INPUT_SOURCE_AIN1,
	INPUT_SOURCE_AIN2,
	INPUT_SOURCE_AIN3,        
	INPUT_SOURCE_IIS0=0x10,
	INPUT_SOURCE_IIS1,
	INPUT_SOURCE_IIS2,
	INPUT_SOURCE_HOSTIIS0,
	INPUT_SOURCE_HOSTIIS1,
	INPUT_SOURCE_SPDIF,	
	INPUT_SOURCE_NOISE=0x1E,
	INPUT_SOURCE_SINE,
	INPUT_SOURCE_MAX,
}eAudio_InputSource;
/*Audio analog Input mode define*/
typedef enum {
	eINPUT_ANALOG_DIFFERENTIAL1 = 0,
	eINPUT_ANALOG_DIFFERENTIAL2,
	eINPUT_ANALOG_HIGHCRMM2,
	eINPUT_ANALOG_HIGHCRMM1,
	eINPUT_ANALOG_HIGHCRMM0_5,
	eINPUT_ANALOG_IPOD,
	eINPUT_ANALOG_MAX
} eInput_Analog_Mode;



/*---------------audio channel define-------------------------*/
#define TEF663x_PFLCHANNEL 0x0001   //front left channel     
#define TEF663x_PFRCHANNEL 0x0002	//front right channel
#define TEF663x_PRLCHANNEL 0x0004	//rear left channel
#define TEF663x_PRRCHANNEL 0x0008	//rear right channel
#define TEF663x_SWLCHANNEL 0x0010	//subwoofer left channel
#define TEF663x_SWRCHANNEL 0x0020	//subwoofer right channel
#define TEF663x_SLCHANNEL 0x0040	//secondary left channel
#define TEF663x_SRCHANNEL 0x0080	//secondary right channel
#define TEF663x_PCHANNEL 0x0100		 //overall primary channel
#define TEF663x_SCHANNEL 0x0200		//overall secondary channel
#define TEF663x_NCHANNEL 0x0400		//overall navigation channel
#define TEF663x_TCHANNEL 0x0800		//overall phone channel
/*---------------end of audio channel define-------------------------*/

/*Hero GPIO Config number define*/
#define HERO_GPIO_0 0
#define HERO_GPIO_1 1
#define HERO_GPIO_2 2
#define HERO_GPIO_3 3
#define HERO_GPIO_4 4
#define HERO_GPIO_5 5
#define HERO_GPIO_6 6
#define HERO_GPIO_7 7
#define HERO_GPIO_8 8

/*GPIO function ID*/
#define GPIO_FUNID_FASTMUTE 0x110F
#define GPIO_FUNID_DEEMPHASIS 0x110C
#define GPIO_FUNID_HDRADIOBLENDING 0x1107
#define GPIO_FUNID_LIMITER 0x1106
#define GPIO_FUNID_CLICKCLACK 0x1105
#define GPIO_DISCONNCT 0xFFF2
extern  const U16 table_dB2Lin[] ;

/*iic wrape function*/
extern U8 WriteDataToHero(I2CDrv_MemType_t memType, U16 address,U8 dataLength,S32*  pData);
extern U8 WriteScratchDataToHero(I2CDrv_MemType_t memType,U16 address,
                                                U8          dataLength,
                                                U32*  pData);
extern void WriteHero_MEM_1Word(I2CDrv_MemType_t memType, U16 address,S32  Data);
extern void WriteHero_MEM_Tab(const sMemAddr_Data *tab);
extern void TEF663x_EasyProgrammWrite(U32 Data);
extern S8 Hero_TransDouble2XYHexData( U8 Type, double DoubleData,S32 *BufferP);
extern S8 Hero_ReadBack(U8 Addr,U8 Num,U8 *BufferP);

/*audio driver function*/

/*superposition driver*/
extern void Audio_DrvSuperPositionConnect(U16 AddOnIndex, eAudio_Feature_t Feature, eAudioChannel Channel);
extern void Audio_DrvSuperPositionGain(U32 StreamIndex, U32 AddOnIndex, S32 PresetLeft, S32 PresetRight) ;
extern void Audio_DrvConnectExtInputTo(char nChannel);
/*polychime driver*/
extern void Audio_DrvPolyChimeLoadPreset(void);
extern void Audio_DrvPolyChimeSetPara(sAudioPolyChimeParameter_t para);
extern void Audio_DrvPolyChimeStop(void);
extern void Audio_DrvPolyChimeStart(void);
extern void Audio_DrvPolyChimeSetup(char mode);

/*tone control driver*/
extern U8 Audio_DrvBassSet(Audio_Stream_t Stream,S8 Gain,eAudioBassCentFreq_t Freq);
extern U8 Audio_DrvMidSet(Audio_Stream_t Stream,S8 Gain,eAudioMidCentFreq_t Freq);
extern void Audio_DrvBassSetGain(Audio_Stream_t Stream,S8 Gain);
extern U8 Audio_DrvTrebSet(Audio_Stream_t Stream,S8 Gain,eAudioTrebCentFreq_t Freq);

/*fader set*/
extern void Audio_DrvFaderSet(eAudioChannel Channel, U32 Level);
/*PEQ*/
extern U8 Audio_DrvSetPEQScale( eAudioChannel channel, S8 Gain );
extern U8 Audio_DrvPEQSet(eAudioChannel Channel, U32 Band, U32* Coefs);
extern U8 Audio_DrvPEQConfig(U32 CenterOrder, U32 SubWooferOrder);

/*Balance set */
extern void Audio_DrvBalanceSet(Audio_Stream_t Stream, eAudioChannel Channel, U32 Level);

/*dc filter set*/
extern void Audio_DrvDCFilterSet(Audio_Stream_t Stream, U32 OnOff);
/*nav phone filter set*/
extern void Audio_DrvNavPhoneFilterSet(eAudio_Feature_t Feature, U32 Section, U32 * Coefs);

/*expander/compressor*/
extern void Audio_DrvExpanderSet(S8 Threshold);
extern void Audio_DrvCompExpControl(U16 comp_onoff, U16 exp_onoff);
extern void Audio_DrvCompressorSet(eAudioComp_Curve CurveType, U16 Ratio, U16 Gain, U16 Pone);

/*clip/limiter*/
extern void Audio_DrvLimiterFlagGenerSet(eAudioChannel section, S8 clipdetectMask, U32 polarity);
extern void Audio_DrvLimiterSet(eAudioChannel section, S8 gain, U8 onoff);
extern void Audio_DrvLimiterConfigurationSel(U8 config);
extern void Audio_DrvSoftClipDetectSet(U8 section, S8 ThreGain);

U8 Audio_DrvPathConnect(eAudio_Feature_t From, eAudio_Feature_t To);

/*input set*/
//extern void Audio_DrvSetInputAnalogMode(eAudio_InputSource Input,eInput_Analog_Mode mode);
extern void Audio_DrvSetStreamInput(Audio_Stream_t stream,eAudio_InputSource Input);
extern U8 Audio_DrvSetADInMode(U8 ADNum, U8 Mode);
extern U8 Audio_DrvSetI2SInMode(U8 ADNum, U8 Mode);
extern U8 Audio_DrvSetSPDIFInMode(U8 Mode);
/*click clack*/
extern void Audio_DrvClickClack_Locate(eAudio_ClickClackWavTable_t locate);
extern void Audio_DrvClickClack_ModeSel(eAudio_ClickClackControlMode_t mode);
extern void Audio_DrvClickClack_Gen(eAudio_ClickClackControlMode_t Mode,eAudio_ClickClackSound_t Sound );
extern void Audio_DrvDisableClickClack(void);

/*chime PDC*/
extern void Audio_DrvEnableChime(void);
extern void Audio_DrvDisableChime(void);
extern void Audio_DrvStopChime(void);
extern void Audio_DrvSetChimeMasterVol(S8 vol);
extern void Audio_DrvEnablePDC(void);
extern void Audio_DrvDisablePDC(void);

/*sine/noise*/
extern void Audio_DrvSinGenSwitch(U8 OnOff);
extern void Audio_DrvNoiseGenSwitch(U8 OnOff);
extern void Audio_DrvSetNoiseType(U8 Type);
extern S8 Audio_DrvSetSinFreq(U32 Freq);

/*mute*/
extern S8 Audio_DrvSoftMuteSwitch(U16 Channel,U8 OnOff);
extern S8 Audio_DrvFastMuteConfig(U8 Pin,U8 Polarity,double FallTime,double RiseTime);
extern void Audio_DrvSoftMuteConfig(U16 Channel,U16 Ta,U16 Tr);

/*GSA*/
extern S8 Audio_DrvReadGSABarData(eHero_GSABarIDDef BarID,U8 *BufferP);
extern S8 Audio_DrvGSASwitch(U8 OnOff);



/*set volume*/
extern S8 Audio_DrvSetVolume(Audio_Stream_t Channel,double Volume);
/*loudness */
extern S8 Audio_DrvLoudnessSwitch(U8 OnOff);

/*DAC Switch*/
extern S8 Audio_DrvPeripheralSwitch(U8 PeripheralID, U8 OnOff);

/*GEQ*/
extern S8 Audio_DrvCalcuteGeq(U8 Geq_Band,sGeq_ParaDef *pGeqPara);
extern S8 Audio_DrvConfigGeqBand(U8 Band,S32 Geq_DescaleData);
/*source scaling*/
extern S8 Audio_DrvSetSourceScaling(Audio_Stream_t Channel, double ScalingdB);
/*delay*/
extern S8 Audio_DrvSetDelay(U16 Channel, double Time);
#endif 
