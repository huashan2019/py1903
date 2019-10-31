#include "include.h"

//GEQ 
U8 Geq_ActiveBand;
U8 Geq_UserModeMaxGainRel;
S32Y Geq_DescaleData;
sGeq_ParaDef Geq_Para[GEQ_MAXBAND];
static double Geq_AcualMaxGain;

U8 PInputSource_Backup=0;








/*-----------------------------------------------------------------------
Function name:		Audio_SetVolume	  
Input:	Channel: seen the define
		Lev:the level of the volume step ,not the real dB value
Output:			
Description:set volume and the loudness 
------------------------------------------------------------------------*/
static U8 Audio_LoudnessOn;
void Audio_SetVolume(Audio_Stream_t Channel,U8 Lev)
{
	S32 ADSP_Y_Loudf_StatLev_Data;
	S8 Vol;
	//if(Channel!=PrimaryStr)  //just primary channel with this project
	//	return;
	if(Lev<=SET_VOLUME_LEV_MIN)//check the volume step range
		Lev=SET_VOLUME_LEV_MIN;
	if(Lev>SET_VOLUME_LEV_MAX)//check the volume step range
		Lev=SET_VOLUME_LEV_MAX;
	Vol=tblVolLevelTrans[Lev];	  //get the correspanded hero data
	if(Vol>SET_VOLUME_PRIMARY_SECONDARY_MAX)//check the volume range
		Vol=SET_VOLUME_PRIMARY_SECONDARY_MAX;
	if(Vol<SET_VOLUME_PRIMARY_SECONDARY_MIN)//check the volume range
		Vol=SET_VOLUME_PRIMARY_SECONDARY_MIN;

	Audio_DrvSetVolume(Channel, Vol);//set volume

	if((Channel==PrimaryStr)&&(Audio_LoudnessOn==AUDIO_ON))	   //loudness on
	{
		//check the volume with the Volume-loudness curve
		if(Vol>0)//loudness will be off
		{
			ADSP_Y_Loudf_StatLev_Data=0x07FF;
		}     
		else if(Vol<-25)//loudness is the max 
		{
			ADSP_Y_Loudf_StatLev_Data=0x0000;
		} 
		else//normal range
		{
			ADSP_Y_Loudf_StatLev_Data=table_Loudness_BassB_curve[0-(S8)Vol];
		}
		WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_Loudf_StatLev,ADSP_Y_Loudf_StatLev_Data);

	}
}

/*beep generator table */
const sMemAddr_Data BeepChimeTab[] = {
	
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_SeqStat_Bk,		0x07FF},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_CrosFStp_Bk,		0x12937},	//fs=TEF663x_ADSP_SAMPLEFREQ, crossfader=10ms,
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArLinStpA_Bk,		0x74198B},	 //fs=TEF663x_ADSP_SAMPLEFREQ, attach time=0.1ms,release time=160ms, Exponential Release
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_ArType_Bk,		0x0001},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArRelCoef_Bk,		0x7FED61},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TA_Bk,			0x033D},		//fs=TEF663x_ADSP_SAMPLEFREQ, TA=300ms
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TB_Bk,			0x0562},		//fs=TEF663x_ADSP_SAMPLEFREQ, TB=500ms
	
	{I2CDRV_X_MEM,	ADSP_X_Tsq_repeate_Bk,		0x0002},		//repeatition=1
	
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ChangeThreshold,	0x4026E7},	//chime change threshold=-6dB
	{I2CDRV_X_MEM,	ADSP_X_Tsq_DecayThreshold,	0x0000},		//chime decay threshold=3000ms,release time=10ms,

	//slot 1  
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Ltone_Bk,		0x076F},		//freq=1689, attn=-13dB, clipboost=10dB, sequencer=150ms, wave=sin
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Htone_Bk,		0x07C4},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Vtone_Bk,		0x0DFE},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_T1_Bk,			0x0114},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Clip_Bk,		0x00B5},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Type_Bk,		0x0000},

	//slot 2
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Ltone_Bk,		0x06AD},		// freq=882, attn=-13dB, clipboost=10dB, sequencer=150ms, wave=sin
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Htone_Bk,		0x07EF},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Vtone_Bk,		0x0CD1},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_T2_Bk,			0x00C1},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Clip_Bk,		0x00E4},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Type_Bk,		0x0000},
	
	{I2CDRV_X_MEM,	ADSP_X_Tsq_addmul_Bk,		0x0000},		//Amount of original=0dB,left=right=-66dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_c02_Bk,			0x0800},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_c1_Bk,			0x0FFF},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_c3_Bk,			0x0FFF},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_Copied,			0x0800},

	//end flag
	{I2CDRV_INVALID,	0,			0},
};	  
/*--------------------------------------------------------------------------
* Function :Beep()								
* Input      :type   0,1                                                                                       
* Output      :null                                                                                        
* FUNCTION    :create beep sound                              
*-------------------------------------------------------------------------*/
void Beep(char type)
{
	if(type==0)
	       Audio_SetChimeMode(CHIMEMODE_CONFIRMATION);
	else {  
	       WriteHero_MEM_Tab(BeepChimeTab);
	}
}

