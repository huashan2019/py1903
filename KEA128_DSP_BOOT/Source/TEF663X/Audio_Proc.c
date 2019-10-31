/***********************************************************************//**
 * @file		audio.c
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
 #include "include.h"
 

//  variable define   

static U32 AudioProcessTimer;
 /*Audio mode*/
AUDIO_MODE AudioMode;
/*mute flag*/
U8 SoftMuteState;		//TRUE  = mute,
/*EQ mode*/
U8 EQMode=0;
/*volume level*/
S8 MainVolume_Lev;
/*loudness on flag*/
U8 Hero_LoudnessOn=0;
/*tone gain*/
S8 BassGain=0;
S8 MidGain=0;
S8 TrebleGain=0; 

/*banlance level*/
S8 gBanlanceLevel=0;
/*fader level*/
S8 gFaderLevel=0;
/*key beep enable flag*/
char F_BeepEnable=AUDIO_ON;
/*mic mix enable flag*/
char F_MicEnable=DISABLE;

/*Fader enable flag*/
char F_FadeEnable=DISABLE;

/*GSA Read buffer*/
U8 Hero_GSABuffer[9];

//function declaration
void Audio_Single_Set(unsigned char TempWorkMode);
void Audio_Channel_Gain_Select(UCHAR CurrWorkMode);
void Audio_Data_Amend_Process(void);
void Audio_Disp_Control_Process(void);
///void Audio_Key_Timeout(DISPLAY_TIMEOUT_TYPE type,U16 time);
void VolumeChangeProc(void);
void BassChangeProc(void);
void MidChangeProc(void);
void TrebleChangeProc(void);
void BanlanceChangeProc(void);
void FaderChangeProc(void);
void BeepChangeProc(void);
void MixMicChangeProc(void);
void LoudnessSwitch(U8 OnOff);
void LoudnessChangeProc(void);
void GeqChangeProc(U8 Band);
void EQModeChangeProc(void);
void FadeSetProc(void);


/*------------------------------------------------------------------------
 Function:Audio_Set(..)
 Input: NULL
 OutPut:NULL
 Description:
          system mode audio set
--------------------------------------------------------------------------*/
 void Audio_Set(void)
{	
	///Audio_Single_Set(Curr_Work_Mode);
}
/*------------------------------------------------------------------------
 Function:Audio_Set(..)
 Input: NULL
 OutPut:NULL
 Description:
          system mode audio set
--------------------------------------------------------------------------*/
void Audio_Single_Set(unsigned char TempWorkMode)
{
   	Audio_Channel_Gain_Select(TempWorkMode);
}

void Audio_Channel_Gain_Select(UCHAR CurrWorkMode)
{
	
}

/*-------------------------------------------------------------------------
 Function:Audio_Proc
 Input:      NULL
 OutPut:   NULL
 Description:
                Audio setup process
---------------------------------------------------------------------------*/
void Audio_Proc(void)
{
        /*check system status and work mode*/
	///if((F_PowerOff)||(F_Hero_InitOK==0)||(Curr_Work_Mode==GUI)||(Curr_Work_Mode==Align))
		///return;
	/* process*/
	Audio_Data_Amend_Process();

}
/*-------------------------------------------------------------------------
 Function:Beep_Proc
 Input:      NULL
 OutPut:   NULL
 Description:
              beep when key pressed for the first time
----------------------------------------------------------------------------*/
void Beep_Proc(void)
{
	
}
void Audio_Mode_Change(void)
{
	if(!TimerHasExpired(&AudioProcessTimer))//timer is running
	{
		AudioMode++;
		if((EQMode!=HERO_GEQMODE_USER)&&(AudioMode>=AudioMode_UserEQBand1)
		&&(AudioMode<=AudioMode_UserEQBand5))
		{
			AudioMode = AudioMode_Bass;
		}
		if(AudioMode >= AUDIO_MODE_LAST)
		       AudioMode = AudioMode_Bass;
	}

	if((EQMode!=HERO_GEQMODE_USER)&&(AudioMode>=AudioMode_UserEQBand1)
	&&(AudioMode<=AudioMode_UserEQBand5))
	{
	      AudioMode = AudioMode_Bass;
	}

	if(AudioMode < AudioMode_Bass)
	      AudioMode = AudioMode_Bass;

	TimerSet(&AudioProcessTimer,AUDIO_PROCESS_TIMER);
	///AudioModeDisplay(AUDIO_PROCESS_TIMER);
}
/*------------------------------------------------------------------------
 Function:Audio_Mute_Key(..)
 Input: NULL
 OutPut: NULL
 Description:mute key proccess
-------------------------------------------------------------------------*/
void Audio_Mute_Key(void)
{
	
}
/*------------------------------------------------------------------------
 Function:Audio_Key_Process(..)
 Input: NULL
 OutPut: NULL
 Description:audio setup key proccess
-------------------------------------------------------------------------*/
void Audio_Key_Process(void)
{
	
}

