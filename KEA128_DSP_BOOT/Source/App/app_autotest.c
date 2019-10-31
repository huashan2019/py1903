/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_autotest.c  ---   自动化测试
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL 
***************************************************************************************************
*/
#include "include.h"
SCH_U8 AUTO_TEST_DataRx[40];
SCH_U8 AUTO_TEST_DataTx[40];

///===============================================
#define HEAD_ADDRESS_AUTOTEST     0x55
#define HEAD_ADDRESS_AUTOMCU      0xaa
typedef struct
{
	SCH_U8 AutoRxData[40];
	SCH_U8 AutoRxData_Length;
	SCH_U8 AutoRxData_Flag;
	SCH_U8 AutoTxData[40];
	SCH_U8 AutoTxData_Length;
	SCH_U8 AutoTxData_Parity;
	SCH_U8 AutoCheck_ResendCounte;
	SCH_U8 AutoCheck_ResendTimer;
	SCH_U8 AutoCheck_Busy;
	SCH_U8 AutoCheck_Ack;
}AUTO_MODUEL_S;
AUTO_MODUEL_S AutoModuel; 

#define AutoRx_Head         AutoModuel.AutoRxData[0]
#define AutoRx_Head1        AutoModuel.AutoRxData[1]
#define AutoRx_Length       AutoModuel.AutoRxData[2]
#define AutoRx_MessageID    AutoModuel.AutoRxData[3]
#define AutoRx_SubID        AutoModuel.AutoRxData[4]
#define AutoRx_Data         AutoModuel.AutoRxData[5]
#define AutoTx_Head         AutoModuel.AutoTxData[0]
#define AutoTx_Head1        AutoModuel.AutoTxData[1]
#define AutoTx_Length       AutoModuel.AutoTxData[2]
#define AutoTx_MessageID    AutoModuel.AutoTxData[3]
#define AutoTx_SubID        AutoModuel.AutoTxData[4]
#define AutoTx_Data         AutoModuel.AutoTxData[5]




void AutoTest_ACK(void)
{
	SCH_U8 buf[6];
	buf[0]=HEAD_ADDRESS_AUTOTEST;
	buf[1]=HEAD_ADDRESS_AUTOMCU;
	buf[2]=0x06;
	buf[3]=AutoRx_MessageID; 
	buf[4]=AutoRx_SubID;
	buf[5]=GetCheckData_Xor(buf, 5);
	UartTxData(Uart_AUTOTEST, buf, 6);
}