/*--------------------------------------------------------------------------
* Function :BeepInit()								
* Input      :null                                                                                        
* Output      :null                                                                                        
* FUNCTION    :Beep Init                           
*-------------------------------------------------------------------------*/
void BeepInit(void)
{
	/*enable chime*/
	Audio_DrvEnableChime();

	/*impose chime on FR*/
	Audio_ImposeChimeOnFR(0);

	/*set beep volume*/
	Audio_DrvSetChimeMasterVol(BEEP_VOL);
}




/*-----------------------------------------------------------------------
Function name:	Audio_ImposeNavOnFront
Input:			Gain
Output:			
Description:	
              Impose Nav channel to Front,if you want to cancel this Impose,
			  set the input gain as -71 db
------------------------------------------------------------------------*/
void Audio_ImposeNavOnFront(S8 gain)
{
        Audio_DrvSuperPositionGain(1,1,gain,gain);
}
/*-----------------------------------------------------------------------
Function name:	Audio_ImposePhoneOnRear
Input:			Gain
Output:			
Description:	
              Impose Nav channel to rear,if you want to cancel this Impose,
			  set the input gain as -71 db
------------------------------------------------------------------------*/
void Audio_ImposePhoneOnRear(S8 gain)
{
        Audio_DrvSuperPositionGain(2,2,gain,gain);
}
/*-----------------------------------------------------------------------
Function name:	Audio_ImposeChimeOnFR
Input:			Gain
Output:			
Description:	
              Impose Chime channel to FR,if you want to cancel this Impose,
			  set the input gain as -71 db
------------------------------------------------------------------------*/
void Audio_ImposeChimeOnFR( S8 gain)
{
        Audio_DrvSuperPositionGain(1,4,-66,gain);
}
/*-----------------------------------------------------------------------
Function name:	Audio_ImposeExtOnFR
Input:			Gain
Output:			
Description:	
              Impose Ext channel to FR,if you want to cancel this Impose,
			  set the input gain as -71 db
------------------------------------------------------------------------*/
void Audio_ImposeExtOnFR( S8 gain)
{
        Audio_DrvSuperPositionGain(1,3,-66,gain);
}



/*-----------------------------------------------------------------------
Input:	OnOff:0=off,others=on
Output:			
Description:loudness on /off
------------------------------------------------------------------------*/
void Audio_SetLoudness(U8 OnOff)
{
	  if((OnOff!=AUDIO_ON)&&(OnOff!=AUDIO_OFF))
	  	 return;
	  Audio_LoudnessOn=OnOff;
	  Audio_DrvLoudnessSwitch(OnOff);//set the loudness switch
}
static U8 Audio_MaxToneGain=0;
/*----------------------------------------------------------------------------
* Function :Audio_SetBass						
* Input      :Stream  :                   
*              :Gain  :                                     
* Output      :null                                                                                       
* FUNCTION    :set bass                     
-------------------------------------------------------------------------------*/
void Audio_SetBass(Audio_Stream_t Stream,S8 Gain)
{    
      /*check parameters Gain*/
   	if(Gain<(0-TEF663x_TONEGAINMAX))
		Gain = (0-TEF663x_TONEGAINMAX);
	else if(Gain > TEF663x_TONEGAINMAX)
		Gain =  TEF663x_TONEGAINMAX;
	/*Set Treble Fc=100Hz*/		
	//Audio_DrvBassSet(Stream, *pGain,BASS_CENTER_FRQ);
	Audio_DrvBassSetGain(Stream, Gain);
	//update Vol_DesScalBMTP
	if(Gain>Audio_MaxToneGain)
	{
          Audio_MaxToneGain=Gain;
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalBMTP,table_dB2Lin[Audio_MaxToneGain]);
	}
}
/*----------------------------------------------------------------------------
* Function :Audio_SetMid						
* Input      :Stream  :                   
*              :Gain  :                                     
* Output      :null                                                                                       
* FUNCTION    :set mid                    
-------------------------------------------------------------------------------*/
void Audio_SetMid(Audio_Stream_t Stream,S8 Gain)
{      
      /*check parameters Gain*/
      if(Gain<(0-TEF663x_TONEGAINMAX))
		Gain = (0-TEF663x_TONEGAINMAX);
	else if(Gain > TEF663x_TONEGAINMAX)
		Gain =  TEF663x_TONEGAINMAX;
	/*Set Treble Fc=500Hz*/
	Audio_DrvMidSet(Stream, Gain,MID_CENTER_FRQ);
     //update Vol_DesScalBMTP
	if(Gain>Audio_MaxToneGain)
	{
          Audio_MaxToneGain=Gain;
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalBMTP,table_dB2Lin[Audio_MaxToneGain]);
	}
	
}
/*----------------------------------------------------------------------------
* Function :Audio_SetTreb						
* Input      :Stream  :                   
*              :Gain  :                                     
* Output      :null                                                                                       
* FUNCTION    :set t reble                    
-------------------------------------------------------------------------------*/
void Audio_SetTreb(Audio_Stream_t Stream,S8 Gain)
{     
       /*check parameters Gain*/
       if(Gain<(0-TEF663x_TONEGAINMAX))
		Gain = (0-TEF663x_TONEGAINMAX);
	else if(Gain > TEF663x_TONEGAINMAX)
		Gain =  TEF663x_TONEGAINMAX;
	/*Set Treble Fc=10000Hz*/
	Audio_DrvTrebSet(Stream, Gain,TREBLE_CENTER_FRQ);
	//update Vol_DesScalBMTP
	if(Gain>Audio_MaxToneGain)
	{
          Audio_MaxToneGain=Gain;
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalBMTP,table_dB2Lin[Audio_MaxToneGain]);
	}
}

