/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_arm.c  ---   与ARM通讯
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

 SCH_U8 BtSPPCONFlag;
 SCH_U8 BtGATTCONFlag;

Uart_T Uart_ARM = SCH_Uart_BT;
///===============================================
#define HEAD_ADDRESS_ARM     0xAA
#define HEAD_ADDRESS_MCU     0x55

#define AT_START_END_A       0x41//A
#define AT_START_END_T       0x54//T
#define AT_START_END_SM      0x2D//-
#define AT_START_END_B       0x42//B
#define AT_START_END_SPACE   0x20// 
#define AT_START_END_END     0x0D//END

typedef struct
{
	SCH_U8 ArmRxData[256];
	SCH_U8 ArmRxData_Length;
	SCH_U8 ArmRxData_Flag;
	SCH_U8 ArmTxData[128];
	SCH_U8 ArmTxData_Length;
	SCH_U8 ArmTxData_Parity;
	SCH_U8 ArmCheck_ResendCounte;
	SCH_U8 ArmCheck_ResendTimer;
	SCH_U8 ArmCheck_Busy;
	SCH_U8 ArmCheck_Ack;
}ARM_MODUEL_S;

ARM_MODUEL_S BtRxModuel; 
ARM_MODUEL_S ArmModuel; 
#define ArmRx_Head1     ArmModuel.ArmRxData[0]
#define ArmRx_Head2     ArmModuel.ArmRxData[1]
#define ArmRx_Length    ArmModuel.ArmRxData[2]
#define ArmRx_Sequence  ArmModuel.ArmRxData[3]
#define ArmRx_GroupID   ArmModuel.ArmRxData[4]
#define ArmRx_SubID     ArmModuel.ArmRxData[5]
#define ArmRx_Data      ArmModuel.ArmRxData[6]
#define ArmTx_Head1     ArmModuel.ArmTxData[10]
#define ArmTx_Head2     ArmModuel.ArmTxData[11]
#define ArmTx_Length    ArmModuel.ArmTxData[12]
#define ArmTx_Sequence  ArmModuel.ArmTxData[13]
#define ArmTx_GroupID   ArmModuel.ArmTxData[14]
#define ArmTx_SubID     ArmModuel.ArmTxData[15]
#define ArmTx_Data      ArmModuel.ArmTxData[16]
///===============================================
void Arm_ACK(SCH_U8 ack_type)
{
#if 0
#if 1
#if 1
SCH_U8 buf[17];


buf[0] = 'A';
buf[1] = 'T';
buf[2] = '-';
buf[3] = 'B';/*DATA TYPE:0X0102 SPP数据，0x0101 BLE数据*/
buf[4] = 0x20;


buf[5] = 0x42;/*length H:从0x80开始，0x0d结束*/
buf[6] = 0x4c;/*ngth L*/
buf[7] = 0x45;
buf[8] = 0x44;
buf[9] = 0x41;/*\r*/

buf[10]=0x54;
buf[11]=0x41;
buf[12]=0x20;
buf[13]=0x31;
buf[14]=0x0d;/*\r*/
UartTxData(Uart_ARM, buf, 15);


#else
SCH_U8 buf[158],i;

buf[0] = 'A';
buf[1] = 'T';
buf[2] = '-';
buf[3] = 'B';/*DATA TYPE:0X0102 SPP数据，0x0101 BLE数据*/

if(BtGATTCONFlag)
	buf[4] = 0x01;
else if(BtSPPCONFlag)
	buf[4] = 0x02;
else
{
	buf[4] = 0x20;
	////printf("\n\n--DATA TYPE ERR--\n\n");
}


buf[5] = 'S';/*length H:从0x80开始，0x0d结束*/
buf[6] = 'P';/*length L*/
buf[7] = 'P';
buf[8] = 'D';
buf[9] = 'A';/*\r*/

buf[10]='T';
buf[11]='A';
buf[12]='I';
buf[13]='N'; 
buf[14]='D';
buf[15]=' ';
buf[16] = 0x31;/*\r*/
buf[17] = 0x33;/*\r*/
buf[18] = 0x37;/*\r*/

buf[19] = 0x2C;/*\r*/

for(i = 0;i<137;i++)
{
	buf[20+i] = 0x00;/*\r*/
}
buf[20] = 0xAA;/*\r*/
buf[21] = 0x55;/*\r*/
buf[22] = 0x89;
buf[23] = 0x0;
buf[24] = 0x08;
buf[25] = 0x02;

buf[20+137-1] = 0x7c;/*\r*/
buf[20+137] = 0xD;/*\r*/
//buf[35] = ;/*\r*/



UartTxData(Uart_ARM, buf, 158);
#endif
#else
	SCH_U8 buf[36];

	buf[0] = 'A';
	buf[1] = 'T';
	buf[2] = '-';
	buf[3] = 'B';/*DATA TYPE:0X0102 SPP数据，0x0101 BLE数据*/

	if(BtGATTCONFlag)
		buf[4] = 0x01;
	else if(BtSPPCONFlag)
		buf[4] = 0x02;
	else
	{
		buf[4] = 0x20;
		////printf("\n\n--DATA TYPE ERR--\n\n");
	}


	buf[5] = 'S';/*length H:从0x80开始，0x0d结束*/
	buf[6] = 'P';/*length L*/
	buf[7] = 'P';
	buf[8] = 'D';
	buf[9] = 'A';/*\r*/

	buf[10]='T';
	buf[11]='A';
	buf[12]='I';
	buf[13]='N'; 
	buf[14]='D';
	buf[15]=' ';
	buf[16] = 0x31;/*\r*/
	buf[17] = 0x35;/*\r*/
	buf[18] = 0x2C;/*\r*/
	
	buf[19] = 0xAA;/*\r*/
	buf[20] = 0x55;/*\r*/
	buf[21] = 0x0F;/*\r*/
	buf[22] = 0xBF;/*\r*/
	buf[23] = 0x08;/*\r*/
	buf[24] = 0x03;/*\r*/
	buf[25] = 0x0;/*\r*/
	buf[26] = 0x1;/*\r*/
	buf[27] = 0x5D;/*\r*/
	buf[28] = 0x4C;/*\r*/
	
	buf[29] = 0x0;/*\r*/
	buf[30] = 0x3F;/*\r*/
	buf[31] = 0xE7;/*\r*/
	buf[32] = 0xD8;/*\r*/
	buf[33] = 0x54;/*\r*/
	buf[34] = 0xD;/*\r*/
	//buf[35] = ;/*\r*/
	

	
	UartTxData(Uart_ARM, buf, 35);
#endif
#else
	SCH_U8 buf[17];


	buf[0] = 'A';
	buf[1] = 'T';
	buf[2] = '>';
	buf[4] = 0x01;/*DATA TYPE:0X0102 SPP数据，0x0101 BLE数据*/

	if(BtGATTCONFlag)
		buf[3] = 0x01;
	else if(BtSPPCONFlag)
		buf[3] = 0x02;
	else
	{
		buf[3] = 0x02;
		//printf("\n\n--DATA TYPE ERR--\n\n");
	}

	
	buf[5] = 0x00;/*length H:从0x80开始，0x0d结束*/
	buf[6] = 9;/*length L*/
	buf[7] = 0x80;
	buf[8] = 0x00;
	buf[9] = 0x0d;/*\r*/

	buf[10]=HEAD_ADDRESS_MCU;
	buf[11]=HEAD_ADDRESS_ARM;
	buf[12]=6;
	buf[13]=ArmRx_Sequence; 
	buf[14]=ack_type;
	buf[15]=GetCheckData_Xor(&buf[10], 5);
	buf[16] = 0x0d;/*\r*/
	UartTxData(Uart_ARM, buf, 6+11);
#endif
}


