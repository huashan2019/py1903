/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : main.c
**  Description : This file is the application.
**  Author      : WenjunHu
**  Created on  : 2017.04.06
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

void SysHardware_Init(void)
{
	Bsp_Clock_Init();
	Bsp_SysTick_Init();
	Bsp_GPIO_Init();
	Bsp_WDOG_Init();  
	Bsp_UART_Init();
	Bsp_SPI_Init();
	Bsp_ADC_Init();
	Bsp_FLASH_Init();
	Print_Init();
}
void SysSoftware_Init(void)
{
	SysDataInit();
	DspInit();
	Check_Uart();
}
////============================================================================================================
int main(void)
{
  	SCH_INT_DISABLE;
	SysHardware_Init();
	SysSoftware_Init();
	Bsp_Relocation_Int();///Reset the interrupt vector table
	SCH_INT_ENABLE;
	Printf("APP Begin -- Software Version : %s \n", MCU_VERSION);
	Printf("Code generation time : %s %s \n", __DATE__, __TIME__);
	while(1)
	{
		FeedDog();
		SCH_Task_Execute();
		TASK_AccOff_Pro();
		BT_DataRxPro(Uart_CONNECT, &BtRxModuel);
		BT_DataRxPro(Uart_OTHER,   &PcRxModuel_0);
		BT_DataRxPro(Uart_OTHER1,  &PcRxModuel_1);
		
	if(BtTemplag/*sch_compare(Data,"A2DPSTAT 1",8)==TRUE*/)
	{/*BLEADV*/
		BtTemplag = 0;
		UartTxData(SCH_Uart_BT,BT_VER_GET,sizeof(BT_VER_GET)-1);
	}
	
	if(BtTemplag1/*sch_compare(Data,"A2DPSTAT 1",8)==TRUE*/)
	{/*BLEADV*/
		BtTemplag1 = 0;
		UartTxData(SCH_Uart_BT,BT_HFDISC_SET,sizeof(BT_HFDISC_SET)-1);
	}
	}
}
