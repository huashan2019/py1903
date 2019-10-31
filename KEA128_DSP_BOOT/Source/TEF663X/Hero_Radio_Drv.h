#ifndef _HERO_RADIO_DRV_H

#define _HERO_RADIO_DRV_H

//============mac define==================//



#define Radio_ReadBufferLen 0x16

/*tuner read address define*/
#define Radio_STATUS 0
#define Radio_LEVEL 1
#define Radio_USN 2
#define Radio_WAM 3
#define Radio_FOF 4
#define Radio_IFBW 5
#define Radio_MOD 6
#define Radio_AGCATT 0x12
#define TEF663x_IDENT 0x15 

/*hero tuner mode */
#define Radio_POWERDOWNMODE 0
#define Radio_PRESETMODE 0x10
#define Radio_SEARCHMODE 0x20
#define Radio_AFUPDATEMODE 0x30
#define Radio_JUMPMODE 0x40
#define Radio_CHECKMODE 0x50


extern void RadioDrv_Stop(void);
extern void RadioDrv_UpdatePll(U8 Mode,U8 Band,U16 Pll);

extern void RadioDrv_ReadData(U8 Num);
extern U8 RadioDrv_GetData(U8 Addr);
extern U8 RadioDrv_CheckStereoIndicator(void);

#endif