SCH_U8 sch_compare(void const *COM_A, void const *COM_B, SCH_U16 n)  
{
	SCH_U8 *t = (SCH_U8 *)COM_A;
	SCH_U8 *f = (SCH_U8 *)COM_B;
	while(n--)
	{
		if(*t++ != *f++)
			return FALSE;
	}
	return TRUE;
}

void AtDataAnalyse(SCH_U8 *Data)
{

	if(sch_compare(Data,"DATAINIT ",9)==TRUE)
	{
	//	UartTxData(SCH_Uart_BT,BT_NAME_GET,sizeof(BT_NAME_GET)-1);
	}
	//if(sch_compare(Data,"GLBD 0",6)==TRUE)
	//	UartTxData(SCH_Uart_BT,BT_NAME_SET,sizeof(BT_NAME_SET)-1);

	
	if(sch_compare(Data,"SPPCONN ",8)==TRUE)
	{/*APP CONNECT*/
		BtGATTCONFlag = 0;
		BtSPPCONFlag = 1;
		SysWaitMs(1);
	}
	else if(sch_compare(Data,"SPPDISC ",8)==TRUE)
	{
		BtSPPCONFlag = 0;
		SysWaitMs(1);
	}
	
	if(sch_compare(Data,"BLECONN ",8)==TRUE)
	{/*BLE CONNECT*/
		BtSPPCONFlag = 0;
		BtGATTCONFlag = 1;
		SysWaitMs(1);
	}
	else if(sch_compare(Data,"BLEDISC ",8)==TRUE)
	{
		BtGATTCONFlag = 0;
		SysWaitMs(1);
		//UartTxData(SCH_Uart_BT,BT_BLEADV_SET,sizeof(BT_BLEADV_SET)-1);
	}


	if(sch_compare(Data,"SPPDATA ",8)==TRUE)
	{/*TX OK*/
		SysWaitMs(1);
	}
	else
	{
	}
	
	if(sch_compare(Data,"SNKINIT ",8)==TRUE)
	{
	
		//UartTxData(SCH_Uart_BT,BT_NAME_SET,sizeof(BT_NAME_SET)-1);
	}
	else
	{/*init ok*/
	}

}

