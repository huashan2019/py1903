/***********************************************************************//**
 * @file		Hero.c
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
#include "typedef.h"



































/*-----------------Scaling config start----------------------*/
//GEQ + TONE + VOLUME + CHANNEL GAIN + CHANNEL PEQ=48.16dB
//chang the dB data to hex Y memory data and replace below

#define ADSP_Y_Vol_DesScalBMTP_Data 0x0200    //ref to max tone gain 		12db
#define ADSP_Y_Vol_DesScalGEq_Data 0x0200   //ref to max geq gain			12db
#define  ADSP_X_Vol_OneOverMaxBoostP_Data  0x081385//ref to GeqMaxResp+ToneBoostMaxP

//peq gain max =6db
#define ADSP_Y_Vol_ScalF_Data   0x400   //ref to max front peq gain                        
#define ADSP_Y_Vol_ScalR_Data   0x400    //ref to max rear peq gain                      
#define ADSP_Y_Vol_ScalSwL_Data 0x400  //ref to max subwoofer left peq gain                       
#define ADSP_Y_Vol_ScalSwR_Data 0x400  //ref to max subwoofer right peq gain 

// the max gain is used do not need to compensate
//comp=48.16dB-(GEQ(12) + TONE(12) + CHANNEL PEQ(6) + MAX VOLUME(12)+ CHANNEL GAIN(6))
#define ADSP_Y_Vol_UpScalF_Data 0x07FF 	 //front compensation
#define ADSP_Y_Vol_UpScalR_Data 0x07FF   	 //rear compensation
#define ADSP_Y_Vol_UpScalSwL_Data 0x07FF	//subwoofer left compensation 
#define ADSP_Y_Vol_UpScalSwR_Data 0x07FF	 //subwoofer right compensation	
/*-----------------Scaling config end-----------------------*/

/*-----------------channel gain config start----------------------*/
//chang the dB data to hex Y memory data and replace below
//all channel use gain is max
#define ADSP_Y_Vol_ChanGainPFL_DefaultData  0x07FF	   //front left channel gain
#define ADSP_Y_Vol_ChanGainPFR_DefaultData  0x07FF	   //front rigt channel gain
#define ADSP_Y_Vol_ChanGainPRL_DefaultData  0x07FF	   //rear left channel gain
#define ADSP_Y_Vol_ChanGainPRR_DefaultData  0x07FF	   //rear right channel gain
#define ADSP_Y_Vol_ChanGainSwL_DefaultData  0x07FF	   //subwoofer left channel gain
#define ADSP_Y_Vol_ChanGainSwR_DefaultData  0x07FF	   //subwoofer right channel gain
#define ADSP_Y_Vol_ChanGainSL_DefaultData   0x07FF	   //secondary left channel gain
#define ADSP_Y_Vol_ChanGainSR_DefaultData   0x07FF	   //secondary right channel gain
#define ADSP_Y_Vol_ChanGainN_DefaultData    0x07FF	   //navigation channel gain
#define ADSP_Y_Vol_ChanGainT_DefaultData    0x07FF	   //phone channel gain
/*-----------------channel gain config end----------------------*/

/* Spurious signal*/
const U8 SpuriosSignal_init_tab1[]= {0xFF, 0x00, 0x04, 0x01, 0x84, 0xFE, 0xC5, 0xFF, 0xC7 };
const U8 SpuriosSignal_init_tab2[]= {
	0xFF, 0x00, 0x04, 0x0D, 0x68, 0x01, 0x2C, 0x23, 0xD2, 0xFE, 0xC5, 0x24, 0xAE, 0x01, 0x3B, 0x27,
	0x06, 0xFE, 0xC0, 0x27, 0x2E ,0x01 ,0x3B, 0x27, 0x7E, 0x01, 0x3B, 0x27, 0xA6 ,0x01, 0x2C, 0x27, 
	0xCE, 0xFE, 0xD4, 0x20, 0x9E, 0xFE ,0xD4, 0x25, 0xC1, 0xFE, 0xD4, 0x29, 0xE5, 0xFE, 0xC5, 0x29,
	0xEA, 0x01, 0x2C, 0x24, 0x0E, 0xFE, 0xD4, 0x28, 0x2D, 0x01, 0x2C, 0x29 ,0xEF, 0x01, 0x2C, 0x22,
	0xE7, 0x01, 0x2C, 0x22, 0xF1, 0xFE, 0xD4, 0x21, 0xA2 ,0xFE, 0xD4, 0x22, 0x88  };	
const U8 SpuriosSignal_init_tab3[]={ 
 0xFF,0x00,0x04,0x01,0xC0,0x00,0x00,0xFD,0xE8,0x80,0x01,0x43,0xD4,0x00,0x01,0x46,
 0x2D,0x80,0x01,0x6C,0x74,0x00,0x01,0x6E,0xCD,0x80,0x01,0x95,0x14,0x00,0x01,0x97, 
 0x6D};
const U8 SpuriosSignal_init_tab4[]= {0xFF,0x00,0x04,0x01,0xE8,0x00,0x00,0x02, 0x97 };

/*REQUIRED_INITIALISATION**: Host I2S Spurious*/
const U8 I2S_Spurious_Init[]={ 0xFF,0xFF, 0xFF, 0xB0, 0x74, 0x00,0x00,0x00,0xFF};
/*REQUIRED_INITIALISATION**: VCO operation point control*/
const U8 VCO_Control_init_tab1[]={ 
0xF0,0x80,0x00,0x5C,0x00,0xF6,0x1D,0x14,0x00,0x50,0x10,0x7C,0x60,0x80,0x0A,0x7C, 
0x00,0x00,0x00,0x40,0x00,0x06,0x1D,0x49,0x00,0x00,0x10,0x40,0x00,0x00,0x1B,0x49, 
0x00,0x00,0xF4,0x40,0x00,0x01,0x1A,0x49,0x00,0x80,0x2A,0x5C,0x02,0xF0,0x1A,0x14, 
0x00,0x50,0x90,0x14,0x00,0xF6,0x1C,0x68,0x00,0x01,0xFF,0x14,0x00,0x90,0x50,0x50, 
0x00,0x01,0x00,0x14,0x00,0x30,0x00,0x7C,0x10,0x80,0x16,0x7C,0x58,0xD4,0x10,0x68, 
0x00,0x02,0x00,0x08,0x00,0x50,0x50,0x14,0x00,0x30,0x10,0x5C,0x03,0xF0,0x1B,0x14, 
0x00,0x30,0xC0,0x7C,0x10,0x80,0x1D,0x14,0x00,0x70,0x10,0x5C,0x00,0xF6,0x1E,0x14, 
0x00,0x50,0x10,0x14,0x00,0xD0,0x10,0x14,0x00,0xF6,0x1E,0x5C,0x00,0xF6,0x1D,0x14, 
0x00,0x30,0x00,0x7C,0x10,0x80,0x27,0x14,0x00,0x50,0xD0,0x14,0x00,0xE0,0x10,0x14, 
0x00,0x70,0x10,0x14,0x00,0xF6,0x1E,0x14,0x00,0x50,0xD0,0x14,0x00,0xD0,0x10,0x7F, 
0xFC,0x01,0x15,0x14,0x00,0xF0,0x1A,0x7C,0x00,0x00,0x00,0x68,0x00,0x10,0x00,0x5C, 
0x00,0xF0,0x20,0x14,0x00,0x50,0x10,0x50,0x00,0x00,0x01,0x14,0x00,0x30,0x00,0x7C, 
0x58,0x54,0x50,0x7F,0xFC,0x01,0x1B,0x7C,0x68,0x74,0x10,0x26,0x00,0xF0,0x0A
};
const U8 VCO_Control_init_tab2[]={ 0xF0,0x01,0x15,0x00,0x05,0xBA};
const U8 VCO_Control_init_tab3[]={ 0xF0,0x01,0x14,0x00,0x80,0x00};

