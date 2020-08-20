///=========================
#ifndef __MW_GPIO_H
#define __MW_GPIO_H

typedef struct 
{
	SCH_U8 IO_Status;
	SCH_U8 IO_SamplingCounter;
}IO_DET_T;

extern SCH_BOOL GPIO_ReadInputPin(GPIO_PinType GPIO_Pin);
///=======================================================================================ACC===========
///#define IO_DSP_TEST                 GPIOC
#define GPIO_DSP_TEST_DECT           GPIO_PTI5///
#define DSP_TEST_DET_LEVEL           GPIO_ReadInputPin(GPIO_DSP_TEST_DECT)
#define DSP_TEST_DET_LVON            (LOW_LEVEL==DSP_TEST_DET_LEVEL)
#define DSP_TEST_DET_LVOFF           (HIGH_LEVEL==DSP_TEST_DET_LEVEL)
extern SCH_BOOL DSP_OFF_FLAG;
extern void DSP_TEST_Init(void);
extern void DSP_Test_Detect(void);
extern SCH_BOOL AMP_OFF_FLAG;
extern SCH_BOOL AMP1_OFF_FLAG;

extern void AMP_Init(void);
extern void AMP_Diag_Detect(void);
///===================ACC END============================


///==============================================================================================BT========
///#define DSP_RESET_CTL		    
#define IO_BT_RESET                GPIOA
#define GPIO_BT_RESET_CTL          GPIO_PTA7//////
#define BT_RESET_RELEASE           GPIO_PinSet(GPIO_BT_RESET_CTL)
#define BT_RESET_HOLD              GPIO_PinClear(GPIO_BT_RESET_CTL)
///===================BT END========================================   



///==============================================================================================SYS PWR========      
///#define IO_SYS_POWER             GPIOA
#define GPIO_SYS_POWER_CTL      	GPIO_PTB2//////
#define TurnOn_SYS_POWER        	GPIO_PinSet(GPIO_SYS_POWER_CTL)
#define TurnOff_SYS_POWER       	GPIO_PinClear(GPIO_SYS_POWER_CTL)



#define GPIO_PRE_AMP_CTL      	GPIO_PTE5//////
#define TurnOn_PRE_AMP        	GPIO_PinSet(GPIO_PRE_AMP_CTL)
#define TurnOff_PRE_AMP       	GPIO_PinClear(GPIO_PRE_AMP_CTL)


extern void SYS_Power_Ctl(SCH_BOOL OnOff);
///#define IO_ACC_EN             GPIOB
#define GPIO_ACC_EN_CTL      	GPIO_PTF5//////
#define TurnOn_ACC_EN        	GPIO_PinSet(GPIO_ACC_EN_CTL)
#define TurnOff_ACC_EN       	GPIO_PinClear(GPIO_ACC_EN_CTL)
extern void ACC_EN_Ctl(SCH_BOOL OnOff);
///========================================SYS PWR END========================= 

///=============================================================================================SYS_3V3=======
#define IO_SYS_3V3              GPIOB
#define GPIO_SYS_3V3_CTL      	GPIO_PTH7//////
#define SYS_3V3_ON          	GPIO_PinSet(GPIO_SYS_3V3_CTL)
#define SYS_3V3_OFF         	GPIO_PinClear(GPIO_SYS_3V3_CTL)
///===================ACC_3V3 END========================= 

///=============================================================================================SYS_12V=======
#define IO_SYS_12V                  GPIOB
#define GPIO_SYS_12V_CTL      	    GPIO_PTE1
#define SYS_12V_ON        	        GPIO_PinSet(GPIO_SYS_12V_CTL)
#define SYS_12V_OFF       	        GPIO_PinClear(GPIO_SYS_12V_CTL)
///===================SYS_12V END========================= 

///==========================================================================================AMP===¹¦·Å========      
///#define AMP_CTL		    
#define IO_AMP                GPIOB
#define GPIO_AMP_CTL          GPIO_PTF7//////
#define TurnOn_AMP_STBY        	  GPIO_PinSet(GPIO_AMP_CTL)
#define TurnOff_AMP_STBY       	  GPIO_PinClear(GPIO_AMP_CTL)
///#define AMP_BEEP		    
#define IO_AMP_BEEP           GPIOA
#define GPIO_AUDIO_SWITCH         GPIO_PTC5//////
#define TurnOn_AUDIO_SWITCH       GPIO_PinSet(GPIO_AUDIO_SWITCH)
#define TurnOff_AUDIO_SWITCH      GPIO_PinClear(GPIO_AUDIO_SWITCH)
///#define IO_AMP_DET         GPIOB
#define GPIO_AMP_DECT         GPIO_PTH1///
#define AMP_DET_LEVEL         GPIO_ReadInputPin(GPIO_AMP_DECT)
#define AMP_DET_LVON          (HIGH_LEVEL==AMP_DET_LEVEL)
#define AMP_DET_LVOFF         (LOW_LEVEL==AMP_DET_LEVEL)

#define GPIO_AMP1_DECT         GPIO_PTH1///
#define AMP1_DET_LEVEL         GPIO_ReadInputPin(GPIO_AMP1_DECT)
#define AMP1_DET_LVON          (HIGH_LEVEL==AMP1_DET_LEVEL)
#define AMP1_DET_LVOFF         (LOW_LEVEL==AMP1_DET_LEVEL)



///===================AMP END========================= 



extern void GPIOInit(void);
#endif