/*注意:

  Fader设置函数 Audio_DrvFaderSet(eAudioChannel Channel, U32 Level)中Level参数范围[0,63]
  该函数将Fader 划分从0到63共64个等级，表格tbl_Fader_Level[]中的数表示要
  设置Fader的Level值,所以他的范围也是在[[0,63],Level值越大，表示衰减越小。
*/

const char tbl_Fader_Level[FADER_LEVEL_MAX+1]=
{
	 63,62,61,60,58,56,54,52,50,48,/*0-9*/
	 45,42,39,36,33,30,28,26,24,22,/*10-19*/
	 18,16,14,12,10, 8, 6, 4,  2,  1, 0/*20-30*/
};
/*----------------------------------------------------------------------------
* Function :Audio_SetFader						
* Input      :Lev                 
*                                          
* Output      :null                                                                                       
* FUNCTION    :set  Fader                    
-------------------------------------------------------------------------------*/
void Audio_SetFader( S8 *Lev)
{
	/*check paremeters level*/
	if(*Lev<(0-FADER_LEVEL_MAX))
		*Lev = (0-FADER_LEVEL_MAX);
	else if(*Lev > FADER_LEVEL_MAX)
		*Lev =  FADER_LEVEL_MAX;

	if((*Lev)>=0)
		Audio_DrvFaderSet(Front,tbl_Fader_Level[*Lev]);
	else
		Audio_DrvFaderSet(Rear,tbl_Fader_Level[-(*Lev)]);
}
/*注意:
  Balance设置函数Audio_DrvBalanceSet(Audio_Stream_t Stream,eAudioChannel Channel, U32 Level)中Level
  参数范围[0,63]   表格tbl_Balance_Level[]中的数表示要
  设置Balance的Level值,所以他的范围也是在[[0,63],Level值越大，表示对相应通道的衰减越小
*/
const char tbl_Balance_Level[BANLENCE_LEVEL_MAX+1]=
{
        63,56,50,42,34,28,22,18,14,10,6,4,2,1,0
};
/*----------------------------------------------------------------------------
* Function :Audio_SetBalance						
* Input      :Lev                 
*                Stream                          
* Output      :null                                                                                       
* FUNCTION    :set  banlance                    
-------------------------------------------------------------------------------*/
void Audio_SetBalance(Audio_Stream_t Stream,S8 *Lev)
{ 
	S8 Level=*Lev;
	/*check parameters*/
	if(Level<(0-BANLENCE_LEVEL_MAX))
		Level = (0-BANLENCE_LEVEL_MAX);
	else if(Level > BANLENCE_LEVEL_MAX)
		Level =  BANLENCE_LEVEL_MAX;
	*Lev=Level;
	if(Level>=0)
		Audio_DrvBalanceSet(PrimaryStr,Right,tbl_Balance_Level[Level]);
	else
		Audio_DrvBalanceSet(PrimaryStr,Left,tbl_Balance_Level[-Level]);   
}