/*-----------------primary channel volume config start----------------------*/

U32 Hero_Init_Timer;


/*init step*/
Hero_InitStepDef Hero_Init_Step;
byte_field Hero_Flag0;












void TimerSet(U32 *STimer, U32 TimeLength)
{
	*STimer = TimeLength;
}



U8 TimerHasExpired (U32 *STimer)
{
	if(*STimer == 0)
		return TRUE;
	return FALSE;
}























/*-------------------------------------------------------------------------------------------------------------------
function define start
---------------------------------------------------------------------------------------------------------------------*/
S8 Hero_Reset(void);
void Hero_Init_ImproveRadioSettingA(void);
void Hero_Init_ImproveRadioSettingA2(void);
void Hero_Init_ImproveAudioSettingA(void);
void Hero_Init_ImproveRadioSettingB(void);
void Hero_Init_ImproveAudioSettingB(void);
void Hero_Init_ImproveAudioSettingC(void);
SCH_U8 Send_Usecase_Command(U8 UseCase);
 void Send_keycode_Command(U8 dPAR1,U8 dPAR2,U8 dPAR3);
 
S8 Hero_Keycode(void);
S8 Hero_AcitveDevice(void);
void Hero_RadioConfigInit(void);
S8 Hero_InitAudioPointer(void);
S8 Hero_InitPeripheral(void);
S8 Hero_InitGPIO(void);
S8 Hero_InitSource(void);
S8 Hero_InitAudio(void);
S8 Hero_InitRadio(void);

S8 Audio_ScalingInit(void);
S8 Audio_InitChannelGain(void);
S8 Audio_LoudnessInit(void);
void Audio_PEQInit(void);
void Audio_DCFilterInit(void);
void Audio_NavPhoneFilterInit(void);
void Audio_CompExpInit(void);
void Audio_LimiterClipDetectInit(void);
S8 Audio_LoudnessInit(void);
void Audio_GSAInit(void);
/*hero power manager*/
void Hero_ShutDownAllGen(void);
void Hero_WarmBootInit(void);
void Audio_DelayInit(void);
/*-------------------------------------------------------------------------------------------------------------------
function define end
---------------------------------------------------------------------------------------------------------------------*/

void Hero_Init_SpuriosSignal(void)
{
	I2C1_WriteData(TEF663x_SlaveAddr,(U8 *)SpuriosSignal_init_tab1,9);	 
	I2C1_WriteData(TEF663x_SlaveAddr,(U8 *)SpuriosSignal_init_tab2,77);	 
	I2C1_WriteData(TEF663x_SlaveAddr,(U8 *)SpuriosSignal_init_tab3,33);	 
	I2C1_WriteData(TEF663x_SlaveAddr,(U8 *)SpuriosSignal_init_tab4,9);	 
}

void Hero_Init_IIS_VCOControl(void)
{
  	I2C1_WriteData(TEF663x_SlaveAddr,(U8 *)I2S_Spurious_Init,9);	 
	I2C1_WriteData(TEF663x_SlaveAddr,(U8 *)VCO_Control_init_tab1,0xcf);	 
	I2C1_WriteData(TEF663x_SlaveAddr,(U8 *)VCO_Control_init_tab2,6);	 
	I2C1_WriteData(TEF663x_SlaveAddr,(U8 *)VCO_Control_init_tab3,6);	
}
/*-----------------------------------------------------------------------
Input:	
Output:	
Description:  hero cold boot init proc
------------------------------------------------------------------------*/
void Init_Hero(void)
{
	if((Hero_Init_Timer>0)||(F_Hero_InitOK == 1))  //init step is not end or already inited
		return;
	switch(Hero_Init_Step)
	{
		case HERO_INIT_STEP_COLDBOOT: 
			F_Hero_InitOK = 0;
			if(Hero_Reset()==HERO_STEP_INIT_OK)   //hero hardware reset
			{
			    Hero_Init_Step = HERO_INIT_STEP_AUDIO_IMPROVE_A;
			}
			break;
		case HERO_INIT_STEP_AUDIO_IMPROVE_A:
			Hero_Init_ImproveRadioSettingA();   	//improve audio cmd for V1C 
			TimerSet(&Hero_Init_Timer,2);      		 //wait to ensure the time between this cmd and improve audio cmd B
			Hero_Init_Step = HERO_INIT_STEP_RADIO_IMPROVE_A;
			break;
		case HERO_INIT_STEP_RADIO_IMPROVE_A: 	// hero init data
			Hero_Init_ImproveAudioSettingA(); 	//improve radio cmd for V1C   
			Hero_Init_ImproveRadioSettingA2();
			Hero_Init_SpuriosSignal();
			Hero_Init_IIS_VCOControl();
			///Hero_AcitveDevice();
			TimerSet(&Hero_Init_Timer,2);        	//wait to ensure the time between this cmd and improve radio cmd B
			Hero_Init_Step = HERO_INIT_STEP_KEYCODE1;
			break;				
		
		case HERO_INIT_STEP_KEYCODE1:
			///if(Hero_Keycode()==HERO_STEP_INIT_OK)    //send keycode to enable the advance function
			{
				TimerSet(&Hero_Init_Timer,7);
				Hero_Init_Step = HERO_INIT_STEP_USECASE;
			}
			break;
		case HERO_INIT_STEP_USECASE:
			if(Send_Usecase_Command(7)==TRUE) // set the usecase 1
				Hero_Init_Step = HERO_INIT_STEP_ACTIVE_DEVICE;
			break;
		case HERO_INIT_STEP_ACTIVE_DEVICE:
			Hero_AcitveDevice();//A preset or search tuning action is needed to get the Hero out of power down mode
		
			Hero_Init_Step = HERO_INIT_STEP_RADIO_IMPROVE_B;
			break;
		case HERO_INIT_STEP_RADIO_IMPROVE_B:
			Hero_Init_ImproveRadioSettingB();//improve radio cmd for V1C   
			Hero_Init_Step = HERO_INIT_STEP_AUDIO_IMPROVE_B;
			break;
		case HERO_INIT_STEP_AUDIO_IMPROVE_B:
			Hero_Init_ImproveAudioSettingB();//improve audio cmd for V1C   
			Hero_Init_Step = HERO_INIT_STEP_DAC;
			break;
		case HERO_INIT_STEP_DAC:
			Audio_DrvPeripheralSwitch(PERIOHERALID_FDAC,1);	  //enable the front DAC
			Audio_DrvPeripheralSwitch(PERIOHERALID_RDAC,1);	  //enable the rear DAC
			Hero_Init_Step=HERO_INIT_STEP_AUDIO_POINTER;
			break;
		case HERO_INIT_STEP_AUDIO_POINTER:
			if(Hero_InitAudioPointer()==HERO_STEP_INIT_OK)     //config the audio pointer if needed
				Hero_Init_Step = HERO_INIT_STEP_PERIPHERAL;
			break;
		case HERO_INIT_STEP_PERIPHERAL:
			if(Hero_InitPeripheral()==HERO_STEP_INIT_OK)   //config the audio input mode
				Hero_Init_Step = HERO_INIT_STEP_GPIO;
			break;
		case HERO_INIT_STEP_GPIO:
			Hero_InitGPIO();//config the GPIO function
			Hero_Init_Step = HERO_INIT_STEP_SOURCE;
			break;
		case HERO_INIT_STEP_SOURCE:
			if(Hero_InitSource()==HERO_STEP_INIT_OK)//config the input source
				Hero_Init_Step = HERO_INIT_STEP_AUDIO;
			break;
		case HERO_INIT_STEP_AUDIO:
			if(Hero_InitAudio()==HERO_STEP_INIT_OK)//init the ADSP module functions
				Hero_Init_Step = HERO_INIT_STEP_RADIO;
			break;
		case HERO_INIT_STEP_RADIO:
			if(Hero_InitRadio()==HERO_STEP_INIT_OK)//init tuner parameters
				Hero_Init_Step = HERO_INIT_STEP_AudioImproveC;
			break;	
		case HERO_INIT_STEP_AudioImproveC:
			Hero_Init_ImproveAudioSettingC();//improve audio cmd for V1C   
			Hero_Init_Step = HERO_INIT_STEP_END;
			break;	
		case HERO_INIT_STEP_END:
			F_Hero_InitOK = 1;
			Hero_Init_Step = HERO_INIT_STEP_COLDBOOT;
			Audio_SetPrimaryInputSource(INPUT_SOURCE_IIS1);			
			Audio_DrvSetStreamInput(NavigationStr,INPUT_SOURCE_SPDIF);
			break;
	}
}
/*-----------------------------------------------------------------------
Input:	
Output:	HERO_STEP_INITTING: hero is initting
		HERO_STEP_INIT_OK: hero has already inited the radio config, the system can go through next step		
Description: reset the hero and wait untill the hero is able to communicate 	  
------------------------------------------------------------------------*/
S8 Hero_Reset(void)
{/*
	static U8 Step=0;

	if(!TimerHasExpired(&Hero_Init_Timer))
	{
		return HERO_STEP_INITTING;
	}	

	switch(Step)
	{
		case 0:
		{
			TimerSet(&Hero_Init_Timer,3);   //see um for this time delay
			p_RESET_HERO_OFF();		  //set the reset pin high
			Step=1;
			return(HERO_STEP_INITTING);
		}
		case 1:
		{
			TimerSet(&Hero_Init_Timer,2);   //see um for this time delay
			p_RESET_HERO_ON();		  //set the reset pin low
			Step=2;
			return(HERO_STEP_INITTING);
		}
		case 2:
		{
			TimerSet(&Hero_Init_Timer,60);   //wait for the hero able to read and write
			p_RESET_HERO_OFF();		   //set the reset pin high
			Step=3;
			return(HERO_STEP_INITTING);
		}
		case 3:				   //this will be the last init step
		{
			Step=0;
			return(HERO_STEP_INIT_OK);
		}
	}*/
	return(HERO_STEP_INIT_OK);
}

