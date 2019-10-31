/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_663x_init.c  ---  NXP   TEF663X
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.02.13
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"





SCH_U8 Hero_Reset(void)
{//hero hardware reset
	static SCH_U8 Step=0;
	switch(Step)
	{
		case 0:
		{
			TUNER_POWER_ON();
			TUNER_RESET_RELEASE;
			Step=1;
			return(HERO_STEP_INITTING);
		}
		case 1:
		{
			TUNER_RESET_HOLD;
			App_Radio.nRadioInitTimer=T96MS_8;
			Step=2;
			return(HERO_STEP_INITTING);
		}
		case 2:
		{
			TUNER_RESET_RELEASE;
			App_Radio.nRadioInitTimer=T32MS_8;
			Step=3;
			return(HERO_STEP_INITTING);
		}
		case 3:
		{
			Step=0;
			return(HERO_STEP_INIT_OK);
		}
	}
	return(HERO_STEP_INIT_OK);
}

void Hero_Init_ImproveAudioSettingA(void)
{
	SCH_U8 Buffer[6];    
    /*Prepare for (Host I2S) Spurious */
	Buffer[0]=0xE1;  
	Buffer[1]=0xC1;
	Buffer[2]=0xBC; 
	Buffer[3]=0x11;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,4);
}

void Hero_Init_ImproveRadioSettingA(void)
{
	SCH_U8 Buffer[6];    
	//Prepare for AM divider/IFAD
	Buffer[0]=0xE1;  
	Buffer[1]=0xD0;
	Buffer[2]=0x61; 
	Buffer[3]=0x23;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,4);	 //start to send data to i2c1 bus
	//Prepare for AM divider/IQC
	Buffer[0]=0xE1;  
	Buffer[1]=0xB0;
	Buffer[2]=0x56; 
	Buffer[3]=0x65;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,4);	 //start to send data to i2c1 bus
	//Set IQC
	Buffer[0]=0xF0;  
	Buffer[1]=0x00;
	Buffer[2]=0x46; 
	Buffer[3]=0x01;
	Buffer[4]=0x10;
	Buffer[5]=0x22;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus
}

void Send_keycode_Command(SCH_U8 dPAR1,SCH_U8 dPAR2,SCH_U8 dPAR3)
{
	SCH_U8 TempData[4];
	TempData[0]=0xE1;
	TempData[1]=dPAR1;
	TempData[2]=dPAR2;
	TempData[3]=dPAR3;
	Hero_WriteData(TEF663x_SlaveAddr,TempData,4);	 //start to send data to i2c1 bus	
}

