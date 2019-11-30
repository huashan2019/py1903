/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_at.c  ---   AT÷∏¡Ó
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.02
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
SCH_U8 BT_Addr[12];
SCH_U8 BtSPPCONFlag;
SCH_U8 BtGATTCONFlag;
SCH_U8 BtTemplag;
SCH_U8 BtTemplag1;


Rx_BLE_DATA      BtRxBLE;
extern SCH_U8 GetCheckData_Xor(SCH_U8 *data, SCH_U8 Len);


void AtDataAnalyse(SCH_U8 *Data)
{

	if(sch_compare(Data,"DATAINIT ",9)==TRUE)
	{
	
	//	UartTxData(SCH_Uart_BT,BT_NAME_GET,sizeof(BT_NAME_GET)-1);
	}
	//if(sch_compare(Data,"GLBD 0",6)==TRUE)
	//	UartTxData(SCH_Uart_BT,BT_NAME_SET,sizeof(BT_NAME_SET)-1);
	if(sch_compare(Data,"HFCONN ",6)==TRUE)
	{/*BT ADDR*/
		sch_memcpy(BT_Addr,&Data[9],sizeof(BT_Addr));
		PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0C));

		//SysWaitMs(1);
	}

	if(sch_compare(Data,"GLBD ",4)==TRUE)
	{/*BT ADDR*/
		sch_memcpy(BT_Addr,&Data[7],sizeof(BT_Addr));
		PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0C));

		//SysWaitMs(1);
	}
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
		UartTxData(SCH_Uart_BT,BT_BLEADV_SET,sizeof(BT_BLEADV_SET)-1);
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
		
		UartTxData(SCH_Uart_BT,BT_BLEADV_SET,sizeof(BT_BLEADV_SET)-1);
		UartTxData(SCH_Uart_BT,BT_NAME_SET,sizeof(BT_NAME_SET)-1);
	}
	else
	{/*init ok*/
	}

}
///===============================================