/*-----------------------------------------------------------------------
Input:	Mode:0-4=preset mode,5=user define		
		CHIMEMODE_CONFIRMATION 0
		CHIMEMODE_FASTENSEATBELT 1
		CHIMEMODE_KEYINIGNITION 2
		CHIMEMODE_CRUISECONTROL 3
		CHIMEMODE_FRONTALERT 4
		CHIMEMODE_USER 5
Output:	NULL		
Description: 	set the chime mode 
------------------------------------------------------------------------*/
void Audio_SetChimeMode(U8 Mode)
{
    switch(Mode)
	{
		case CHIMEMODE_CONFIRMATION:
		{
			TEF663x_EasyProgrammWrite(ADSP_EASYP_Chime_PSBle1); //see the para in UM
			break;
		}
		case CHIMEMODE_FASTENSEATBELT:
		{
			TEF663x_EasyProgrammWrite(ADSP_EASYP_Chime_PSFsb); //see the para in UM
			break;
		}
		case CHIMEMODE_KEYINIGNITION:
		{
			TEF663x_EasyProgrammWrite(ADSP_EASYP_Chime_PSKii); //see the para in UM
			break;
		}
		case CHIMEMODE_CRUISECONTROL:
		{
			TEF663x_EasyProgrammWrite(ADSP_EASYP_Chime_PSCci); //see the para in UM
			break;
		}
		case CHIMEMODE_FRONTALERT:
		{
			TEF663x_EasyProgrammWrite(ADSP_EASYP_Chime_PSAlt); //see the para in UM
			break;
		}
		case CHIMEMODE_USER:
		{
			Audio_SetUserChime();	  //set the user define chime mode
			break;
		}
		default:
		{
			break;
		}
	}
}