/*-----------------------------------------------------------------------
Input:	
Output:			
Description: send cmd to improve radio setting for V1C
------------------------------------------------------------------------*/
void Hero_Init_ImproveRadioSettingA(void)
{
	U8 Buffer[6];    

	//Prepare for AM divider/IFAD
	Buffer[0]=0xE1;  
	Buffer[1]=0xD0;
	Buffer[2]=0x61; 
	Buffer[3]=0x23;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,4);	 //start to send data to i2c1 bus

	//Prepare for AM divider/IQC
	Buffer[0]=0xE1;  
	Buffer[1]=0xB0;
	Buffer[2]=0x56; 
	Buffer[3]=0x65;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,4);	 //start to send data to i2c1 bus

	//Set IQC
	Buffer[0]=0xF0;  
	Buffer[1]=0x00;
	Buffer[2]=0x46; 
	Buffer[3]=0x01;
	Buffer[4]=0x10;
	Buffer[5]=0x22;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus

}

void Hero_Init_ImproveRadioSettingA2(void)
{
     U8 Buffer[17];
	 
	/* Prepare setting VCO operation point control*/
     Buffer[0]=0xE1;  
	Buffer[1]=0x40;
	Buffer[2]=0xD9; 
	Buffer[3]=0xB1;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,4);	 
     /*Image suppression*/
     Buffer[0]=0xf0;  
	Buffer[1]=0x00;
	Buffer[2]=0x46; 
	Buffer[3]=0x01;
	Buffer[4]=0x10;
	Buffer[5]=0x22;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 
	/* FM Startup*/
	Buffer[0]=0xFF;  
	Buffer[1]=0xFF;
	Buffer[2]=0xFF; 
	Buffer[3]=0xb0;
	Buffer[4]=0x3c;
	Buffer[5]=0x10;
     Buffer[6]=0x0;  
	Buffer[7]=0x01;
	Buffer[8]=0x00; 
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,9);	
	Buffer[0]=0xFF;  
	Buffer[1]=0xc0;
	Buffer[2]=0x60; 
	Buffer[3]=0x00;
	Buffer[4]=0x20;
	Buffer[5]=0x0;
     Buffer[6]=0x0;  
	Buffer[7]=0x0;
	Buffer[8]=0x04; 
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,9);	
     
