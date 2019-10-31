/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_radio_tef663X.c  ---  
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
RadioStruct Radio;

SCH_U8 Tuner_SendString(SCH_U8 *buff,unsigned char length)
{
	I2C0_Tx(App_Radio.TunerAddr,buff,length);
	return	SUCCESS;
}
void Tuner_ReadString(unsigned char *buff,unsigned char length)
{
	I2C0_Rx(App_Radio.TunerAddr,buff,length);
}



void NXP663X_ReadStatus(void)
{
	Tuner_ReadString(Radio.ReadBuff, 7);
	radioinfo.SMeter=Radio.ReadBuff[Radio_LEVEL];
	radioinfo.Usn=Radio.ReadBuff[Radio_USN];
	radioinfo.Wam=Radio.ReadBuff[Radio_WAM];
}

SCH_U8 NXP663X_Get_Smeter_Indicator(void)
{
		NXP663X_ReadStatus();
		return	radioinfo.SMeter;
}
typedef enum
{
	TEF665X_MODULE_FM = 0,
	TEF665X_MODULE_LM = 1,
	TEF665X_MODULE_MW = 2,
	TEF665X_MODULE_SW = 3
} TEF663x_MODULE;
void  NXP663X_TuneFreq(SCH_U16 frequency, SCH_U8 band,SCH_U8  tuning_mode)
{
	SCH_U8 tuner_buffer[4], band_value;
	switch (band)
	{
		case BAND_FM1:
		case BAND_FM2:
		case BAND_FM3:
			band_value=TEF665X_MODULE_FM;
			break;
		case BAND_AM1:
		case BAND_AM2:
			band_value = TEF665X_MODULE_MW;
			break;
		default:
			break;
	}
	tuner_buffer[0] = 0x00;
	tuner_buffer[1] = band_value+tuning_mode;
	tuner_buffer[2] = (u8)((frequency&0xFF00)>>8);
	tuner_buffer[3] = (u8)(frequency&0x00FF);
	Tuner_SendString(tuner_buffer, 4);
}
void NXP663X_Get_Stereo_Indicator(void)
{
	NXP663X_ReadStatus();
	if (NXP663X_Stereo_Indicator(Radio.ReadBuff[0]) && (Radio.ReadBuff[1]>=0x28))
		F_TunerStereo=1;	
	else
		F_TunerStereo=0;	
}
SCH_U8 NXP663X_ReadIFCounter(void)
{
      NXP663X_ReadStatus();

	return Radio.ReadBuff[Radio_FOF];
}
SCH_U8 NXP663X_CheckGoodIFC(SCH_U8 measure_time)
{
	SCH_U8 result, IFC_Register, IFC_Avaiable_time, IFC_Value,IFC_CN;
	IFC_Register = NXP663X_ReadIFCounter();
	IFC_Value = (SCH_U8)(NXP663X_IFC_Counter_Value(IFC_Register));
	if (radio_band < BAND_AM1)
	{
		if (IFC_Value <= NXP663X_FM_STOP_IFCResult)
			result = GOOD_IFC;
		else
			result = BAD_IFC;
	}
	else
	{
		if (IFC_Value <= NXP663X_AM_STOP_IFCResult)
			result = GOOD_IFC;
		else
			result = BAD_IFC;
	}
	return result;
}
SCH_U8 NXP663X_CheckGoodQuality(void)
{		
	SCH_U8 level,usn,wam,Offset;
	SCH_U8	radio_mod;
	SCH_U8 result;
   NXP663X_ReadStatus();
   level=Radio.ReadBuff[Radio_LEVEL];
   usn=Radio.ReadBuff[Radio_USN];
   wam=Radio.ReadBuff[Radio_WAM];
   radio_mod=Radio.ReadBuff[Radio_MOD];
   Offset=Radio.ReadBuff[Radio_FOF];
   Offset&=~0x80;
   if(radio_mod==0x00)
   	return NO_STATION;  
	if (radio_band<FM_BAND_NUM )
	{	

		if (level <radioinfo.def_Smeter)
			result = BAD_STOP_LEVEL;	 
		else if (usn > NXP663X_FM_STOP_USN)
			result = BAD_STOP_USN;
		else if (wam > NXP663X_FM_STOP_WAM)
			result = BAD_STOP_WAM;
		else 
			result = ALL_QUAL_Good;
	}
	else
	{
		if (level < radioinfo.def_Smeter)
			result = BAD_STOP_LEVEL;
		else 
			result = ALL_QUAL_Good;
	}
	return result;  
}

u8 NXP663X_QUAL_DATA_AVAILABLE(void)
{
	u8 TempData,threshold;
	NXP663X_ReadStatus();
	TempData=Radio.ReadBuff[Radio_STATUS]&0xE0;
	if(radio_band<BAND_AM1)//FM
		threshold=Radio_FM_Status;//0x20;
	else
		threshold=Radio_AM_Status;//0XA0
	return	(TempData>=threshold);
}
void NXP663X_Tuner_BandChange(void)
{
	Radio_Para_WriteAll();	
}

void Radio663X_DLL_Loader(void)
{
	Printf("RadioType --- NXP663X \n");
	App_Radio.pMute=NULL;///AudioMute;
	App_Radio.pTunerPowerOn=NULL;///NXP663X_PowerOn;
	App_Radio.pGetSmeterIndicator=NXP663X_Get_Smeter_Indicator;
	App_Radio.pTunerSetFrequency=NXP663X_TuneFreq;
	App_Radio.pTunerReadStatus=NULL;
	App_Radio.pGetStereoIndicator=NXP663X_Get_Stereo_Indicator;
	App_Radio.pTunerCheckGoodIFC=NXP663X_CheckGoodIFC;
	App_Radio.pTunerCheckGoodQuality=NXP663X_CheckGoodQuality;
	App_Radio.pTunerRegisterInit=NULL;
	App_Radio.pTuner_QUAL_DATA_AVAILABLE=NXP663X_QUAL_DATA_AVAILABLE;
	App_Radio.pTuner_BandChange=NXP663X_Tuner_BandChange;
	App_Radio.FmLoc_Level=NXP663X_FM_Loc_Default;
	App_Radio.FmDx_Level=NXP663X_FM_Dx_Default;
	App_Radio.AmLoc_Level=NXP663X_MW_Loc_Default;
	App_Radio.AmDx_Level=NXP663X_MW_Dx_Default;
}















