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
///#define IO_ACC                  GPIOC
#define GPIO_ACC_DECT           GPIO_PTI6///ACC_DECT
#define ACC_DET_LEVEL           GPIO_ReadInputPin(GPIO_ACC_DECT)
#define ACC_DET_LVON            (HIGH_LEVEL==ACC_DET_LEVEL)
#define ACC_DET_LVOFF           (LOW_LEVEL==ACC_DET_LEVEL)
extern void ACC_Init(void);
extern void ACC_Detect(void);
///===================ACC END============================

///=========================================================================================REV===========
///#define IO_REV                  GPIOC
#define GPIO_REV_DECT           GPIO_PTI3///
#define REV_DET_LEVEL           GPIO_ReadInputPin(GPIO_REV_DECT)
#define REV_DET_LVON            (LOW_LEVEL==REV_DET_LEVEL)
#define REV_DET_LVOFF           (HIGH_LEVEL==REV_DET_LEVEL)
extern void REV_Init(void);
extern void REV_Detect(void);
///===================REV END============================

///=========================================================================================CAM===========
#define IO_CAM_PWR              GPIOB
#define GPIO_CAM_PWR_CTL      	GPIO_PTG3///
#define TurnOn_CAM_PWR        	GPIO_PinSet(GPIO_CAM_PWR_CTL)
#define TurnOff_CAM_PWR       	GPIO_PinClear(GPIO_CAM_PWR_CTL)
///===================CAM END============================
///=========================================================================================ILL============
///#define IO_ILL                  GPIOA
#define GPIO_ILL_DECT           GPIO_PTD0///
#define ILL_DET_LEVEL           GPIO_ReadInputPin(GPIO_ILL_DECT)
#define ILL_DET_LVON            (LOW_LEVEL==ILL_DET_LEVEL)
#define ILL_DET_LVOFF           (HIGH_LEVEL==ILL_DET_LEVEL)
extern void ILL_Init(void);
extern void ILL_Detect(void);
///===================ILL END============================
 
///=====================================================================================PARK --  SPEED========
///#define IO_PARK               GPIOA
#define GPIO_PARK_DECT           GPIO_PTD1///
#define PARK_DET_LEVEL           GPIO_ReadInputPin(GPIO_PARK_DECT)
#define PARK_DET_LVON            (LOW_LEVEL==PARK_DET_LEVEL)
#define PARK_DET_LVOFF           (HIGH_LEVEL==PARK_DET_LEVEL)
extern void PARK_Init(void);
extern void PARK_Detect(void);
///===================PARK END============================

///===========================================================================================MIC==============
///#define IO_MIC                  GPIOB
#define GPIO_MIC_DECT           GPIO_PTH0///
#define MIC_DET_LEVEL           GPIO_ReadInputPin(GPIO_MIC_DECT)
#define MIC_DET_LVON            (LOW_LEVEL==MIC_DET_LEVEL)
#define MIC_DET_LVOFF           (HIGH_LEVEL==MIC_DET_LEVEL)
///===================ACC END============================

///===========================================================================================AUX==============
///#define IO_AUX                  GPIOE
#define GPIO_AUX_DECT          
#define AUX_DET_LEVEL           GPIO_ReadInputPin(GPIO_AUX_DECT)
#define AUX_DET_LVON            (LOW_LEVEL==AUX_DET_LEVEL)
#define AUX_DET_LVOFF           (HIGH_LEVEL==AUX_DET_LEVEL)
///===================AUX END============================
///===========================================================================================POWER_KEY========
///#define IO_POWER_KEY             GPIOC
#define GPIO_POWER_KEY_DECT         GPIO_PTI5///
#define POWER_KEY_DET_LEVEL         GPIO_ReadInputPin(GPIO_POWER_KEY_DECT)
#define POWER_KEY_DET_LVON          (LOW_LEVEL==POWER_KEY_DET_LEVEL)
#define POWER_KEY_DET_LVOFF         (HIGH_LEVEL==POWER_KEY_DET_LEVEL)
extern void POWER_KEY_Init(void);
#define IO_POWER_LED                GPIOA
#define GPIO_POWER_LED_CTL      	GPIO_PTC0//////
#define TurnOn_POWER_LED        	GPIO_PinSet(GPIO_POWER_LED_CTL)
#define TurnOff_POWER_LED       	GPIO_PinClear(GPIO_POWER_LED_CTL)
extern void LED_Power_Ctl(void);
///===================POWER_KEY END============================