/*-----------------------------------------------------------------------
		  
Input:			
Output:			
Description: You should calcute all data and then make them to several table, when you want you change the config ,check the table	 
------------------------------------------------------------------------*/
static const sMemAddr_Data UserChimeTab[] = {
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_SeqStat_Bk,		0x07FF},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_ChimeVol_Bk,		0x00CB},		//master volume=-20dB
	{I2CDRV_X_MEM,	ADSP_X_Tsq_CrosFStp_Bk,		0x12937},	//fs=TEF663x_ADSP_SAMPLEFREQ, crossfader=10ms,
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArLinStpA_Bk,		0x6312},	 	//fs=TEF663x_ADSP_SAMPLEFREQ, attach time=30ms,release time=10ms, Exponential Release
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_ArType_Bk,		0x0001},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArRelCoef_Bk,		0x7ED821},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TA_Bk,			0x0004A1},		//fs=TEF663x_ADSP_SAMPLEFREQ, TA=430ms
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TB_Bk,			0x0562},		//fs=TEF663x_ADSP_SAMPLEFREQ, TB=500ms
	
	{I2CDRV_X_MEM,	ADSP_X_Tsq_repeate_Bk,		0x01F5},		//repeatition=500
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ChangeThreshold,	0x4026E7},	//chime change threshold=-6dB
	{I2CDRV_X_MEM,	ADSP_X_Tsq_DecayThreshold,	0x0000},		//chime decay threshold=3000ms,release time=10ms,

	//slot 1  
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Ltone_Bk,		0x0795},		//freq=50, attn=-12dB, clipboost=3dB, sequencer=100ms, wave=sin
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Htone_Bk,		0x07FF},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Vtone_Bk,		0x0DFE},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_T1_Bk,			0x0114},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Clip_Bk,		0x00B5},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Type_Bk,		0x0000},

	//slot 2
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Ltone_Bk,		0x0158},		// freq=200, attn=-8dB, clipboost=5dB, sequencer=70ms, wave=sin
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Htone_Bk,		0x07FF},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Vtone_Bk,		0x0CD1},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_T2_Bk,			0x00C1},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Clip_Bk,		0x00E4},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Type_Bk,		0x0000},

	//slot 3 
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T3b1Ltone_Bk,		0x070A},		//freq=1000, attn=-5dB, clipboost=10dB, sequencer=80ms, wave=triangular
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T3b1Htone_Bk,		0x005C},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T3Vtone_Bk,		0x0B80},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_T3_Bk,			0x00DD},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T3Clip_Bk,		0x0195},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T3Type_Bk,		0x0001},

	//slot 4 
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T4b1Ltone_Bk,		0x0177},		//freq=2800, attn=-4dB, clipboost=10dB, sequencer=150ms, wave=sin	
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T4b1Htone_Bk,		0x075F},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T4Vtone_Bk,		0x0AF4},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_T4_Bk,			0x019D},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T4Clip_Bk,		0x0195},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T4Type_Bk,		0x0000},
	
	{I2CDRV_X_MEM,	ADSP_X_Tsq_addmul_Bk,		0x0000},		//Amount of original=0dB,left=right=-66dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_c02_Bk,			0x0800},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_c1_Bk,			0x0FFF},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_c3_Bk,			0x0FFF},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_Copied,			0x0800},

	//end flag
	{I2CDRV_INVALID,	0,			0},
};
/*-----------------------------------------------------------------------
Input:	NULL
Output:	NULL		
Description: 	set the para the hero to call the user define mode chime,this is a fixed para  mode,
			change the para and calcute them and write with your own function
------------------------------------------------------------------------*/
void Audio_SetUserChime(void)
{
	WriteHero_MEM_Tab(UserChimeTab);
}
/*-----------------------------------------------------------------------
Input:	Num: 0=PDC1,others=PDC2	
		PDCMode:demo PDC mode
			TEF663x_PDCMODESLOW 0
			TEF663x_PDCMODEMID 1
			TEF663x_PDCMODEFAST 2
			TEF663x_PDCMODEROLL 3
Output:			
Description: 	set the pdc mode ,,this is a fixed para  mode,
			change the para and calcute them and write with your own function
------------------------------------------------------------------------*/
static const sMemAddr_Data PDCSlowTab[] = {
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArLinStpA_Bk,	0x05CE14},	 //fs=TEF663x_ADSP_SAMPLEFREQ, attach time=2ms,release time=18ms
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArRelCoef_Bk,	0x00A51F},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TA_Bk,		0x000053},	 //fs=TEF663x_ADSP_SAMPLEFREQ, on time TA=30ms,Off time TB=388ms
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TB_Bk,		0x00042D},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Ltone_Bk,0x0343},		//fs=TEF663x_ADSP_SAMPLEFREQ,Out Freq=1500Hz
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Htone_Bk,0x07D1},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Clip_Bk,	0x0080},		//Clip boost=0dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Ltone_Bk,	0x0B80},		//FL=-5dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Htone_Bk,		0x0BFE},	//FR=-6dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Vtone_Bk,		0x0DFE},//RL=-12dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Clip_Bk,		0x0800},//RR=0dB
	{I2CDRV_X_MEM,	ADSP_X_Tsq_repeate_Bk,		0x001DB9},	//fs=TEF663x_ADSP_SAMPLEFREQ,Fade out=25ms
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Vtone_Bk,		0x0800},		//amplititude=10dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_Copied,			0x07FF},		//update the pdc data

	//end flag
	{I2CDRV_INVALID,	0,			0},
};
static const sMemAddr_Data PDCMiddleTab[] = {
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArLinStpA_Bk,	0x05CE14},	 //fs=TEF663x_ADSP_SAMPLEFREQ, attach time=2ms,release time=18ms
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArRelCoef_Bk,	0x00A51F},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TA_Bk,		0x000053},	 //fs=TEF663x_ADSP_SAMPLEFREQ, on time TA=30ms,Off time TB=388ms
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TB_Bk,		0x000274},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Ltone_Bk,0x0343},		//fs=TEF663x_ADSP_SAMPLEFREQ,Out Freq=1500Hz
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Htone_Bk,0x07D1},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Clip_Bk,	0x0080},		//Clip boost=0dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Ltone_Bk,	0x0800},//FL=-5dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Htone_Bk,		0x0800},	//FR=-6dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Vtone_Bk,		0x0800},//RL=-12dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Clip_Bk,		0x0800},//RR=0dB
	{I2CDRV_X_MEM,	ADSP_X_Tsq_repeate_Bk,		0x001DB9},	//fs=TEF663x_ADSP_SAMPLEFREQ,Fade out=25ms
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Vtone_Bk,		0x0800},		//amplititude=10dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_Copied,			0x07FF},		//update the pdc data

	//end flag
	{I2CDRV_INVALID,	0,			0},
};
static const sMemAddr_Data PDCFastTab[] = {
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArLinStpA_Bk,	0x05CE14},	 //fs=TEF663x_ADSP_SAMPLEFREQ, attach time=2ms,release time=18ms
	{I2CDRV_X_MEM,	ADSP_X_Tsq_ArRelCoef_Bk,	0x00A51F},
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TA_Bk,		0x000053},	 //fs=TEF663x_ADSP_SAMPLEFREQ, on time TA=30ms,Off time TB=388ms
	{I2CDRV_X_MEM,	ADSP_X_Tsq_TB_Bk,		0x0000B9},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Ltone_Bk,0x0343},		//fs=TEF663x_ADSP_SAMPLEFREQ,Out Freq=1500Hz
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1b1Htone_Bk,0x07D1},
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Clip_Bk,	0x0080},		//Clip boost=0dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Ltone_Bk,	0x0800},//FL=-5dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2b1Htone_Bk,		0x0800},	//FR=-6dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Vtone_Bk,		0x0800},//RL=-12dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T2Clip_Bk,		0x0800},//RR=0dB
	{I2CDRV_X_MEM,	ADSP_X_Tsq_repeate_Bk,		0x001DB9},	//fs=TEF663x_ADSP_SAMPLEFREQ,Fade out=25ms
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_T1Vtone_Bk,		0x0800},		//amplititude=10dB
	{I2CDRV_Y_MEM,	ADSP_Y_Tsq_Copied,			0x07FF},		//update the pdc data
	
	//end flag
	{I2CDRV_INVALID,	0,			0},
};

