/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_radio.c  ---   收音
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

Radio_T App_Radio;
_Radio_Info_  radioinfo;
_AS_RADIO_INFO_  AsRadioInfo[MAX_AS_STATION];


const SCH_U16 FM_INIT_FREQ[MAX_REGION][6]=   
{  
	8790,9010,9810,10610,10790, 20,     //samerican 100
	8750,9010,9810,10610,10800, 10,     //Region_LATIN       50k       
	8750,9010,9810,10610,10800,  5,     //Region_EU      200k
	6500,6710,6950, 7160, 7400,  3,     //Region_OIRT       100k
	7600,7820,7900, 8000, 9000, 10,     //Region_JAPAN      100k
	8750,9010,9810,10610,10800, 10      //china       100k
};
const SCH_U16 AM_INIT_FREQ[MAX_REGION][6]=  
{ 
	530,610,1000,1400,1710, 10, //samerican     10k
	530,600,1000,1400,1710, 10, //Region_LATIN  9k
	522,603, 999,1404,1611,  9, //Region_EU     9k
	522,603, 999,1404,1629,  9, //Region_OIRT   9k
	522,603, 999,1404,1629,  9, //Region_JAPAN  9k
	531,612, 999,1404,1629,  9  //china         9k
};
#define FM_Max_Freq     FM_INIT_FREQ[radio_region][4]
#define FM_Min_Freq     FM_INIT_FREQ[radio_region][0]
#define FM_FreqStep     FM_INIT_FREQ[radio_region][5]
#define AM_Max_Freq     AM_INIT_FREQ[radio_region][4]
#define AM_Min_Freq     AM_INIT_FREQ[radio_region][0]
#define AM_FreqStep     AM_INIT_FREQ[radio_region][5]
////==============================================================================================TypeCheck===========
const SCH_U8 TunerChipAddr[MAX_TUNER_TYPE]=
{
	0xC0,
	0xC6
};
const pfunction TunerDllLoader[MAX_TUNER_TYPE]=
{
	SCH_NULL,
	Radio663X_DLL_Loader
};
SCH_BOOL TunerTypeCheck(void)
{
	SCH_U8 i;
	App_Radio.TunerType=MAX_TUNER_TYPE;
	for(i=0;i<MAX_TUNER_TYPE;i++)
	{
		App_Radio.TunerAddr=TunerChipAddr[i];
		if(I2C0_IsFindAddr(App_Radio.TunerAddr))
		{
			Printf("RadioTypeCheck OK --- %d \n",i);
			App_Radio.TunerTypeCheck=TRUE;
			App_Radio.TunerType=(_RadioSysType_t)i;
			break;
		}
	}
	if(App_Radio.TunerType==MAX_TUNER_TYPE)
	{
		Printf("RadioTypeCheck fail \n");
		return FALSE;
	}
	if(TunerDllLoader[App_Radio.TunerType])
		(*TunerDllLoader[App_Radio.TunerType])();
	return TRUE;
}
////==============================================================================================TypeCheck END===========

////==============================================================================================LoadArea===========
void LoadArea(void)
{
	SCH_U8 l_cBand,l_cPrsNo;   
	for(l_cPrsNo = 0; l_cPrsNo < MAX_PRESET_NUM; l_cPrsNo ++)
	{  
		for(l_cBand=BAND_FM1;l_cBand<MAX_BAND_NUM;l_cBand++)
		{
			if(l_cBand<FM_BAND_NUM )
				radio_memfreq[l_cBand][l_cPrsNo].freq=FM_INIT_FREQ[radio_region][l_cPrsNo<=4 ? l_cPrsNo : 0]; 
			else
				radio_memfreq[l_cBand][l_cPrsNo].freq=AM_INIT_FREQ[radio_region][l_cPrsNo<=4 ? l_cPrsNo : 0];
		}
	}
	F_LocalDx =DX;
	for(l_cBand=BAND_FM1;l_cBand < MAX_BAND_NUM ;l_cBand++)
	{
       	radioinfo.freq[l_cBand]=radio_memfreq[l_cBand][0].freq;
       	radioinfo.Preset[l_cBand]=1;
	}
}
////==============================================================================================LoadArea END===========

