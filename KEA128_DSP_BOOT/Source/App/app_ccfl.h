///=========================
#ifndef __APP_CCFL_H
#define __APP_CCFL_H


#define MIN_BRIGHTNESS                0
#define MAX_BRIGHTNESS               20
#define DEF_ILL_BRIGHTNESS           0
#define DEF_USER_BRIGHTNESS          15


///============================================================================================CCFL_POWER========      
///#define CCFL_POWER_CTL		    
#define IO_CCFL_POWER               GPIOA
#define GPIO_CCFL_POWER_CTL      	GPIO_PTD4//////
#define TurnOn_CCFL_POWER        	GPIO_PinSet(GPIO_CCFL_POWER_CTL)
#define TurnOff_CCFL_POWER       	GPIO_PinClear(GPIO_CCFL_POWER_CTL)
///===================CCFL_POWER END========================= 

extern void CCFL_Power_Init(void);
extern void CCFL_Power_Ctl(SCH_BOOL OnOff);
extern void CcflInit(void);
extern SCH_U8 GetBrightness(void);
extern void SetBrightness(SCH_U8 data);
extern void SetCCFL_DelayTime(SCH_U8 time);
extern void TASK_CCFL_Pro(void);
#endif