void Audio_SetPDC(U8 PDCNum,U8 PDCMode)
{
	WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_Tsq_SeqStat_Bk,PDCNum? 2 : 1);//PDC channel select

	switch(PDCMode)
	{
		case TEF663x_PDCMODESLOW:
				WriteHero_MEM_Tab(PDCSlowTab);
				break;

		case TEF663x_PDCMODEMID:   
				WriteHero_MEM_Tab(PDCMiddleTab);
				break;
		case TEF663x_PDCMODEFAST:	  
				WriteHero_MEM_Tab(PDCFastTab);
				break;
	}
}




//GEQ_MAXMODE: 0=shut down,  max-1=user define
#define TEF663x_YMEMGEQ0DBGAINDATA 0

const S16Y ClassGeq_Table[GEQ_MAXBAND][7]=
{
	{0x5f6,0x2de, 0x004, 0x5bb, 0xc08, 0x1ac, 0x7f7 }, /* fc =   60, G = 12, fs = SAMP_FREQ_44KHZ */  //Class mode
	{0x000,0x147, 0x028, 0x28e, 0xc50, 0x104, 0x7a9 }, /* fc =  573, G =  0, fs = SAMP_FREQ_44KHZ */
	{0x000,0x2bf, 0x054, 0x57e, 0xca8, 0x0e5, 0x739 }, /* fc = 1260, G =  0, fs = SAMP_FREQ_44KHZ */
	{0x000,0x000, 0x0a9, 0x000, 0xd52, 0x000, 0x62b }, /* fc = 2770, G =  0, fs = SAMP_FREQ_44KHZ */
	{0x306,0x000, 0x190, 0x000, 0xf20, 0x000, 0x163 } /* fc = 9000, G =  8, fs = SAMP_FREQ_44KHZ */
};

const S16Y PopGeq_Table[GEQ_MAXBAND][7]=
{
	{ 0x27a, 0x44a, 0x008, 0x094, 0xc11, 0x520, 0x7ee }, /* fc =  118, G =  7, fs = SAMP_FREQ_44KHZ */   //pop mode
	{ 0xfc8, 0x6c3, 0x02c, 0x586, 0xc59, 0x64c, 0x79f }, /* fc =  573, G = -1, fs = SAMP_FREQ_44KHZ */
	{ 0x000, 0x2bf, 0x054, 0x57e, 0xca8, 0x0e5, 0x739 }, /* fc = 1260, G =  0, fs = SAMP_FREQ_44KHZ */
	{ 0x000, 0x000, 0x0a9, 0x000, 0xd52, 0x000, 0x62b }, /* fc = 2770, G =  0, fs = SAMP_FREQ_44KHZ */
	{ 0x18e, 0x000, 0x190, 0x000, 0xf20, 0x000, 0x163 }	 /* fc = 9000, G =  5, fs = SAMP_FREQ_44KHZ */
};

const S16Y RockGeq_Table[GEQ_MAXBAND][7]=
{
	{0x453,0x2de, 0x004, 0x5bb, 0xc08, 0x1ac, 0x7f7 }, /* fc =   60, G = 10, fs = SAMP_FREQ_44KHZ */  //rock mode
	{0x000,0x147, 0x028, 0x28e, 0xc50, 0x104, 0x7a9 }, /* fc =  573, G =  0, fs = SAMP_FREQ_44KHZ */
	{0x000,0x2bf, 0x054, 0x57e, 0xca8, 0x0e5, 0x739 }, /* fc = 1260, G =  0, fs = SAMP_FREQ_44KHZ */
	{0x085,0x000, 0x136, 0x000, 0xe6c, 0x000, 0x39a }, /* fc = 6100, G =  2, fs = SAMP_FREQ_44KHZ */
	{0x03e,0x000, 0x190, 0x000, 0xf20, 0x000, 0x163 } /* fc = 9000, G =  1, fs = SAMP_FREQ_44KHZ */
};