///============================================================================================================
void Tuner_Init(void)
{
	SCH_U8 loc_dx;
	radioinfo.SeekProcState = Seek_Idle;
	loc_dx = F_LocalDx;
	F_TunerDispIcon=0x00;
	F_LocalDx = loc_dx;
	if(App_Radio.pTunerRegisterInit)
		App_Radio.pTunerRegisterInit(radio_band, GEN_TUNING_MODE_STANDARD);
}
SCH_BOOL Radio_Power_On(void)
{
	if(App_Radio.pTunerPowerOn)
		return App_Radio.pTunerPowerOn();
	return TRUE;
}
void RadioMute(SCH_U8 type)
{
	///if(AudioSource==SOURCE_TUNER)///20170821 修改收音界面按X后再回收音没声音
	{
		if(App_Radio.pMute)
			App_Radio.pMute(type);
		F_TunerMute = type;
	}
}
void Tuner_Get_Smeter_Indicator(void)
{
	SCH_U8 F_TunerSmeter;
	static SCH_U8 F_TunerSmeterBak;
	if(FrontSource!=SOURCE_TUNER
		||radioinfo.SeekProcState!=Seek_Idle)
	{
		F_TunerSmeter=0;
		return;
	}
	if(App_Radio.pGetSmeterIndicator)
		F_TunerSmeter=App_Radio.pGetSmeterIndicator();
	if(F_TunerSmeter!=F_TunerSmeterBak)
	{
		F_TunerSmeterBak=F_TunerSmeter;
		PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_INFO); 
	}
}
void Tuner_Get_Stereo_Indicator(void)
{
	static SCH_U8 F_TunerStereoBak;
	if(radio_band>=FM_BAND_NUM
		||FrontSource!=SOURCE_TUNER
		||radioinfo.SeekProcState!=Seek_Idle)
	{
		F_TunerStereoBak=0;
		F_TunerStereo=0;
		return;
	}
	if(App_Radio.pGetStereoIndicator)
		F_TunerStereo = App_Radio.pGetStereoIndicator();
	if(F_TunerStereo!=F_TunerStereoBak)
	{
		F_TunerStereoBak=F_TunerStereo;		
		PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_INFO);
	}
}
void SetDefSmeter(void)
{
	if(radio_band<FM_BAND_NUM )
	{
		if(F_LocalDx)
			radioinfo.def_Smeter=App_Radio.FmLoc_Level;
		else
			radioinfo.def_Smeter=App_Radio.FmDx_Level;
	}
	else
	{
		if(F_LocalDx)
			radioinfo.def_Smeter=App_Radio.AmLoc_Level;
		else
			radioinfo.def_Smeter=App_Radio.AmDx_Level;
	}
}
void Tuner_SetFrequency(SCH_U16 frequency, SCH_U8 band,SCH_U8  tuning_mode)
{
	SetDefSmeter();
	if(App_Radio.pTunerSetFrequency)
		App_Radio.pTunerSetFrequency(frequency, band,tuning_mode);
}
SCH_U8 QUAL_DATA_AVAILABLE(void)
{
	if(App_Radio.pTuner_QUAL_DATA_AVAILABLE)
		return App_Radio.pTuner_QUAL_DATA_AVAILABLE();
	return TRUE;////
}
SCH_U8 Tuner_CheckGoodQuality(void)
{
	SCH_U8 result=0 ;
	if(App_Radio.pTunerCheckGoodQuality)
		result =App_Radio.pTunerCheckGoodQuality();
	///if((radio_region==Region_CHN )&&(radio_freq==9000||radio_freq==10800||radio_freq==9600))
		///return BAD_STOP_LEVEL;
	return result;		
}
SCH_U8 Tuner_CheckGoodIFC(void)
{
	if(App_Radio.pTunerCheckGoodIFC)
		return App_Radio.pTunerCheckGoodIFC(1);
	return GOOD_IFC;////
}
void RadioBandChange(SCH_U8 radioband)
{
	RadioMute(RadioMuteOn);
	if(App_Radio.pTunerSetFrequency)
		Tuner_SetFrequency(radio_freq, radio_band,GEN_TUNING_MODE_PRESET);
	if(App_Radio.pTuner_BandChange)
		App_Radio.pTuner_BandChange();
}