#define RESPONSE_YES     0x01
#define RESPONSE_NO      0x00
void AutoDataAnalyse(void)
{
	SCH_U8 *pData=&AutoRx_Data;
	///AutoTest_ACK();
	if(AutoRx_MessageID != M2T_COMPANY_CMD 
		&& AutoRx_MessageID != M2T_RAD_TEST_CMD 
		&& AutoRx_MessageID !=M2T_EQ_TEST_CMD 
		&& AutoRx_MessageID !=M2T_OTHER_CMD)    
		return;
	if((!Get_AutoTestMode_Flag)&&(AutoRx_MessageID!=M2T_COMPANY_CMD))
		return;		
	switch(AutoRx_MessageID)
	{
		case M2T_COMPANY_CMD:
			
			switch(AutoRx_SubID)
			{
				case T_WORK_ON:
					Set_AutoTestMode_Flag;
					///PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;					
				case T_WORK_OFF:
					Clr_AutoTestMode_Flag;
					///PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;					
				case T_WORK_QU:
					///PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,NONE));
					break;					
				case T_WORK_ACC_OFF:
					///Set_SLEEP_Mode;
					///SysPower.nPowerState=POWER_SYSTEM_STANDBY;	
					///PostMessage(AUTO_MODULE, M2T_COMPANY_CMD,SCH_WORD(T_WORK_OFF,RESPONSE_YES));
					break;					
				case T_WORK_DEFAULT:					
					break;					
				case T_WORK_RESET:
					while(1);					
				case T_WORK_PWR_ON :					
					break;
				default:
					break;
			}		
			break;	
		case M2T_EQ_TEST_CMD:
			switch(AutoRx_SubID)
			{
				case T_EQ_VOL_SET:
					if(pData[0]>MAX_VOLUME||pData[1]>MAX_VOLUME||pData[2]>MAX_VOLUME)
					{
						PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_NO));
						break;
					}
					nVolume =  pData[0];
					nNavi_Level = pData[1];
					nBt_Level = pData[2];
					Adj_Volume();		
					PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_VOLUME);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;	
				case T_EQ_VOL_QU:
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,NONE));
					break;
				case T_EQ_BAS_SET:
					SetASP_MBASE(pData[0]);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_EQ);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_BAS_QU:
					PostMessage(AUTO_MODULE,AutoRx_MessageID, SCH_WORD(AutoRx_SubID,NONE));
					break;						
				case T_EQ_MID_SET:
					SetASP_MIDDLE(pData[0]);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_EQ);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_MID_QU:
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,NONE));
					break;						
				case T_EQ_TRE_SET:
					SetASP_MTREBLE(pData[0]);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_EQ);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_TRE_QU:
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,NONE));
					break;						
				case T_EQ_LOUD_SET:
					SetASP_MLOUD(pData[0]);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_EQ);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_MUTE_SET:
					if(pData[0] == 0x00)
						Clr_SYS_MUTE;
					else
						Set_SYS_MUTE;
					Mute_State=MUTE_UNMUTE_STATE;
					PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_VOLUME);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_SBWOOF_SET:
					SetASP_SUB(pData[0]);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_EQ);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_FBL_SET:
					SetASP_FAD(pData[0]);
					SetASP_BAL(pData[1]);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_EQ);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_FBL_QU:
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,NONE));
					break;						
				case T_EQ_RESET:
					SetASP_EQ(DEFAULT_EQ_MODE);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_EQ);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_SET_SUBWOOF_FILT:
					SetASPSUB_F(pData[0]);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_SETUP);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_SUB_GAIN:
					SetASP_SUB(pData[0]);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_EQ);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				case T_EQ_INFO_QU:
					PostMessage(AUTO_MODULE,AutoRx_MessageID, SCH_WORD(AutoRx_SubID,NONE));
					break;						
				case T_EQ_SET:
					SetASP_EQ(pData[0]);
					PostMessage(ARM_MODULE,M2A_AUDIO_CMD, ASP_EQ);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;						
				default:break;
			}
			break;			
		case M2T_RAD_TEST_CMD:
			switch(AutoRx_SubID)
			{
				case T_RAD_BAND_SET:
					PostMessage(TUNER_MODULE,EVT_TUN_BAND,pData[0]);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;	
				case T_RAD_FRE_SET:
					PostMessage(TUNER_MODULE,EVT_TUN_FREQ,SCH_WORD(pData[0],pData[1]));
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;
				case T_RAD_FRE_QU:
					PostMessage(AUTO_MODULE,AutoRx_MessageID, SCH_WORD(AutoRx_SubID,NONE));
					break;							
				case T_RAD_SEEK_SET:
					if(pData[0] == 0x00)
					{
						PostMessage(TUNER_MODULE, EVT_TUN_SEEK,0);
						PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					}
					else if(pData[0] == 0x01)
					{
						PostMessage(TUNER_MODULE, EVT_TUN_SEEK,1);
						PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					}
					else if(pData[0] == 0x02)
					{
						PostMessage(TUNER_MODULE, EVT_TUN_SEEK_BREAK,1);
						PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					}
					else if(pData[0] == 0x03)
					{
						PostMessage(TUNER_MODULE, EVT_TUN_AS,0);
						PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					}
					else
					{
						PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_NO));
					}
					break;							
				case T_RAD_LOC_SET:
					PostMessage(TUNER_MODULE, EVT_TUN_DX_LOC,pData[0]);
					PostMessage(AUTO_MODULE, AutoRx_MessageID,SCH_WORD(AutoRx_SubID,RESPONSE_YES));
					break;							
				case T_RAD_FRE_MIN_MAX_QU:
					PostMessage(AUTO_MODULE,AutoRx_MessageID, SCH_WORD(AutoRx_SubID,NONE));
					break;							
				case T_RAD_ST_QU:
					PostMessage(AUTO_MODULE,M2T_RAD_TEST_CMD, SCH_WORD(AutoRx_SubID,NONE));
					break;							
				default:break;
			}
			break;
		case M2T_OTHER_CMD:
			switch(AutoRx_SubID)
			{
				case 0x0C:
					PostMessage(AUTO_MODULE,AutoRx_MessageID, SCH_WORD(AutoRx_SubID,NONE));
					break;
				case 0x0D:
					PostMessage(AUTO_MODULE,AutoRx_MessageID, SCH_WORD(AutoRx_SubID,NONE));
					break;
				default:break;
			}
			break;
		default:break;
	}
}