///=================================================================================================USB=========
///#define IO_USB                 GPIOC
#define GPIO_USB_OC_DECT          GPIO_PTI6///
#define USB_OC_LEVEL              GPIO_ReadInputPin(GPIO_USB_OC_DECT)
#define USB_OC_LVON               (LOW_LEVEL==USB_OC_LEVEL)
#define USB_OC_LVOFF              (HIGH_LEVEL==USB_OC_LEVEL)
///#define IO_USB                 GPIOA
#define GPIO_USB_EN_CTL           GPIO_PTB4///
#define USB_EN_ON                 GPIO_PinSet(GPIO_USB_EN_CTL)
#define USB_EN_OFF                GPIO_PinClear(GPIO_USB_EN_CTL)
///#define IO_USB1                GPIOB
#define GPIO_USB1_OC_DECT         GPIO_PTE5///
#define USB1_OC_LEVEL             GPIO_ReadInputPin(GPIO_USB1_OC_DECT)
#define USB1_OC_LVON              (LOW_LEVEL==USB1_OC_LEVEL)
#define USB1_OC_LVOFF             (HIGH_LEVEL==USB1_OC_LEVEL)
///#define IO_USB1                GPIOA
#define GPIO_USB1_EN_CTL          GPIO_PTB5///
#define USB1_EN_ON                GPIO_PinSet(GPIO_USB1_EN_CTL)
#define USB1_EN_OFF               GPIO_PinClear(GPIO_USB1_EN_CTL)
///#define IO_USB_POWER           GPIOB
#define GPIO_USB_POWER_CTL        GPIO_PTE6///
#define USB_POWER_ON()            GPIO_PinSet(GPIO_USB_POWER_CTL)
#define USB_POWER_OFF()           GPIO_PinClear(GPIO_USB_POWER_CTL)
///===================USB END============================

///============================================================================================KEY_LED===========      
///#define KEY_LED_CTL		    (GPIOH->ODR & GPIO_KEY_LED_CTL)
#define IO_KEY_LED              GPIOA
#define GPIO_KEY_LED_CTL      	GPIO_PTC0//////
#define TurnOn_KEY_LED        	GPIO_PinSet(GPIO_KEY_LED_CTL)
#define TurnOff_KEY_LED       	GPIO_PinClear(GPIO_KEY_LED_CTL)
extern void LED_Key_Ctl(void);
///===================KEY_LED END========================= 
///==============================================================================================DSP========      
///#define DSP_RESET_CTL		    
#define IO_DSP_RESET                GPIOB
#define GPIO_DSP_RESET_CTL          GPIO_PTF1//////
#define DSP_RESET_RELEASE        	GPIO_PinSet(GPIO_DSP_RESET_CTL)
#define DSP_RESET_HOLD              GPIO_PinClear(GPIO_DSP_RESET_CTL) 
///#define AD1938_RESET_CTL		    
#define IO_AD1938_RESET             GPIOA
#define GPIO_AD1938_RESET_CTL       GPIO_PTA6//////
#define AD1938_RESET_RELEASE        GPIO_PinSet(GPIO_AD1938_RESET_CTL)
#define AD1938_RESET_HOLD           GPIO_PinClear(GPIO_AD1938_RESET_CTL)
///#define IO_AUDIO               GPIOA
#define GPIO_AUDIO_DECT           GPIO_PTD6///
#define AUDIO_DET_LEVEL           GPIO_ReadInputPin(GPIO_AUDIO_DECT)
#define AUDIO_DET_LVON            (LOW_LEVEL==AUDIO_DET_LEVEL)
#define AUDIO_DET_LVOFF           (HIGH_LEVEL==AUDIO_DET_LEVEL)
///#define 		    
#define IO_SPI_SS                GPIOB
#define GPIO_SPI_SS_CTL          GPIO_PTG7//////
#define SPI_SS_RELEASE           GPIO_PinSet(GPIO_SPI_SS_CTL)
#define SPI_SS_HOLD              GPIO_PinClear(GPIO_SPI_SS_CTL)
///#define IO_REM_EN               GPIOA
#define GPIO_REM_EN_CTL      	    GPIO_PTC2///
#define TurnOn_REM_EN        	    GPIO_PinSet(GPIO_REM_EN_CTL)
#define TurnOff_REM_EN       	    GPIO_PinClear(GPIO_REM_EN_CTL)
///===================DSP END========================================  

