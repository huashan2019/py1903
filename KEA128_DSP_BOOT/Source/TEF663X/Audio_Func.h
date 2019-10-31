#ifndef _AUDIO_FUNC_H
#define _AUDIO_FUNC_H


/*volume value max*/
#define SET_VOLUME_PRIMARY_SECONDARY_MAX	TEF663X_MAINVOLUMEMAX
/*volume value min*/
#define SET_VOLUME_PRIMARY_SECONDARY_MIN	HERO_PCHANNEL_MAINVOLUMEMIN


/*Beep define*/
/*    beep volume define ,rang -66~0,this value decide chime output volume, user can change according your need    */
#define BEEP_VOL  -26

/*tone control center freqency define*/
#define BASS_CENTER_FRQ   _100_HZ
#define MID_CENTER_FRQ   _500_HZ
#define TREBLE_CENTER_FRQ   _10000_HZ

/*GEQ Mode define*/
#define HERO_GEQMODE_NONE 0
#define HERO_GEQMODE_CLASS 1
#define HERO_GEQMODE_POP 2
#define HERO_GEQMODE_ROCK 3
#define HERO_GEQMODE_USER 4		//the real eq mode is 5,this is the last mode
#define GEQ_MAXBAND 5

/*fader max level define*/
#define FADER_LEVEL_MAX  30

/*banlance max level define*/
#define BANLENCE_LEVEL_MAX  14

/*pdc mode define*/
#define TEF663x_PDCMODESLOW 0
#define TEF663x_PDCMODEMID 1
#define TEF663x_PDCMODEFAST 2
#define TEF663x_PDCMODEROLL 3
#define TEF663x_PDCMODENULL 0xFF

/*scaling config */


/*primary channel volume define start*/
#define HERO_PCHANNEL_MAINVOLUMEMAX	TEF663X_MAINVOLUMEMAX
#define HERO_PCHANENL_MAINVOLUMESTEPMAX 37
#define HERO_PCHANENL_MAINVOLUMESTEPdB 2 //the max volume should be equal to int * HERO_PCHANENL_MAINVOLUMESTEPdB
#define HERO_PCHANNEL_MAINVOLUMEMIN (TEF663X_MAINVOLUMEMAX-HERO_PCHANENL_MAINVOLUMESTEPdB*HERO_PCHANENL_MAINVOLUMESTEPMAX)

/*chime mode define*/
#define CHIMEMODE_CONFIRMATION 0
#define CHIMEMODE_FASTENSEATBELT 1
#define CHIMEMODE_KEYINIGNITION 2
#define CHIMEMODE_CRUISECONTROL 3
#define CHIMEMODE_FRONTALERT 4
#define CHIMEMODE_USER 5
extern U8 PInputSource_Backup;

/* Volume Level setting */
#define SET_VOLUME_LEV_MAX	(37)
#define SET_VOLUME_LEV_MIN	(0)

static const S8 tblVolLevelTrans[ SET_VOLUME_LEV_MAX + 1 ] = {
	/* Primary/Secondary Volume gain : -62~+12[dB] */
	  -62,-60, -58, -56, -54, -52, -50,-48, //  0 -  7
	  -46, -44, -42, -40, -38, -36, -34, -32, -30,-28, // 8 - 17
	  -26, -24, -22, -20, -18, -16, -14, -12, -10,-8, // 18 - 27
	  -6,  -4,  -2,   0,   2,   4,   6,   8,  10,12 // 28 - 37
};
/*-primary stream volume config end-----*/
//volume set with loudness
/* Loudness as a function of Volume (in dB) */
/* this is for Bass Boost Curve*/
static const S16 table_Loudness_BassB_curve[26] =
{ 
    0x0784, 0x076a, 0x0750, 0x072c, 0x0703, 0x06d6, 0x06a5, 0x066c, /* 0, .. -7 dB */
    0x062d, 0x05e7, 0x0599, 0x0544, 0x04e7, 0x0483, 0x0418, 0x03a6, /* -8, .. -15 dB */
    0x032d, 0x02b2, 0x0234, 0x01b8, 0x0142, 0x00d8, 0x007e, 0x003a, /* -16, .. -23 dB */    
    0x000f, 0x0000                                                  /* -24, -25 dB */
};

/*volume set*/
extern void Audio_SetVolume(Audio_Stream_t Channel,U8 Lev);

/*loudness set*/
extern void Audio_SetLoudness(U8 OnOff);

/*Tone Control Function*/
extern void Audio_SetBass(Audio_Stream_t Stream,S8 Gain);
extern void Audio_SetMid(Audio_Stream_t Stream,S8 Gain);
extern void Audio_SetTreb(Audio_Stream_t Stream,S8 Gain);

/*fader set*/
extern void Audio_SetFader( S8 *Lev);

/*banlance set*/
extern void Audio_SetBalance(Audio_Stream_t Stream,S8 *Lev);

/*GEQ*/
extern S8 Audio_SetEQMode(U8 GeqMode);
extern void Audio_GEQUserConfigInit(void);
extern S8 Audio_SetGEQUserConfig(U8 Band,S8 up);
extern double Audio_GetGeqGain(U8 band);

/*superposition*/
extern void Audio_ImposeExtOnFR( S8 gain);
extern void Audio_ImposeChimeOnFR( S8 gain);
extern void Audio_ImposeNavOnFront(S8 gain);
extern void Audio_ImposePhoneOnRear(S8 gain);

/*PolyChime Function*/
extern void Audio_SetChimeMode(U8 Mode);
extern void Audio_SetUserChime(void);
extern void Audio_SetPDC(U8 PDCNum,U8 PDCMode);

/*input source switch*/
extern void Audio_SetPrimaryInputSource(eAudio_InputSource InputSource);
extern void Audio_SourceInputSel(Audio_Stream_t stream,eAudio_InputSource Input,double scal );

/*Beep function*/
extern void Beep(char type);
extern void BeepInit(void);
#endif