void AutotestDataRxPro(void)
{
	while(UartRxCnt(Uart_AUTOTEST))
	{
		switch(AutoModuel.AutoRxData_Flag)
		{
			case 0:
				UartRxData(Uart_AUTOTEST, AutoModuel.AutoRxData, 1);
				if(AutoModuel.AutoRxData[0] != HEAD_ADDRESS_AUTOMCU)
					break;
				AutoModuel.AutoRxData_Flag = 1;
				break;
			case 1:
				UartRxData(Uart_AUTOTEST, &AutoModuel.AutoRxData[1], 1);
				if(AutoModuel.AutoRxData[1] != HEAD_ADDRESS_AUTOTEST)
				{
					AutoModuel.AutoRxData_Flag = 0;
					break;
				}
				AutoModuel.AutoRxData_Flag = 2;
				break;
			case 2:
				UartRxData(Uart_AUTOTEST, &AutoModuel.AutoRxData[2], 1);
				AutoModuel.AutoRxData_Length = AutoModuel.AutoRxData[2];
				if(AutoModuel.AutoRxData_Length > 40||AutoModuel.AutoRxData_Length < 3)///length 不超过30
				{
					AutoModuel.AutoRxData_Flag = 0;
					break;
				}
				AutoModuel.AutoRxData_Flag = 3;

			case 3:
				if(UartRxData(Uart_AUTOTEST, &AutoModuel.AutoRxData[3], AutoModuel.AutoRxData_Length-3)==FALSE)
					return;
				///if(AutoModuel.AutoRxData[AutoModuel.AutoRxData_Length-1] == GetCheckData_Xor(AutoModuel.AutoRxData,AutoModuel.AutoRxData_Length-1))
				{					
					sch_memcpy(AUTO_TEST_DataRx,&AutoModuel.AutoRxData[2],AutoModuel.AutoRxData_Length-3);
					PostMessage(ARM_MODULE,M2A_AUTO_TEST_CMD,0x00);
					AutoDataAnalyse();
				}
				AutoModuel.AutoRxData_Flag = 0;
				AutoModuel.AutoRxData_Length = 0;
				break;
			default:
				break;
		}
	}
}



