///=========================
#ifndef __APP_MAIN_H
#define __APP_MAIN_H

typedef enum
{
	CHANNEL_IDLE,
	CHANNEL_FRONT_SOURCE_CHG,
	CHANNEL_FRONT_SOURCE_CHG_END
}CHANNEL_STATE;

typedef struct 
{
	SOURCE ChannelSource;
	CHANNEL_STATE nChannelState;
	SCH_U16 Channel_Timer;
}Channel_T;

extern Channel_T SysChannel;


typedef enum
{
	EVT_MAIN_NONE,
	EVT_SRC_POWER_ON,
	EVT_MAIN_FRONT_SOURCE_SET
}SOURCE_EVT;



extern void TASK_Main_Pro(void);
#endif