void ArmDataAnalyse(void)
{
	SCH_U8 *pData=&ArmRx_Data;
	if(ArmRx_GroupID<COMM_NACK_NG)           
		Arm_ACK(COMM_ACK);
	switch(ArmRx_GroupID)
	{
		case A2M_ACK:///===========================================================================================ACK================
			if(ArmRx_Sequence==ArmTx_Sequence)
			{
				ArmModuel.ArmCheck_Ack=0;
				ArmModuel.ArmCheck_Busy=0;
			}
			break;
		case A2M_SYS_CMD:///===========================================================================================A2M_SYS_CMD 0x01================
			switch(ArmRx_SubID)
			{	
				case A2M_MCU_VER:
					PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_MCU_VER);
					break;
				case A2M_WORK_STATE:
					PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_WORK_STATE);
					break;				
				default:break;
			}
			break;
		case A2M_IAP_CMD:///===========================================================================================A2M_IAP_CMD 0x08================
			switch(ArmRx_SubID)
			{
				case A2M_IAP_REQ:
					if(pData[0]==0x00)
						Set_IAP_Mode;
					break;
				case A2M_IAP_DATA:
					if(Iap.IAP_Sn == (SCH_U16)(((SCH_U16)pData[0]<<8)+pData[1]))
					{
						sch_memcpy(Iap.IAP_Data,&pData[2],ArmRx_Length-9);
						Iap.IAP_DataOk = 1;
					}
					break;
				case A2M_IAP_C_L:
					Iap.IAP_Len = 0x00000000;
					Iap.IAP_Len |= pData[0]<<24;
					Iap.IAP_Len |= pData[1]<<16;
					Iap.IAP_Len |= pData[2]<<8;
					Iap.IAP_Len |= pData[3];
					Iap.IAP_Checksum = 0x00000000;
					Iap.IAP_Checksum |= pData[4]<<24;
					Iap.IAP_Checksum |= pData[5]<<16;
					Iap.IAP_Checksum |= pData[6]<<8;
					Iap.IAP_Checksum |= pData[7];
					break;
				case A2M_IAP_RESET:
					SCH_INT_DISABLE;
					SysFlashInit();
					Out_IapMode();
					BootloaderJumpToApp();
					while(1);
				default:break;
			}
			break;
		default:break;
	}
}

SCH_U8 Data_Addr;

