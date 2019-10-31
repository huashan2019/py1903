///=========================
#ifndef __APP_MUTE_H
#define __APP_MUTE_H



typedef enum  
{
	SOFTON,
	SOFTOFF,
	HARDON,
	HARDOFF
}MUTE_T;

typedef enum
{
	MUTE_IDLE_STATE,
	MUTE_REVERSING,
	WAIT_REVERSE_END,
	MUTE_UNMUTE_STATE
}MUTE_STATE;
extern MUTE_STATE Mute_State;



extern void AudioMute(MUTE_T MUTE_Type);
extern void TASK_Mute_Pro(void);

#endif