/* Host I2S Spurious signal*/
	Buffer[0]=0xFF;  
	Buffer[1]=0xFF;
	Buffer[2]=0xFF; 
	Buffer[3]=0xb0;
	Buffer[4]=0x74;
	Buffer[5]=0x0;
     Buffer[6]=0x0;  
	Buffer[7]=0x0;
	Buffer[8]=0xff; 
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,9);	
}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: send cmd to improve Audio setting for V1C
------------------------------------------------------------------------*/
void Hero_Init_ImproveAudioSettingA(void)
{
	U8 Buffer[6];    
    /*Prepare for (Host I2S) Spurious */
	Buffer[0]=0xE1;  
	Buffer[1]=0xC1;
	Buffer[2]=0xBC; 
	Buffer[3]=0x11;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,4);	 

}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: send cmd to improve radio setting for V1C
------------------------------------------------------------------------*/
void Hero_Init_ImproveRadioSettingB(void)
{
	U8 Buffer[6];    
     /*IF ADC performance*/
	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x11; 
	Buffer[3]=0x00;
	Buffer[4]=0xDC;
	Buffer[5]=0x00;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 
     /*Audio Rear output startup*/
	Buffer[0]=0xF2;  
	Buffer[1]=0x06;
	Buffer[2]=0xE9; 
	Buffer[3]=0x7F;
	Buffer[4]=0xFF;
	Buffer[5]=0xFF;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 
     /* AM LO frequency*/	
	Buffer[0]=0xF0;  
	Buffer[1]=0x40;
	Buffer[2]=0x0A; 
	Buffer[3]=0x07;
	Buffer[4]=0xFF;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,5);	

	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x36;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	

	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0D; 
	Buffer[3]=0x80;
	Buffer[4]=0x00;
	Buffer[5]=0x00;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus

	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x37;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus

	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x36;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus

	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0D; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x01;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus

	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x5B;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus

	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x5A;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus

	Buffer[0]=0xF0;  
	Buffer[1]=0x40;
	Buffer[2]=0x0A; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,5);	 //start to send data to i2c1 bus
}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: send cmd to improve Audio setting for V1C
------------------------------------------------------------------------*/
void Hero_Init_ImproveAudioSettingB(void)
{
	U8 Buffer[13];    
    /*ADC performance*/
	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x20;
	Buffer[5]=0x0A;
	Buffer[6]=0x00;
	Buffer[7]=0xB5;
	Buffer[8]=0xBE;
	Buffer[9]=0x00;
	Buffer[10]=0x12;
	Buffer[11]=0x73;
	Buffer[12]=0xC8;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,13);	 //start to send data to i2c1 bus

	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x18;
	Buffer[5]=0x47;
	Buffer[6]=0x00;
	Buffer[7]=0xF8;
	Buffer[8]=0x00;
	Buffer[9]=0x47;
	Buffer[10]=0x00;
	Buffer[11]=0xF8;
	Buffer[12]=0x00;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,13);	 //start to send data to i2c1 bus

	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x2C;
	Buffer[5]=0x00;
	Buffer[6]=0x00;
	Buffer[7]=0x07;
	Buffer[8]=0x00;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,9);	 //start to send data to i2c1 bus
	
	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x48;
	Buffer[5]=0x00;
	Buffer[6]=0x11;
	Buffer[7]=0x80;
	Buffer[8]=0x80;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,9);	 //start to send data to i2c1 bus
	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x50;
	Buffer[5]=0x00;
	Buffer[6]=0x10;
	Buffer[7]=0x80;
	Buffer[8]=0x80;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,9);	 //start to send data to i2c1 bus
}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: send cmd to improve Audio setting for V1C
------------------------------------------------------------------------*/
void Hero_Init_ImproveAudioSettingC(void)
{
	U8 Buffer[9];    

	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x2C;
	Buffer[5]=0x00;
	Buffer[6]=0x00;
	Buffer[7]=0x01;
	Buffer[8]=0x00;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,9);	 //start to send data to i2c1 bus

}
/*-----------------------------------------------------------------------
Input:	UseCase: hero usecase value:1~4,7
Output:			
Description: set hero work usecase	  
------------------------------------------------------------------------*/
SCH_U8 Send_Usecase_Command(U8 UseCase)
{
	U8 TempData[2];
	U8 Data;

	TempData[0]=TEF663x_SUBADDR_USECASE;
	TempData[1]=UseCase<<4;

	I2C1_WriteData(TEF663x_SlaveAddr,TempData,2);	 //start to send data to i2c1 bus
	Hero_ReadBack(TEF663x_SUBADDR_USECASE,1,&Data);//check if write is ok
	if(Data==TempData[1])
	{
		return TRUE;
	}
	return FALSE;
} 

/*-----------------------------------------------------------------------
Input:	
Output:	HERO_STEP_INITTING: hero is initting
		HERO_STEP_INIT_OK: hero has already inited the radio config, the system can go through next step
Description: send keycode to hero 	  
------------------------------------------------------------------------*/
S8 Hero_Keycode(void)
{
	static U8 Step=0;

	if(Hero_Init_Timer)
	{
		return HERO_STEP_INITTING;
	}	

	switch(Step)
	{
		case 0:
		{
			Send_keycode_Command(0x80,0x00,0x01); //all features are available within 3 hours.
			Step++;
			return(HERO_STEP_INITTING);
		}
		case 1:				   //this will be the last init step
		{
			Step=0;
			return(HERO_STEP_INIT_OK);
		}
	}
	return(HERO_STEP_INIT_OK);
}
/*-----------------------------------------------------------------------

Input:	NULL		
Output:	NULL		
Description: 	active hero device from power down mode 
------------------------------------------------------------------------*/
S8 Hero_AcitveDevice(void)
{
	Tuner_Update_ProgCnt(Radio_PRESETMODE);///(Radio_SEARCHMODE);//set tuner to current Freq with search mode
	return SUCCESS;	 
}

/*-----------------------------------------------------------------------
Input:	
Output:			 
Description: set scaling, see scaling data value define for more
------------------------------------------------------------------------*/
static const sMemAddr_Data ScalingInitTab[] = {
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_DesScalBMTP,	ADSP_Y_Vol_DesScalBMTP_Data},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_DesScalGEq,	ADSP_Y_Vol_DesScalGEq_Data},
	{I2CDRV_X_MEM,	ADSP_X_Vol_OneOverMaxBoostP,	ADSP_X_Vol_OneOverMaxBoostP_Data},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ScalF,		ADSP_Y_Vol_ScalF_Data},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ScalR,		ADSP_Y_Vol_ScalR_Data},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ScalSwL,		ADSP_Y_Vol_ScalSwL_Data},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ScalSwR,		ADSP_Y_Vol_ScalSwR_Data}, 
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_UpScalF,		ADSP_Y_Vol_UpScalF_Data},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_UpScalR,		ADSP_Y_Vol_UpScalR_Data}, 
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_UpScalSwL,	ADSP_Y_Vol_UpScalSwL_Data}, 
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_UpScalSwR,	ADSP_Y_Vol_UpScalSwR_Data}, 
 	
	//end flag
	{I2CDRV_INVALID,	0,			0},
};

S8 Audio_ScalingInit(void)
{        
	WriteHero_MEM_Tab(ScalingInitTab);//write scaling data to hero

	return SUCCESS;
}
/*-----------------------------------------------------------------------
		  
Input:	
Output:			
Description: set channel gain, see value define for more
------------------------------------------------------------------------*/
static const sMemAddr_Data ChannelGainInitTab[] = {
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainPFL,	ADSP_Y_Vol_ChanGainPFL_DefaultData},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainPFR,	ADSP_Y_Vol_ChanGainPFR_DefaultData},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainPRL,	ADSP_Y_Vol_ChanGainPRL_DefaultData},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainPRR,	ADSP_Y_Vol_ChanGainPRR_DefaultData},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainSwL,	ADSP_Y_Vol_ChanGainSwL_DefaultData},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainSwR,	ADSP_Y_Vol_ChanGainSwR_DefaultData},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainSL,	ADSP_Y_Vol_ChanGainSL_DefaultData}, 
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainSR,	ADSP_Y_Vol_ChanGainSR_DefaultData},
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainN,	ADSP_Y_Vol_ChanGainN_DefaultData}, 
	{I2CDRV_Y_MEM,	ADSP_Y_Vol_ChanGainT,	ADSP_Y_Vol_ChanGainT_DefaultData}, 
 	
	//end flag
	{I2CDRV_INVALID,	0,			0},
};
S8 Audio_InitChannelGain(void)
{
	WriteHero_MEM_Tab(ChannelGainInitTab);//write channel gain data to hero
	
	return SUCCESS;                  
}