SCH_U8 Hero_Keycode(void)
{//send keycode to enable the advance function
	static SCH_U8 Step=0;
	switch(Step)
	{
		case 0:
		{
			Send_keycode_Command(0x80,0x00,0x01); //all features are available within 3 hours.
			Step=1;
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
SCH_U8 Hero_ReadBack(SCH_U8 Addr,SCH_U8 Num,SCH_U8 *BufferP)
{
	SCH_U8 Buffer[2];
	if(((Addr+Num)>0x40)||(Num==0)||(BufferP==NULL))
	{
		return FALSE;
	}
	Buffer[0]=0xE0;
	Buffer[1]=Addr;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,2);	 //start to send data to i2c1 bus
	SysWaitMs(2);
	Hero_ReadData(TEF663x_SlaveAddr,BufferP,Num);
	return SUCCESS;
}

SCH_U8 Send_Usecase_Command(U8 UseCase)
{
	SCH_U8 TempData[2];
	SCH_U8 Data;
	TempData[0]=TEF663x_SUBADDR_USECASE;
	TempData[1]=UseCase<<4;
	Hero_WriteData(TEF663x_SlaveAddr,TempData,2);	 //start to send data to i2c1 bus
	Hero_ReadBack(TEF663x_SUBADDR_USECASE,1,&Data);//check if write is ok
	if(Data==TempData[1])
	{
		return HERO_STEP_INIT_OK;
	}
	return HERO_STEP_INITTING;
} 

SCH_U8 Hero_AcitveDevice(void)
{///===default FM87.5
	SCH_U8 Buffer[4];
	Buffer[0]=0x00;  
	Buffer[1]=Radio_PRESETMODE;
	Buffer[2]=0x22; 
	Buffer[3]=0x2E;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,4);	 //start to send data to i2c1 bus
}
void Hero_Init_ImproveRadioSettingB(void)
{
	SCH_U8 Buffer[6];    
     /*IF ADC performance*/
	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x11; 
	Buffer[3]=0x00;
	Buffer[4]=0xDC;
	Buffer[5]=0x00;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	 
     /*Audio Rear output startup*/
	Buffer[0]=0xF2;  
	Buffer[1]=0x06;
	Buffer[2]=0xE9; 
	Buffer[3]=0x7F;
	Buffer[4]=0xFF;
	Buffer[5]=0xFF;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	 
     /* AM LO frequency*/	
	Buffer[0]=0xF0;  
	Buffer[1]=0x40;
	Buffer[2]=0x0A; 
	Buffer[3]=0x07;
	Buffer[4]=0xFF;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,5);	
	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x36;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	
	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0D; 
	Buffer[3]=0x80;
	Buffer[4]=0x00;
	Buffer[5]=0x00;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus
	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x37;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus
	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x36;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus
	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0D; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x01;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus
	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x5B;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus
	Buffer[0]=0xF8;  
	Buffer[1]=0x00;
	Buffer[2]=0x0E; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Buffer[5]=0x5A;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus
	Buffer[0]=0xF0;  
	Buffer[1]=0x40;
	Buffer[2]=0x0A; 
	Buffer[3]=0x00;
	Buffer[4]=0x00;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,5);	 //start to send data to i2c1 bus
}
void Hero_Init_ImproveAudioSettingB(void)
{
	SCH_U8 Buffer[13];    
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
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,13);	 //start to send data to i2c1 bus
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
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,13);	 //start to send data to i2c1 bus
	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x2C;
	Buffer[5]=0x00;
	Buffer[6]=0x00;
	Buffer[7]=0x07;
	Buffer[8]=0x00;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,9);	 //start to send data to i2c1 bus
	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x48;
	Buffer[5]=0x00;
	Buffer[6]=0x11;
	Buffer[7]=0x80;
	Buffer[8]=0x80;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,9);	 //start to send data to i2c1 bus
	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x50;
	Buffer[5]=0x00;
	Buffer[6]=0x10;
	Buffer[7]=0x80;
	Buffer[8]=0x80;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,9);	 //start to send data to i2c1 bus
}
void Audio_DrvPeripheralSwitch(U8 PeripheralID, U8 OnOff)
{
	SCH_U8 Buffer[3];
	Buffer[0]=0xA9;  
	Buffer[1]=PeripheralID;
	if(OnOff)
	{
		Buffer[2]=DAC_ON;
	}
	else
	{
		Buffer[2]=DAC_OFF;
	}
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,3);
}
void Audio_DrvSetSPDIFInMode(U8 Mode)
{
	SCH_U8 Buffer[3];    
	if(Mode>2)
	{
		return;
	}
	Buffer[0]=0xA9;
	Buffer[1]=0x1E;
	Buffer[2]=Mode; 
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,3);	 //start to send data to i2c1 bus	    
}

void Audio_DrvSetADInMode(U8 ADNum, U8 Mode)
{
	SCH_U8 Buffer[3];    
	if((ADNum>3)||(Mode>5))
	{
		return;
	}
	Buffer[0]=0xA9;  //subaddr of the peripheral
	Buffer[1]=ADNum;
	Buffer[2]=Mode; 
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,3);	 //start to send data to i2c1 bus
}
void Audio_DrvSetI2SInMode(U8 I2SNum, U8 Mode)
{
	SCH_U8 Buffer[3];    
	if((I2SNum>3)||(Mode>5))
	{
		return;
	}
	Buffer[0]=0xA9;  //subaddr of the peripheral
	if(I2SNum==3)
	{ 
		Buffer[1]=0x14;
	}
	else
	{
		Buffer[1]=0x0A+I2SNum;
	}
	Buffer[2]=Mode; 
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,3);	 //start to send data to i2c1 bus
}

