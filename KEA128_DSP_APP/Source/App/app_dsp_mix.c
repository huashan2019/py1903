/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_dsp_mix.c  ---   ”Îdsp ºÏ≤‚
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.04
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

void Dsp_Mix_Init(void)
{
	SCH_U16 index,index0;
	for(index=0;index<(DSP_CHANNEL_CNT+1);index++)
	{
		for(index0=0;index0<(DSP_INPUT_CNT+1);index0++)
		{
			if((index==0x00)||(index0==0x00)||(index==index0))
				App_Dsp.Dsp_Data.MixData[index][index0] = DSP_MIXER_DEFAULT;
			else
				App_Dsp.Dsp_Data.MixData[index][index0] = DSP_MIXER_MIN;
		}
	}
}
///================================================================================================Mix========
const SCH_U16 Mix_Mixer_addr[DSP_CHANNEL_CNT+1]=
{
	0x0000,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN00_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN10_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN20_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN30_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN40_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN50_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN60_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN70_ADDR
};
SCH_BOOL Dsp_Mix_Mixer(SCH_U8 Channel,SCH_U8 *data)
{
	double Data;
	SCH_U8 index,buff[4] = {0x00, 0x00, 0x00, 0x00};
	if(Channel > DSP_CHANNEL_CNT || Channel == 0x00)
		return FALSE;
	for(index=0;index<DSP_INPUT_CNT;index++)
	{
		*data = LimitMaxMin(DSP_MIXER_MIN,*data,DSP_MIXER_MAX);
		Data = (double)*data-144;
		SIGMASTUDIOTYPE(pow(10,Data/20),buff);
		SIGMA_SAFELOAD_WRITE_REGISTER(DEVICE_ADDR_IC_1, Mix_Mixer_addr[Channel]+index, 1, buff);
		App_Dsp.Dsp_Data.MixData[Channel][index+1] = *data++;
	}
	return TRUE;
}
///================================================================================================Mix END========
const SCH_U16 Mix_Mixer_Input_addr[DSP_INPUT_CNT]=
{
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN0_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN1_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN2_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN3_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN4_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN5_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN6_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN7_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN8_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN9_ADDR
};
SCH_BOOL Dsp_Mix_Input(SCH_U8 data)
{
	double Data;
	SCH_U8 index,buff[4] = {0x00, 0x00, 0x00, 0x00};
	for(index=0;index<DSP_INPUT_CNT;index++)
	{
		Data = (double)data-144;
		SIGMASTUDIOTYPE(pow(10,Data/20),buff);
		SIGMA_SAFELOAD_WRITE_REGISTER(DEVICE_ADDR_IC_1, Mix_Mixer_Input_addr[index], 1, buff);
	}
	return TRUE;
}