/*-----------------------------------------------------------------------
Input:	
Output:			
Description: 	init loudness 
------------------------------------------------------------------------*/
static const sMemAddr_Data LoudnessInitTab[] = {
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_StatLev,		0x07FF},//bass control
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_StatLevT,	0x07FF},//treble control

	//below don`t known because not inmentioned in um, just find in GUI          
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KpBL,		0x02B6},
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KpBH,		0x0809},
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KmBL,		0x0273},
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KmBH,		0x0012},
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KpTL,		0x02B6}, 
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KpTH,		0x02B6}, 
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KmTL,		0x0273}, 
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KmTH,		0x0012}, 
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_GchanCorrect,		0x0400}, 
 	
	//end flag
	{I2CDRV_INVALID,	0,			0},
};

S8 Audio_LoudnessInit(void)
{
      TEF663x_EasyProgrammWrite(ADSP_EASYP_Loudf_StaticCoupled);
	WriteHero_MEM_Tab(LoudnessInitTab);
	return SUCCESS;
}




/*-----------------------------------------------------------------------
Input:	
Output:			
Description: update use of the radio setting	  
------------------------------------------------------------------------*/
void Hero_RadioConfigInit(void)
{
	U8 TempData[3];

	//set audio sample rate frequency shift mode
	TempData[0]=0xC9;
	TempData[1]=0x02;
	TempData[2]=0x01;
	I2C1_WriteData(TEF663x_SlaveAddr,TempData,3);	 //start to send data to i2c1 bus	
}
/*-----------------------------------------------------------------------
Input:	dPAR1,dPAR2,dPAR3: keycode
Output:			
Description: sent keycode to hero	  
------------------------------------------------------------------------*/
 void Send_keycode_Command(U8 dPAR1,U8 dPAR2,U8 dPAR3)
{
	U8 TempData[4];
	TempData[0]=TEF663x_SUBADDR_KEYCODE;
	TempData[1]=dPAR1;
	TempData[2]=dPAR2;
	TempData[3]=dPAR3;

	I2C1_WriteData(TEF663x_SlaveAddr,TempData,4);	 //start to send data to i2c1 bus	
	Printf("keycode_Command \n");
}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: 	  graphical spectrum analyzer init
------------------------------------------------------------------------*/
void Audio_GSAInit(void)
{
	Audio_DrvGSASwitch(1);	  //enable the gsa
	WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Gsa_InPntr,ADSP_X_PChannelMode_OutL);  //set input to be the GSA source
}
/*-----------------------------------------------------------------------
Input:	
Output:	HERO_STEP_INITTING: hero is initting
		HERO_STEP_INIT_OK: hero has already inited the radio config, the system can go through next step		
Description: init the audio pointers 	  
------------------------------------------------------------------------*/
S8 Hero_InitAudioPointer(void)
{
	static U8 Step=0;

	if(Hero_Init_Timer)
	{
		return HERO_STEP_INITTING;
	}	

	switch(Step)
	{
		case 0:
		{
			Audio_LimiterClipDetectInit();   //clip detect and limiter init
			Step++;
			return(HERO_STEP_INITTING);
		}
		case 1:				   //this will be the last init step
		{
			Step=0;
			return(HERO_STEP_INIT_OK);
		}
	}
	return(HERO_STEP_INIT_OK);
}
/*-----------------------------------------------------------------------
Input:	
Output:	HERO_STEP_INITTING: hero is initting
		HERO_STEP_INIT_OK: hero has already inited the radio config, the system can go through next step		
Description: init the peripherals if used 	  
------------------------------------------------------------------------*/
S8 Hero_InitPeripheral(void)
{
	static U8 Step=0;

	if(Hero_Init_Timer)
	{
		return HERO_STEP_INITTING;
	}	

	switch(Step)
	{
		case 0:    //set the SPDIF input mode
		{
			Audio_DrvSetSPDIFInMode(1);          //DIGITAL mode
			Step++;
			return(HERO_STEP_INITTING);
		}
		case 1:    //set the AD input mode
		{
			Audio_DrvSetADInMode(0,0);          //AD_0, diffrential mode  2Vrms
			Audio_DrvSetADInMode(1,2);          //AD_1,High common mode, 2 Vrms
			Audio_DrvSetADInMode(2,2);          //AD_2,High common mode, 2 Vrms
			Audio_DrvSetADInMode(3,0);          //AD_2,diffrential mode  2Vrms
			Step++;
			return(HERO_STEP_INITTING);
		}
		case 2:    //set the I2S input mode
		{
			Audio_DrvSetI2SInMode(0,0);          //I2S_0, philips
			Audio_DrvSetI2SInMode(1,0);          //I2S_1,
			Audio_DrvSetI2SInMode(2,0);          //I2S_2,philips
			Audio_DrvSetI2SInMode(3,0);          //Host I2S,philips
			Step++;
			return(HERO_STEP_INITTING);
		}
		case 3:				   //this will be the last init step
		{
			Step=0;
			return(HERO_STEP_INIT_OK);
		}
	}
	return(HERO_STEP_INIT_OK);
}

/*-----------------------------------------------------------------------
Function name:	void AudioPEQRRSet( void)
Input:			NULL                           
Output:			NULL
Description:	
               Set PEQ RR channel
------------------------------------------------------------------------*/
/*this table is RearRight channel Parametric EQ setting ,band0- band7,
filters parametrics can get by NXP gui software*/
const U32 tbl_PEQ_Flat_Curve[PEQ_BAND_MAX][10]={
	/*BAND 1:flat,0db*/
	{0x01F2,0x0007 ,0x03E4,0x0C0E, 0x0275, 0x07F1, 0x0000},

	/*BAND 2:flat,0db*/
	{0x0313,0x000e ,0x0627,0x0C1c, 0x0342, 0x07e2, 0x0000},

	/*BAND 3:flat,0db*/
	{0x02F7 ,0x001C ,0x05EE ,0x0C38 ,0x0035,0x07C4,0x0000},

	/*BAND 4:flat,0db*/
	{0x02F7 ,0x001C ,0x05EE ,0x0C38 ,0x0035,0x07C4,0x0000},

	/*BAND 5:flat */
	{0,0x400,0,0,0,0,0,0,0,0,},

	/*BAND 6:flat */
	{0,0x400,0,0,0,0,0,0,0,0},

	/*BAND 7:flat */
	{0,0x400,0,0,0,0,0,0,0,0},
};
void AudioPEQRRSet(void)
{
	int band;
	for(band=BAND1;band<PEQ_BAND_MAX;band++)
	{
	     Audio_DrvPEQSet(RearRight,band,(U32*)tbl_PEQ_Flat_Curve[band]);
	}
}