void ArmDataRxPro(void)
{
	SCH_U8 RxLength,i;


	while(UartRxCnt(Uart_ARM))
	{
		FeedDog();

		if(Uart_ARM == SCH_Uart_PC)
		{
			switch(ArmModuel.ArmRxData_Flag)
			{
				case 0:
					UartRxData(Uart_ARM, ArmModuel.ArmRxData, 1);
					if(ArmModuel.ArmRxData[0] != HEAD_ADDRESS_ARM)
						break;
					Printf("APP 0xaa :\n");
					ArmModuel.ArmRxData_Flag = 1;
					break;
				case 1:
					UartRxData(Uart_ARM, &ArmModuel.ArmRxData[1], 1);
					if(ArmModuel.ArmRxData[1] != HEAD_ADDRESS_MCU)
					{
						ArmModuel.ArmRxData_Flag = 0;
						break;
					}
					ArmModuel.ArmRxData_Flag = 2;
					Printf("APP 0x55 :\n");
					break;
				case 2:
					UartRxData(Uart_ARM, &ArmModuel.ArmRxData[2], 1);
					ArmModuel.ArmRxData_Length = ArmModuel.ArmRxData[2];
					if(ArmModuel.ArmRxData_Length >= 256||ArmModuel.ArmRxData_Length <= 3)///length 不超过30
					{
						ArmModuel.ArmRxData_Flag = 0;
						break;
					}
					ArmModuel.ArmRxData_Flag = 3;
					break;
				case 3:
					if(UartRxData(Uart_ARM, &ArmModuel.ArmRxData[3], ArmModuel.ArmRxData_Length-3)==FALSE)
						return;
					
					Printf("APP end1 :\n");
					if(ArmModuel.ArmRxData[ArmModuel.ArmRxData_Length-1] == GetCheckData_Xor(ArmModuel.ArmRxData,ArmModuel.ArmRxData_Length-1))
					{
						ArmDataAnalyse();
					}
					ArmModuel.ArmRxData_Flag = 0;
					ArmModuel.ArmRxData_Length = 0;
					break;
				default:
					break;
			}

		}
		else
		{
			switch(ArmModuel.ArmRxData_Flag)
			{
				case 0:
					UartRxData(Uart_ARM, ArmModuel.ArmRxData, 1);
					if(ArmModuel.ArmRxData[0] != AT_START_END_A)
					break;
					
					ArmModuel.ArmRxData_Flag = 1;
					break;
				case 1:
					UartRxData(Uart_ARM, &ArmModuel.ArmRxData[1], 1);
					if(ArmModuel.ArmRxData[1] != AT_START_END_T)
					break;
					
					ArmModuel.ArmRxData_Flag = 2;
					break;
				case 2:
					UartRxData(Uart_ARM, &ArmModuel.ArmRxData[2], 1);
					if(ArmModuel.ArmRxData[2] != AT_START_END_SM)
						break;
						
						ArmModuel.ArmRxData_Flag = 3;
					break;
					
				case 3:
					UartRxData(Uart_ARM, &ArmModuel.ArmRxData[3], 1);
					if(ArmModuel.ArmRxData[3] != AT_START_END_B)
						break;
							
					ArmModuel.ArmRxData_Flag = 4;
					break;
			
					
				case 4:
					UartRxData(Uart_ARM, &ArmModuel.ArmRxData[4], 1);
					if(ArmModuel.ArmRxData[4] != AT_START_END_SPACE)
							break;
					ArmModuel.ArmRxData_Flag = 5;
					ArmModuel.ArmRxData_Length = 0x00;
					break;
			
			
				case 5:
					UartRxData(Uart_ARM, &ArmModuel.ArmRxData[ArmModuel.ArmRxData_Length+5], 1);
					if(sch_compare(&ArmModuel.ArmRxData[5],"SPPDATAIND",10)==TRUE 
						|| sch_compare(&ArmModuel.ArmRxData[5],"BLEDATAIND",10)==TRUE)
					{
			
						RxLength = 0;
			
						if(ArmModuel.ArmRxData[17] == 0x2c || ArmModuel.ArmRxData[18] == 0x2c  || ArmModuel.ArmRxData[19] == 0x2c)
						{
							if(ArmModuel.ArmRxData[15] ==0x20 && ArmModuel.ArmRxData[17] ==0x2c)
							{
								RxLength = ArmModuel.ArmRxData[16]-0x30-1;
								Data_Addr = 17;
								//Printf("LEN1 ");
								
							}
							else if(ArmModuel.ArmRxData[15] ==0x20 && ArmModuel.ArmRxData[18] ==0x2c)
							{
								RxLength = (ArmModuel.ArmRxData[16]-0x30)*10 + ArmModuel.ArmRxData[17]-0x30;
								Data_Addr = 18;
								//Printf("LEN2 ");
								
							}
							else if(ArmModuel.ArmRxData[15] ==0x20 && ArmModuel.ArmRxData[19] ==0x2c)
							{
								RxLength = (ArmModuel.ArmRxData[16]-0x30)*100 + (ArmModuel.ArmRxData[17]-0x30)*10+ (ArmModuel.ArmRxData[18]-0x30) + 1 ;
								Data_Addr = 19;
								//Printf("LEN2 ");
								
							}
							
						}
			
						
			
						if((ArmModuel.ArmRxData_Length >= 14 + RxLength) && RxLength)	
						{
							if(ArmModuel.ArmRxData[ArmModuel.ArmRxData_Length + 5] == 0x0d)
							{
								u16 i;
								sch_memcpy(ArmModuel.ArmRxData,&ArmModuel.ArmRxData[Data_Addr+1],ArmModuel.ArmRxData[Data_Addr+3]);
								//Printf("APP data :\n");
								//for(i = 0;i<ArmModuel.ArmRxData[Data_Addr+3];i++)
								{
								//	Printf(" %x ",ArmModuel.ArmRxData[i]);
								}
			
								if(ArmModuel.ArmRxData[ArmModuel.ArmRxData[2]-1] != GetCheckData_Xor(ArmModuel.ArmRxData,ArmModuel.ArmRxData[2]-1)) 
								{
								
								}
								else 
								ArmDataAnalyse();
							}
							ArmModuel.ArmRxData_Length=0;
							ArmModuel.ArmRxData_Flag = 0x00;
						}
						else
						{
							ArmModuel.ArmRxData_Length++;
							if(ArmModuel.ArmRxData_Length >= 256)
							{
								ArmModuel.ArmRxData_Flag = 0x00;
								Printf("APP end1 :\n");
							}
						}
						
					}
					else if(ArmModuel.ArmRxData[ArmModuel.ArmRxData_Length + 5] == AT_START_END_END)
					{
						Printf("APP cmd :\n");
						AtDataAnalyse(&ArmModuel.ArmRxData[5]);
						ArmModuel.ArmRxData_Length=0;
						ArmModuel.ArmRxData_Flag = 0x00;
					}
					else
					{
			
						ArmModuel.ArmRxData_Length++;
			
						if(ArmModuel.ArmRxData_Length >= 256)
						{
							ArmModuel.ArmRxData_Flag = 0x00;
							Printf("APP end2 :\n");
						}
					}
					break;
	#if 0
				case 2:
					UartRxData(Uart_ARM, &ArmModuel.ArmRxData[2], 1);
					ArmModuel.ArmRxData_Length = ArmModuel.ArmRxData[2];
					if(ArmModuel.ArmRxData_Length >= 150||ArmModuel.ArmRxData_Length <= 3)///length 不超过30
					{
						ArmModuel.ArmRxData_Flag = 0;
						break;
					}
					ArmModuel.ArmRxData_Flag = 3;
					break;
				case 3:
					if(UartRxData(Uart_ARM, &ArmModuel.ArmRxData[3], ArmModuel.ArmRxData_Length-3)==FALSE)
						return;
					if(ArmModuel.ArmRxData[ArmModuel.ArmRxData_Length-1] == GetCheckData_Xor(ArmModuel.ArmRxData,ArmModuel.ArmRxData_Length-1))
					{
						ArmDataAnalyse();
					}
					ArmModuel.ArmRxData_Flag = 0;
					ArmModuel.ArmRxData_Length = 0;
					break;
	#endif	
				default:
					break;
			}

		}

	}
}
///==============================================================================================================================