/*-----------------------------------------------------------------------
Function name:	Audio_Data_CMD		  
Input:	NULL
Output:	NULL		
Description:Do audio setup proccess
------------------------------------------------------------------------*/
void Audio_Data_CMD(void)
{
	AUDIO_MODE mode;

	mode = TimerHasExpired(&AudioProcessTimer)? AudioMode_Volume : AudioMode;

	switch(mode)
	{
	case AudioMode_Idel:
	case AudioMode_Volume:
	default:
		VolumeChangeProc();
		break;
	case AudioMode_Bass:
		BassChangeProc();
		break;
	case AudioMode_Mid:
		MidChangeProc();
		break;
	case AudioMode_Treble:
		TrebleChangeProc();
		break;
	case AudioMode_Balance:
		BanlanceChangeProc();
		break;
	case AudioMode_Fader:
		FaderChangeProc();
		break;
	case AudioMode_Beep:
		BeepChangeProc();
		break;
	case AudioMode_MixMic:
		MixMicChangeProc();
		break;
	case AudioMode_Fade:
		FadeSetProc();
		break;
	case AudioMode_UserEQBand1:
	case AudioMode_UserEQBand2:
	case AudioMode_UserEQBand3:
	case AudioMode_UserEQBand4:
	case AudioMode_UserEQBand5:
		GeqChangeProc(AudioMode - (AudioMode_UserEQBand1-1));
		break;
	case AudioMode_Loudness:
		LoudnessChangeProc();
		break;			
	}
}

void Audio_Data_Amend_Process(void)
{	
	/*to judge Audio in which work mode according to key value*/
	Audio_Key_Process();

	/*Change Audio Ic data <bass,Mid,treble,balance,fader,volume>*/		
	Audio_Data_CMD();
}

U16 Hero_ActiveVolumeChannel=TEF663x_PCHANNEL;

#define HERO_PCHANNEL_VOLUME_LEV 20
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: init the volume data	 
------------------------------------------------------------------------*/
void VolumeProcInit(void)
{
	MainVolume_Lev= HERO_PCHANNEL_VOLUME_LEV;
	Hero_LoudnessOn=0; 

	Audio_SetVolume(PrimaryStr,MainVolume_Lev); 

	WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Vol_Boost12dB,0);
}

/*-----------------------------------------------------------------------
Input:	
Output:			
Description: 	 check if volume need to be changed and set the new volume to hero
------------------------------------------------------------------------*/
void VolumeChangeProc(void)
{
	S8 WheelKey;///=GetWheelKey();   //get the wheel key value
		
	switch(Hero_ActiveVolumeChannel)//the wheel just set the current active channel volume
	{
		case TEF663x_PCHANNEL:
		{
			if(WheelKey)//wheel active
			{
				MainVolume_Lev+=WheelKey;//volume change

				if(MainVolume_Lev<=SET_VOLUME_LEV_MIN)//check the volume range
				{
					MainVolume_Lev=SET_VOLUME_LEV_MIN;//min volume= mute

					if(SoftMuteState==FALSE)//check if it is muteed now
					{
						///MuteOn();
						SoftMuteState=TRUE;
					}
				}
				else if(MainVolume_Lev>SET_VOLUME_LEV_MAX)//check the volume range
				{
					MainVolume_Lev=SET_VOLUME_LEV_MAX;
						
					if(SoftMuteState==TRUE)////it was muted before
					{
						///MuteOff();
						SoftMuteState=FALSE;
					}
				}
				else//not exceed the volume range
				{
					if(SoftMuteState==TRUE)//it was muted before
					{
						///MuteOff();
						SoftMuteState=FALSE;
					}
				}

				//write the new volume data to hero
				Audio_SetVolume(PrimaryStr, MainVolume_Lev);  //just primary channel with this project
				///Display_SetTimeout(DISPLAY_TIMEOUT_VOL,1000);//volume display for 1s
			}
			break;
		}
	}   
}

/*-----------------------------------------------------------------------
Function name:	void BassChangeProc(void)
Input:			NULL
Output:			NULL
Description:	
                Bass set process function
------------------------------------------------------------------------*/

void BassChangeProc(void)
{
	
}

/*-----------------------------------------------------------------------
Function name:	void TrebleChangeProc(void)
Input:			NULL
Output:			NULL
Description:	
               Treble set process function
------------------------------------------------------------------------*/
void TrebleChangeProc(void)
{
	
}
/*-----------------------------------------------------------------------
Function name:	void MidChangeProc(void)
Input:			NULL
Output:			NULL
Description:	
               Mid set process function
------------------------------------------------------------------------*/

void MidChangeProc(void)
{
	
}