void Radio_FreqInc(void)
{
	if(radio_band<FM_BAND_NUM)
	{
		radio_freq += FM_FreqStep;
		if (radio_freq > FM_Max_Freq)
			radio_freq = FM_Min_Freq;
	}
	else
	{
		radio_freq += AM_FreqStep;
		if(radio_freq > AM_Max_Freq)
			radio_freq = AM_Min_Freq;
	}
}
void Radio_FreqDec(void)
{
	if(radio_band<FM_BAND_NUM)
	{
		radio_freq -= FM_FreqStep;
		if(radio_freq < FM_Min_Freq)
			radio_freq = FM_Max_Freq;
	}
	else
	{
		radio_freq -= AM_FreqStep;
		if(radio_freq < AM_Min_Freq)
			radio_freq = AM_Max_Freq;
	}
}
void Frequency_Change1Step(void)
{
	if(F_TunerSeekIcon==Seek_DW)
	{
		Radio_FreqDec();
	}
	else
	{
		Radio_FreqInc();
	}
}
/********************************************************************************
**  Function      : Radio_Freq_Sort
**  Author        : wenjunHu
**  Created on    : 20170824
**  Description   : 排序自动存台
**  Return        : void
********************************************************************************/
void Radio_Freq_Sort(SCH_U16 freq,SCH_U8 SMeter)
{
	SCH_U8 index;
	_AS_RADIO_INFO_ *pAsFreq;
	_AS_RADIO_INFO_  BKAsRadioInfo[MAX_AS_STATION]={0};
	pAsFreq = AsRadioInfo;
	sch_memcpy(BKAsRadioInfo,AsRadioInfo,sizeof(_AS_RADIO_INFO_)*radio_AS_Cnt);
	if(SMeter > BKAsRadioInfo[0].SMeter)
	{
		AsRadioInfo[0].SMeter = SMeter;
		AsRadioInfo[0].freq = freq;
		sch_memcpy(&AsRadioInfo[1],BKAsRadioInfo,sizeof(_AS_RADIO_INFO_)*radio_AS_Cnt);
	}
	else
	{
		for(index=0;index<radio_AS_Cnt;index++)
		{
			(*pAsFreq).SMeter = BKAsRadioInfo[index].SMeter;
			(*pAsFreq).freq = BKAsRadioInfo[index].freq;
			if(SMeter<=BKAsRadioInfo[index].SMeter && SMeter>BKAsRadioInfo[index+1].SMeter)
			{
				pAsFreq++;
				(*pAsFreq).SMeter = SMeter;
				(*pAsFreq).freq = freq;
			}
			pAsFreq++;
		}
	}
}
/********************************************************************************
**  Function      : Radio_AS_Smeter_Save
**  Author        : wenjunHu
**  Created on    : 20170824
**  Description   : 保存自动存台
**  Return        : void
********************************************************************************/
void Radio_AS_Smeter_Save(void)
{
	SCH_U8 BandPreset;
	_AS_RADIO_INFO_ *pAsFreq;
	if(radio_band<FM_BAND_NUM )
	{
		BandPreset=FM_BAND_NUM*MAX_PRESET_NUM;
		pAsFreq = radio_memfreq[BAND_FM1];
	}	
	else
	{
		BandPreset=AM_BAND_NUM*MAX_PRESET_NUM;
		pAsFreq = radio_memfreq[BAND_AM1];
	}
	if(radio_AS_Cnt<MAX_AS_STATION)
	{
		Radio_Freq_Sort(radio_freq,radio_Smeter);
		PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_FREQLIST);
		sch_memcpy(pAsFreq,AsRadioInfo,sizeof(_AS_RADIO_INFO_)*GetMin(radio_AS_Cnt+1,BandPreset));
	}
	radio_AS_Cnt++;
}
/********************************************************************************
**  Function      : CalculateCurrentFreqIndex
**  Author        : wenjunHu
**  Created on    : 20170824
**  Description   : 当前频点是否为预设台
**  Return        : 预设台号
********************************************************************************/
SCH_U8 CalculateCurrentFreqIndex(SCH_U16 freq)
{  
    SCH_U8 i;
	if(freq==radio_memfreq[radio_band][radio_curpreset-1].freq)
		return radio_curpreset;
	for(i=0;i<MAX_PRESET_NUM;i++)
	{
		if( radio_memfreq[radio_band][i].freq==freq)
		{
			radio_curpreset=(SCH_U8)(i+1);
			return radio_curpreset;
		}
	}
	radio_curpreset=0;
	return radio_curpreset ;
}
SCH_BOOL IsGoodSigna_In_PresetSCAN(void)
{
    SCH_U8 result;
    result=Tuner_CheckGoodQuality();		
    if (result == ALL_QUAL_Good)      
		return TRUE;
    else if(result==BAD_STOP_LEVEL)   
    {
		return FALSE;
    }
    else if(result==BAD_STOP_USN) 
    {
		if(radioinfo.Usn>4)
			return FALSE;
    }
	return TRUE; 
}
void Seek_Preset(void)
{
	radioinfo.SeekProcState=Seek_Prest;
	radioinfo.SeekWaitTimer=2;
}
void Seek_Start(void)
{
	radioinfo.SeekWaitTimer=3;
	radioinfo.SeekProcState=Seek_Request;
	radio_start_freq = radio_freq;
	RadioMute(RadioMuteOn);
}
void TunerReSortAsMemfreq(void)
{
	if(F_TunerASIcon==0)
		return;
	if(radio_AS_Cnt)
	{
		if(radio_band<=BAND_FM3)
		{
			radio_band=BAND_FM1;
			radioinfo.Preset[BAND_FM1]=1;
			radioinfo.freq[BAND_FM1] = radio_memfreq[BAND_FM1][0].freq;
			radioinfo.Preset[BAND_FM2]=1;
			radioinfo.freq[BAND_FM2] = radio_memfreq[BAND_FM2][0].freq;
			radioinfo.Preset[BAND_FM3]=1;
			radioinfo.freq[BAND_FM3] = radio_memfreq[BAND_FM3][0].freq;
		}
		else
		{
			radio_band=BAND_AM1;
			radioinfo.Preset[BAND_AM1]=1;
			radioinfo.freq[BAND_AM1] = radio_memfreq[BAND_AM1][0].freq;
			radioinfo.Preset[BAND_AM2]=1;
			radioinfo.freq[BAND_AM2] = radio_memfreq[BAND_AM2][0].freq;     
		}
	}
}
void RadioSeekFinish(void)
{
	if(radio_freq==radio_start_freq )
	{
		TunerReSortAsMemfreq();
		F_TunerASIcon = 0;
		F_TunerSeekIcon = 0;
		F_TunerScanIcon = 0;
		radioinfo.SeekProcState=Seek_Prest;
	}       
}
void Seek_Next(void)
{
	radioinfo.SeekProcState = Seek_Request;
	RadioSeekFinish();
}
void Seek_Stop(void)
{
	radioinfo.SeekProcState=Seek_Success_End;
	radioinfo.SeekWaitTimer=2;
}
SCH_BOOL TunerSeekBreak(void) 
{
	if(radioinfo.SeekProcState!=Seek_Idle
		||F_TunerScanIcon==1
		||F_TunerASIcon==1
		||F_TunerSeekIcon!=Seek_NONE
		||F_TunerPSIcon==1)
	{
		return TRUE;
	}
	return FALSE; 
}
void Radio_PresetLoad(SCH_U8 channel)
{
	radio_freq = radio_memfreq[radio_band][channel-1].freq;
	radioinfo.SeekProcState=Seek_Prest ;
}
SCH_U16 Seek_Freq_Check(SCH_U16 freq)
{
   if(radio_band<FM_BAND_NUM)
   {
	   if(freq>FM_Max_Freq)
		   freq=FM_Max_Freq;
	   else if(freq<FM_Min_Freq)
		   freq=FM_Min_Freq;				
	   return (freq-FM_Min_Freq)/FM_FreqStep*FM_FreqStep+FM_Min_Freq;
   }
   else
   {
	   if(freq>AM_Max_Freq)
		   freq=AM_Max_Freq;
	   else if(freq<AM_Min_Freq)
		   freq=AM_Min_Freq;	
	   return (freq-AM_Min_Freq)/AM_FreqStep*AM_FreqStep+AM_Min_Freq;
   }
}

