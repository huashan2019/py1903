/***********************************************************************//**
 * @file		Hero.h
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
#ifndef __HERO_INIT_H
#define __HERO_INIT_H













#define I2C1_WriteData     I2C0_Tx
#define I2C1_ReadData      I2C0_Rx
#define TimerDelayMs       SysWaitMs






extern U32 Hero_Init_Timer;


extern U8 TimerHasExpired (U32 *STimer);


extern void TimerSet(U32 *STimer, U32 TimeLength);









#define S32Y S32   //just define for update
#define S16Y S16   //just define for update

/*sub address define*/
#define TEF663x_SUBADDR_KEYCODE 0xE1
#define TEF663x_SUBADDR_USECASE 0x3F

#define TEF663X_FIXEDBOOST 48.16   //dB
#define TEF663x_LOUDBOOSTMAX 12   //dB
#define TEF663X_MAINVOLUMEMAX 12   //dB
#define TEF663x_GEQGAINMAX 12      //dB
#define TEF663x_PEQGAINMAX 6      //dB
#define TEF663x_TONEGAINMAX 12      //dB
#define TEF663x_CHANNELGAINMAX (TEF663X_FIXEDBOOST-TEF663X_MAINVOLUMEMAX-TEF663x_TONEGAINMAX-TEF663x_GEQGAINMAX-TEF663x_PEQGAINMAX)


/*hero init step define*/
typedef enum
{
	HERO_INIT_STEP_COLDBOOT=0,
	HERO_INIT_STEP_RESET, 
	HERO_INIT_STEP_RADIO_IMPROVE_A,
	HERO_INIT_STEP_AUDIO_IMPROVE_A,
	HERO_INIT_STEP_KEYCODE1,
	HERO_INIT_STEP_USECASE,
	HERO_INIT_STEP_ACTIVE_DEVICE,
	HERO_INIT_STEP_RADIO_IMPROVE_B,
	HERO_INIT_STEP_AUDIO_IMPROVE_B,
	HERO_INIT_STEP_DAC,
	HERO_INIT_STEP_AUDIO_POINTER,
	HERO_INIT_STEP_PERIPHERAL,
	HERO_INIT_STEP_GPIO,
	HERO_INIT_STEP_SOURCE,
	HERO_INIT_STEP_AUDIO,
	HERO_INIT_STEP_RADIO,
	HERO_INIT_STEP_AudioImproveC,
	HERO_INIT_STEP_LIMITER,
	HERO_INIT_STEP_TUNER,
	HERO_INIT_STEP_CLICKCLACK,
	HERO_INIT_STEP_POLYCHIME,
	HERO_INIT_STEP_DELAY,
	HERO_INIT_STEP_SCALING,
	HERO_INIT_STEP_CHANNELGAIN,
	HERO_INIT_STEP_SOFTMUTE,
	HERO_INIT_STEP_LOUDNESS,
	HERO_INIT_STEP_VOLUME,
	HERO_INIT_STEP_GEQ,
	HERO_INIT_STEP_PEQ,
	HERO_INIT_STEP_DCFilter,
	HERO_INIT_STEP_NavPhoneFilter,
	HERO_INIT_STEP_FASTMUTE,
	HERO_INIT_STEP_CompExp,
	HERO_INIT_STEP_LimiterClipDet,
	HERO_INIT_STEP_Beep,
	HERO_INIT_STEP_RadioConfig,
	HERO_INIT_STEP_GSA,
	HERO_INIT_STEP_ADC,
	HERO_INIT_STEP_END
} Hero_InitStepDef;

#define HERO_STEP_INITTING 0
#define HERO_STEP_INIT_OK 1

/*Hero init flag define*/
extern byte_field Hero_Flag0;
#define F_Hero_InitOK Hero_Flag0.data_Bbit.B0


extern void Init_Hero(void);
extern void Hero_PowerSwitch(U8 OnOff);
extern void Hero_WarmBootInit(void);
#endif