///==============================================================================================BT========
///#define DSP_RESET_CTL		    
#define IO_BT_RESET                GPIOA
#define GPIO_BT_RESET_CTL          GPIO_PTA7//////
#define BT_RESET_RELEASE           GPIO_PinSet(GPIO_BT_RESET_CTL)
#define BT_RESET_HOLD              GPIO_PinClear(GPIO_BT_RESET_CTL)
///===================BT END========================================   


///==============================================================================================ARM========      
///#define ARM_RESET_CTL		    
#define IO_ARM_RESET                GPIOA
#define GPIO_ARM_RESET_CTL          GPIO_PTC2//////
#define ARM_RESET_RELEASE        	GPIO_PinSet(GPIO_ARM_RESET_CTL)
#define ARM_RESET_HOLD              GPIO_PinClear(GPIO_ARM_RESET_CTL) 	    
///#define ARM_REV_CTL	---   mcu to arm rev	    
#define IO_ARM_REV                  GPIOA
#define GPIO_ARM_REV_CTL            GPIO_PTD5//////
#define ARM_REV_ON             	    GPIO_PinSet(GPIO_ARM_REV_CTL)
#define ARM_REV_OFF       	        GPIO_PinClear(GPIO_ARM_REV_CTL)  
///#define IO_NAVI_GATE             GPIOA
#define GPIO_NAVI_GATE_DECT         GPIO_PTD6///
#define NAVI_GATE_LEVEL             GPIO_ReadInputPin(GPIO_NAVI_GATE_DECT)
#define NAVI_GATE_LVON              (LOW_LEVEL==NAVI_GATE_LEVEL)
#define NAVI_GATE_LVOFF             (HIGH_LEVEL==NAVI_GATE_LEVEL)
///#define IO_ARM_WORK              GPIOA
#define GPIO_ARM_WORK_DECT          GPIO_PTD7///
#define ARM_WORK_LEVEL              GPIO_ReadInputPin(GPIO_ARM_WORK_DECT)
#define ARM_WORK_ERROR              (LOW_LEVEL==ARM_WORK_LEVEL)
#define ARM_WORK_NORMAL             (HIGH_LEVEL==ARM_WORK_LEVEL)
///#define IO_ARM_PWR               GPIOA
#define GPIO_ARM_PWR_CTL      	    GPIO_PTC0///
#define TurnOn_ARM_PWR        	    GPIO_PinSet(GPIO_ARM_PWR_CTL)
#define TurnOff_ARM_PWR       	    GPIO_PinClear(GPIO_ARM_PWR_CTL)
extern void NAVI_GATE_Detect(void);  
extern void ARM_WORK_GATE_Detect(void);
///========================================ARM END========================= 

///==============================================================================================SYS PWR========      
///#define IO_SYS_POWER             GPIOA
#define GPIO_SYS_POWER_CTL      	GPIO_PTB2//////
#define TurnOn_SYS_POWER        	GPIO_PinSet(GPIO_SYS_POWER_CTL)
#define TurnOff_SYS_POWER       	GPIO_PinClear(GPIO_SYS_POWER_CTL)
extern void SYS_Power_Ctl(SCH_BOOL OnOff);
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

///===============================================================================================G4 G3=========      
///#define G4_POWER_CTL		    
#define IO_G4_POWER                 GPIOB
#define GPIO_G4_POWER_CTL           GPIO_PTF1//////
#define G4_POWER_ON()        	    GPIO_PinSet(GPIO_G4_POWER_CTL)
#define G4_POWER_OFF()       	    GPIO_PinClear(GPIO_G4_POWER_CTL)
///#define G4_RESET_CTL		    
#define IO_G4_RESET                 GPIOA
#define GPIO_G4_RESET_CTL           GPIO_PTA7//////
#define G4_RESET_RELEASE        	GPIO_PinSet(GPIO_G4_RESET_CTL)
#define G4_RESET_HOLD       	    GPIO_PinClear(GPIO_G4_RESET_CTL)
///#define G3_WAKEUP_CTL		    
#define IO_G3_WAKEUP                GPIOB
#define GPIO_G3_WAKEUP_DECT         GPIO_PTE4//////
#define G3_WAKEUP_DET_LEVEL         GPIO_ReadInputPin(GPIO_G3_WAKEUP_DECT)
#define G3_WAKEUP_DET_LVON          (LOW_LEVEL==G3_WAKEUP_DET_LEVEL)
#define G3_WAKEUP_DET_LVOFF         (HIGH_LEVEL==G3_WAKEUP_DET_LEVEL)
///===================G4 END========================= 