/*-----------------------------------------------------------------------
Input:	GeqMode: 	HERO_GEQMODE_NONE 0
					HERO_GEQMODE_CLASS 1
					HERO_GEQMODE_POP 2
					HERO_GEQMODE_ROCK 3
					HERO_GEQMODE_USER 4
Output:			
Description: read para from geq table and write to hero	  
------------------------------------------------------------------------*/
S8 Audio_SetEQMode(U8 GeqMode)
{
	U8 Geq_Band;
	S32 Data;
	U32 Buffer[8];									 

	if(GeqMode==HERO_GEQMODE_NONE)//flat mode equal to disable
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_GEq_Gc1,TEF663x_YMEMGEQ0DBGAINDATA);    //454BUL
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_GEq_Gc2,TEF663x_YMEMGEQ0DBGAINDATA);    
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_GEq_Gc3,TEF663x_YMEMGEQ0DBGAINDATA); 
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_GEq_Gc4,TEF663x_YMEMGEQ0DBGAINDATA); 
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_GEq_Gc5,TEF663x_YMEMGEQ0DBGAINDATA); 
	}
	else if(GeqMode<HERO_GEQMODE_USER)//preset mode
	{
		const S16Y (*ptab)[7];

		switch(GeqMode){
			case HERO_GEQMODE_CLASS:
				Data =table_dB2Lin[12]/*0x05f6*/;//the descale data
				ptab = ClassGeq_Table;//get the para table pointer
				break;

			case HERO_GEQMODE_POP:
				Data = /*0x027a*/table_dB2Lin[7];//the descale data
				ptab = PopGeq_Table;//get the para table pointer
				break;

			case HERO_GEQMODE_ROCK:
				Data = /*0x0453*/table_dB2Lin[10];//the descale data
				ptab = RockGeq_Table;//get the para table pointer
				break;
		}

		//band 1 double presicion
		Geq_Band=0;
		Buffer[0]=ptab[Geq_Band][1];		
		Buffer[1]=ptab[Geq_Band][2];		
		Buffer[2]=ptab[Geq_Band][3];		
		Buffer[3]=ptab[Geq_Band][4];		 
		Buffer[4]=ptab[Geq_Band][5];		
		Buffer[5]=ptab[Geq_Band][6];		 
		Buffer[6]=ptab[Geq_Band][0];		
		WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b10L_REL, ADSP_EASYP_UpdateCoeff_Coeff7, Buffer);
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data); 

		//band 2 double presicion
		Geq_Band=1;
		Buffer[0]=ptab[Geq_Band][1];		 
		Buffer[1]=ptab[Geq_Band][2];		 
		Buffer[2]=ptab[Geq_Band][3];		 
		Buffer[3]=ptab[Geq_Band][4];		 
		Buffer[4]=ptab[Geq_Band][5];		 
		Buffer[5]=ptab[Geq_Band][6];		 
		Buffer[6]=ptab[Geq_Band][0];		 
		WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b20L_REL, ADSP_EASYP_UpdateCoeff_Coeff7, Buffer);
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data); 

		//band 3 double presicion
		Geq_Band=2;			
		Buffer[0]=ptab[Geq_Band][1];		 
		Buffer[1]=ptab[Geq_Band][2];		 
		Buffer[2]=ptab[Geq_Band][3];		 
		Buffer[3]=ptab[Geq_Band][4];		 
		Buffer[4]=ptab[Geq_Band][5];		 
		Buffer[5]=ptab[Geq_Band][6];		 
		Buffer[6]=ptab[Geq_Band][0];		 
		WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b30L_REL, ADSP_EASYP_UpdateCoeff_Coeff7, Buffer);
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data); 

		//band 4 single presicion
		Geq_Band=3;
		Buffer[0]=ptab[Geq_Band][2];		 		 
		Buffer[1]=ptab[Geq_Band][4];		 		 
		Buffer[2]=ptab[Geq_Band][6];				 
		Buffer[3]=ptab[Geq_Band][0];		 
		WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b40_REL, ADSP_EASYP_UpdateCoeff_Coeff4, Buffer);
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data); 

		//band 5 single presicion
		Geq_Band=4;
		Buffer[0]=ptab[Geq_Band][2];		 		 
		Buffer[1]=ptab[Geq_Band][4];		 		 
		Buffer[2]=ptab[Geq_Band][6];				 
		Buffer[3]=ptab[Geq_Band][0];		 
		WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b50_REL, ADSP_EASYP_UpdateCoeff_Coeff4, Buffer);			
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data); 
	    }
	else if(GeqMode==HERO_GEQMODE_USER)	  //user define mode
	{
		for(Geq_Band=0;Geq_Band<GEQ_MAXBAND;Geq_Band++)
		{
			Audio_DrvConfigGeqBand(Geq_Band,Geq_DescaleData);
		}
	}
	else
	{
		return PARA_ERROR;
	}

	return SUCCESS;
}



