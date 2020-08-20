///=========================
#ifndef __APP_LED_H
#define __APP_LED_H

///==============================================================================================LED==========      

#define GPIO_LED_NORMAL_CTL      	   GPIO_PTA0
#define TurnOn_NORMAL_LED        	   GPIO_PinSet(GPIO_LED_NORMAL_CTL)
#define TurnOff_NORMAL_LED       	   GPIO_PinClear(GPIO_LED_NORMAL_CTL)


#define GPIO_LED_FAIL_CTL      	   GPIO_PTD2
#define TurnOn_FAIL_LED        	   GPIO_PinSet(GPIO_LED_FAIL_CTL)
#define TurnOff_FAIL_LED       	   GPIO_PinClear(GPIO_LED_FAIL_CTL)
extern void LED_IO_Init(void);
///===================LED END========================= 

extern void TASK_LED_pro(void);


#endif