///================================================================================================GPS=======      
///#define GPS_POWER_CTL		    
#define IO_GPS_POWER                GPIOA
#define GPIO_GPS_POWER_CTL          GPIO_PTD3//////
#define GPS_POWER_ON        	    GPIO_PinSet(GPIO_GPS_POWER_CTL)
#define GPS_POWER_OFF       	    GPIO_PinClear(GPIO_GPS_POWER_CTL)
///===================GPS END========================= 

///===============================================================================================TUNER=======      
///#define TUNER_RESET_CTL		
#define IO_TUNER_RESET           GPIOA
#define GPIO_TUNER_RESET_CTL     GPIO_PTD2//////
#define TUNER_RESET_RELEASE      GPIO_PinSet(GPIO_TUNER_RESET_CTL)
#define TUNER_RESET_HOLD       	 GPIO_PinClear(GPIO_TUNER_RESET_CTL)
#define GPIO_TUNER_POWER_CTL     GPIO_PTG2
#define TUNER_POWER_ON()         GPIO_PinSet(GPIO_TUNER_POWER_CTL)
#define TUNER_POWER_OFF()        GPIO_PinClear(GPIO_TUNER_POWER_CTL) 
///#define ANT_CTL		    
#define IO_ANT                GPIOB
#define GPIO_ANT_CTL          GPIO_PTG1//////
#define TurnOn_ANT        	  GPIO_PinSet(GPIO_ANT_CTL)
#define TurnOff_ANT       	  GPIO_PinClear(GPIO_ANT_CTL)
#define GPIO_ANT_DET          GPIO_PTA6//////
#define ANT_DET_LEVEL         GPIO_ReadInputPin(GPIO_ANT_DET)
#define ANT_DET_LVON          (LOW_LEVEL==ANT_DET_LEVEL)
#define ANT_DET_LVOFF         (HIGH_LEVEL==ANT_DET_LEVEL)
///===================TUNER END========================= 

///==========================================================================================AMP===¹¦·Å========      
///#define AMP_CTL		    
#define IO_AMP                GPIOB
#define GPIO_AMP_CTL          GPIO_PTE5//////
#define TurnOn_AMP        	  GPIO_PinSet(GPIO_AMP_CTL)
#define TurnOff_AMP       	  GPIO_PinClear(GPIO_AMP_CTL)
///#define AMP_BEEP		    
#define IO_AMP_BEEP           GPIOA
#define GPIO_AMP_BEEP         GPIO_PTC5//////
#define TurnOn_AMP_BEEP       GPIO_PinSet(GPIO_AMP_BEEP)
#define TurnOff_AMP_BEEP      GPIO_PinClear(GPIO_AMP_BEEP)
///#define IO_AMP_DET         GPIOB
#define GPIO_AMP_DECT         GPIO_PTB5///
#define AMP_DET_LEVEL         GPIO_ReadInputPin(GPIO_AMP_DECT)
#define AMP_DET_LVON          (LOW_LEVEL==AMP_DET_LEVEL)
#define AMP_DET_LVOFF         (HIGH_LEVEL==AMP_DET_LEVEL)
///===================AMP END========================= 

///==============================================================================================MUTE==========      
///#define MUTE_CTL            (GPIOH->ODR & GPIO_MUTE_CTL)
#define IO_MUTE                GPIOB
#define GPIO_MUTE_CTL      	   GPIO_PTE2//////
#define TurnOn_MUTE        	   GPIO_PinSet(GPIO_MUTE_CTL)
#define TurnOff_MUTE       	   GPIO_PinClear(GPIO_MUTE_CTL)
extern void MUTE_Ctl(SCH_BOOL OnOff);
///===================MUTE END========================= 

///==============================================================================================B+_DET==========      
#define IO_VOL_DET             GPIOC
#define GPIO_VOL_DET      	   GPIO_PTI4//////
#define VOL_DET_LEVEL          GPIO_ReadInputPin(GPIO_VOL_DET)
#define VOL_DET_LVON           (LOW_LEVEL==GPIO_VOL_DET)
#define VOL_DET_LVOFF          (HIGH_LEVEL==GPIO_VOL_DET)
///===================B+_DET END========================= 


extern void GPIOInit(void);
#endif
