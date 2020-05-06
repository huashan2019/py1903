/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_connect.c
**  Description : This file is the application.
**  Author      : WenjunHu
**  Created on  : 2019.04.02
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
Uart_T Uart_CONNECT = SCH_Uart_BT;
Uart_T Uart_OTHER   = SCH_Uart_PC;
Uart_T Uart_OTHER1  = SCH_Uart_PC1;
///===================================================================================
SCH_BOOL Get_Uart_Choose(SCH_U32 DATA)
{
	SCH_U32 UART_Data;
	Flash_Quick_RD(FLASH_DATA_UART, &UART_Data);
	return (UART_Data == DATA) ? TRUE : FALSE;
}
void Set_Uart_Choose(SCH_U32 DATA)
{
	Flash_Quick_WR(FLASH_DATA_UART, DATA);
}
void Check_Uart(void)
{
	if(Get_Uart_Choose(UART_CHOOSE_PC))
	{
		Uart_CONNECT = SCH_Uart_PC;
		Uart_OTHER   = SCH_Uart_BT;
		Uart_OTHER1  = SCH_Uart_PC1;
	}
	else if(Get_Uart_Choose(UART_CHOOSE_PC1))
	{
		Uart_CONNECT = SCH_Uart_PC1;
		Uart_OTHER   = SCH_Uart_BT;
		Uart_OTHER1  = SCH_Uart_PC;
	}
}


void Change_Uart(Uart_T Uart)
{

	if(App_Dsp.DSP_Updata_State == UpData_Idle)
	{
		if(Uart == Uart_OTHER)
			ExchangeTwoVariable_U8(&Uart_CONNECT,&Uart_OTHER);
		else if(Uart == Uart_OTHER1)
			ExchangeTwoVariable_U8(&Uart_CONNECT,&Uart_OTHER1);
		if(Uart_CONNECT == SCH_Uart_PC)
		{
			Set_Uart_Choose(UART_CHOOSE_PC);
		}
		else if(Uart_CONNECT == SCH_Uart_PC1)
		{
			Set_Uart_Choose(UART_CHOOSE_PC1);
		}
		else
		{
			Set_Uart_Choose(UART_CHOOSE_BT);
		}
	}
}
///============================================================================

SCH_U8 RxData_TEMP[80];
SCH_U8 Data_Addr;
SCH_U8 Data_Num;


