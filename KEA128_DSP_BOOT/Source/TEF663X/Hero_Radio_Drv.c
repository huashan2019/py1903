

#include "include.h"

/*read data buffer*/
U8 Radio_ReadBuffer[Radio_ReadBufferLen];



/*-----------------------------------------------------------------------
Function name:	RadioDrv_Stop
Input:			
Output:			
Description:	 set tuner into powerdown mode
------------------------------------------------------------------------*/
void RadioDrv_Stop(void)
{
	U8 TempData[2];

	TempData[0]=0;
	TempData[1]=Radio_POWERDOWNMODE;
	I2C1_WriteData(TEF663x_SlaveAddr,TempData,2);
}

/*-----------------------------------------------------------------------
Function name:	RadioDrv_ReadData
Input:			
Output:			
Description:	 read data from hero and save data to Radio_ReadBuffer[]
------------------------------------------------------------------------*/
void RadioDrv_ReadData(U8 Num)
{
	I2C1_ReadData(TEF663x_SlaveAddr,Radio_ReadBuffer,Num);
}

/*-----------------------------------------------------------------------
Function name:	RadioDrv_GetData
Input:	DataType		
Output:			
Description:	 
------------------------------------------------------------------------*/
U8 RadioDrv_GetData(U8 Addr)
{
	switch(Addr)
	{
		case Radio_STATUS:
		case Radio_LEVEL:
		case Radio_USN:
		case Radio_WAM:
		case Radio_FOF:
		case Radio_IFBW:
		case Radio_MOD:
		case Radio_AGCATT: 
		case TEF663x_IDENT:		
			break;
		
		default:
		{
			return ERROR;
		}
	}
    	return(Radio_ReadBuffer[Addr]);	 
}

/*-----------------------------------------------------------------------
Function name:	RadioDrv_UpdatePll
Input:			
Output:			
Description:	 write pll data to hero 
------------------------------------------------------------------------*/
void RadioDrv_UpdatePll(U8 Mode,U8 Band,U16 Pll)
{
	U16 i;
	U8 TempData[4];
	U8 Buffer[3];

	i=Pll;
	TempData[3]=(U8)i;
	i>>=8;
	TempData[2]=(U8)i;
	TempData[1]=Mode+Band;
	TempData[0]=0x00;

	for(i=0;i<10;i++)
	{/*if write failed ,rewite*/
		I2C1_WriteData(TEF663x_SlaveAddr,TempData,4);		
		Hero_ReadBack(TempData[0],3,Buffer);
		/*check write right*/
		if((Buffer[0]==TempData[1])&&(Buffer[1]==TempData[2])&&(Buffer[2]==TempData[3]))	
		{
			break;
		}
		TimerDelayMs(2);
    }
}
/*-----------------------------------------------------------------------
Function name:	RadioDrv_CheckStereoIndicator
Input:			
Output:			
Description:	 check stereo indicator
------------------------------------------------------------------------*/
U8 RadioDrv_CheckStereoIndicator(void)
{
	U8 Status;
	/*read status data*/	   
	//RadioDrv_ReadData(1);
	/*Get status data*/
	Status=RadioDrv_GetData(Radio_STATUS);
	if(Status&0x08)   
	{
	 /*  STEREO*/   	         
	 	return TRUE; 
	} 
	return FALSE; 
	
}