SCH_U8 Hero_InitPeripheral(void)
{
	static U8 Step=0;
	switch(Step)
	{
		case 0:    //set the SPDIF input mode
		{
			Audio_DrvSetSPDIFInMode(1);          //analog mode
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
		case 3:
		{
			Step=0;
			return(HERO_STEP_INIT_OK);
		}
	}
	return(HERO_STEP_INIT_OK);
}
void Hero_Init_ImproveAudioSettingC(void)
{
	SCH_U8 Buffer[9];
	Buffer[0]=0xFF;  
	Buffer[1]=0xC0;
	Buffer[2]=0x60; 
	Buffer[3]=0x70;
	Buffer[4]=0x2C;
	Buffer[5]=0x00;
	Buffer[6]=0x00;
	Buffer[7]=0x01;
	Buffer[8]=0x00;
	Hero_WriteData(TEF663x_SlaveAddr,Buffer,9);
}


void TASK_663x_Hero_Init(void)
{
	if(Hero663x.F_HeroInitOk)
		return;
	if(Hero663x.Hero_Init_Timer)
	{
		Hero663x.Hero_Init_Timer--;
		if(Hero663x.Hero_Init_Timer)
			return;
	}
	switch(Hero663x.nHeroInitState)
	{
		case HERO_INIT_STEP_COLDBOOT:
			if(Hero_Reset()==HERO_STEP_INIT_OK)
			    Hero663x.nHeroInitState = HERO_INIT_STEP_AUDIO_IMPROVE_A;
			break;
		case HERO_INIT_STEP_AUDIO_IMPROVE_A:
			Hero_Init_ImproveAudioSettingA();
			Hero663x.nHeroInitState = HERO_INIT_STEP_RADIO_IMPROVE_A;
			break;
		case HERO_INIT_STEP_RADIO_IMPROVE_A: 
			Hero_Init_ImproveRadioSettingA();
			Hero663x.nHeroInitState = HERO_INIT_STEP_KEYCODE1;
			break;
		case HERO_INIT_STEP_KEYCODE1:
			if(Hero_Keycode()==HERO_STEP_INIT_OK)
				Hero663x.nHeroInitState = HERO_INIT_STEP_USECASE;
			break;		
		case HERO_INIT_STEP_USECASE:
			if(Send_Usecase_Command(1)==HERO_STEP_INIT_OK) // set the usecase 1
				Hero663x.nHeroInitState = HERO_INIT_STEP_ACTIVE_DEVICE;
			break;
		case HERO_INIT_STEP_ACTIVE_DEVICE:
			Hero_AcitveDevice();//A preset or search tuning action is needed to get the Hero out of power down mode
			Hero663x.nHeroInitState = HERO_INIT_STEP_RADIO_IMPROVE_B;
			break;
		case HERO_INIT_STEP_RADIO_IMPROVE_B:
			Hero_Init_ImproveRadioSettingB();//improve radio cmd for V1C   
			Hero663x.nHeroInitState = HERO_INIT_STEP_AUDIO_IMPROVE_B;
			break;
		case HERO_INIT_STEP_AUDIO_IMPROVE_B:
			Hero_Init_ImproveAudioSettingB();//improve audio cmd for V1C   
			Hero663x.nHeroInitState = HERO_INIT_STEP_DAC;
			break;		
		case HERO_INIT_STEP_DAC:
			Audio_DrvPeripheralSwitch(PERIOHERALID_FDAC,1);	  //enable the front DAC
			Audio_DrvPeripheralSwitch(PERIOHERALID_RDAC,1);	  //enable the rear DAC
			Hero663x.nHeroInitState=HERO_INIT_STEP_AUDIO_POINTER;
			break;		
		case HERO_INIT_STEP_AUDIO_POINTER:
			//if(Hero_InitAudioPointer()==HERO_STEP_INIT_OK)//config the audio pointer if needed
				Hero663x.nHeroInitState = HERO_INIT_STEP_PERIPHERAL;
			break;		
		case HERO_INIT_STEP_PERIPHERAL:
			if(Hero_InitPeripheral()==HERO_STEP_INIT_OK)   //config the audio input mode
				Hero663x.nHeroInitState = HERO_INIT_STEP_GPIO;
			break;
		case HERO_INIT_STEP_GPIO:
			///Hero_InitGPIO();//config the GPIO function
			Hero663x.nHeroInitState = HERO_INIT_STEP_SOURCE;
			break;		
		case HERO_INIT_STEP_SOURCE:
			//if(Hero_InitSource()==HERO_STEP_INIT_OK)//config the input source
				Hero663x.nHeroInitState = HERO_INIT_STEP_AUDIO;
			break;
		case HERO_INIT_STEP_AUDIO:
			if(Hero_InitAudio()==HERO_STEP_INIT_OK)//init the ADSP module functions
				Hero663x.nHeroInitState = HERO_INIT_STEP_RADIO;
			break;
		case HERO_INIT_STEP_RADIO:
			if(Hero_InitRadio()==HERO_STEP_INIT_OK)//init tuner parameters
				Hero663x.nHeroInitState = HERO_INIT_STEP_AudioImproveC;
			break;	
		case HERO_INIT_STEP_AudioImproveC:
			Hero_Init_ImproveAudioSettingC();//improve audio cmd for V1C   
			Hero663x.nHeroInitState = HERO_INIT_STEP_END;
			break;	
		case HERO_INIT_STEP_END:
			Hero663x.F_HeroInitOk = 1;
			Hero663x.nHeroInitState = HERO_INIT_STEP_COLDBOOT;
			break;
	}
}