/*-----------------------------------------------------------------------
		  
Input:	up>0 is up, <0 = down
Output:			
Description: server of the user define geq, change the GEQ gain value
------------------------------------------------------------------------*/
S8 Audio_SetGEQUserConfig(U8 Band,S8 up)
{
	U8 i;

	if((Band>GEQ_MAXBAND)||(Band==HERO_GEQMODE_NONE))
	return PARA_ERROR;

	Geq_ActiveBand=Band-1;

	if(up >0)
	{
		Geq_Para[Geq_ActiveBand].Gain++;                          // 1 step=1 dB
		if(Geq_Para[Geq_ActiveBand].Gain>TEF663x_GEQGAINMAX)//check the range of geq gain
		{
			Geq_Para[Geq_ActiveBand].Gain=TEF663x_GEQGAINMAX;
		}

		if(Geq_Para[Geq_ActiveBand].Gain>Geq_AcualMaxGain)//check the max gain of the 5 bands
		{
			Geq_AcualMaxGain=Geq_Para[Geq_ActiveBand].Gain;
			Geq_UserModeMaxGainRel=Geq_ActiveBand;
			Geq_DescaleData=table_dB2Lin[(S8)Geq_Para[Geq_UserModeMaxGainRel].Gain];			
		}
		Audio_DrvCalcuteGeq(Geq_ActiveBand,&Geq_Para[Geq_ActiveBand]);//translate the geq para to hero data
		Audio_DrvConfigGeqBand(Geq_ActiveBand,Geq_DescaleData);	//config one band of geq	
	}
	else if(up < 0 )
	{
		Geq_Para[Geq_ActiveBand].Gain--;                          // 1 step=1 dB
		if(Geq_Para[Geq_ActiveBand].Gain<(0.0-TEF663x_GEQGAINMAX))//check the gain range
		{
			Geq_Para[Geq_ActiveBand].Gain=(0.0-TEF663x_GEQGAINMAX);
		}

		if(Geq_UserModeMaxGainRel==Geq_ActiveBand)//check the max gain of 5 bands
		{
			for(i=0;i<GEQ_MAXBAND;i++)
			{
				if(Geq_Para[i].Gain>Geq_AcualMaxGain)
				{
					Geq_AcualMaxGain=Geq_Para[i].Gain;//get the max gain of 5 bands
					Geq_UserModeMaxGainRel=i;
				}
			}
			if(Geq_AcualMaxGain>0)//get the descale data
			{
				Geq_DescaleData=table_dB2Lin[(S8)Geq_AcualMaxGain];
			}
			else//all bands` gain are less than 0
			{
				Geq_DescaleData=0x07FF;
			}							
		}
		Audio_DrvCalcuteGeq(Geq_ActiveBand,&Geq_Para[i]);//translate the geq para to hero data
		Audio_DrvConfigGeqBand(Geq_ActiveBand,Geq_DescaleData);	//config one band of geq		
	}

	return SUCCESS;
}


/*-----------------------------------------------------------------------
		  
Input:	
Output:			
Description: init the user define geq data
------------------------------------------------------------------------*/
void Audio_GEQUserConfigInit(void)
{
	U8 i;

	Geq_ActiveBand=0;
	Geq_Para[0].Fc=50;//set the default band freq
	Geq_Para[1].Fc=200;
	Geq_Para[2].Fc=800;
	Geq_Para[3].Fc=3200;
	Geq_Para[4].Fc=12800;
	Geq_AcualMaxGain=0;
	Geq_DescaleData=0x07FF;
	for(i=0;i<GEQ_MAXBAND;i++)
	{
		Geq_Para[i].Fs=TEF663x_ADSP_SAMPLEFREQ;
		Geq_Para[i].Q=1.0;
		Geq_Para[i].Gain=0.0;
		Audio_DrvCalcuteGeq(i,&Geq_Para[i]);//thrans the default data to hero data format
	}

}

/*-----------------------------------------------------------------------
Input:	band: geq band 
Output:	correspond geq band gain		
Description: 	get geq gain
------------------------------------------------------------------------*/
double Audio_GetGeqGain(U8 band)
{
	return Geq_Para[band].Gain;
}

/*-----------------------------------------------------------------------
Function name:	Audio_SourceInputSel
Input:			stream
                                      PrimaryStr,
                                      SecondaryStr ,
                                      NavigationStr,
                                      PhoneStr      
                                 Input
                                       input select
                             
                                 scal :source scaling [-60,6.02db]
                                     
Output:			null
Description:	
------------------------------------------------------------------------*/
void Audio_SourceInputSel(Audio_Stream_t stream,eAudio_InputSource Input,double scal )
{
	Audio_DrvSetStreamInput( stream,Input);
	Audio_DrvSetSourceScaling(stream, scal);
}
/*-----------------------------------------------------------------------
Function name:	Audio_SetPrimaryInputSource
Input:			
Output:			
Description:	 
------------------------------------------------------------------------*/
void Audio_SetPrimaryInputSource(eAudio_InputSource InputSource)
{
     double scal;
	if(PInputSource_Backup==InputSource) 
		return ;
	PInputSource_Backup=InputSource;
	scal=-3;
     if(InputSource==INPUT_SOURCE_RADIO)
	 	 scal=6;
	Audio_SourceInputSel(PrimaryStr,InputSource,scal);
}