///============================================================================================Radio_SeekPro=====
void Radio_SeekPro(void)
{
	static SCH_U8 nTunerErrCnt=0;
	radioinfo.StereoDispTimer++;
	if(radioinfo.StereoDispTimer >= T480MS_8)
	{
		radioinfo.StereoDispTimer = 0;///500ms
		Tuner_Get_Smeter_Indicator();
		Tuner_Get_Stereo_Indicator();
	}
	if(radioinfo.SeekWaitTimer)
		radioinfo.SeekWaitTimer--;
	switch(radioinfo.SeekProcState)
	{
		case Seek_Idle:
			if(F_TunerScanIcon)
			{
				radio_ScanDelayTimer++;
				if(radio_ScanDelayTimer >= T10S_8) 
				{
					radio_ScanDelayTimer = 0;
					Seek_Next();
				}
			}
			else if(F_TunerPSIcon)
			{
				radio_ScanDelayTimer++;
				if(radio_ScanDelayTimer >= T3S_8) 
				{
					if(IsGoodSigna_In_PresetSCAN()==FALSE) 			 
					{
						radio_ScanDelayTimer=T10S_8;	   
					}
				}
				if(radio_ScanDelayTimer >= T10S_8) 
				{
					radio_ScanDelayTimer = 0;
					if(++radio_curpreset>MAX_PRESET_NUM) 
					{	 
						radio_curpreset=1; 
						F_TunerPSIcon=0;
						PostMessage(TUNER_MODULE, EVT_TUN_PS_LISTEN ,radio_curpreset);
					}
				}
			}
			break;
		case Seek_Prest :
			if(radioinfo.SeekWaitTimer)
				break;
			Tuner_SetFrequency(radio_freq, radio_band,GEN_TUNING_MODE_PRESET);
			Seek_Stop();
			break;
		case Seek_Request:
			if(radioinfo.SeekWaitTimer) 
				break;
			Frequency_Change1Step();
			Tuner_SetFrequency(radio_freq, radio_band,GEN_TUNING_MODE_SEARCH);
			radioinfo.SeekProcState=Seek_Search_Freqency;
		case Seek_Search_Freqency:
			PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_INFO);
			radioinfo.SeekWaitTimer=5;
			radioinfo.SeekProcState =Seek_Wait_Qual_Data_Available;						  
			break;
		case Seek_Wait_Qual_Data_Available:
			if(radioinfo.SeekWaitTimer)  
				break;
			if(QUAL_DATA_AVAILABLE()||(nTunerErrCnt++)==T96MS_8)
			{
				nTunerErrCnt=0;
				radioinfo.SeekProcState = Seek_Read_Quality;
			}
			break;
		case Seek_Read_Quality:
			if(Tuner_CheckGoodQuality() == ALL_QUAL_Good)
			{
				radioinfo.SeekWaitTimer=5;
				radioinfo.SeekProcState = Seek_Wait_IFC_Available;
			}
			else
			{
				radioinfo.SeekWaitTimer=3;
				Seek_Next();
			}
			break;
		case Seek_Wait_IFC_Available: 
			if (Tuner_CheckGoodIFC() == GOOD_IFC)
				radioinfo.SeekProcState =	Seek_Check_Station;
			else
				Seek_Next();
		case Seek_Check_Station :
			if(F_TunerASIcon)
			{
				Radio_AS_Smeter_Save();
				Seek_Next();
			}
			else if(F_TunerScanIcon||F_TunerSeekIcon)
			{
				Seek_Stop();
			}
			else
			{
				Seek_Next();
			}
			break;
		case Seek_Success_End:
			if(radioinfo.SeekWaitTimer)
				break; 
			radioinfo.SeekProcState=Seek_Idle;
			Tuner_SetFrequency(radio_freq, radio_band, GEN_TUNING_MODE_END);
			radio_curpreset=CalculateCurrentFreqIndex(radio_freq);
			PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_INFO);
			PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_FREQLIST);	 
			F_TunerSeekIcon=0;
			RadioMute(RadioMuteOff);
			break;
		default:break;
	}	
}
///============================================================================================Radio_SeekPro END=====

