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
SCH_U8 BT_Addr[12+1];/*add one byte for HPF connect stat*/
SCH_U8 BLE_GVER[30];

SCH_U8 BtSPPCONFlag;
SCH_U8 bGUKAIBLE;

SCH_U8 BtGATTCONFlag;
SCH_U8 BtPHFCONFlag;

SCH_U8 BtTemplag;
SCH_U8 BtTemplag1; 


Rx_BLE_DATA      BtRxBLE;
extern SCH_U8 GetCheckData_Xor(SCH_U8 *data, SCH_U8 Len);


void AtDataAnalyse(SCH_U8 *Data)
{
	SCH_U8 SPPDATA_STATUS,BLE_STATUS;
	if(sch_compare(Data,"DATAINIT ",9)==TRUE)
	{
		PCSTATFlag = 1;
	//	UartTxData(SCH_Uart_BT,BT_NAME_GET,sizeof(BT_NAME_GET)-1);
	}
	if(sch_compare(Data,"GVER ",4)==TRUE)
	{
		bGUKAIBLE = 0;
		sch_memcpy(BLE_GVER,&Data[5],sizeof(BLE_GVER));
		if(sch_compare(BLE_GVER,"PY072",5)==TRUE)	bGUKAIBLE = 1;

		
		if(/*BtSPPCONFlag || BtGATTCONFlag || */!PCSTATFlag)
		PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x02,0x0C));

		UartTxData(SCH_Uart_BT,BT_NAME_SET,sizeof(BT_NAME_SET)-1);
	}
	if(sch_compare(Data,"HFCONN ",6)==TRUE)
	{/*BT ADDR*/
		sch_memcpy(BT_Addr,&Data[9],sizeof(BT_Addr)-1);
		if(/*BtSPPCONFlag || BtGATTCONFlag || */!PCSTATFlag)
		PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0C));

		//SysWaitMs(1);
	}
	else if(sch_compare(Data,"HFDISC ",6)==TRUE)
	{
	}

	if(sch_compare(Data,"A2DPCONN ",9)==TRUE)
	{
		if(Data[9]-0x30 == 0) BtPHFCONFlag = 1;
	}
	else if(sch_compare(Data,"A2DPDISC ",9)==TRUE)
	{
		if(Data[9]-0x30 == 0) BtPHFCONFlag = 0;
	}
	if(sch_compare(Data,"GLBD ",4)==TRUE)
	{/*BT ADDR*/
		sch_memcpy(BT_Addr,&Data[7],sizeof(BT_Addr)-1);
		if(/*BtSPPCONFlag || BtGATTCONFlag || */!PCSTATFlag)
		PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0C));

		//SysWaitMs(1);
	}
	if(sch_compare(Data,"SPPCONN ",8)==TRUE)
	{/*APP CONNECT*/
		sch_memcpy(&SPPDATA_STATUS,&Data[8],1);
		if(SPPDATA_STATUS == 0X30)
		{
			BtGATTCONFlag = 0;
			BtSPPCONFlag = 1;
		}
		SysWaitMs(1);
	}
	else if(sch_compare(Data,"SPPDISC ",8)==TRUE)
	{
		BtSPPCONFlag = 0;
		SysWaitMs(1);
		UartTxData(SCH_Uart_BT,BT_BLEADV_SET,sizeof(BT_BLEADV_SET)-1);
	}
	
	if(sch_compare(Data,"BLECONN ",8)==TRUE)
	{/*BLE CONNECT*/
		sch_memcpy(&BLE_STATUS,&Data[8],1);
		if(BLE_STATUS == 0X30)
		{
			BtSPPCONFlag = 0;
			BtGATTCONFlag = 1;
		}

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

	if(sch_compare(Data,"GOTA ",5)==TRUE)
	{/*GOTA OK*/
		SysWaitMs(1);
		//if(Data[5]-0x30 == 0) 
		//UartTxData(SCH_Uart_BT,BT_BLEADV_SET,sizeof(BT_BLEADV_SET)-1);
	}
	else
	{
	
	}

	
	if(sch_compare(Data,"SNKINIT ",8)==TRUE)
	{
		PCSTATFlag = 0;
		//UartTxData(SCH_Uart_BT,BT_GOTA_STAT_GET,sizeof(BT_GOTA_STAT_GET));
		
		UartTxData(SCH_Uart_BT,BT_BLEADV_SET,sizeof(BT_BLEADV_SET)-1);
		UartTxData(SCH_Uart_BT,BT_NAME_SET,sizeof(BT_NAME_SET)-1);
	}
	else
	{/*init ok*/
	}

}
///===============================================