void BT_DataRxPro(Uart_T Uart,Rx_MODUEL_S *RxModuel)
{
	SCH_U8 RxLength;
	while(UartRxCnt(Uart))
	{
		FeedDog();
		switch(RxModuel->RxData_Flag)
		{
			case 0x00:
				UartRxData(Uart, &RxModuel->RxData[0], 1);
				if(RxModuel->RxData[0] == HEAD_ADDRESS_BT)///协议数据
				{
					RxModuel->RxData_Flag = 0x01;
				}
				else if(RxModuel->RxData[0] == AT_START_END_A)///(\r)蓝牙AT指令
				{
					RxModuel->RxData_Flag = 0x02;
				}
				else if(RxModuel->RxData[0] == HEAD_DEBUG_0)///debug命令
				{
					RxModuel->RxData_Flag = 0x03;
				}
				break;
			case 0x01:
				UartRxData(Uart, &RxModuel->RxData[1], 1);
				if(RxModuel->RxData[1] == HEAD_ADDRESS_MCU)///协议数据
				{
					RxModuel->RxData_Flag = 0x10;
				}
				else
				{
					RxModuel->RxData_Flag = 0x00;
				}
				break;
			case 0x02:
				UartRxData(Uart, &RxModuel->RxData[1], 1);
				if(RxModuel->RxData[1] == AT_START_END_T)///(\n)蓝牙AT指令
				{
					RxModuel->RxData_Flag = 0x04;
					RxModuel->RxData_Length = 0x00;
				}
				else
				{
					RxModuel->RxData_Flag = 0x00;
				}
				break;
			case 0x03:
				UartRxData(Uart, &RxModuel->RxData[1], 1);
				if(RxModuel->RxData[1] == HEAD_DEBUG_1)//////debug命令
				{
					RxModuel->RxData_Flag = 0x30;
					RxModuel->RxData_Length = 0;
				}
				else
				{
					RxModuel->RxData_Flag = 0x00;
				}
				break;

			case 0x04:
				UartRxData(Uart, &RxModuel->RxData[2], 1);
				if(RxModuel->RxData[2] == AT_START_END_SM)///(\n)蓝牙AT指令
				{
					RxModuel->RxData_Flag = 0x05;
					RxModuel->RxData_Length = 0x00;
				}
				else
				{
					RxModuel->RxData_Flag = 0x00;
				}
				break;

			case 0x05:
				UartRxData(Uart, &RxModuel->RxData[3], 1);
				if(RxModuel->RxData[3] == AT_START_END_B)///(\n)蓝牙AT指令
				{
					RxModuel->RxData_Flag = 0x06;
					RxModuel->RxData_Length = 0x00;
				}
				else
				{
					RxModuel->RxData_Flag = 0x00;
				}
				break;
				
			case 0x06:
				UartRxData(Uart, &RxModuel->RxData[4], 1);
				if(RxModuel->RxData[4] == AT_START_END_SPACE)///(\n)蓝牙AT指令
				{
					RxModuel->RxData_Flag = 0x21;
					RxModuel->RxData_Length = 0x00;
				}
				else
				{
					RxModuel->RxData_Flag = 0x00;
				}
				break;
				
			case 0x10:///===========================================以下协议解析=======================
				UartRxData(Uart, &RxModuel->RxData[2], 1);
				RxModuel->RxData_Length = RxModuel->RxData[2];
				if(RxModuel->RxData_Length > 80||RxModuel->RxData_Length < 3)///length 不超过80
				{
					RxModuel->RxData_Flag = 0x00;
					break;
				}
				RxModuel->RxData_Flag = 0x11;
				break;
			case 0x11:
				if(UartRxData(Uart, &RxModuel->RxData[3], RxModuel->RxData_Length-3)==FALSE)
					return;
				if(RxModuel->RxData[RxModuel->RxData_Length-1] == GetCheckData_Xor(RxModuel->RxData,RxModuel->RxData_Length-1))
				{
					if(Uart_CONNECT != Uart)
					{
						Change_Uart(Uart);
						sch_memcpy(BtRxModuel.RxData,RxModuel->RxData,RxModuel->RxData_Length);
					}
					
					BtDataAnalyse();
				}
				RxModuel->RxData_Flag = 0x00;
				RxModuel->RxData_Length = 0;
				return;///break;
			case 0x20:///===========================================以下AT指令解析=======================
				UartRxData(Uart, &RxModuel->RxData[RxModuel->RxData_Length + 5], 1);

				if(RxModuel->RxData[RxModuel->RxData_Length + 5] == AT_START_END_SPACE)
				{
					RxModuel->RxData_Flag = 0x21;		
					//RxModuel->RxData_Length;
				}
				else
				{
				
					
				}
				
				RxModuel->RxData_Length++;
				if(RxModuel->RxData_Length > 150)
					RxModuel->RxData_Flag = 0x00;
				break;
			case 0x21:
				UartRxData(Uart, &RxModuel->RxData[RxModuel->RxData_Length + 5], 1);

				//if(RxModuel->RxData[RxModuel->RxData_Length + 5] == AT_START_END_END)
				//{
					//sch_memcpy(&RxModuel,&RxModuel->RxData[5],11);
					if(sch_compare(&RxModuel->RxData[5],"SPPDATAIND",10)==TRUE 
						|| sch_compare(&RxModuel->RxData[5],"BLEDATAIND",10)==TRUE)
					{
						
						RxLength = 0;

						if(RxModuel->RxData[17] == 0x2c || RxModuel->RxData[18] == 0x2c)
						{
							if(RxModuel->RxData[15] ==0x20 && RxModuel->RxData[17] ==0x2c)
							{
								RxLength = RxModuel->RxData[16]-0x30-1;
								Data_Addr = 17;
							}
							else if(RxModuel->RxData[15] ==0x20 && RxModuel->RxData[18] ==0x2c)
							{
								RxLength = (RxModuel->RxData[16]-0x30)*10 + RxModuel->RxData[17]-0x30 ;
								Data_Addr = 18;
							}
						}
						
						if((RxModuel->RxData_Length >= 14 + RxLength) && RxLength)	
						{
						
							if(RxModuel->RxData[RxModuel->RxData_Length + 5] == 0x0d)
							{
								U8 i ;
								if(Uart_CONNECT != Uart)
								{
									Change_Uart(Uart);
								}
								sch_memcpy(BtRxModuel.RxData,&RxModuel->RxData[Data_Addr+1],RxModuel->RxData[Data_Addr+3]);
								Printf("Rx data : ");
								for( i = 0;i<RxModuel->RxData[Data_Addr+3];i++)
								Printf(" %x",BtRxModuel.RxData[i]);

								if(BtRxModuel.RxData[BtRx_Length-1] != GetCheckData_Xor(&BtRx_Head1,BtRx_Length-1)) 
								{
								
								}
								else BtDataAnalyse();
							}
							RxModuel->RxData_Length=0;
							RxModuel->RxData_Flag = 0x00;
						}
						else
						{
							RxModuel->RxData_Length++;
							if(RxModuel->RxData_Length > 100)
								RxModuel->RxData_Flag = 0x00;
						}
						
					}
					else if(RxModuel->RxData[RxModuel->RxData_Length + 5] == AT_START_END_END)
					{
						AtDataAnalyse(&RxModuel->RxData[5]);
						RxModuel->RxData_Length=0;
						RxModuel->RxData_Flag = 0x00;
					}

				//}
				
				else
				{
					RxModuel->RxData_Length++;
					if(RxModuel->RxData_Length > 100)
						RxModuel->RxData_Flag = 0x00;
				}
				
				break;
			case 0x30:///===========================================以下Debug指令解析=======================
				UartRxData(Uart, &RxModuel->RxData[2], 1);
				Debug_CMD(RxModuel->RxData[2]);
				RxModuel->RxData_Flag = 0x00;
				break;
			default:break;
		}
	}
}