void MCU2AUTO_TxService(void)
{
	MESSAGE pMsg;
	SCH_U8 *pData=&AutoTx_Data;
	SCH_U8 sub_id,length_data=0,index,i=0;
	if(FALSE==GetMessage(AUTO_MODULE,&pMsg))
		return;
	sub_id= MSB(pMsg.prm);
	index = LSB(pMsg.prm);
	switch(pMsg.ID)
	{				
		/*case M2T_COMPANY_CMD:
			switch(sub_id)
			{
				case T_WORK_ON:	
				case T_WORK_OFF:
					pData[length_data++]=index;
					break;
				case T_WORK_QU:
					pData[length_data++]=Get_AutoTestMode_Flag;
					break;
				case T_WORK_ACC_OFF:
				case T_WORK_DEFAULT:
				case T_WORK_RESET:
				case T_WORK_PWR_ON:
					break;
				default:break;
			}
			break;*/		
		case M2T_EQ_TEST_CMD:
			switch(AutoRx_SubID)
			{		
				case T_EQ_VOL_SET:
					pData[length_data++]=index;					
					break;
				case T_EQ_VOL_QU:
					pData[length_data++]=nVolume;
					pData[length_data++]=nNavi_Level;
					pData[length_data++]=nBt_Level;
					break;
				case T_EQ_BAS_SET:
					pData[length_data++]=index;	
					break;
				case T_EQ_BAS_QU:
					pData[length_data++]=nBass_Level;
					break;							
				case T_EQ_MID_SET:
					pData[length_data++]=index;	
					break;		
				case T_EQ_MID_QU:
					pData[length_data++]=nMiddle_Level;
					break;					
				case T_EQ_TRE_SET:
					pData[length_data++]=index;	
					break;				
				case T_EQ_TRE_QU:
					pData[length_data++]=nTreble_Level;
					break;	
				case T_EQ_LOUD_SET:
					pData[length_data++]=index;	
					break;
				case T_EQ_MUTE_SET:
					pData[length_data++]=index;	
					break;
				case T_EQ_SBWOOF_SET:
					pData[length_data++]=index;	
					break;
				case T_EQ_FBL_SET:
					pData[length_data++]=index;	
					break;
				case T_EQ_FBL_QU:
					pData[length_data++]=nFade_Level;
					pData[length_data++]=nBalance_Level;
					break;		
				case T_EQ_RESET:
					pData[length_data++]=index;	
					break;
				case T_EQ_SET_SUBWOOF_FILT:
					pData[length_data++]=index;	
					break;
				case T_EQ_SUB_GAIN:
					pData[length_data++]=index;	
					break;
				case T_EQ_INFO_QU:
					pData[length_data++]=nSubLevel;
					pData[length_data++]=nSub_F;
					pData[length_data++]=nEQ_Mode;
					break;
				case T_EQ_SET:
					pData[length_data++]=index;	
					break;
				default:
					break;
			}
			break;			
		case M2T_RAD_TEST_CMD:
			switch(AutoRx_SubID)
			{		
				case T_RAD_BAND_SET:
					pData[length_data++]=index;	
					break;
				case T_RAD_FRE_SET:
					pData[length_data++]=index;	
					break;
				case T_RAD_FRE_QU:
					pData[length_data++]=radio_band;
					pData[length_data++]=MSB(radio_freq);
					pData[length_data++]=LSB(radio_freq);					
					break;
				case T_RAD_SEEK_SET:
					pData[length_data++]=index;	
					break;
				case T_RAD_LOC_SET:
					pData[length_data++]=index;
					break;
				case T_RAD_FRE_MIN_MAX_QU:
					pData[length_data++]=MSB(MIN_FREQ());
					pData[length_data++]=LSB(MIN_FREQ());	
					pData[length_data++]=MSB(MAX_FREQ());
					pData[length_data++]=LSB(MAX_FREQ());			
					break;				
				case T_RAD_ST_QU:
					pData[length_data++]=F_TunerStereo;
					break;
				default:break;
			}
			break;							
		case M2T_SRC_TEST_CMD:
		case M2T_MEDIA_TEST_CMD:
		case M2T_BLU_TOOTH_CMD:
		case M2T_NAV_TEST_CMD:
		case M2T_OTHER_CMD:
			if(sub_id==0x0C)
			{	
				pData[length_data++]=Get_REV_Flag;	
				break;
			}
			else if(sub_id==0x0D)
			{	
				pData[length_data++]=Get_REV_Info;	
				break;
			}
		case M2T_VER_CMD:
		case M2T_COMPANY_CMD:
			for(i = 0;i<AUTO_TEST_DataTx[0];i++)
				pData[length_data++]=AUTO_TEST_DataTx[3+i];
			break;
		default:break;
	}
	AutoTx_Head = HEAD_ADDRESS_AUTOTEST;
	AutoTx_Head1 = HEAD_ADDRESS_AUTOMCU;
	AutoTx_Length = length_data+6;
	AutoTx_MessageID = pMsg.ID;
	AutoTx_SubID = sub_id;
	AutoModuel.AutoTxData_Parity = GetCheckData_Xor(AutoModuel.AutoTxData,AutoTx_Length-1);
	AutoModuel.AutoTxData[AutoTx_Length-1] = AutoModuel.AutoTxData_Parity;
	UartTxData(Uart_AUTOTEST, AutoModuel.AutoTxData, AutoTx_Length);
	AutoModuel.AutoCheck_Ack=1;
	AutoModuel.AutoCheck_ResendCounte=0;
	AutoModuel.AutoCheck_ResendTimer=0;
	AutoModuel.AutoCheck_Busy=1;
}


void TASK_Auto_Pro(void)
{
	/*if(AutoModuel.AutoCheck_Ack)
	{
		AutoModuel.AutoCheck_ResendTimer++;
		if(AutoModuel.AutoCheck_ResendTimer==T48MS_8)
		{  	
			AutoModuel.AutoCheck_ResendTimer=0;
			if(++AutoModuel.AutoCheck_ResendCounte<3)
			{
				 UartTxData(Uart_AUTOTEST, AutoModuel.AutoTxData, AutoTx_Length+3);
			}
			else
			{
				AutoModuel.AutoCheck_ResendCounte=0;
				AutoModuel.AutoCheck_Busy=0;
				AutoModuel.AutoCheck_Ack=0;
			}
		}
	}
	else if(!AutoModuel.AutoCheck_Busy)*/
	{
		MCU2AUTO_TxService();
	}
}