/*this table is RearLeft channel Parametric EQ setting ,band0- band7,
filters parametrics can get by NXP gui software  ??*/
const U32 tbl_PEQ_RL_Curve[PEQ_BAND_MAX][10]={
	/*BAND 1:100HZ,Q=1.0,Gain=-4db*/
	{0x0375, 0x000B, 0x06EB, 0x0C16, 0x0770 ,0x07E8 ,0x0F43},

	/*BAND 2:200HZ,Q=1.0,Gain=-2db*/
	{0x0059 ,0x0012 ,0x00B2 ,0x0C24 ,0x00C5 ,0x07DB ,0x0F97},

	/*BAND 3:400HZ,Q=1.0,Gain=-1db*/
	{0x05CD ,0x001F ,0x039A ,0x0C3F ,0x029F,0x07BD,0x0FC8},

	/*BAND 4:800HZ,Q=1.0,Gain=0db*/
	{ 0x01B1 ,0x0037 ,0x0362 ,0x0C6E ,0x0009 ,0x0785, 0x0000},

	/*BAND 5:1600HZ,Q=1.0,Gain=-2db*/
	{0x0492 ,0x03E5 ,0x02A7 ,0x092F ,0x0374 ,0x0319 ,0x0559 ,0x06D0 ,0x07F9 ,0x0D00},

	/*BAND 6:3200HZ,Q=1.0,Gain=-4db*/
	{0x05A1 ,0x039B ,0x01C5,0x0AB9 ,0x05F9 ,0x0244 ,0x063A ,0x0546 ,0x0465 ,0x0E1F},

	/*BAND 7:6400HZ,Q=1.0,Gain=-6db*/
	{0x057D ,0x030C ,0x02EC ,0x0D6F ,0x05EF ,0x0123 ,0x0513 ,0x0290,0x0493 ,0x0FCF},

};



/*-----------------------------------------------------------------------
Function name:	void AudioPEQRLSet( void)
Input:			NULL                           
Output:			NULL
Description:	
               Set PEQ RL channel
------------------------------------------------------------------------*/
void AudioPEQRLSet(void)
{
	int band;
	for(band=BAND1;band<PEQ_BAND_MAX;band++)
	{
	     Audio_DrvPEQSet(RearLeft,band,(U32*)tbl_PEQ_Flat_Curve[band]);
	}
}
/*-----------------------------------------------------------------------
Input:			NULL                           
Output:			NULL
Description:	
               Set PEQ FL channel
------------------------------------------------------------------------*/
void AudioPEQFLSet(void)
{
	int band;
	for(band=BAND1;band<PEQ_BAND_MAX;band++)   //using the same para to rear left channel
	{
	     Audio_DrvPEQSet(FrontLeft,band,(U32*)tbl_PEQ_Flat_Curve[band]);
	}

}
/*-----------------------------------------------------------------------
Input:			NULL                           
Output:			NULL
Description:	
               Set PEQ FR channel
------------------------------------------------------------------------*/
void AudioPEQFRSet(void)
{
	int band;
	for(band=BAND1;band<PEQ_BAND_MAX;band++)//using the same para to rear left channel
	{
	     Audio_DrvPEQSet(FrontLeft,band,(U32*)tbl_PEQ_Flat_Curve[band]);
	}

}

/*-----------------------------------------------------------------------
Input:			NULL                           
Output:			NULL
Description:	
               Set PEQ init
------------------------------------------------------------------------*/
void Audio_PEQInit(void)
{
	/*set RL PEQ*/ 
	AudioPEQRLSet();
	/*set RR PEQ*/
	AudioPEQRRSet();

	/*set RL PEQ */
	AudioPEQFLSet();
	/*set RR PEQ*/
	AudioPEQFRSet();
}
/*-----------------------------------------------------------------------
Input:			NULL                           
Output:			NULL
Description:	
               dc filter  init
------------------------------------------------------------------------*/
void Audio_DCFilterInit(void)
{
	/*set primary DCFilter On*/
	Audio_DrvDCFilterSet(PrimaryStr, AUDIO_ON);
}	

 /*-----------------------------------------------------------------------
Input:			locate:USE_RAM,USE_ROM                     
Output:			
Description:	
              Init ClickClack Feature
------------------------------------------------------------------------*/
void ClickClack_Init(void)
{	
	/*extern input connect to click clack generator*/
	Audio_DrvSuperPositionConnect(3,ClickClack,Left);
	/*impose ext on FR*/
	Audio_ImposeExtOnFR(-10);			 
	/*Click clack use ROM*/
	Audio_DrvClickClack_Locate(USE_ROM);
	/*use IIC Command control*/
	Audio_DrvClickClack_ModeSel(I2C_CONTROL);
}


/*-----------------------------------------------------------------------
Input:			NULL                   
Output:			NULL
Description:	
                  Audio PolyChime Initialize
------------------------------------------------------------------------*/
void AudioPolyChimeInit(){
/*Do Nothing Here*/
 
}
/*-----------------------------------------------------------------------
Input:			NULL
Output:			NULL
Description:	
                Auido Tone Init
------------------------------------------------------------------------*/
void AudioToneInit(void)
{
	S8 dBVal=0;
	Audio_DrvBassSet(PrimaryStr, dBVal, _100_HZ); /* primary, bass, +12 db gain, fc = 100 */
	Audio_DrvMidSet(PrimaryStr, dBVal,_500_HZ); /* primary, mid, +12 db gain, fc = 500 */
	Audio_DrvTrebSet(PrimaryStr,dBVal, _10000_HZ); /* primary, treble, +12 db gain, fc = 10kHz */
}
/*Filter Paras
   type: 1st LP shlv
   gain: 0db
   cutoff freq:3400hz*/
U32 tbl_NavPhone_Filter1Coefs[10]=
{
       0x0000 ,0x0200 ,0x06F2 ,0x0ECA ,0x0000,0x0000,0x021C,0x026A,0x0000,0x0000 
};



/*Filter Paras
   type: 1st HP shlv
   gain: 0db
   cutoff freq:100hz*/
U32 tbl_NavPhone_Filter2Coefs[10]=
{
       0x0000 ,0x0200 ,0x01F2 ,0x0E07 ,0x0000,0x0000,0x041B,0x03F1,0x0000,0x0000
};

/*-------------------------------------------------------------------------
input:
     none
output:
      none
 description:
      Nav/Phone filter init,this function is called in Init_Hero

--------------------------------------------------------------------------*/
void Audio_NavPhoneFilterInit(void)
{
	/*Nav Filter set :band pass (100hz--3400hz)*/
	Audio_DrvNavPhoneFilterSet(NavigationFilter,0,tbl_NavPhone_Filter1Coefs);
	Audio_DrvNavPhoneFilterSet(NavigationFilter,1,tbl_NavPhone_Filter2Coefs);
	/*Phone Filter set: band pass (100hz--3400hz)*/
	Audio_DrvNavPhoneFilterSet(PhoneFilter,0,tbl_NavPhone_Filter1Coefs);
	Audio_DrvNavPhoneFilterSet(PhoneFilter,1,tbl_NavPhone_Filter2Coefs);

    //phone stream input and volume set
	Audio_DrvSetStreamInput(PhoneStr,INPUT_SOURCE_AIN2);
	Audio_DrvSetVolume(PhoneStr,-26);
}

/*----------------------------------------------------------------------------
* Input       :null                                                                                                                                      
* Output      :null                                                                                       
* FUNCTION    :Init compressor expander ,this function be called in Init_Hero                       
*---------------------------------------------------------------------------*/
void Audio_CompExpInit(void)
{
	/*enable Compressor and expander*/
	Audio_DrvCompExpControl(AUDIO_OFF,AUDIO_OFF);

	/*Expander parametric set,set expander Threshold -60db*/
	Audio_DrvExpanderSet(-60);

	/*compressor parametric set*/
	Audio_DrvCompressorSet(CompCurve_High, 0, 0, 0);//cure_High ratio=3.0,
}