void M2A_TxService(void)
{
	MESSAGE pMsg;
	SCH_U8 *pData=&ArmTx_Data;
	SCH_U8 length_data = 0;
	SCH_U8 sub_id;
	if(FALSE==GetMessage(ARM_MODULE,&pMsg))
		return;
	sub_id=LSB(pMsg.prm);
	switch(pMsg.ID)
	{
		case M2A_SYS_CMD:
			switch(sub_id)
			{	
				case M2A_MCU_VER:
					length_data=sizeof(MCU_VERSION);
					sch_memcpy(pData,MCU_VERSION,length_data);
					break;
				case M2A_WORK_STATE:
					pData[length_data++]=0x01;
					break;					
				default:break;
			}
			break;			
		case M2A_IAP_CMD:
			switch(sub_id)
			{
				case M2A_IAP_REQ_C_L:
					pData[length_data++]=0x00;
					pData[length_data++]=0x00;
					break;
				case M2A_IAP_REQ_DATA:
					pData[length_data++]=(SCH_U8)(Iap.IAP_Sn>>8);
					pData[length_data++]=(SCH_U8)Iap.IAP_Sn;
					break;
				case M2A_IAP_REQ_RESET:
					pData[length_data++]=0x00;
					pData[length_data++]=0x00;
					break;
				default:break;
			}
			break;
		default:break;
	}
	#if 1
	ArmModuel.ArmTxData[0] = 'A';
	ArmModuel.ArmTxData[1] = 'T';
	ArmModuel.ArmTxData[2] = '>';
	ArmModuel.ArmTxData[3] = 0x01;/*DATA TYPE:0X0102 SPP数据，0x0101 BLE数据*/
	
	if(BtGATTCONFlag)
		ArmModuel.ArmTxData[4] = 0x01;
	else if(BtSPPCONFlag)
		ArmModuel.ArmTxData[4] = 0x02;
	else
	{
		ArmModuel.ArmTxData[4] = 0x02;
		//printf("\n\n--DATA TYPE ERR--\n\n");
	}
	ArmModuel.ArmTxData[5] = 0x00;/*length H:从0x80开始，0x0d结束*/
	ArmModuel.ArmTxData[6] = length_data+7+3;/*length L*/
	ArmModuel.ArmTxData[7] = 0x80;
	ArmModuel.ArmTxData[8] = 0x00;
	ArmModuel.ArmTxData[9] = 0x0d;/*\r*/

	ArmTx_Head1 = HEAD_ADDRESS_MCU;
	ArmTx_Head2 = HEAD_ADDRESS_ARM;
	ArmTx_Length = length_data+7;
	ArmTx_Sequence++;
	ArmTx_GroupID = pMsg.ID;
	ArmTx_SubID = sub_id;
	ArmModuel.ArmTxData_Parity = GetCheckData_Xor(&ArmTx_Head1,ArmTx_Length-1);
	ArmModuel.ArmTxData[ArmTx_Length+10-1] = ArmModuel.ArmTxData_Parity;
	ArmModuel.ArmTxData[ArmTx_Length+10] = 0x0d;/*\r*/

	UartTxData(Uart_ARM, ArmModuel.ArmTxData, ArmTx_Length+1+10);
	ArmModuel.ArmCheck_Ack=1;
	ArmModuel.ArmCheck_ResendCounte=0;
	ArmModuel.ArmCheck_ResendTimer=0;
	ArmModuel.ArmCheck_Busy=1;
	#else
	ArmTx_Head1 = HEAD_ADDRESS_MCU;
	ArmTx_Head2 = HEAD_ADDRESS_ARM;
	ArmTx_Length = length_data+7;
	ArmTx_Sequence++;
	ArmTx_GroupID = pMsg.ID;
	ArmTx_SubID = sub_id;
	ArmModuel.ArmTxData_Parity = GetCheckData_Xor(ArmModuel.ArmTxData,ArmTx_Length-1);
	ArmModuel.ArmTxData[ArmTx_Length-1] = ArmModuel.ArmTxData_Parity;
	UartTxData(Uart_ARM, ArmModuel.ArmTxData, ArmTx_Length);
	ArmModuel.ArmCheck_Ack=1;
	ArmModuel.ArmCheck_ResendCounte=0;
	ArmModuel.ArmCheck_ResendTimer=0;
	ArmModuel.ArmCheck_Busy=1;
	#endif
}
///==========================================================================================================================
void TASK_Arm_Pro(void)
{
	/*if(Get_OsStartOk==OFF||Get_SysPower_Flag==OFF)  
	{
		return;
	}
	if(ArmModuel.ArmCheck_Ack)
	{
		ArmModuel.ArmCheck_ResendTimer++;
		if(ArmModuel.ArmCheck_ResendTimer==T200MS_8)
		{  	
			ArmModuel.ArmCheck_ResendTimer=0;
			if(++ArmModuel.ArmCheck_ResendCounte<3)
			{
				 UartTxData(Uart_ARM, ArmModuel.ArmTxData, ArmTx_Length);
			}
			else
			{
				ArmModuel.ArmCheck_ResendCounte=0;
				ArmModuel.ArmCheck_Busy=0;
				ArmModuel.ArmCheck_Ack=0;
			}
		}
	}
	else if(!ArmModuel.ArmCheck_Busy)*/
	{
		M2A_TxService();
	}
}