///============================================================================================Radio_MessagePro=====
void Radio_MessagePro(void)
{
	MESSAGE pMsg;
	SCH_U8 ntemp,index;
	if(FALSE==GetMessage(TUNER_MODULE,&pMsg))
		return;	
	ntemp=(SCH_U8)pMsg.prm; 
	switch(pMsg.ID)
	{
		case EVT_TUN_BAND:
			if(TunerSeekBreak())
			{
				PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
				break;
			}
			if(ntemp<MAX_BAND_NUM)
			{
				radio_band=ntemp;
			}
			else if(ntemp==0xFF)
			{
				radio_band++;
				radio_band %= MAX_BAND_NUM ;
			}
			RadioBandChange(radio_band); 
			radioinfo.SeekProcState=Seek_Prest;
			break;
		case EVT_TUN_AREA:
			if(radio_region==ntemp)
				break;
			radio_region=ntemp;
			radio_band=BAND_FM1;
			LoadArea();
			if(App_Radio.pTuner_BandChange)
				App_Radio.pTuner_BandChange();
			radioinfo.SeekProcState=Seek_Prest;
			break;
		case EVT_TUN_SEEK:/******automatically seek one valid station*******/	
			if(TunerSeekBreak())
			{
				PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
				break;
			}  
			if(ntemp==0)///down
			{
				F_TunerSeekIcon = Seek_DW;
			}
			else
			{
				F_TunerSeekIcon = Seek_UP;
			}
			Seek_Start();
			break;
		case EVT_TUN_MANUAL:
			if(TunerSeekBreak())
			{
				PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
				break;
			}  
			if(ntemp==0)
			{
				Radio_FreqDec();
			}
			else
			{
				Radio_FreqInc();
			}
			radioinfo.SeekProcState=Seek_Prest ;
			RadioMute(RadioMuteOn);
			break;
		case EVT_TUN_BANDSCAN:
			if(TunerSeekBreak())
			{
				PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
				break;
			}
			F_TunerPSIcon=1;
			radio_curpreset=1;
			radio_freq = radio_memfreq[radio_band][radio_curpreset-1].freq;
			PostMessage(TUNER_MODULE, EVT_TUN_PS_LISTEN ,radio_curpreset);
			break;
		case EVT_TUN_SCAN:
       		if(TunerSeekBreak())
			{
				PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
			 	break;
			}  
			F_TunerScanIcon=1;
			Seek_Start();
		 	break;			
		case EVT_TUN_AS:
			if(TunerSeekBreak())
			{
				PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
				break;
			}
			radio_AS_Cnt=0;
			F_TunerASIcon=1;
			F_LocalDx =LOC;
			PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_INFO);
			sch_memset(&AsRadioInfo,0x00,sizeof(AsRadioInfo));
			if(radio_band<=BAND_FM3)
			{
				radio_band=BAND_FM1;
				radio_freq=FM_Min_Freq;
				for(index=0;index<MAX_PRESET_NUM;index++)
				{
					radio_memfreq[BAND_FM1][index].SMeter = 0x00;
					radio_memfreq[BAND_FM1][index].freq = FM_Min_Freq;
					radio_memfreq[BAND_FM2][index].SMeter = 0x00;
					radio_memfreq[BAND_FM2][index].freq = FM_Min_Freq;
					radio_memfreq[BAND_FM3][index].SMeter = 0x00;
					radio_memfreq[BAND_FM3][index].freq = FM_Min_Freq;
				}
			}
			else
			{ 
				radio_band=BAND_AM1;
				radio_freq=AM_Min_Freq;
				for(index=0;index<MAX_PRESET_NUM;index++)
				{
					radio_memfreq[BAND_AM1][index].SMeter = 0x00;
					radio_memfreq[BAND_AM1][index].freq = AM_Min_Freq;
					radio_memfreq[BAND_AM2][index].SMeter = 0x00;
					radio_memfreq[BAND_AM2][index].freq = AM_Min_Freq;
				}
			}
			Seek_Start();
			Tuner_SetFrequency(radio_freq, radio_band,GEN_TUNING_MODE_SEARCH);
			PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_INFO);
			break;
		case EVT_TUN_LISTEN:
       		if(TunerSeekBreak())
			{
				PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
				break;
			}
			if(ntemp==0||radio_curpreset==ntemp) 
				break;       	   
			radio_curpreset=ntemp;
			Radio_PresetLoad(ntemp);	                  
			break;
		case EVT_TUN_PLAY_UP_DOWN:
			if(TunerSeekBreak())
			{
				PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
				break;
			}
			if(ntemp==DOWN)
			{
				if(radio_curpreset<=1)
				{
					if(radio_band==BAND_FM3)
						radio_band = BAND_FM2;
					else if(radio_band==BAND_FM2)
						radio_band = BAND_FM1;
					else if(radio_band==BAND_FM1)
						radio_band = BAND_FM3;
					if(radio_band==BAND_AM2)
						radio_band = BAND_AM1;
					else if(radio_band==BAND_AM1)
						radio_band = BAND_AM2;
					radio_curpreset=MAX_PRESET_NUM;
					radio_freq = radio_memfreq[radio_band][radio_curpreset-1].freq;
					PostMessage(TUNER_MODULE,EVT_TUN_BAND,radio_band);
				}
				else
				{
					radio_curpreset--;
					Radio_PresetLoad(radio_curpreset);	
				}
			}
			else
			{
				if(radio_curpreset>=MAX_PRESET_NUM)
				{
					if(radio_band==BAND_FM1)
						radio_band = BAND_FM2;
					else if(radio_band==BAND_FM2)
						radio_band = BAND_FM3;
					else if(radio_band==BAND_FM3)
						radio_band = BAND_FM1;
					if(radio_band==BAND_AM1)
						radio_band = BAND_AM2;
					else if(radio_band==BAND_AM2)
						radio_band = BAND_AM1;
					radio_curpreset=1;
					radio_freq = radio_memfreq[radio_band][radio_curpreset-1].freq;
					PostMessage(TUNER_MODULE,EVT_TUN_BAND,radio_band);
				}
				else
				{
					radio_curpreset++;
					Radio_PresetLoad(radio_curpreset);	
				}
			}	
			break;
		case EVT_TUN_PS_LISTEN:
            radio_curpreset=ntemp;
            Radio_PresetLoad(ntemp);		
			break;
		case EVT_TUN_SAVE:
			if(ntemp<1||ntemp>MAX_PRESET_NUM||ntemp==radio_curpreset)
				break;
			radio_memfreq[radio_band][ntemp-1].freq=radio_freq; 
			PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_FREQLIST);
			break;
		case EVT_TUN_FREQ:
	        if(TunerSeekBreak())
			 	break;
			radio_freq=Seek_Freq_Check(pMsg.prm);
			radioinfo.SeekProcState=Seek_Prest ;
			RadioMute(RadioMuteOn);
		 	break;
		case EVT_TUN_ON:
			RadioStartOn(); 
			break;
		case EVT_TUN_OFF:
			RadioPowerOff();
			break;
		case EVT_MUTE_ON_OFF:
			if(ntemp)
				RadioMute(RadioMuteOn);
			else
				RadioMute(RadioMuteOff);
			break;
		case EVT_MUTE_OFF:
			RadioMute(RadioMuteOff);
			break;
		case EVT_MUTE_ON:
			RadioMute(RadioMuteOn);
			break;
		case EVT_TUN_SEEK_BREAK:
			if(TunerSeekBreak())
			{
				F_TunerScanIcon=0;
				F_TunerASIcon=0;
				F_TunerSeekIcon=Seek_NONE;
				F_TunerPSIcon=0;
				Seek_Stop();
			}
			break;			
		case EVT_TUN_DX_LOC:
			F_LocalDx =ntemp;
			PostMessage(ARM_MODULE,M2A_RADIO_CMD,RADIO_INFO);
			break;
        default:break;
	}
}
///===================================================================================Radio_MessagePro END=====

