///=========================
#ifndef __APP_RADIO_TEF663X_H
#define __APP_RADIO_TEF663X_H

#define BAD_STOP_LEVEL		1
#define BAD_STOP_USN			2
#define BAD_STOP_WAM			3
#define ALL_QUAL_Good			4
#define BAD_OFFSET				5
#define BAD_STOP_SNR			6
///#define NO_STATION			7

#define NO_IFC_Result_Availble		0
#define BAD_IFC					       1
#define GOOD_IFC					2

#define NXP663X_FM_STOP_USN				70//��̨Խ�࣬��ֵԽ��
#define NXP663X_FM_STOP_WAM				60//��̨Խ�࣬��ֵԽ��
#define NXP663X_FM_STOP_IFCResult		10//Offset//��̨Խ�࣬��ֵԽ��30//60//��̨Խ�࣬��ֵԽ��

#define NXP663X_AM_STOP_WAM				0x00//0x05
#define NXP663X_AM_STOP_USN				0x00//0x05
#define NXP663X_AM_STOP_IFCResult		15//Offset

#define NXP663X_IFC_Counter_Value(register)				(register&0x7F)
#define NXP663X_IFC_Available_Measure_Time(register)		((register&0xE0)>>5)
#define NXP663X_Stereo_Indicator(register)				(register&0x08)
#define NXP663X_IFC_Cn_Value(register)					((register&0x20)>>5)

#define NXP663X_DB(x)	((U8)((x+8)*2))	
#define  NXP663X_FM_Loc_Default         NXP663X_DB(30)//����   (-8db ~ 119.5db)
#define  NXP663X_FM_Dx_Default         NXP663X_DB(24)//Զ��/  NXP663X_DB(18)//Զ��
#define  NXP663X_MW_Loc_Default         NXP663X_DB(60)//
#define  NXP663X_MW_Dx_Default          NXP663X_DB(55)///NXP663X_DB(35);//(55)//

typedef struct Radio_struct {
	SCH_U8 TunerBuff[40];
	SCH_U8 ReadBuff[15];////byte 0: status byte1: level byte2: usn_wam byte3: IF counter u84: ID
} RadioStruct;
extern  RadioStruct Radio;





extern void Radio663X_DLL_Loader(void);


#endif