/*--------------------------------------------------------------------------
 Input 					
 Output      :null                                                                                        
 FUNCTION    :Limiter Clip detect init                                   
-------------------------------------------------------------------------*/
void Audio_LimiterClipDetectInit(void)
{
	/* select limiter configuration 1*/
	Audio_DrvLimiterConfigurationSel(1);

	/*set  front&rear limiter  -1db,*/
	Audio_DrvLimiterSet(Front,-1,AUDIO_ON);

	/*set subcenter limiter  */
	Audio_DrvLimiterSet(SubCenter,-1,AUDIO_ON);

	/*set subwoofer limiter */
	Audio_DrvLimiterSet(SubWoofer,-1,AUDIO_ON);

	/* Front/Rear Limiter Flag generator set,SCD0  ,hight active*/
	Audio_DrvLimiterFlagGenerSet(Front,LimiterFlag_Mask_SCD0,0);
	/*SubCenter Limiter Flag generator set,SCD1  high active*/
	Audio_DrvLimiterFlagGenerSet(SubCenter,LimiterFlag_Mask_SCD1,0);
	/*Subwoofer Limiter Flag generator set,SCD2 , high active*/
	Audio_DrvLimiterFlagGenerSet(SubWoofer,LimiterFlag_Mask_SCD2,0);

	/*Set Clip detector 0 treshold gain=-20 dbFS*/
	Audio_DrvSoftClipDetectSet(SCD0,-20);
	/*Set Clip detector 1 treshold gain=-20 dbFS*/
	Audio_DrvSoftClipDetectSet(SCD1,-20);
	/*Set Clip detector 2 treshold gain=-20 dbFS*/
	Audio_DrvSoftClipDetectSet(SCD2,-20);
}