///=============================================================================================POWER INIT======
void Radio_PowerOnInit(void)
{  
	if(App_Radio.nRadioInitTimer)
	{
		App_Radio.nRadioInitTimer--;
		return;
	}
	switch(App_Radio.RadioPwrState)
	{
		case RADIO_INIT_IDLE:
			break;
		case RADIO_POWER_EN:
			TUNER_POWER_ON();
			TUNER_RESET_RELEASE;
			App_Radio.RadioPwrState = RADIO_RESET_WAIT;
			break;
		case RADIO_RESET_WAIT:
			TUNER_RESET_HOLD;
			App_Radio.nRadioInitTimer=T96MS_8;
			App_Radio.RadioPwrState = RADIO_RESET_RELEASE;
			break;
		case RADIO_RESET_RELEASE:
			TUNER_RESET_RELEASE;
			App_Radio.RadioPwrState = RADIO_TYPE_CHECK;
			break;
		case RADIO_TYPE_CHECK:
			if(TunerTypeCheck())
				App_Radio.RadioPwrState = RADIO_POWER_ON;
			break;
		case RADIO_POWER_ON:
			if(Radio_Power_On()==TRUE)
			{
				App_Radio.nRadioInitTimer=T96MS_8;
				App_Radio.RadioPwrState = RADIO_POWER_INIT;
			}
			break;
		case RADIO_POWER_INIT:
			Tuner_Init();
			App_Radio.RadioPwrState = RADIO_NORMAL;
			break;
		case RADIO_NORMAL:
			break;
		case RADIO_CLOSE:
			if(TunerSeekBreak())
			{
				PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
			}
			else
			{
				PostMessage(TUNER_MODULE,EVT_MUTE_ON,NONE);
				App_Radio.RadioPwrState = RADIO_NORMAL;
			}
			break;
		case RADIO_POWEROFF:
			TUNER_POWER_OFF();
			App_Radio.RadioPwrState=RADIO_INIT_IDLE;
			break;
	}       
}
///===========================================================================================POWER INIT END======

///============================================================================RadioInit=========================
void RadioInit(void)
{
	sch_memset(&App_Radio, 0x00, sizeof(App_Radio));
	sch_memset(&radioinfo, 0x00, sizeof(radioinfo));
	radio_band=BAND_FM1;
	radio_region=RADIO_DEFAULT_REGION;
	LoadArea();
}
///============================================================================RadioInit END=========================

void TASK_Radio_Pro(void)
{
	if(Get_SysPower_Flag==OFF)  
	{
		TurnOff_ANT;
		return;
	}
	Radio_PowerOnInit();
	Radio_MessagePro();
	Radio_SeekPro();
	if(AudioSource==SOURCE_TUNER||FrontSource==SOURCE_TUNER)
	{
		if(SysPower.nPowerState == POWER_NORMAL_RUN)
			TurnOn_ANT;
	}
	else
	{
		TurnOff_ANT;
	}
}


SCH_U16 GetFreqRang(SCH_U8 flag)
{
	if(radio_band<=BAND_FM3)
	{
		return flag ? FM_Max_Freq : FM_Min_Freq;
	}
	else
	{
		return flag ? AM_Max_Freq : AM_Min_Freq;
	}
}