/*-----------------------------------------------------------------------
Function name:	void BanlanceChangeProc(void)
Input:		NULL                                     
Output:	        NULL
Description:	
                    change banlance key process
------------------------------------------------------------------------*/
void BanlanceChangeProc(void)
{
	
}

/*-----------------------------------------------------------------------
Function name:	void FaderChangeProc(void)
Input:		NULL                                     
Output:	        NULL
Description:	
                    change fader key process
------------------------------------------------------------------------*/
void FaderChangeProc(void)
{
	
}



/*--------------------------------------------------------------------------
* Function :BeepChangeProc()								
* Input      :null                                                                                        
* Output      :null                                                                                        
* FUNCTION    :Beep setup key process                      
*--------------------------------------------------------------------------*/

void BeepChangeProc(void)
{
	
}
/*--------------------------------------------------------------------------
* Function :MixMicChangeProc()								
* Input      :null                                                                                        
* Output      :null                                                                                        
* FUNCTION    :mix mic function key process                      
*--------------------------------------------------------------------------*/

void MixMicChangeProc(void)
{
	
}
/*--------------------------------------------------------------------------
* Function :FadeSetProc()								
* Input      :null                                                                                        
* Output      :null                                                                                        
* FUNCTION    :                     
*--------------------------------------------------------------------------*/

void FadeSetProc(void)
{
	
}
/*-----------------------------------------------------------------------
Input:	OnOff: 0=off, others=on
Output:			
Description: 	check the loudness state and set 
------------------------------------------------------------------------*/
void LoudnessSwitch(U8 OnOff)
{
	if(Hero_LoudnessOn==OnOff)
	{
		return;
	}

	Hero_LoudnessOn=OnOff;	  
	Audio_SetLoudness(OnOff);
	//need reset volume
	Audio_SetVolume(PrimaryStr, MainVolume_Lev);
}
/*--------------------------------------------------------------------------
* Function :LoudnessChangeProc()								
* Input      :null                                                                                        
* Output      :null                                                                                        
* FUNCTION    :loudness setup key process                      
*--------------------------------------------------------------------------*/
void LoudnessChangeProc(void)
{       
	
}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: 	init the data of the geq user mode 
------------------------------------------------------------------------*/
void Audio_GEQUserModeInit(void)
{
	if(EQMode!=HERO_GEQMODE_USER)
	{
		EQMode=HERO_GEQMODE_USER;
		Audio_SetEQMode(EQMode);
	}
}


/*--------------------------------------------------------------------------
* Function :GeqChangeProc()								
* Input      :null                                                                                        
* Output      :null                                                                                        
* FUNCTION    :GEQ Setup key process                      
*--------------------------------------------------------------------------*/
void GeqChangeProc(U8 Band)
{
	
}


/*-----------------------------------------------------------------------
Input:	
Output:			
Description: change the GEQ mode	  
------------------------------------------------------------------------*/
void EQModeChangeProc(void)
{
	EQMode++;
	if(EQMode>HERO_GEQMODE_USER)//check the geq mode range
	{
		EQMode=0;
	}
	Audio_SetEQMode(EQMode);//set GEQ mode
}

/*
void Audio_TestForHSAE(void)
{  
    static  eAudio_InputSource Source=INPUT_SOURCE_RADIO;
   if(Source==INPUT_SOURCE_RADIO)
	{
	   Source=INPUT_SOURCE_SINE;   
	}else
	{
	   Source=INPUT_SOURCE_RADIO;   
	}
  // MuteOn();   
   Audio_SetPrimaryInputSource(Source);
  
   Audio_SetVolume(PrimaryStr, MainVolume_Lev);
   Audio_SetBalance(PrimaryStr,&gBanlanceLevel);
   Audio_SetFader(&gFaderLevel); 	  
   Audio_SetBass(PrimaryStr, &BassGain);
   Audio_SetMid(PrimaryStr, &MidGain);
   Audio_SetTreb(PrimaryStr, &TrebleGain);   
    Audio_SetTreb(PrimaryStr, &TrebleGain);    
	Audio_SetTreb(PrimaryStr, &TrebleGain);   
	 Audio_SetTreb(PrimaryStr, &TrebleGain);   
	  Audio_SetTreb(PrimaryStr, &TrebleGain);   
	   Audio_SetTreb(PrimaryStr, &TrebleGain);   
	    Audio_SetTreb(PrimaryStr, &TrebleGain);   
	    Audio_SetTreb(PrimaryStr, &TrebleGain);   
		 Audio_SetTreb(PrimaryStr, &TrebleGain);   

   //Audio_SetLoudness(Hero_LoudnessOn);
  // MuteOff();
}	 */
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: 	gsa proc  
------------------------------------------------------------------------*/
void Audio_GSAProc(void)
{
	
}