/*-----------------------------------------------------------------------
Input:	
Output:	HERO_STEP_INITTING: hero is initting
		HERO_STEP_INIT_OK: hero has already inited the radio config, the system can go through next step		
Description: config the GPIO 	  
------------------------------------------------------------------------*/
S8 Hero_InitGPIO(void)
{
	static U8 Step=0;

	if(!TimerHasExpired(&Hero_Init_Timer))
	{
		return HERO_STEP_INITTING;
	}	

	switch(Step)
	{
		case 0:
		{
			Audio_DrvFastMuteConfig(HERO_GPIO_2,1,5,5);//init fastmute,GPIO 2=fastmute function,and release attack time are 5ms,polarity=high
			Step++;
			return(HERO_STEP_INITTING);
		}
		case 1:				   //this will be the last init step
		{
			Step=0;
			return(HERO_STEP_INIT_OK);
		}
	}
	return(HERO_STEP_INIT_OK);
}
/*-----------------------------------------------------------------------
Input:	
Output:	HERO_STEP_INITTING: hero is initting
		HERO_STEP_INIT_OK: hero has already inited the radio config, the system can go through next step		
Description: select the source input 	  
------------------------------------------------------------------------*/
S8 Hero_InitSource(void)
{
	static U8 Step=0;

	if(!TimerHasExpired(&Hero_Init_Timer))
	{
		return HERO_STEP_INITTING;
	}	

	switch(Step)
	{
		case 0:       //use the default input so no use to change
		{
			Step++;
			return(HERO_STEP_INITTING);
		}
		case 1:				   //this will be the last init step
		{
			Step=0;
			return(HERO_STEP_INIT_OK);
		}
	}
	return(HERO_STEP_INIT_OK);
}
/*-----------------------------------------------------------------------
Input:	
Output:	HERO_STEP_INITTING: hero is initting
		HERO_STEP_INIT_OK: hero has already inited the radio config, the system can go through next step		
Description: config all the audio config 	  
------------------------------------------------------------------------*/
S8 Hero_InitAudio(void)
{
	static Hero_InitStepDef Audio_InitStep=HERO_INIT_STEP_SCALING;

	if(!TimerHasExpired(&Hero_Init_Timer))
	{
		return HERO_STEP_INITTING;
	}	

	switch(Audio_InitStep)
	{
		case HERO_INIT_STEP_SCALING:
		{
			Audio_ScalingInit();  //init scaling data
			Audio_InitStep=HERO_INIT_STEP_CHANNELGAIN;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_CHANNELGAIN:
		{
			Audio_InitChannelGain(); //init every channel gain
			Audio_InitStep=HERO_INIT_STEP_PERIPHERAL;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_PERIPHERAL:
		{
			Audio_DrvPeripheralSwitch(0x28,1); //enable the ADC module
			Audio_InitStep=HERO_INIT_STEP_SOFTMUTE;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_SOFTMUTE:
		{
			Audio_DrvSoftMuteSwitch(0xFFFF,0);		//shut down all channels softmute
			Audio_InitStep=HERO_INIT_STEP_LOUDNESS;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_LOUDNESS:
		{
			Audio_LoudnessInit();//init loudness
			Audio_InitStep=HERO_INIT_STEP_VOLUME;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_VOLUME:
		{
			VolumeProcInit();//init volume and set a default volume
			Audio_InitStep=HERO_INIT_STEP_GSA;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_GSA:
		{
			Audio_GSAInit();//graphical spectrum analyzer init
			Audio_InitStep=HERO_INIT_STEP_GEQ;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_GEQ:
		{
			Audio_SetEQMode(EQMode);//set the graphical EQ mode to flat
			Audio_InitStep=HERO_INIT_STEP_PEQ;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_PEQ:
		{
			Audio_PEQInit();//init every channels parametric EQ
			Audio_InitStep=HERO_INIT_STEP_DCFilter;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_DCFilter:
		{
			Audio_DCFilterInit();//DC filter init config
			Audio_InitStep=HERO_INIT_STEP_NavPhoneFilter;
			return(HERO_STEP_INITTING);
		}			 
		case HERO_INIT_STEP_NavPhoneFilter:
		{
			Audio_NavPhoneFilterInit();//navigation and phone channels filter init
			///Audio_SourceInputSel(NavigationStr,INPUT_SOURCE_SPDIF,0);
			Audio_ImposeNavOnFront(0);
			Audio_DrvSetVolume(NavigationStr,-10);
			Audio_InitStep=HERO_INIT_STEP_CompExp;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_CompExp:
		{
			Audio_CompExpInit();//compressor and expander init
			Audio_InitStep=HERO_INIT_STEP_Beep;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_Beep:
		{
			BeepInit();//beep init
			Audio_InitStep=HERO_INIT_STEP_CLICKCLACK;
			return(HERO_STEP_INITTING);
		}
		#if 0
		case HERO_INIT_STEP_CLICKCLACK:
		{
			ClickClack_Init();//click-clack init
			Audio_InitStep=HERO_INIT_STEP_POLYCHIME;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_POLYCHIME:
		{
			AudioPolyChimeInit();//poly chime init
			Audio_InitStep=HERO_INIT_STEP_DELAY;
			return(HERO_STEP_INITTING);
		}
		#else 
		case HERO_INIT_STEP_CLICKCLACK:
		{
			ClickClack_Init();//click-clack init
			Audio_InitStep=HERO_INIT_STEP_DELAY;
			return(HERO_STEP_INITTING);
		}		
		#endif
		case HERO_INIT_STEP_DELAY:
		{
			Audio_DelayInit();//delay init
			Audio_InitStep=HERO_INIT_STEP_END;
			return(HERO_STEP_INITTING);
		}
		case HERO_INIT_STEP_END:				   //this will be the last init step
		{
			Audio_InitStep=HERO_INIT_STEP_SCALING;
			return(HERO_STEP_INIT_OK);
		}
		default:
		{
			Audio_InitStep=HERO_INIT_STEP_SCALING;
			return(HERO_STEP_INIT_OK);
		}
	}
}
/*-----------------------------------------------------------------------
Input:	
Output:	HERO_STEP_INITTING: hero is initting
		HERO_STEP_INIT_OK: hero has already inited the radio config, the system can go through next step
Description: config radio parameters 	  
------------------------------------------------------------------------*/
S8 Hero_InitRadio(void)
{
	static U8 Step=0;

	if(!TimerHasExpired(&Hero_Init_Timer))
	{
		return HERO_STEP_INITTING;
	}	

	switch(Step)
	{
	    case 0:
		{
			Hero_RadioConfigInit();//radio special function set
			Step++;
			return(HERO_STEP_INITTING);
		}
	    case 1:
		{
			Radio_Para_WriteAll();//write config data to here
			Step++;
			return(HERO_STEP_INITTING);
		}
		case 2:				   //this will be the last init step
		{
			Step=0;
			return(HERO_STEP_INIT_OK);
		}
	}
	return(HERO_STEP_INIT_OK);
}
/*-----------------------------------------------------------------------
Input:	 OnOff: 0=off,others=on
Output:	
Description: 	hero power switch  
------------------------------------------------------------------------*/
void Hero_PowerSwitch(U8 OnOff)
{/*
	if(OnOff)
	{
	//power on not call this 
	}
	else
	{
		Hero_ShutDownAllGen();    //must shut down all the generator before power down
		TimerDelayMs(30);         //wait for 30ms for the generator to shut down
		Tuner_Stop();             //hero shut down
		F_Hero_InitOK=0;
	}*/
}
/*-----------------------------------------------------------------------
Input:	
Output:	
Description: 	shut down all the generator  
------------------------------------------------------------------------*/
void Hero_ShutDownAllGen(void)
{
	Audio_DrvDisableChime();
	Audio_DrvDisableClickClack();  
	Audio_DrvPolyChimeStop();
	Audio_DrvSinGenSwitch(0);  //shut down sin and noise generator, sin and noise use the same disable
}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: warm boot init
------------------------------------------------------------------------*/
void Hero_WarmBootInit(void)
{
	static Hero_InitStepDef Hero_WarmBootInitStep=HERO_INIT_STEP_AUDIO_IMPROVE_A;

	if(!TimerHasExpired(&Hero_Init_Timer)||F_Hero_InitOK == 1)
		return;
	F_Hero_InitOK=0;
	switch(Hero_WarmBootInitStep)
	{
		case HERO_INIT_STEP_AUDIO_IMPROVE_A:
			Hero_Init_ImproveAudioSettingA();   	//improve audio cmd for V1C 
			TimerSet(&Hero_Init_Timer,2);      		 //wait to ensure the time between this cmd and improve audio cmd B
			Hero_WarmBootInitStep = HERO_INIT_STEP_RADIO_IMPROVE_A;
			break;
		case HERO_INIT_STEP_RADIO_IMPROVE_A: 	// hero init data
			Hero_Init_ImproveRadioSettingA(); 	//improve radio cmd for V1C   
			TimerSet(&Hero_Init_Timer,2);        	//wait to ensure the time between this cmd and improve radio cmd B
			Hero_WarmBootInitStep = HERO_INIT_STEP_ACTIVE_DEVICE;
			break;
		case HERO_INIT_STEP_ACTIVE_DEVICE:
			Hero_AcitveDevice();//A preset or search tuning action is needed to get the Hero out of power down mode
			Hero_WarmBootInitStep = HERO_INIT_STEP_RADIO_IMPROVE_B;
			break;
		case HERO_INIT_STEP_RADIO_IMPROVE_B:
			Hero_Init_ImproveRadioSettingB();//improve radio cmd for V1C   
			Hero_WarmBootInitStep = HERO_INIT_STEP_AUDIO_IMPROVE_B;
			break;
		case HERO_INIT_STEP_AUDIO_IMPROVE_B:
			Hero_Init_ImproveAudioSettingB();//improve audio cmd for V1C   
			Hero_WarmBootInitStep = HERO_INIT_STEP_DAC;
			break;			
		case HERO_INIT_STEP_DAC:
			Audio_DrvPeripheralSwitch(PERIOHERALID_FDAC,1);	 //enable the front DAC
			Audio_DrvPeripheralSwitch(PERIOHERALID_RDAC,1);	 //enable the rear DAC
			Hero_WarmBootInitStep=HERO_INIT_STEP_PERIPHERAL;
			break;
		case HERO_INIT_STEP_PERIPHERAL:
			if(Hero_InitPeripheral()==HERO_STEP_INIT_OK)//config the audio input mode
			Hero_WarmBootInitStep = HERO_INIT_STEP_GPIO;
			break;
		case HERO_INIT_STEP_GPIO:
//			Hero_InitGPIO();//config the GPIO function
			Hero_WarmBootInitStep = HERO_INIT_STEP_SOURCE;
			break;
		case HERO_INIT_STEP_SOURCE:
//			if(Hero_InitSource()==HERO_STEP_INIT_OK)//config the input source
				Hero_WarmBootInitStep = HERO_INIT_STEP_AUDIO;
			break;
		case HERO_INIT_STEP_AUDIO:
//			if(Hero_InitAudio()==HERO_STEP_INIT_OK)//init the ADSP module functions
				Hero_WarmBootInitStep = HERO_INIT_STEP_RADIO;
			break;	
		case HERO_INIT_STEP_RADIO:
//			if(Hero_InitRadio()==HERO_STEP_INIT_OK)//init tuner parameters
				Hero_WarmBootInitStep = HERO_INIT_STEP_AudioImproveC;
			break;	
		case HERO_INIT_STEP_AudioImproveC:
			Hero_Init_ImproveAudioSettingC();//improve audio cmd for V1C   
			Hero_WarmBootInitStep = HERO_INIT_STEP_END;
			break;			
		case HERO_INIT_STEP_END:
			Hero_WarmBootInitStep = HERO_INIT_STEP_AUDIO_IMPROVE_A;
			F_Hero_InitOK=1;
			break;
		default:
			Hero_WarmBootInitStep = HERO_INIT_STEP_AUDIO_IMPROVE_A;
			break;
	}
}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: delay init
------------------------------------------------------------------------*/
void Audio_DelayInit(void)
{
	U16 Channel;

	//all channels
	Channel=TEF663x_PFLCHANNEL+TEF663x_PFRCHANNEL+TEF663x_PRLCHANNEL
		+TEF663x_PRRCHANNEL+TEF663x_SWLCHANNEL+TEF663x_SWRCHANNEL;
	Audio_DrvSetDelay(Channel,0);//all channel delay 0ms	
}


