#include "include.h"

/*------------------struct-----------------------------*/
//Geq_UserModeTable[x][0]    [x][1]    [x][2]    [x][3]    [x][4]     [x][5]    [x][6]    
//       Gcx                  bx0L      bx0H      ax2L      ax2H       ax1L      ax1H
S16Y Geq_UserModeTable[GEQ_MAXBAND][7];
/*-----------------Geq config end----------------------*/
/*change db value to 16 y mem data
  e.g:
      power(10,db/20)*2048   =table_dB2Lin[-db]

*/
const U16 table_dB2Lin[73 + 1] =
{
	0x07FF,   /*  00dB =  1.00 */
	0x0721,   /* -01dB =  0.89 */
	0x065A,   /* -02dB    */
	0x05A8,   /* -03dB    */
	0x050A,   /* -04dB    */
	0x047D,   /* -05dB    */
	0x0400,   /* -06dB    */
	0x0390,   /* -07dB    */
	0x032D,   /* -08dB    */
	0x02D4,   /* -09dB    */
	0x0285,   /* -10dB    */
	0x023F,   /* -11dB    */
	0x0200,   /* -12dB    */
	0x01C8,   /* -13dB    */
	0x0196,   /* -14dB    */
	0x016A,   /* -15dB    */
	0x0143,   /* -16dB    */
	0x011F,   /* -17dB    */
	0x0100,   /* -18dB    */
	0x00E4,   /* -19dB    */
	0x00CB,   /* -20dB    */
	0x00B5,   /* -21dB    */
	0x00A1,   /* -22dB    */
	0x0090,   /* -23dB    */
	0x0080,   /* -24dB    */
	0x0072,   /* -25dB    */
	0x0066,   /* -26dB    */
	0x005B,   /* -27dB    */
	0x0051,   /* -28dB    */
	0x0048,   /* -29dB    */
	0x0040,   /* -30dB    */
	0x0039,   /* -31dB    */
	0x0033,   /* -32dB    */
	0x002D,   /* -33dB    */
	0x0028,   /* -34dB    */
	0x0024,   /* -35dB    */
	0x0020,   /* -36dB    */
	0x001D,   /* -37dB    */
	0x0019,   /* -38dB    */
	0x0017,   /* -39dB    */
	0x0014,   /* -40dB    */
	0x0012,   /* -41dB    */
	0x0010,   /* -42dB    */
	0x000E,   /* -43dB    */
	0x000D,   /* -44dB    */
	0x000B,   /* -45dB    */
	0x000A,   /* -46dB =  0.01 */
	0x0009,   /* -47dB =  0.00 */
	0x0008,   /* -48dB    */
	0x0007,   /* -49dB    */
	0x0006,   /* -50dB    */
	0x0006,   /* -51dB    */
	0x0005,   /* -52dB    */
	0x0004,   /* -53dB    */
	0x0004,   /* -54dB    */
	0x0004,   /* -55dB    */
	0x0003,   /* -56dB    */
	0x0003,   /* -57dB    */
	0x0003,   /* -58dB    */
	0x0002,   /* -59dB    */
	0x0002,   /* -60dB    */
	0x0002,   /* -61dB    */
	0x0002,   /* -62dB    */
	0x0001,   /* -63dB    */
	0x0001,   /* -64dB    */
	0x0001,   /* -65dB =  0.00 */
	0x0001,   /* -66dB =  0.00 */
	0x0001,   /* -67dB =  0.00 */
	0x0001,   /* -68dB =  0.00 */
	0x0001,   /* -69dB =  0.00 */
	0x0001,   /* -70dB =  0.00 */
	0x0001,   /* -71dB =  0.00 */
	0x0001,   /* -72dB =  0.00 */
	0x0000    /* -Infinite dB     */
};/* dB2Lin */


/*-----------------------------------------------------------------------
Input:	type: HERO_XDATA=0,HERO_YDATA=1,HERO_2YDATA=2
        	data: double to trans -1~1	
        	BufferP:pointer to translated data 
Output:	error		
Description: double type data exchange hero X/Y memory data format	 
------------------------------------------------------------------------*/
S8 Hero_TransDouble2XYHexData( U8 Type, double DoubleData,S32 *BufferP)
{
	S32 Data;
	double DoubleData1;

	if((DoubleData<-1.0000)||(DoubleData>1.0000))	   // over flow,hero just use the data between -1.0~1.0
	{
		return PARA_ERROR;
	}

	if(BufferP==NULL)
	{
		return PARA_ERROR;
	}

	if(Type==HERO_YDATA)	  //Y memory data 12 bits
	{
		if(DoubleData==1)
		{
			Data=0x07FF;//bug,nick 2012-02-16 
		}
		else
		{
			DoubleData1=DoubleData*((double)2048);
			if(DoubleData1>0) DoubleData1+=0.5;
			else DoubleData1-=0.5;
			Data=(S32)(DoubleData1);		//see UM for calculate
		}
		*BufferP=Data;
	}
	else if(Type==HERO_XDATA)//X memory data
	{
		if(DoubleData==1)
		{
			Data=0x7FFFFF;
		}
		else
		{
			DoubleData1=DoubleData*((double)8388608);
			if(DoubleData1>0) DoubleData1+=0.5;
			else DoubleData1-=0.5;
			Data=(S32)(DoubleData1);	//see UM for calculate
		}
		*BufferP=Data;	    
	}
	else if(Type==HERO_2YDATA)		// 2 Y memory data
	{
		if(DoubleData==1)
		{
			Data=0x7FFFFF;
		}
		else
		{
			DoubleData1=DoubleData*((double)8388608);
			if(DoubleData1>0)
				 DoubleData1+=0.5;
			else
				 DoubleData1-=0.5;
			Data=(S32)(DoubleData1);	//see UM for calculate
		}

		*BufferP=(Data>>12);
		*BufferP&=0x0FFF;
		BufferP++;
		*BufferP=Data&0x0FFF;
		*BufferP>>=1;
	}
	else
	{ 
		return PARA_ERROR;
	}

	return( SUCCESS );
}
/*-------------------------------------------------------------------------
 Function:WriteDataToHero(..)
 Input: 
      memType: I2CDRV_X_MEM,I2CDRV_Y_MEM
	  address: SubAddress 
	  dataLength:write data length(note unit is U32)
	  pData: write data buffer
 OutPut:
      Null
---------------------------------------------------------------------------*/
U8 WriteDataToHero(I2CDrv_MemType_t memType, U16 address,U8 dataLength,S32*  pData)
{
	U8 buffer[56];
	U8 byteOrder=0;
	U8 idx;
	U8 ret=ERROR;

	if((0 == pData) || (0 == dataLength))
	{/* No data to write */
	   return ret;
	}

	byteOrder=0;
	/* updated only for DSP2 */
	buffer[byteOrder++] = 0xF2;
	switch(memType)
	    {
		case I2CDRV_X_MEM:
			    buffer[byteOrder++] = (U8)((address & 0xff00) >> 8);/* Memory location address MS byte */
			    buffer[byteOrder++] = (U8)(address & 0xff);/* Memory location address LS byte */

			    for(idx = 0;idx < dataLength;idx++)		/*Fill send buffer*/
			    {
				      buffer[byteOrder++] = (U8)((pData[idx] & 0xff0000)>>16);
			            buffer[byteOrder++] = (U8)((pData[idx] & 0xff00)>>8);
			            buffer[byteOrder++] = (U8)(pData[idx] & 0xff);
			     }
		    break;

		case I2CDRV_Y_MEM:
			    buffer[byteOrder++] = 0x40 |(U8)((address & 0xff00) >> 8);/* Memory location address MS byte */
			    buffer[byteOrder++] = (U8)(address & 0xff);/* Memory location address LS byte */  
			    for(idx = 0;idx < dataLength;idx++)
			     {
			            buffer[byteOrder++] = (U8)((pData[idx] & 0xff00)>>8);
			            buffer[byteOrder++] = (U8)(pData[idx] & 0xff);
			      }
			    break;
		case I2CDRV_EP_MEM:
			    buffer[byteOrder++] = 0X80 | (U8)((address & 0xff00) >> 8);/* Memory location address MS byte */
			    buffer[byteOrder++] = (U8)(address & 0xff);/* Memory location address LS byte */
			    break;
		default:
		           break;
	   }
	if(byteOrder==0)
	    return ret;	

	ret=I2C1_WriteData(TEF663x_SlaveAddr,buffer,byteOrder);

	return ret;
}

/*-----------------------------------------------------------------------
Input:	Addr: the config data addr
             Num: the num of data to be read
             BufferP: data read out buffer
Output:			
Description:  read radio config data		 
------------------------------------------------------------------------*/
S8 Hero_ReadBack(U8 Addr,U8 Num,U8 *BufferP)
{
	U8 Buffer[2];

	if(((Addr+Num)>0x40)||(Num==0)||(BufferP==NULL))
	{
		return PARA_ERROR;
	}

	Buffer[0]=0xE0;
	Buffer[1]=Addr;
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,2);	 //start to send data to i2c1 bus

	TimerDelayMs(2);
	I2C1_ReadData(TEF663x_SlaveAddr,BufferP,Num);
	return SUCCESS;
}


/*---------------------------------------------------------------------------
 Function:  WriteHero_MEM_1Word
 Input: 
          memType: I2CDRV_X_MEM,I2CDRV_Y_MEM
	  address: SubAddress 
	  Data:
 OutPut:
 Description: write one word to here
-----------------------------------------------------------------------------*/
void WriteHero_MEM_1Word(I2CDrv_MemType_t memType, U16 address,S32  Data)
{
	WriteDataToHero(memType,address,1,&Data);
}
/*----------------------------------------------------------------------------
 Function:  WriteHero_MEM_Tab
 Input: 
	  tab:
 OutPut:
          Description: write one word to here
-----------------------------------------------------------------------------*/
void WriteHero_MEM_Tab(const sMemAddr_Data *tab)
{
	while(tab->type != I2CDRV_INVALID)
	{
		WriteHero_MEM_1Word(tab->type, tab->addr,tab->data);
		tab++;
	}
}
/*-----------------------------------------------------------------------
		  
Input:	Data:the data will be write the the ADSP_X_EasyP_Index		
Output:	NULL		
Description: 	hero easy programm write api 
------------------------------------------------------------------------*/
void TEF663x_EasyProgrammWrite(U32 Data)
{
	U8 Buffer[6];

	Buffer[0]=0xF2;
	Buffer[1]=(U8)(ADSP_X_EasyP_Index>>8);	  //addr is 2 bytes
	Buffer[2]=(U8)ADSP_X_EasyP_Index;	

	while((Buffer[0]!=0)&&(Buffer[1]!=0)&&(Buffer[2]!=0))   //check if the easyprogramm routing is able to write 
	{
		Buffer[0]=0xF2;
		Buffer[1]=(U8)(ADSP_X_EasyP_Index>>8);	  //addr is 2 bytes
		Buffer[2]=(U8)ADSP_X_EasyP_Index;
		I2C1_WriteData(TEF663x_SlaveAddr,Buffer,3);	 //start to send data to i2c1 bus  

		I2C1_ReadData(TEF663x_SlaveAddr,Buffer,3);	 //start to read to buffer
		Printf("%x %x %x \n",Buffer[0],Buffer[1],Buffer[2]);
	}

	//prepare the data to send
	Buffer[0]=0xF2;
	Buffer[1]=(U8)(ADSP_X_EasyP_Index>>8);	  //addr is 2 bytes
	Buffer[2]=(U8)ADSP_X_EasyP_Index;
	Buffer[3]=(U8)(Data>>16);	 //X memory so 3 bytes
	Buffer[4]=(U8)(Data>>8); 
	Buffer[5]=(U8)Data;

	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,6);	 //start to send data to i2c1 bus  
}
/*----------------------------------------------------------------------------
 Function:WriteScratchDataToHero(..)
 Input: 
      memType: I2CDRV_X_MEM,I2CDRV_Y_MEM
	  address: SubAddress 
	  dataLength:write data length(note unit is U32)
	  pData: write data buffer
 OutPut:
      Null
 Description:
        Write scratch data to hero
-----------------------------------------------------------------------------*/
U8 WriteScratchDataToHero(I2CDrv_MemType_t memType,
                                                U16 address,
                                                U8          dataLength,
                                                 U32*  pData)
{/* Generic routine for deferred writing of variable number of bytes maximum 246 as there can be 10 bytes extra due to 
 device address + I2C Monitor cmd and (address + size) bytes of I2C monitor program */

	U8    byteOrder = 0, idx, easyP_reads = 2;

	/* For robustness, allow for 2 reads (should be sufficient with I2C speed versus DSP speed)
	to clear scratchpad by DSP if initially still in use. */

	U8 ret;

	U32   scrMemAdd = (U32)ADSP_Y_UpdatC_Coeff0_REL;/* Write the group of coefficients to the scratch register, starting at ADSP_Y_UpdatC_Coeff0 */
	U32   dstMemAdd = (U32)ADSP_X_SrcSw_OrMask_REL;
	/* Specify the YMEM destination of the first coefficient in the scratch register by loading */
	/* ADSP_X_SrcSw_OrMask with the relative YMEM address of the first coefficient in the coefficient group */
	U32   easyPrgMemAdd = (U32)ADSP_X_EasyP_Index_REL;
	/* Write the (hexadecimal) number of coefficients to be copied in ADSP_X_EasyP_Index */

	U8 buffer[64];
	U32 numOfBytes;

	 /* Legacy implementation for DSP2 */
	memType = memType; /* Uses only Y-mem scratchpad;parameter is therefore ignored */

	if((0 == pData) && (0 == dataLength))
	{/* No data to write */
	    ret = ERROR;
	}

	else if (dataLength <= 12)
	 {/* Easy Programming command for coefficient update */
	     do
	       {
	            /* =============== EASY PROGRAMMING  for deferred data copy begins ================= */

	            /* Confirm that ADSP_X_EasyP_Index_REL location contains Zero value before writing new command 
	            or copying any data to scratch pad */
	            byteOrder = 0;
	            buffer[byteOrder++] = 0xF2;/* Indicate to I2C Monitor that Address follows before actual data */
	            buffer[byteOrder++] = (U8)((easyPrgMemAdd & 0x0f00) >> 8);/* Memory location address MS byte */
	            buffer[byteOrder++] = (U8)(easyPrgMemAdd & 0xff);/* Memory location address LS byte */

	            numOfBytes = byteOrder;/*  I2C Mon cmd(1) + MemLoc(4) + Easy Programming instruction or command */

	            ret=I2C1_WriteData(TEF663x_SlaveAddr,buffer,byteOrder);

	            byteOrder = 0;
	            buffer[byteOrder++] = (U8)(0xff);
	            buffer[byteOrder++] = (U8)(0xff);
	            buffer[byteOrder++] = (U8)(0xff);

	            numOfBytes = byteOrder;/* number of bytes to read + 1 */
	   
	          I2C1_ReadData(TEF663x_SlaveAddr,buffer,numOfBytes);  
	            easyP_reads--;	
			 //if(easyP_reads==0)
			 	///Dbg_printf("erro1\n");
	        }
	        while(((0 != buffer[0]) || (0 != buffer[1]) || (0 != buffer[2])) && (easyP_reads > 0));
	        /* Wait till the ADSP_X_EasyP_Index_REL location is cleared & 
	        make 'easyP_reads' number of attempts to read the location while not cleared,
	        if it's still not cleared then exit*/

	        if((0 == buffer[0]) && (0 == buffer[1]) && (0 == buffer[2]))
	            {/* ADSP_X_EasyP_Index_REL cleared */

	                /* Fresh count for read attempts */
	                easyP_reads = 2;

	                /* The scratch buffer size is limited, which needs to be managed by the RAC (reference audio controller) */
	                byteOrder = 0;
	  
	                buffer[byteOrder++] = (U8)(0xF2);/* Indicate to I2C Monitor that Address follows before actual data */

	                /* Scratch memory in YMEM - only for DSP2 */
	                buffer[byteOrder++] = 0x40 |(U8)((scrMemAdd & 0xff00) >> 8);/* Memory location address MS byte */
	                buffer[byteOrder++] = (U8)(scrMemAdd & 0xff);/* Memory location address LS byte */

	              
	 
	                for(idx = 0;idx < dataLength;idx++)
	                 {
	                        buffer[byteOrder++] = (U8)((pData[idx] & 0xff00)>>8);
	                        buffer[byteOrder++] = (U8)(pData[idx] & 0xff);
	                  }
	                    
	                numOfBytes = byteOrder;/* I2C Mon cmd(1) + MemLoc(4) */

	                 ret=I2C1_WriteData(TEF663x_SlaveAddr,buffer,numOfBytes);
			
	                if((U8)ret == SUCCESS)
	                {
	                    /* Scratch memory in XMEM - only for DSP2 */
	                    byteOrder = 1;

	                    buffer[byteOrder++] = (U8)((dstMemAdd & 0xff00) >> 8);/* Memory location address MS byte */
	                    buffer[byteOrder++] = (U8)(dstMemAdd & 0xff);/* Memory location address LS byte */

	                    buffer[byteOrder++] = (U8)(0);/* Dummy to fill THREE bytes for XMEM write */
	                    buffer[byteOrder++] = (U8)((address & 0xff00) >> 8);/* "Relative" Memory location address MS byte */
	                    buffer[byteOrder++] = (U8)(address & 0xff);/* "Relative" Memory location address LS byte */

	                    numOfBytes = byteOrder;

	                    ret=I2C1_WriteData(TEF663x_SlaveAddr,buffer,numOfBytes);
	                    if((U8)ret == SUCCESS)
	                    {
	                        /* Flag the Easy_P index to copy data from scratch pad to the destination address provided */
	                        byteOrder = 0;                        
	                        buffer[byteOrder++] = (U8)(0xF2);/* Indicate to I2C Monitor that Address follows before actual data */

	                        buffer[byteOrder++] = (U8)((easyPrgMemAdd & 0xFF00) >> 8);/* Memory location address MS byte */
	                        buffer[byteOrder++] = (U8)(easyPrgMemAdd & 0xFF);/* Memory location address LS byte */

	                        buffer[byteOrder++] = 0;
	                        buffer[byteOrder++] = 0;
	                        buffer[byteOrder++] = (U8)(dataLength & 0xFF);

	                        numOfBytes = byteOrder;/*  + I2C Mon cmd(1) + MemLoc(4) + Easy Programming instruction or command */

	                         ret=I2C1_WriteData(TEF663x_SlaveAddr,buffer,numOfBytes);
	                        /* Write the (hexadecimal) number of coefficients to be copied in ADSP_X_EasyP_Index_REL */
	                    }
	                }
	            }
	        }
	        /* =============== EASY PROGRAMMING for deferred data copy ends ================= */
	    
	   else if((0 == pData) && (dataLength > 12))
	    {/* Other Easy Programming instructions */
	        do
	        {
	            /* Confirm that ADSP_X_EasyP_Index_REL location contains Zero value before writing new command */

	            byteOrder = 0;
	            buffer[byteOrder++] = 0xF2;/* Indicate to I2C Monitor that Address follows before actual data */

	            buffer[byteOrder++] = (U8)((easyPrgMemAdd & 0xFF00) >> 8);/* Memory location address MS byte */
	            buffer[byteOrder++] = (U8)(easyPrgMemAdd & 0xFF);/* Memory location address LS byte */

	            numOfBytes = byteOrder;/* I2C Mon cmd(1) + MemLoc(4) + Easy Programming instruction or command */

	            ret=I2C1_WriteData(TEF663x_SlaveAddr,buffer,numOfBytes);

	            byteOrder = 0;       
	            buffer[byteOrder++] = 0xff;
	            buffer[byteOrder++] = 0xff;
	            buffer[byteOrder++] =0xff;

	            numOfBytes = byteOrder;/* number of bytes to read + 1 */

	            I2C1_ReadData(TEF663x_SlaveAddr,buffer,numOfBytes);  

	            easyP_reads--;
	        }
	        while(((0 != buffer[0]) || (0 != buffer[1]) || (0 != buffer[2])) && (easyP_reads > 0));
	        /* Wait till the ADSP_X_EasyP_Index_REL location is cleared & 
	        make 'easyP_reads' number of attempts to read the location while not cleared,
	        if it's still not cleared then exit*/
	         if((0 == buffer[0]) && (0 == buffer[1]) && (0 == buffer[2]))
	            {/* ADSP_X_EasyP_Index/_REL cleared */

	                /* Fresh count for read attempts */
	                easyP_reads = 2;

	                byteOrder = 0;
	                buffer[byteOrder++] = 0xF2;/* Indicate to I2C Monitor that Address follows before actual data */

	                switch(memType)
	                {/* updated only for DSP2 */
		                case I2CDRV_X_MEM:
		                    buffer[byteOrder++] = (U8)((address & 0xff00) >> 8);/* Memory location address MS byte */
		                    buffer[byteOrder++] = (U8)(address & 0xff);/* Memory location address LS byte */

		                    buffer[byteOrder++] = 0;
		                    buffer[byteOrder++] = 0;
		                    buffer[byteOrder++] = (U8)(dataLength & 0xff);/* Easy Programming Instruction */
		                    break;

		                case I2CDRV_Y_MEM:
		                    buffer[byteOrder++] = 0x40 |(U8)((address & 0xff00) >> 8);/* Memory location address MS byte */
		                    buffer[byteOrder++] = (U8)(address & 0xff);/* Memory location address LS byte */

		                    buffer[byteOrder++] = 0;
		                    buffer[byteOrder++] = (U8)(dataLength & 0xff);/* Easy Programming Instruction */
		                    break;

		                case I2CDRV_EP_MEM:
		                    buffer[byteOrder++] = 0x80 | (U8)((address & 0xff00) >> 8);/* Memory location address MS byte */
		                    buffer[byteOrder++] = (U8)(address & 0xff);/* Memory location address LS byte */
		                    break;

		                default:
		                    ret = ERROR;
		                    break;
	                }

	                numOfBytes = byteOrder;/* DevAddr(1) + I2C Mon cmd(1) + MemLoc(4) + Easy Programming instruction or command */

	                ret=I2C1_WriteData(TEF663x_SlaveAddr,buffer,numOfBytes);
	                /* Write the (hexadecimal) number of coefficients to be copied in ADSP_X_EasyP_Index_REL */
	            }
	     
	    }
	    else
	    {/* Unexpected data pointer value */
	       ret = ERROR;
	    }
	return ret;
}


/*-----------------------------------------------------------------------
Function name:	Audio_DrvGetAddress
Input:		  
                                 From                                     
                                 AddressPointer                                                                        
Output:			null
Description: returns the appropriate label via * AddressPointer as a function of the From feature 
------------------------------------------------------------------------*/
static void Audio_DrvGetAddress(eAudio_Feature_t From, S32 *AddressPointer)
{
	static const U16 OutputAddress[] =
	{
		0x0UL, 0x0UL,                                      /* Not Used, Balance = 1, */ 
		0x0UL, 0x0UL,                                      /* ChannelGain = 2, ChannelMute = 3, */
		ADSP_X_Chime_Cl_Out_REL,ADSP_X_Chime_Cl_Out_REL,                                      /* ChimeGenerator = 4, ClickClack = 5, */
		0x0UL, 0x0UL,                                      /* ClipFrontRear = 6, ClipSub = 7, */
		0x0UL, 0x0UL,                                      /* ClipCenter = 8, ClipRearSecondary = 9, ->RESERVED */
		ADSP_X_CompExp_OutPL_REL, ADSP_X_DCfilt_OutPL_REL, /* Compressor = 10, DC = 11, */
		0x0UL, 0x0UL,    /* DeEmphasis = 12, Delay = 13, */
		0x0UL, 0x0UL,                                      /* Fader = 14, FastMute = 15, */
		ADSP_X_GEq_OutFL_REL, ADSP_X_GPF_OutL_REL,         /* GEQ = 16, GPAF1a = 17, */
		ADSP_X_GPF_OutR_REL, ADSP_X_GPF2_OutL_REL,         /* GPAF1b = 18, GPAF2 = 19, */
		ADSP_X_GPF3_OutL_REL, ADSP_X_GPF3_OutR_REL,        /* GPAF3a = 20, GPAF3b = 21, */
		ADSP_X_GPF4_OutL_REL, ADSP_X_GPF5_OutL_REL,        /* GPAF4 = 22, GPAF5 = 23, */
		ADSP_X_IBblnd_OutL_REL,                            /* HDBlend = 24, */
		0x0UL, ADSP_X_Loudf_OutL_REL,                      /* Limiter = 25, Loudness = 26, */
		ADSP_X_MkSubwLeft_REL, 0x0UL,                      /* MakeSubCenter  = 27, Noise = 28, */
		0x0UL, ADSP_X_Eq_OutFL_REL,                        /* ParkingDistanceControl = 29, PEQ = 30, */
		0x0UL, 0x0UL,                                      /* Sine = 31, SoftAudioMute = 32, */
		ADSP_X_PChannelMode_OutL_REL, 0x0UL,               /* SourceScaling = 33, SRC1 = 34, */
		0x0UL, 0x0UL,                                      /* SRC2 = 35, SRC3 = 36, */
		ADSP_X_FrontOutL_REL, ADSP_X_ToneOutPL_REL,        /* SuperPosition = 37, Tone = 38, */
		ADSP_X_Vol_OutFL_REL, 0x0UL,                       /* Volume = 39, VoiceFilter = 40, ->RESERVED */
		0x0UL, 0x0UL,                                      /* SecondarySuperposition = 41, ->RESERVED  SecondaryVolume = 42, ->RESERVED */
		0x0UL, 0x0UL,                                      /* FrontDAC = 43, RearDAC = 44 */
		0x0UL, 0x0UL,                                      /* I2SOUT0 = 45, I2SOUT1 = 46 */
		0x0UL, 0x0UL,                                      /* I2SOUT2 = 47, ->RESERVED  I2SOUT3 = 48, ->RESERVED */
		ADSP_X_Navb4EQ_REL, ADSP_X_Phonb4EQ_REL,            /* NavigationFilter = 49, PhoneFilter = 50 */
		ADSP_X_PCHIME_out_REL/*PolyChime         =51*/
	};
	*AddressPointer = (S32)(OutputAddress[From]);
}
#if 0
/*-----------------------------------------------------------------------
Function name:	Audio_DrvSetInputAnalogMode
Input:		  
                                 Input
                                       input select
                                 Mode 
                                       only avilable AIN0~AIN3
                                     
Output:			null
Description:	set ain0~ain3 mode
------------------------------------------------------------------------*/
void Audio_DrvSetInputAnalogMode(eAudio_InputSource Input,eInput_Analog_Mode mode)
{
	U8 buff[3];
	buff[0]=0xA9;
	/*check parameters*/
	if((Input<INPUT_SOURCE_AIN0)||(Input>INPUT_SOURCE_AIN3))
		return;
		
	buff[1]=Input-INPUT_SOURCE_AIN0;//trans to the realy channel ID
	buff[2]=mode;
	I2C1_WriteData(TEF663x_SlaveAddr,buff,3);
}
#endif
/*-----------------------------------------------------------------------
		  
Input:	PeripheralID: see define
		OnOff: 0=off,others=on
Output:			
Description: 	set the peripheral work state 
------------------------------------------------------------------------*/
S8 Audio_DrvPeripheralSwitch(U8 PeripheralID, U8 OnOff)
{
	U8 Buffer[3];

	Buffer[0]=0xA9;  
	Buffer[1]=PeripheralID;
	if(OnOff)
	{
		Buffer[2]=DAC_ON;
	}
	else
	{
		Buffer[2]=DAC_OFF;
	}
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,3);	 //start to send data to i2c1 bus
	return SUCCESS;
}

/*-----------------------------------------------------------------------
Function name:	Audio_DrvSetNavInput
Input:			    
                                 Input
                                       AIN3,IIS0~IIS2,SPDIF
                                  
Output:			null
Description:	Select Nav input
------------------------------------------------------------------------*/
void Audio_DrvSetNavInput(eAudio_InputSource Input)
{
       switch(Input)
	 {
	 	case INPUT_SOURCE_AIN3:
                        WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_EasyP_Index,ADSP_EASYP_SrcSw_AIN3onNav);
			break;
		case INPUT_SOURCE_IIS0:
                        WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_pSRCInputFlag2,ADSP_X_InputFlagIIS0);
			break;
		case INPUT_SOURCE_IIS1:
                        WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_pSRCInputFlag2,ADSP_X_InputFlagIIS1);
			break;
		case INPUT_SOURCE_IIS2:
                        WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_pSRCInputFlag2,ADSP_X_InputFlagIIS2);
			break;
		case INPUT_SOURCE_SPDIF:
			 WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_pSRCInputFlag2,ADSP_X_InputFlagSPDIF);
			break;
		default:
		    break;
	 }
}
/*-----------------------------------------------------------------------
Function name:	Audio_DrvSetPhoneInput
Input:			    
                                 Input
                                       AIN3,IIS0~IIS2,SPDIF
                                  
Output:			null
Description:	select Phone input
------------------------------------------------------------------------*/
void Audio_DrvSetPhoneInput(eAudio_InputSource Input)
{
     switch(Input)
	 {
	 case INPUT_SOURCE_AIN2:
                WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_EasyP_Index,ADSP_EASYP_SrcSw_AIN2onPhone);
		break;
	 case INPUT_SOURCE_IIS0:
                WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_pSRCInputFlag3,ADSP_X_InputFlagIIS0);
		break;
	 case INPUT_SOURCE_IIS1:
                WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_pSRCInputFlag3,ADSP_X_InputFlagIIS1);
		break;
	 case INPUT_SOURCE_IIS2:
                WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_pSRCInputFlag3,ADSP_X_InputFlagIIS2);
		break;
	 case INPUT_SOURCE_SPDIF:
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_pSRCInputFlag3,ADSP_X_InputFlagSPDIF);
		break;
	default:
		break;
	  }

}
/*-----------------------------------------------------------------------
Function name:	Audio_DrvSetStreamInput
Input:			stream
                                      PrimaryStr,
                                      SecondaryStr ,
                                      NavigationStr,
                                      PhoneStr      
                                 Input
                                       input select
                                
                                       
                                     
Output:			null
Description:	set the audio channel input source and the input source mode
------------------------------------------------------------------------*/
void Audio_DrvSetStreamInput(Audio_Stream_t stream,eAudio_InputSource Input/*,U8 mode*/)
{
	U8 buff[2];
#if 1
	/*set analog input mode*/
	//Audio_DrvSetInputAnalogMode(Input,mode);
	if((Input>=INPUT_SOURCE_AIN0)&&(Input<=INPUT_SOURCE_AIN3))
		;///Audio_DrvSetADInMode(Input-INPUT_SOURCE_AIN0, 3);
	else if(Input==INPUT_SOURCE_SPDIF)
		Audio_DrvSetSPDIFInMode(1);
	else if((Input>=INPUT_SOURCE_IIS0)&&(Input<=INPUT_SOURCE_IIS2))
		Audio_DrvSetI2SInMode(Input-INPUT_SOURCE_IIS0, 0);
#endif
	switch(stream)
	{
		case PrimaryStr:
			if(Input!=INPUT_SOURCE_AIN3) //Ain3 not avilable for primary and sencond
			{
				buff[0]=0x20;
				buff[1]=Input;
				I2C1_WriteData(TEF663x_SlaveAddr,buff,2);
			}
			break;
		case SecondaryStr:
			if(Input!=INPUT_SOURCE_AIN3) //Ain3 not avilable for primary and sencond
			{
				buff[0]=0x28;
				buff[1]=Input;
				I2C1_WriteData(TEF663x_SlaveAddr,buff,2);
			}
			break;
		case NavigationStr:
			Audio_DrvSetNavInput(Input);
			break;
		case PhoneStr:
			Audio_DrvSetPhoneInput(Input);
			break;
		default:
			break;		
	}
	
}


/* Connect any of the superposition add-on connections to any of the add-on streams */
/*****************************************************************************
Function       : 
    void Audio_DrvSuperPositionConnect(U16 AddOnIndex,
                                    eAudio_Feature_t Feature,
                                    eAudioChannel Channel)

Input        :
    AddOnIndex
        "Add-Ons"    
        1    Navigation
        2    Phone
        3    Extern
        4    Chime

    Feature
        Navigation
        Phone
        Chime
        Click-Clack

    Channel
         Left/Right

Output        :
    None

Return        :
    None

Description    :
    Connect any of the superposition add-on's 'index' to the output channel of Feature
*****************************************************************************/
void Audio_DrvSuperPositionConnect(U16 AddOnIndex, eAudio_Feature_t Feature, eAudioChannel Channel)
{
	U16 Label = 0;
	S32 FeatureAddress;/* 'From' */

	Audio_DrvGetAddress(Feature, &FeatureAddress);
	switch (AddOnIndex)
	{
		case 1: /* Navigation */
			Label = ADSP_X_Sup_NInPntr_REL;
			break;

		case 2: /* Phone */
			Label = ADSP_X_Sup_TInPntr_REL;
			break;

		case 3: /* Extern */
			Label = ADSP_X_Sup_ExtInPntr_REL;
			break;

		case 4: /* Chime */
			Label = ADSP_X_Sup_ChiInPntr_REL;
			break;    

		default:
			break;
	}
	if (Channel == Right)
	{
		FeatureAddress++; /* choice of the channel only applies to the two I2S Features */
	}
	if (Label != 0)
	{
		WriteHero_MEM_1Word(I2CDRV_X_MEM,Label,FeatureAddress);
	}
}



/* Predefined superposition gains */
/*-----------------------------------------------------------
Function       : 
    Audio_DrvSuperPositionGain(U32 StreamIndex, U32 AddOnIndex, S32 PresetLeft, S32 PresetRight) 

Input        :
    StreamIndex
        "Streams"    
        1    Primary audio stream front channels
        2    Primary audio stream rear channels
        3    Primary audio stream subwoofer/center channels

    AddOnIndex
        "Add-Ons"    
        1    Navigation
        2    Phone
        3    Extern
        4    Chime

    PresetLeft
      0            Flat
    -1            1dB attenuation
    ?           ?
    -66            66 dB attenuation
    MINUS_INF    Infinite attenuation

    PresetRight
      0            Flat
    -1            1dB attenuation
    ?           ?
    -66            66 dB attenuation
    MINUS_INF    Infinite attenuation

Output        :
    None

Return        :
    None

Description    :
    Select any of the predefined gain settings for the superposition matrix location defined by "streams" and "add-ons".

    Note: Not the entire superposition matrix is filled. Navigation and phone add-ons can only be superimposed on primary (front, rear, center, subwoofer) channels.
    Preset for CenterSub Right (SubWoofer) will be ignored
----------------------------------------------------------*/
void Audio_DrvSuperPositionGain(U32 StreamIndex, U32 AddOnIndex, S32 PresetLeft, S32 PresetRight) 
{
    U8 Length = 2;
    U16 Label = 0;
    S32 Gain[2];

    Gain[0] = table_dB2Lin[-PresetLeft];
    Gain[1] = table_dB2Lin[-PresetRight];

    switch (StreamIndex)
    {
        case 1: /* Primary Front */
            switch (AddOnIndex)
               {
                 case 1: /* Navigation */
                       Label = ADSP_Y_Sup_NonFL_REL;
                       break;
                 case 2: /* Phone */
                       Label = ADSP_Y_Sup_TonFL_REL;
                       break;
                 case 3: /* Extern */
                       Label = ADSP_Y_Sup_ExtonFL_REL;
                       break;
                case 4: /* Chime */
                       Label = ADSP_Y_Sup_ChionFL_REL;
                       break;
                default:
                      break;
             }
          break;

    case 2: /* Primary Rear */
          switch (AddOnIndex)
            {
                case 1: /* Navigation */
                      Label = ADSP_Y_Sup_NonRL_REL;
                      break;
                case 2: /* Phone */
                      Label = ADSP_Y_Sup_TonRL_REL;
                      break;
                case 3: /* Extern */
                      Label = ADSP_Y_Sup_ExtonRL_REL;
                      break;
                case 4: /* Chime */
                      Label = ADSP_Y_Sup_ChionRL_REL;
                      break;
                default:
                    break;   
         }
         break;
    case 3: /* Primary SubWoofer/Center */
        switch (AddOnIndex)
         {
               case 1: /* Navigation */
                  Label = ADSP_Y_Sup_NonSW_REL;
                  Length = 1;
                  break;
              case 2: /* Phone */
                  Label = ADSP_Y_Sup_TonSW_REL;
                  break;
              case 3: /* Extern */
                  Label = ADSP_Y_Sup_ExtonSW_REL;
                  break;
              case 4: /* Chime */
                  Label = ADSP_Y_Sup_ChionSW_REL;
                  break;
              default:
                  break;   
         }
        break;
    default:
        break;
    }
    WriteDataToHero(I2CDRV_Y_MEM,Label,Length,Gain);
}




/*-----------------------------------------------------------------------
Function name:	Audio_DrvConnectExtInputTo(char nChannel)
Input:			nChannel:Click_clack,Poly chime                      
Output:			
Description:	
              Connect Ext Input to Click_clack or Poly Chime
------------------------------------------------------------------------*/
void Audio_DrvConnectExtInputTo(char nChannel)
{
	S32 ConnectTo;

	switch(nChannel)
	{
	  case eCHANNEL_CLICK_CLACK:	 //connect to ClickClack feature
		ConnectTo=ADSP_X_Chime_Cl_Out_REL;
		break;
	  case eCHANNEL_POLY_CHIME:	     //connect to Poly chime feature
		ConnectTo=ADSP_X_PCHIME_out_REL;
		break;
	  default:
	      return;
	}
	WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Sup_ExtInPntr_REL,ConnectTo);
}
/*-----------------------------------------------------------------------
Function name:	Audio_DrvClickClack_Gen
Input:			
           Mode:I2C_CONTROL,IO_FLAG      
		   Sound:CLICK,CLACK               
Output:			
Description:	
              Set Click Clack Generator control mode
------------------------------------------------------------------------*/
void Audio_DrvClickClack_Gen(eAudio_ClickClackControlMode_t Mode,eAudio_ClickClackSound_t Sound )
{
	S32 data;

	if(Mode==I2C_CONTROL)   /*I2C control mode*/
	{   
		if(Sound==CLICK)      {/*generator click sound */
			data=0xFFFFFF;
		}	  
		else {/*generator clack sound*/
			data=0x000001;
		}
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_WavTab_Control,data);
	}
	else /*IO_FLAG control mode */
	{
		/* rise edge is click sound
		falling edge is clack	sound*/
		/*data=LPC_GPIO1->FIOPIN0;
		data=data^0x01;
		LPC_GPIO1->FIOSET0=data;*/

	}

}
 /*-----------------------------------------------------------------------
Function name:	ClickClack_GPIOConfig
Input:		Config MUC's GPIO to control ClickClack Generator sound                     
Output:			
Description:	
              Set Click Clack Generator control mode
------------------------------------------------------------------------*/
 void ClickClack_GPIOConfig(void)
 {
      /*Set mcu p1.0 as gpio,output*/
      ///LPC_PINCON->PINSEL2&=~0x00000003;//p1.0 as gpio
      ///LPC_GPIO1->FIODIR|=0x00000001;//set p1.0 output
 }
/*-----------------------------------------------------------------------
Function name:	Audio_DrvClickClack_ModeSel
Input:			mode:I2C_CONTROL,IO_FLAG                     
Output:			
Description:	
              Set Click Clack Generator control mode
------------------------------------------------------------------------*/
void Audio_DrvClickClack_ModeSel(eAudio_ClickClackControlMode_t mode)
{
	S32 data;
	U8 SendDataBuf[6];
	U32 Label=ADSP_X_WavTab_UseIOFlag; //0x057f
	if(mode==I2C_CONTROL)  {/*I2C control*/
		   data=0x000000;
	}
	else {/*IO Control*/
	   ClickClack_GPIOConfig();
	   data=0x000001; 
	 }
	/*Fill Send Buffer,and send*/
	SendDataBuf[0]=0xF2;
	SendDataBuf[1]=(Label>>8)&0xff;
	SendDataBuf[2]=Label&0xff;
	SendDataBuf[3]=(data>>16)&0xff;
	SendDataBuf[4]=(data>>8)&0xff;
	SendDataBuf[5]=data&0xff;
	I2C1_WriteData(TEF663x_SlaveAddr,SendDataBuf,6);	
}

/*-----------------------------------------------------------------------
Function name:	Audio_DrvClickClack_Locate
Input:			locate:USE_RAM,USE_ROM                     
Output:			
Description:	
              Set Click Clack WavTable Locate
------------------------------------------------------------------------*/
void Audio_DrvClickClack_Locate(eAudio_ClickClackWavTable_t locate)
{
	if(locate==USE_ROM)
	{
		/*set wavtab in rom*/
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_WavTab_UseRamFlag,0);
	}
	else/*recently do not use this*/
	{
	}	
}



/*-----------------------------------------------------------------------
Function name:	Audio_DrvPolyChimeSetup
Input:			mode                   
Output:			NULL
Description:	
                  set  PolyChime setup 
------------------------------------------------------------------------*/
void Audio_DrvPolyChimeSetup(char mode)
{
	sAudioPolyChimeParameter_t PChimeUserPara;
	if(mode==USER_DEFINE)
	{
		//sine1 parameter  all the parameter generated by NXP's GUI TEF663x v1.32.17 ;
		PChimeUserPara.SinePara[SINE1].iFreq=0x07ff0159;//sin1 Freq=366HZ
		PChimeUserPara.SinePara[SINE1].iVolume=0x0120;//sin1 Volume=-18db
		//sine2 parameter
		PChimeUserPara.SinePara[SINE2].iFreq=0x07d304ca;//sin2 Freq=1464.4HZ
		PChimeUserPara.SinePara[SINE2].iVolume=0x0020;//sin2 Volume=-36.1db
		//sine3 parameter
		PChimeUserPara.SinePara[SINE3].iFreq=0x07fc0566;//sin3 Freq=400HZ
		PChimeUserPara.SinePara[SINE3].iVolume=0x0120;//sin3 Volume=-18db
		//sine4 parameter
		PChimeUserPara.SinePara[SINE4].iFreq=0x07c100d0;//sin4 Freq=1744.0HZ
		PChimeUserPara.SinePara[SINE4].iVolume=0x0040;//sin4 Volume=-30db
		//sine5 parameter
		PChimeUserPara.SinePara[SINE5].iFreq=0x07f8058f;//sin5 Freq=593HZ
		PChimeUserPara.SinePara[SINE5].iVolume=0x0120;//sin5 Volume=-18db
		//sine6 parameter
		PChimeUserPara.SinePara[SINE6].iFreq=0x078f0424;//sin6 Freq=2337.8HZ
		PChimeUserPara.SinePara[SINE6].iVolume=0x0014;//sin6 Volume=-40db

		//Sinelfo parameter note Offset+Amplitute<=1.0
		PChimeUserPara.SineLFOPara.iFreq=0x000F02;//SineLFO freq=5.05HZ
		PChimeUserPara.SineLFOPara.iOffset=0x0253;//SineLFO offset0.29
		PChimeUserPara.SineLFOPara.iVolume=0x059a;//Amplitute 0.7

		//misc setting
		PChimeUserPara.MiscPara.iDecay=0x07ff063b;//Decay cofficent= 1
		PChimeUserPara.MiscPara.iRepeatTime=0x006d26;//Decay Repeat Time=3s
		PChimeUserPara.MiscPara.iRepetitions=0x0001;//Repetitions=1

		//envloppe1 setting
		PChimeUserPara.EnvloppePara[ENV1].iAttackTime=0x0004f3;//envloppe attack time=0.6005 sec
		PChimeUserPara.EnvloppePara[ENV1].iDecay=0x07FF06C3;//envloppe DECAY time=1.2 sec
		PChimeUserPara.EnvloppePara[ENV1].iDuration=0x000275;//envloppe Duration time=0.8001 sec
		PChimeUserPara.EnvloppePara[ENV1].iVolume=0x0014;//envloppe volume -40db

		//envloppe2 setting
		PChimeUserPara.EnvloppePara[ENV2].iAttackTime=0x0004f3;//envloppe attack time=0.6005 sec
		PChimeUserPara.EnvloppePara[ENV2].iDecay=0x07FF06C3;//envloppe DECAY time=1.2 sec
		PChimeUserPara.EnvloppePara[ENV2].iDuration=0x000275;//envloppe Duration time=0.8001 sec
		PChimeUserPara.EnvloppePara[ENV2].iVolume=0x0006;//envloppe volume -50db

		//envloppe3 setting
		PChimeUserPara.EnvloppePara[ENV3].iAttackTime=0x0004f3;//envloppe attack time=0.6005 sec
		PChimeUserPara.EnvloppePara[ENV3].iDecay=0x07FF06C3;//envloppe DECAY time=1.2 sec
		PChimeUserPara.EnvloppePara[ENV3].iDuration=0x000275;//envloppe Duration time=0.8001 sec
		PChimeUserPara.EnvloppePara[ENV3].iVolume=0x0014;//envloppe volume -40db

		Audio_DrvPolyChimeSetPara(PChimeUserPara); 

	}
	else/*preset mode*/
	{
		Audio_DrvPolyChimeLoadPreset();
	}

}

/*-----------------------------------------------------------------------
Function name:	Audio_DrvPolyChimeStart
Input:			NULL                   
Output:			NULL
Description:	
                  start play poly chime
------------------------------------------------------------------------*/
void Audio_DrvPolyChimeStart(void)
{
	WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_PCHIME_ControlStatusReg,0x01);//play command
}

/*-----------------------------------------------------------------------
Function name:	Audio_DrvPolyChimeStop
Input:			NULL                   
Output:			NULL
Description:	
                 stop play poly chime
------------------------------------------------------------------------*/
void Audio_DrvPolyChimeStop(void)
{
	WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_PCHIME_ControlStatusReg,0x00);//stop play command
}

/*-----------------------------------------------------------------------
Function name:	Audio_DrvPolyChimeLoadPreset
Input:			NULL                   
Output:			NULL
Description:	
                 Load Poly chime IMG Preset Parameter
------------------------------------------------------------------------*/
void Audio_DrvPolyChimeLoadPreset(void)
{
	TEF663x_EasyProgrammWrite(ADSP_EASYP_PCHIME_IMGpreset);
}
/*sine freqecy  setting register list*/
const U16 stbPChimeSineFreAddr[SINE_COUNT]=
{
	ADSP_Y_PCHIME_Freq1_MSB,
	ADSP_Y_PCHIME_Freq2_MSB,
	ADSP_Y_PCHIME_Freq3_MSB,
	ADSP_Y_PCHIME_Freq4_MSB,
	ADSP_Y_PCHIME_Freq5_MSB,
	ADSP_Y_PCHIME_Freq6_MSB
};
/*sine Volume  setting register list*/
const U16 stbPChimeSineVolAddr[SINE_COUNT]=
{	    
	ADSP_Y_PCHIME_Volume1,
	ADSP_Y_PCHIME_Volume2,
	ADSP_Y_PCHIME_Volume3,
	ADSP_Y_PCHIME_Volume4,
	ADSP_Y_PCHIME_Volume5,
	ADSP_Y_PCHIME_Volume6
};
/*envloppe druation setting register list*/
const U16 stbPChimeEnvDurationAddr[ENV_COUNT]=
{
	ADSP_X_PCHIME_Env1_DelaySamples,
	ADSP_X_PCHIME_Env2_DelaySamples,
	ADSP_X_PCHIME_Env3_DelaySamples
};
/*envloppe volume setting register list*/
const U16 stbPChimeEnvVolumeAddr[ENV_COUNT]=
{
	ADSP_Y_PCHIME_Env1_DelayVolume,
	ADSP_Y_PCHIME_Env2_DelayVolume,
	ADSP_Y_PCHIME_Env3_DelayVolume
};

/*envloppe attack setting register list*/
const U16 stbPChimeEnvAttackAddr[ENV_COUNT]=
{
       ADSP_X_PCHIME_Env1_AttackIncr,
   	ADSP_X_PCHIME_Env2_AttackIncr,
   	ADSP_X_PCHIME_Env3_AttackIncr
};
/*envloppe sustain setting register list*/
const U16 stbPChimeEnvSustainAddr[ENV_COUNT]=
{
        ADSP_X_PCHIME_Env1_SustainSamples,
   	ADSP_X_PCHIME_Env2_SustainSamples,
   	ADSP_X_PCHIME_Env3_SustainSamples
};

/*envloppe decay setting register list*/
const U16 stbPChimeEnvDecayAddr[ENV_COUNT]=
{
	ADSP_Y_PCHIME_Env1_DecayCoef_MSB,
	ADSP_Y_PCHIME_Env2_DecayCoef_MSB,
	ADSP_Y_PCHIME_Env3_DecayCoef_MSB
};
/*-----------------------------------------------------------------------
Function name:	Audio_DrvPolyChimeSetPara
Input:			para                   
Output:			NULL
Description:	
                Set PolyChime User Parameter
------------------------------------------------------------------------*/
void Audio_DrvPolyChimeSetPara(sAudioPolyChimeParameter_t para)
{
      U16 Lable;
     S32 data[2];
     char i;
 
     /*init Sine parameter*/
      for(i=0;i<SINE_COUNT;i++)
      {
		/*set Sine freqency*/
		Lable=stbPChimeSineFreAddr[i];
		data[0]=(para.SinePara[i].iFreq&0xffff0000)>>16;
		data[1]=para.SinePara[i].iFreq&0xffff;
		WriteDataToHero(I2CDRV_Y_MEM, Lable,2, data);

		/*set sine volume*/
		Lable=stbPChimeSineVolAddr[i];
		data[0]=para.SinePara[i].iVolume;
		WriteDataToHero(I2CDRV_Y_MEM, Lable,1, data);
      }
     /*init SineLFO parameter*/
     /*set SineLFO freqency*/
     Lable=ADSP_X_PCHIME_AngleIncr_LFO;
     data[0]=para.SineLFOPara.iFreq;
     WriteDataToHero(I2CDRV_X_MEM, Lable,1, data);
	 
     /*set sineLFO amplitude*/
     Lable=ADSP_Y_PCHIME_Ampli_LFO;
     data[0]=para.SineLFOPara.iVolume;
     WriteDataToHero(I2CDRV_Y_MEM, Lable,1, data);
	 
     /*set sineLFO offset*/
     Lable=ADSP_Y_PCHIME_Offset_LFO;
     data[0]=para.SineLFOPara.iOffset;
     WriteDataToHero(I2CDRV_Y_MEM, Lable,1, data);

    /*init Envloppe Parameter*/
     for(i=0;i<ENV_COUNT;i++)
       {
		/*set Envloppe druation */
		Lable=stbPChimeEnvDurationAddr[i];
		data[0]=para.EnvloppePara[i].iDuration;
		WriteDataToHero(I2CDRV_X_MEM, Lable,1, data);

		/*set envloppe volume*/
		Lable=stbPChimeEnvVolumeAddr[i];
		data[0]=para.EnvloppePara[i].iVolume;
		WriteDataToHero(I2CDRV_Y_MEM, Lable,1, data);

		/*set envloppe attack time*/
		Lable=stbPChimeEnvAttackAddr[i];
		data[0]=para.EnvloppePara[i].iAttackTime;
		WriteDataToHero(I2CDRV_X_MEM,Lable,1,data);

		//set envloppe sustain 
		Lable=stbPChimeEnvSustainAddr[i];
		data[0]=para.EnvloppePara[i].iDuration;
		WriteDataToHero(I2CDRV_X_MEM,Lable,1,data);

		/*set envloppe decay*/
		Lable=stbPChimeEnvDecayAddr[i];
		data[0]=(para.EnvloppePara[i].iDecay&0xffff0000)>>16;
		data[1]=para.EnvloppePara[i].iDecay&0xffff;
		WriteDataToHero(I2CDRV_Y_MEM,Lable,2,data);	   	 
   	  }	 

	/*init Misc setting*/
	/*set Mute decay*/
	Lable=ADSP_Y_PCHIME_SMute_DecayCoef_MSB;
	data[0]=(para.MiscPara.iDecay&0xFFFF0000)>>16;
	data[1]=para.MiscPara.iDecay&0x0000FFFF;
	WriteDataToHero(I2CDRV_Y_MEM, Lable,2, data);

	/*Set Repetitions*/
	Lable=ADSP_X_PCHIME_NrRepetitions;
	data[0]=para.MiscPara.iRepetitions;
	WriteDataToHero(I2CDRV_X_MEM, Lable,1, data);

	/*Set Repeat Time*/
	Lable=ADSP_X_PCHIME_RepeatSamples;
	data[0]=para.MiscPara.iRepeatTime;
	WriteDataToHero(I2CDRV_X_MEM, Lable,1, data);
}



/*===========================Audio ToneControl Feature=========*/

/* Tone Control: */

/* Section 0: 60, 80, 100, 200 Hz, Q is always 1*/
/* coefs are dependant on (fs, fc): Gain) is explicitly, hence */
/* it is no entry for the table */
/* b1b are always zero */
/* Table Construction: a1H, a1L, a2H, a2L, b0H, b0L, b1H, b1L, b2H, b2L */
/*
add (zero) coefs to table;datasize compromised at the cost of flexibility 
(i.e. shelving filters possible by simply changing filtercoefs).
*/
static const U16 table_ToneControl_BassBands[40] =
{
    0x7f7, 0x1ac, 0xc08, 0x5bb, 0x004, 0x2de, 0x000, 0x000, 0xffb, 0x522, /* fc =  60, fs = SAMP_FREQ_44KHZ */
    0x7f4, 0x219, 0xc0b, 0x4d8, 0x005, 0x66c, 0x000, 0x000, 0xffa, 0x194, /* fc =  80, fs = SAMP_FREQ_44KHZ */
    0x7f1, 0x275, 0xc0e, 0x3e4, 0x007, 0x1f2, 0x000, 0x000, 0xff8, 0x60e, /* fc = 100, fs = SAMP_FREQ_44KHZ */
    0x7e2, 0x34a, 0xc1c, 0x627, 0x00e, 0x314, 0x000, 0x000, 0xff1, 0x4ec, /* fc = 200, fs = SAMP_FREQ_44KHZ */

    //0x7f7, 0x769, 0xc08, 0x017, 0x004, 0x00b, 0x000, 0x000, 0xffb, 0x7f5, /* fc =  60, fs = SAMP_FREQ_48KHZ */
    //0x7f5, 0x1c4, 0xc0a, 0x557, 0x005, 0x2ab, 0x000, 0x000, 0xffa, 0x555, /* fc =  80, fs = SAMP_FREQ_48KHZ */
   //0x7f2, 0x412, 0xc0d, 0x289, 0x006, 0x545, 0x000, 0x000, 0xff9, 0x2bb, /* fc = 100, fs = SAMP_FREQ_48KHZ */
    //0x7e4, 0x6c3, 0xc1a, 0x3b2, 0x00d, 0x1d9, 0x000, 0x000, 0xff2, 0x627, /* fc = 200, fs = SAMP_FREQ_48KHZ */
};
/*-----------------------------------------------------------------------
Function name:	void Audio_DrvBassSet(Audio_Stream_t Stream,S8 Gain,U32 Freq)
Input:			
             Stream   
                  PrimaryStr        Primary audio stream
                  SecondaryStr    Secondary audio stream
             Gain
                 18         +18dB in specified band in specified channel    
                 ..    
                 0           Flat in specified band in specified channel
                 ..
                 -18        -18dB in specified band in specified channel
                Note: Also intermediate values supported in steps of 1dB
                Note: Prescaler value is depending on maximum of three bands, state needed in API
               Freq          
                 _60_HZ     
                 _80_HZ    
                 _100_HZ   
                 _200_HZ   
                For bass, a 2nd order peaking section(Q=1.0) is used
Output:			NULL
Description:	
                Set Bass parameter
------------------------------------------------------------------------*/
U8 Audio_DrvBassSet(Audio_Stream_t Stream,S8 Gain,eAudioBassCentFreq_t Freq)
{
        U16 Label = 0;
        U32 Coeffs[11] = {0};
        U8 Length = 0;
        U32 TablePointer = 0;
	   U8 ret;
        if (PrimaryStr == Stream)
        {
            Label = ADSP_Y_BMT_a1bHP_REL;
         }
	   else if(SecondaryStr==Stream)
	   {
            Label = ADSP_Y_BMT_a1bHS_REL;
	    }
         Length = 11;
      /* each center frequency has 5 values, frequency number 0 is the 
                       lowest one and first row in the table */
        TablePointer = (U32)(Freq * 10);/* determines the basic entry for this fs and band */

        Coeffs[0] = table_ToneControl_BassBands[TablePointer + 0];
        Coeffs[1] = table_ToneControl_BassBands[TablePointer + 1];
        Coeffs[2] = table_ToneControl_BassBands[TablePointer + 2];
        Coeffs[3] = table_ToneControl_BassBands[TablePointer + 3];
        Coeffs[4] = table_ToneControl_BassBands[TablePointer + 4];
        Coeffs[5] = table_ToneControl_BassBands[TablePointer + 5];
        Coeffs[6] = table_ToneControl_BassBands[TablePointer + 6];
        Coeffs[7] = table_ToneControl_BassBands[TablePointer + 7];
        Coeffs[8] = table_ToneControl_BassBands[TablePointer + 8];
        Coeffs[9] = table_ToneControl_BassBands[TablePointer + 9]; 

        /* 
        * if (gb >= 0)
        *   Gbas = 1/16 * (POWER(10, gb/20) - 1) = 1/16 * POWER(10, gb/20) - 1/16 =
        *   POWER(10, (20 * LOG(1/16))/20) * POWER(10, gb/20) - 1/16
        *   POWER(10, (gb - 24)/20) - 0x080 = _dB2Lin[24 - gb] - 0x080
        * else (gb < 0) 
        *   Gbas = -1/16 * (POWER(10, -(gb/20)) - 1) = -1/16 * (POWER(10, -(gb/20)) + 1/16 =
        *   1/16 - POWER(10, (20 * LOG(1/16))/20) * POWER(10, -(gb/20)) =
        *   1/16 - POWER(10, (- 24 - gb)/20) = 0x080 - _dB2Lin[gb + 24]
        */

        if (Gain >= 0) 
        { 
            Coeffs[10] = table_dB2Lin[24 - Gain] - 0x080;
        }
        else
        {
            Coeffs[10] = 0x080 - table_dB2Lin[24 + Gain];
        }
	  if (Label != 0)
       {
            ret=WriteScratchDataToHero(I2CDRV_Y_MEM,
                                        Label,
                                        Length,
                                        Coeffs);
        }
	  else
	   {
            ret=ERROR;
	   }
	  return ret;
}
void Audio_DrvBassSetGain(Audio_Stream_t Stream,S8 Gain)
{

        U16 Label = 0;

	   S16 data;
        if (PrimaryStr == Stream)
        {
            Label = ADSP_Y_BMT_GbasP;
         }
	   else if(SecondaryStr==Stream)
	   {
            Label = ADSP_Y_BMT_GbasS;
	    }
       /* 
        * if (gb >= 0)
        *   Gbas = 1/16 * (POWER(10, gb/20) - 1) = 1/16 * POWER(10, gb/20) - 1/16 =
        *   POWER(10, (20 * LOG(1/16))/20) * POWER(10, gb/20) - 1/16
        *   POWER(10, (gb - 24)/20) - 0x080 = _dB2Lin[24 - gb] - 0x080
        * else (gb < 0) 
        *   Gbas = -1/16 * (POWER(10, -(gb/20)) - 1) = -1/16 * (POWER(10, -(gb/20)) + 1/16 =
        *   1/16 - POWER(10, (20 * LOG(1/16))/20) * POWER(10, -(gb/20)) =
        *   1/16 - POWER(10, (- 24 - gb)/20) = 0x080 - _dB2Lin[gb + 24]
        */

        if (Gain >= 0) 
        { 
            data = table_dB2Lin[24 - Gain] - 0x080;
        }
        else
        {
            data = 0x080 - table_dB2Lin[24 + Gain];
        }
	   WriteHero_MEM_1Word( I2CDRV_Y_MEM, Label, data);

}

/* Section 1: 500, 700, 1000, 1500 Hz */
/* coefs are dependant on (fs, fc, G(ain)): Q is always 1 */
/* b1m and b2m are always zero */
/* Table Construction: a1H, a1L, a2H, a2L, b0H, b0L in double precision */
static const U16 table_ToneControl_MidBands[456] = 
{
    0x638, 0x25c, 0xdc3, 0x53f, 0x0e1, 0x6a0, /* fc =  500, G = -18, fs = SAMP_FREQ_44KHZ */
    0x65f, 0x35f, 0xd9c, 0x371, 0x0ce, 0x1b8, /* fc =  500, G = -17, fs = SAMP_FREQ_44KHZ */
    0x683, 0x799, 0xd77, 0x679, 0x0bb, 0x73c, /* fc =  500, G = -16, fs = SAMP_FREQ_44KHZ */
    0x6a5, 0x74c, 0xd55, 0x615, 0x0aa, 0x70a, /* fc =  500, G = -15, fs = SAMP_FREQ_44KHZ */
    0x6c5, 0x2f3, 0xd36, 0x1ca, 0x09b, 0x0e5, /* fc =  500, G = -14, fs = SAMP_FREQ_44KHZ */
    0x6e2, 0x334, 0xd19, 0x0f2, 0x08c, 0x479, /* fc =  500, G = -13, fs = SAMP_FREQ_44KHZ */
    0x6fd, 0x0dc, 0xcfe, 0x2bf, 0x07f, 0x160, /* fc =  500, G = -12, fs = SAMP_FREQ_44KHZ */
    0x715, 0x4cf, 0xce5, 0x64c, 0x072, 0x726, /* fc =  500, G = -11, fs = SAMP_FREQ_44KHZ */
    0x72c, 0x007, 0xccf, 0x2a0, 0x067, 0x550, /* fc =  500, G = -10, fs = SAMP_FREQ_44KHZ */
    0x740, 0x388, 0xcba, 0x6b4, 0x05d, 0x35a, /* fc =  500, G =  -9, fs = SAMP_FREQ_44KHZ */
    0x753, 0x05a, 0xca8, 0x182, 0x054, 0x0c1, /* fc =  500, G =  -8, fs = SAMP_FREQ_44KHZ */
    0x763, 0x786, 0xc97, 0x1fd, 0x04b, 0x4ff, /* fc =  500, G =  -7, fs = SAMP_FREQ_44KHZ */
    0x773, 0x211, 0xc87, 0x723, 0x043, 0x791, /* fc =  500, G =  -6, fs = SAMP_FREQ_44KHZ */
    0x781, 0x0f8, 0xc79, 0x7f4, 0x03c, 0x7fa, /* fc =  500, G =  -5, fs = SAMP_FREQ_44KHZ */
    0x78d, 0x52d, 0xc6d, 0x37e, 0x036, 0x5bf, /* fc =  500, G =  -4, fs = SAMP_FREQ_44KHZ */
    0x798, 0x798, 0xc62, 0x0d8, 0x031, 0x06c, /* fc =  500, G =  -3, fs = SAMP_FREQ_44KHZ */
    0x7a3, 0x113, 0xc57, 0x728, 0x02b, 0x794, /* fc =  500, G =  -2, fs = SAMP_FREQ_44KHZ */
    0x7ac, 0x26a, 0xc4e, 0x5a0, 0x027, 0x2d0, /* fc =  500, G =  -1, fs = SAMP_FREQ_44KHZ */
    0x7b4, 0x45d, 0xc46, 0x382, 0x023, 0x1c1, /* fc =  500, G =   0, fs = SAMP_FREQ_44KHZ */
    0x578, 0x534, 0xe7e, 0x19e, 0x13f, 0x0cf, /* fc =  800, G = -18, fs = SAMP_FREQ_44KHZ */
    0x5a9, 0x640, 0xe4c, 0x600, 0x126, 0x300, /* fc =  800, G = -17, fs = SAMP_FREQ_44KHZ */
    0x5d8, 0x48b, 0xe1d, 0x543, 0x10e, 0x6a2, /* fc =  800, G = -16, fs = SAMP_FREQ_44KHZ */
    0x604, 0x719, 0xdf1, 0x065, 0x0f8, 0x432, /* fc =  800, G = -15, fs = SAMP_FREQ_44KHZ */
    0x62e, 0x545, 0xdc7, 0x009, 0x0e3, 0x405, /* fc =  800, G = -14, fs = SAMP_FREQ_44KHZ */
    0x655, 0x6c0, 0xd9f, 0x482, 0x0cf, 0x641, /* fc =  800, G = -13, fs = SAMP_FREQ_44KHZ */
    0x67a, 0x385, 0xd7a, 0x5d4, 0x0bd, 0x2ea, /* fc =  800, G = -12, fs = SAMP_FREQ_44KHZ */
    0x69c, 0x3d0, 0xd58, 0x3c2, 0x0ac, 0x1e1, /* fc =  800, G = -11, fs = SAMP_FREQ_44KHZ */
    0x6bc, 0x016, 0xd38, 0x5d6, 0x09c, 0x2eb, /* fc =  800, G = -10, fs = SAMP_FREQ_44KHZ */
    0x6d9, 0x0fc, 0xd1b, 0x36a, 0x08d, 0x5b5, /* fc =  800, G =  -9, fs = SAMP_FREQ_44KHZ */
    0x6f3, 0x74a, 0xd00, 0x3b7, 0x080, 0x1db, /* fc =  800, G =  -8, fs = SAMP_FREQ_44KHZ */
    0x70c, 0x3e1, 0xce7, 0x5d6, 0x073, 0x6eb, /* fc =  800, G =  -7, fs = SAMP_FREQ_44KHZ */
    0x722, 0x7b9, 0xcd1, 0x0d2, 0x068, 0x469, /* fc =  800, G =  -6, fs = SAMP_FREQ_44KHZ */
    0x737, 0x3d4, 0xcbc, 0x3a4, 0x05e, 0x1d2, /* fc =  800, G =  -5, fs = SAMP_FREQ_44KHZ */
    0x74a, 0x13a, 0xca9, 0x545, 0x054, 0x6a2, /* fc =  800, G =  -4, fs = SAMP_FREQ_44KHZ */
    0x75b, 0x0f2, 0xc98, 0x4aa, 0x04c, 0x255, /* fc =  800, G =  -3, fs = SAMP_FREQ_44KHZ */
    0x76a, 0x400, 0xc89, 0x0ce, 0x044, 0x467, /* fc =  800, G =  -2, fs = SAMP_FREQ_44KHZ */
    0x778, 0x362, 0xc7b, 0x0b2, 0x03d, 0x459, /* fc =  800, G =  -1, fs = SAMP_FREQ_44KHZ */
    0x785, 0x009, 0xc6e, 0x363, 0x037, 0x1b1, /* fc =  800, G =   0, fs = SAMP_FREQ_44KHZ */
    0x50e, 0x539, 0xee4, 0x0c3, 0x172, 0x061, /* fc = 1000, G = -18, fs = SAMP_FREQ_44KHZ */
    0x543, 0x4e7, 0xeae, 0x4be, 0x157, 0x25f, /* fc = 1000, G = -17, fs = SAMP_FREQ_44KHZ */
    0x576, 0x4d3, 0xe7b, 0x0a5, 0x13d, 0x452, /* fc = 1000, G = -16, fs = SAMP_FREQ_44KHZ */
    0x5a7, 0x350, 0xe49, 0x629, 0x124, 0x714, /* fc = 1000, G = -15, fs = SAMP_FREQ_44KHZ */
    0x5d5, 0x70b, 0xe1a, 0x69f, 0x10d, 0x350, /* fc = 1000, G = -14, fs = SAMP_FREQ_44KHZ */
    0x601, 0x70e, 0xdee, 0x302, 0x0f7, 0x181, /* fc = 1000, G = -13, fs = SAMP_FREQ_44KHZ */
    0x62b, 0x2bc, 0xdc4, 0x3ef, 0x0e2, 0x1f7, /* fc = 1000, G = -12, fs = SAMP_FREQ_44KHZ */
    0x652, 0x1ca, 0xd9d, 0x1b2, 0x0ce, 0x4d9, /* fc = 1000, G = -11, fs = SAMP_FREQ_44KHZ */
    0x676, 0x436, 0xd78, 0x44c, 0x0bc, 0x226, /* fc = 1000, G = -10, fs = SAMP_FREQ_44KHZ */
    0x698, 0x242, 0xd56, 0x37d, 0x0ab, 0x1be, /* fc = 1000, G =  -9, fs = SAMP_FREQ_44KHZ */
    0x6b7, 0x466, 0xd36, 0x6ca, 0x09b, 0x365, /* fc = 1000, G =  -8, fs = SAMP_FREQ_44KHZ */
    0x6d4, 0x346, 0xd19, 0x58d, 0x08c, 0x6c6, /* fc = 1000, G =  -7, fs = SAMP_FREQ_44KHZ */
    0x6ee, 0x7ac, 0xcfe, 0x6fa, 0x07f, 0x37d, /* fc = 1000, G =  -6, fs = SAMP_FREQ_44KHZ */
    0x707, 0x27b, 0xce6, 0x22c, 0x073, 0x116, /* fc = 1000, G =  -5, fs = SAMP_FREQ_44KHZ */
    0x71d, 0x4ab, 0xccf, 0x629, 0x067, 0x715, /* fc = 1000, G =  -4, fs = SAMP_FREQ_44KHZ */
    0x731, 0x73d, 0xcbb, 0x1ee, 0x05d, 0x4f7, /* fc = 1000, G =  -3, fs = SAMP_FREQ_44KHZ */
    0x744, 0x337, 0xca8, 0x470, 0x054, 0x238, /* fc = 1000, G =  -2, fs = SAMP_FREQ_44KHZ */
    0x755, 0x1a0, 0xc97, 0x4a6, 0x04b, 0x653, /* fc = 1000, G =  -1, fs = SAMP_FREQ_44KHZ */
    0x764, 0x37c, 0xc88, 0x18c, 0x044, 0x0c6, /* fc = 1000, G =   0, fs = SAMP_FREQ_44KHZ */
    0x43a, 0x461, 0xfac, 0x202, 0x1d6, 0x101, /* fc = 1500, G = -18, fs = SAMP_FREQ_44KHZ */
    0x473, 0x37e, 0xf72, 0x04d, 0x1b9, 0x026, /* fc = 1500, G = -17, fs = SAMP_FREQ_44KHZ */
    0x4ab, 0x369, 0xf38, 0x5f5, 0x19c, 0x2fb, /* fc = 1500, G = -16, fs = SAMP_FREQ_44KHZ */
    0x4e2, 0x169, 0xf00, 0x5c2, 0x180, 0x2e1, /* fc = 1500, G = -15, fs = SAMP_FREQ_44KHZ */
    0x517, 0x309, 0xeca, 0x23b, 0x165, 0x11d, /* fc = 1500, G = -14, fs = SAMP_FREQ_44KHZ */
    0x54a, 0x61b, 0xe95, 0x597, 0x14a, 0x6cb, /* fc = 1500, G = -13, fs = SAMP_FREQ_44KHZ */
    0x57c, 0x0c8, 0xe63, 0x1b9, 0x131, 0x4dc, /* fc = 1500, G = -12, fs = SAMP_FREQ_44KHZ */
    0x5ab, 0x194, 0xe33, 0x028, 0x119, 0x414, /* fc = 1500, G = -11, fs = SAMP_FREQ_44KHZ */
    0x5d7, 0x75c, 0xe05, 0x20c, 0x102, 0x506, /* fc = 1500, G = -10, fs = SAMP_FREQ_44KHZ */
    0x602, 0x158, 0xdda, 0x032, 0x0ed, 0x019, /* fc = 1500, G =  -9, fs = SAMP_FREQ_44KHZ */
    0x629, 0x713, 0xdb1, 0x311, 0x0d8, 0x588, /* fc = 1500, G =  -8, fs = SAMP_FREQ_44KHZ */
    0x64f, 0x068, 0xd8b, 0x2cf, 0x0c5, 0x568, /* fc = 1500, G =  -7, fs = SAMP_FREQ_44KHZ */
    0x671, 0x577, 0xd67, 0x74e, 0x0b3, 0x7a7, /* fc = 1500, G =  -6, fs = SAMP_FREQ_44KHZ */
    0x691, 0x69a, 0xd47, 0x02e, 0x0a3, 0x417, /* fc = 1500, G =  -5, fs = SAMP_FREQ_44KHZ */
    0x6af, 0x45e, 0xd28, 0x4e1, 0x094, 0x270, /* fc = 1500, G =  -4, fs = SAMP_FREQ_44KHZ */
    0x6ca, 0x77a, 0xd0c, 0x4ac, 0x086, 0x256, /* fc = 1500, G =  -3, fs = SAMP_FREQ_44KHZ */
    0x6e4, 0x0c0, 0xcf2, 0x6b6, 0x079, 0x35b, /* fc = 1500, G =  -2, fs = SAMP_FREQ_44KHZ */
    0x6fb, 0x11b, 0xcdb, 0x210, 0x06d, 0x508, /* fc = 1500, G =  -1, fs = SAMP_FREQ_44KHZ */
    0x710, 0x186, 0xcc5, 0x5ba, 0x062, 0x6dd, /* fc = 1500, G =   0, fs = SAMP_FREQ_44KHZ */
};
/*-----------------------------------------------------------------------
Function name:	void Audio_DrvMidSet(Audio_Stream_t Stream,S8 Gain,eAudioMidCentFreq_t Freq)
Input:			
             Stream   
                  PrimaryStr        Primary audio stream
                  SecondaryStr    Secondary audio stream
             Gain
                 18         +18dB in specified band in specified channel    
                 ..    
                 0           Flat in specified band in specified channel
                 ..
                 -18        -18dB in specified band in specified channel
                Note: Also intermediate values supported in steps of 1dB
                Note: Prescaler value is depending on maximum of three bands, state needed in API
               Freq          
                _500_HZ    
                _800_HZ    
                _1000_HZ   
                _1500_HZ  
                For Mid, a 2nd order peaking section(Q=1.0) is used
Output:			NULL
Description:	
                Set Bass parameter
------------------------------------------------------------------------*/
U8 Audio_DrvMidSet(Audio_Stream_t Stream,S8 Gain,eAudioMidCentFreq_t Freq)
{
        U16 Label = 0;
        U32 Coeffs[11] = {0};
        U8 Length = 0;
        U32 TablePointer = 0;
	   U8 ret;
        if (PrimaryStr == Stream)
        {
            Label = ADSP_Y_BMT_a1mHP_REL;
         }
	   else if(SecondaryStr==Stream)
	   {
			Label = ADSP_Y_BMT_a1mHS_REL;
		}
        Length = 7;
        
       /* For every Center Freq and Gain there is a row of 6 values. The first row is
        * identified by Freq number 0 and Gain = -18. Row number 19 is identified by Freq
        *  number 0 and Gain = 0. As only -18 <= Gain <= 0 are allowed the offset needs to be 
        *  18 (dB) * 6 values for every Center Frequency. 
        * For fs = SAMP_FREQ_48KHZ Hz, the table values start at 76 * 6 (= 4 (Freq) * 19 (rows/freq) *
        *   6 (values/row)/
        * 
        * for Gains >= 0 pick the Coeffs for Gain = 0 
        */
        TablePointer = ((18 * 6) + ((((S32)Freq * 19) + ((Gain >= 0) ? 0 : Gain)) * 6));/* determines the basic entry for this fs and band */
        


        Coeffs[0] = table_ToneControl_MidBands[TablePointer + 0];
        Coeffs[1] = table_ToneControl_MidBands[TablePointer + 1];
        Coeffs[2] = table_ToneControl_MidBands[TablePointer + 2];
        Coeffs[3] = table_ToneControl_MidBands[TablePointer + 3];
        Coeffs[4] = table_ToneControl_MidBands[TablePointer + 4];
        Coeffs[5] = table_ToneControl_MidBands[TablePointer + 5];

        /* Gmid = 1/16 * (POWER(10, gb/20) - 1) = 1/16 * (POWER(10, gb/20) - 1/16
        * translates to: Gmid = POWER(10, (gb - 24)/20) - 0x080
        */
        Coeffs[6] = table_dB2Lin[24 - Gain] - 0x080;
	if (Label != 0)
        {
            ret=WriteScratchDataToHero(I2CDRV_Y_MEM,
                                        Label,
                                        Length,
                                        Coeffs);
        }
	else
	{
            ret=ERROR;
	}
	return ret;
}


/* Section 1: 10000, 12500, 15000, 17500 Hz */
/* coefs are dependant on (fs, fc, Gain)    */
/* b1t are always zero */
/* Table Construction: a1, a2, b0, b2, single precision */
static const U16 table_ToneControl_TrebleBands[380] =
{
    0x02d, 0x2cc, 0x366, 0x000, 0xc9a, /* fc = 10000, G = -18, fs = SAMP_FREQ_44KHZ */
    0x031, 0x2ad, 0x357, 0x000, 0xca9, /* fc = 10000, G = -17, fs = SAMP_FREQ_44KHZ */
    0x036, 0x28b, 0x346, 0x000, 0xcba, /* fc = 10000, G = -16, fs = SAMP_FREQ_44KHZ */
    0x03c, 0x267, 0x333, 0x000, 0xccd, /* fc = 10000, G = -15, fs = SAMP_FREQ_44KHZ */
    0x041, 0x240, 0x320, 0x000, 0xce0, /* fc = 10000, G = -14, fs = SAMP_FREQ_44KHZ */
    0x047, 0x216, 0x30b, 0x000, 0xcf5, /* fc = 10000, G = -13, fs = SAMP_FREQ_44KHZ */
    0x04e, 0x1ea, 0x2f5, 0x000, 0xd0b, /* fc = 10000, G = -12, fs = SAMP_FREQ_44KHZ */
    0x054, 0x1bb, 0x2de, 0x000, 0xd22, /* fc = 10000, G = -11, fs = SAMP_FREQ_44KHZ */
    0x05c, 0x18a, 0x2c5, 0x000, 0xd3b, /* fc = 10000, G = -10, fs = SAMP_FREQ_44KHZ */
    0x063, 0x157, 0x2ac, 0x000, 0xd54, /* fc = 10000, G =  -9, fs = SAMP_FREQ_44KHZ */
    0x06b, 0x122, 0x291, 0x000, 0xd6f, /* fc = 10000, G =  -8, fs = SAMP_FREQ_44KHZ */
    0x073, 0x0eb, 0x275, 0x000, 0xd8b, /* fc = 10000, G =  -7, fs = SAMP_FREQ_44KHZ */
    0x07b, 0x0b2, 0x259, 0x000, 0xda7, /* fc = 10000, G =  -6, fs = SAMP_FREQ_44KHZ */
    0x083, 0x079, 0x23c, 0x000, 0xdc4, /* fc = 10000, G =  -5, fs = SAMP_FREQ_44KHZ */
    0x08c, 0x03e, 0x21f, 0x000, 0xde1, /* fc = 10000, G =  -4, fs = SAMP_FREQ_44KHZ */
    0x095, 0x003, 0x202, 0x000, 0xdfe, /* fc = 10000, G =  -3, fs = SAMP_FREQ_44KHZ */
    0x09d, 0xfc8, 0x1e4, 0x000, 0xe1c, /* fc = 10000, G =  -2, fs = SAMP_FREQ_44KHZ */
    0x0a6, 0xf8e, 0x1c7, 0x000, 0xe39, /* fc = 10000, G =  -1, fs = SAMP_FREQ_44KHZ */
    0x0ae, 0xf54, 0x1aa, 0x000, 0xe56, /* fc = 10000, G =   0, fs = SAMP_FREQ_44KHZ */
    0xfcb, 0x302, 0x381, 0x000, 0xc7f, /* fc = 12500, G = -18, fs = SAMP_FREQ_44KHZ */
    0xfc6, 0x2e8, 0x374, 0x000, 0xc8c, /* fc = 12500, G = -17, fs = SAMP_FREQ_44KHZ */
    0xfbf, 0x2cb, 0x365, 0x000, 0xc9b, /* fc = 12500, G = -16, fs = SAMP_FREQ_44KHZ */
    0xfb9, 0x2ab, 0x356, 0x000, 0xcaa, /* fc = 12500, G = -15, fs = SAMP_FREQ_44KHZ */
    0xfb2, 0x289, 0x345, 0x000, 0xcbb, /* fc = 12500, G = -14, fs = SAMP_FREQ_44KHZ */
    0xfaa, 0x265, 0x332, 0x000, 0xcce, /* fc = 12500, G = -13, fs = SAMP_FREQ_44KHZ */
    0xfa2, 0x23d, 0x31f, 0x000, 0xce1, /* fc = 12500, G = -12, fs = SAMP_FREQ_44KHZ */
    0xf99, 0x214, 0x30a, 0x000, 0xcf6, /* fc = 12500, G = -11, fs = SAMP_FREQ_44KHZ */
    0xf90, 0x1e7, 0x2f4, 0x000, 0xd0c, /* fc = 12500, G = -10, fs = SAMP_FREQ_44KHZ */
    0xf86, 0x1b8, 0x2dc, 0x000, 0xd24, /* fc = 12500, G =  -9, fs = SAMP_FREQ_44KHZ */
    0xf7c, 0x187, 0x2c4, 0x000, 0xd3c, /* fc = 12500, G =  -8, fs = SAMP_FREQ_44KHZ */
    0xf71, 0x154, 0x2aa, 0x000, 0xd56, /* fc = 12500, G =  -7, fs = SAMP_FREQ_44KHZ */
    0xf66, 0x11e, 0x28f, 0x000, 0xd71, /* fc = 12500, G =  -6, fs = SAMP_FREQ_44KHZ */
    0xf5b, 0x0e7, 0x274, 0x000, 0xd8c, /* fc = 12500, G =  -5, fs = SAMP_FREQ_44KHZ */
    0xf4f, 0x0af, 0x257, 0x000, 0xda9, /* fc = 12500, G =  -4, fs = SAMP_FREQ_44KHZ */
    0xf43, 0x075, 0x23a, 0x000, 0xdc6, /* fc = 12500, G =  -3, fs = SAMP_FREQ_44KHZ */
    0xf37, 0x03a, 0x21d, 0x000, 0xde3, /* fc = 12500, G =  -2, fs = SAMP_FREQ_44KHZ */
    0xf2a, 0x000, 0x200, 0x000, 0xe00, /* fc = 12500, G =  -1, fs = SAMP_FREQ_44KHZ */
    0xf1e, 0xfc5, 0x1e2, 0x000, 0xe1e, /* fc = 12500, G =   0, fs = SAMP_FREQ_44KHZ */
    0xf8c, 0x328, 0x394, 0x000, 0xc6c, /* fc = 15000, G = -18, fs = SAMP_FREQ_44KHZ */
    0xf80, 0x311, 0x388, 0x000, 0xc78, /* fc = 15000, G = -17, fs = SAMP_FREQ_44KHZ */
    0xf72, 0x2f7, 0x37c, 0x000, 0xc84, /* fc = 15000, G = -16, fs = SAMP_FREQ_44KHZ */
    0xf63, 0x2dc, 0x36e, 0x000, 0xc92, /* fc = 15000, G = -15, fs = SAMP_FREQ_44KHZ */
    0xf53, 0x2be, 0x35f, 0x000, 0xca1, /* fc = 15000, G = -14, fs = SAMP_FREQ_44KHZ */
    0xf42, 0x29d, 0x34f, 0x000, 0xcb1, /* fc = 15000, G = -13, fs = SAMP_FREQ_44KHZ */
    0xf2f, 0x27a, 0x33d, 0x000, 0xcc3, /* fc = 15000, G = -12, fs = SAMP_FREQ_44KHZ */
    0xf1b, 0x255, 0x32a, 0x000, 0xcd6, /* fc = 15000, G = -11, fs = SAMP_FREQ_44KHZ */
    0xf05, 0x22c, 0x316, 0x000, 0xcea, /* fc = 15000, G = -10, fs = SAMP_FREQ_44KHZ */
    0xeee, 0x201, 0x301, 0x000, 0xcff, /* fc = 15000, G =  -9, fs = SAMP_FREQ_44KHZ */
    0xed6, 0x1d4, 0x2ea, 0x000, 0xd16, /* fc = 15000, G =  -8, fs = SAMP_FREQ_44KHZ */
    0xebc, 0x1a4, 0x2d2, 0x000, 0xd2e, /* fc = 15000, G =  -7, fs = SAMP_FREQ_44KHZ */
    0xea1, 0x172, 0x2b9, 0x000, 0xd47, /* fc = 15000, G =  -6, fs = SAMP_FREQ_44KHZ */
    0xe85, 0x13e, 0x29f, 0x000, 0xd61, /* fc = 15000, G =  -5, fs = SAMP_FREQ_44KHZ */
    0xe68, 0x108, 0x284, 0x000, 0xd7c, /* fc = 15000, G =  -4, fs = SAMP_FREQ_44KHZ */
    0xe4a, 0x0d0, 0x268, 0x000, 0xd98, /* fc = 15000, G =  -3, fs = SAMP_FREQ_44KHZ */
    0xe2b, 0x097, 0x24b, 0x000, 0xdb5, /* fc = 15000, G =  -2, fs = SAMP_FREQ_44KHZ */
    0xe0c, 0x05d, 0x22e, 0x000, 0xdd2, /* fc = 15000, G =  -1, fs = SAMP_FREQ_44KHZ */
    0xded, 0x022, 0x211, 0x000, 0xdef, /* fc = 15000, G =   0, fs = SAMP_FREQ_44KHZ */
    0xf6a, 0x344, 0x3a2, 0x000, 0xc5e, /* fc = 17500, G = -18, fs = SAMP_FREQ_44KHZ */
    0xf5a, 0x330, 0x398, 0x000, 0xc68, /* fc = 17500, G = -17, fs = SAMP_FREQ_44KHZ */
    0xf48, 0x319, 0x38d, 0x000, 0xc73, /* fc = 17500, G = -16, fs = SAMP_FREQ_44KHZ */
    0xf34, 0x300, 0x380, 0x000, 0xc80, /* fc = 17500, G = -15, fs = SAMP_FREQ_44KHZ */
    0xf1f, 0x2e5, 0x373, 0x000, 0xc8d, /* fc = 17500, G = -14, fs = SAMP_FREQ_44KHZ */
    0xf07, 0x2c8, 0x364, 0x000, 0xc9c, /* fc = 17500, G = -13, fs = SAMP_FREQ_44KHZ */
    0xeee, 0x2a9, 0x354, 0x000, 0xcac, /* fc = 17500, G = -12, fs = SAMP_FREQ_44KHZ */
    0xed3, 0x286, 0x343, 0x000, 0xcbd, /* fc = 17500, G = -11, fs = SAMP_FREQ_44KHZ */
    0xeb6, 0x262, 0x331, 0x000, 0xccf, /* fc = 17500, G = -10, fs = SAMP_FREQ_44KHZ */
    0xe96, 0x23a, 0x31d, 0x000, 0xce3, /* fc = 17500, G =  -9, fs = SAMP_FREQ_44KHZ */
    0xe75, 0x210, 0x308, 0x000, 0xcf8, /* fc = 17500, G =  -8, fs = SAMP_FREQ_44KHZ */
    0xe51, 0x1e4, 0x2f2, 0x000, 0xd0e, /* fc = 17500, G =  -7, fs = SAMP_FREQ_44KHZ */
    0xe2c, 0x1b5, 0x2da, 0x000, 0xd26, /* fc = 17500, G =  -6, fs = SAMP_FREQ_44KHZ */
    0xe05, 0x183, 0x2c2, 0x000, 0xd3e, /* fc = 17500, G =  -5, fs = SAMP_FREQ_44KHZ */
    0xddb, 0x150, 0x2a8, 0x000, 0xd58, /* fc = 17500, G =  -4, fs = SAMP_FREQ_44KHZ */
    0xdb1, 0x11a, 0x28d, 0x000, 0xd73, /* fc = 17500, G =  -3, fs = SAMP_FREQ_44KHZ */
    0xd85, 0x0e3, 0x271, 0x000, 0xd8f, /* fc = 17500, G =  -2, fs = SAMP_FREQ_44KHZ */
    0xd57, 0x0aa, 0x255, 0x000, 0xdab, /* fc = 17500, G =  -1, fs = SAMP_FREQ_44KHZ */
    0xd29, 0x070, 0x238, 0x000, 0xdc8, /* fc = 17500, G =   0, fs = SAMP_FREQ_44KHZ */
};

/*-----------------------------------------------------------------------
Function name:	void Audio_DrvTrebSet(Audio_Stream_t Stream,S8 Gain,eAudioTrebCentFreq_t Freq)
Input:			
             Stream   
                  PrimaryStr        Primary audio stream
                  SecondaryStr    Secondary audio stream
             Gain
                 18         +18dB in specified band in specified channel    
                 ..    
                 0           Flat in specified band in specified channel
                 ..
                 -18        -18dB in specified band in specified channel
                Note: Also intermediate values supported in steps of 1dB
                Note: Prescaler value is depending on maximum of three bands, state needed in API
               Freq          
                _10000_HZ    
                _12500_HZ    
                _15000_HZ   
                _17500_HZ  
                For treble, a 2nd order peaking section(Q=1.0) is used
Output:			NULL
Description:	
                Set Bass parameter
------------------------------------------------------------------------*/
U8 Audio_DrvTrebSet(Audio_Stream_t Stream,S8 Gain,eAudioTrebCentFreq_t Freq)
{
        U16 Label = 0;
        U32 Coeffs[11] = {0};
        U8 Length = 0;
        U32 TablePointer = 0;
	 U8 ret;
        if (PrimaryStr == Stream)
        {
            Label = ADSP_Y_BMT_a1tP_REL;
        }
	else if(SecondaryStr==Stream)
	{
            Label = ADSP_Y_BMT_a1tS_REL;
	}
        Length = 6;
        
          /* For every Center Frequency and Gain there is a row of 5 values (single precision). The first
            * row is identified by Freq number 0 and Gain = -18. Row number 19 is identified by 
            * Freq number 0 and Gain = 0. As only -18 <= Gain <= 0 are allowed the offset needs to 
            * be 19 (-18 to 0 dB) * 5 values for every Center Frequency. 
            * for Gains >= 0 pick the Coeffs for Gain = 0
            */
        TablePointer = ((18 * 5) + ((((S32)Freq * 19) + ((Gain >= 0) ? 0 : Gain)) * 5));/* determines the basic entry for this fs and band */
        


        Coeffs[0] = table_ToneControl_TrebleBands[TablePointer + 0];
        Coeffs[1] = table_ToneControl_TrebleBands[TablePointer + 1];
        Coeffs[2] = table_ToneControl_TrebleBands[TablePointer + 2];
        Coeffs[3] = table_ToneControl_TrebleBands[TablePointer + 3];
        Coeffs[4] = table_ToneControl_TrebleBands[TablePointer + 4];
     

        /* Gtre = 1/16 * (POWER(10, gb/20) - 1) = 1/16 * (POWER(10, gb/20) - 1/16
        * translates to: Gtre = POWER(10, (gb - 24)/20) - 0x080
        */
        Coeffs[5] = table_dB2Lin[24 - Gain] - 0x080;
	if (Label != 0) {
            ret=WriteScratchDataToHero(I2CDRV_Y_MEM,
                                        Label,
                                        Length,
                                        Coeffs);
         }
	else{
            ret=ERROR;
	}
	return ret;
}





/*---------------------------------------------------------------------------
Function       : 
    void Audio_DrvPathConnect(eAudio_Feature_t From,
                                    eAudio_Feature_t To)

Input        :
    From => Output pointers of the "from" (data source) audio feature

    To   => Input pointers of the "to" (data sink) audio feature

Output        :
    None

Return        :
    None

Description    :
    Connect 'from' feature to 'to' feature by setting input pointer of 'to' feature 
    to output variable of 'from' feature.
*--------------------------------------------------------------------------*/
U8 Audio_DrvPathConnect(eAudio_Feature_t From, eAudio_Feature_t To)
{
    S32 FromLabel[2];
    U8 retVal=0;
    Audio_DrvGetAddress(From, FromLabel);/* returns the label as a function of the From feature */

    switch (To)
    {
      case PEQ: /* 4 stereo pointers */
        FromLabel[1] = FromLabel[0];/* PEQ Front and Rear point to same location */
        retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_FInPntr_REL/* Front */,2,FromLabel);
        retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_SwFInPntr_REL/* SubWoofer */,2,FromLabel);
        break;

      case GEQ: /* 1 stereo pointer */
        retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_GEqInPntr_REL,1,FromLabel);
        break;

      case Tone: /* 1 stereo pointer */
        retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_PToneControl_InPntr_REL,1,FromLabel);
        break;

      case Loudness:  /* 1 stereo pointer */
        retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_Loudf_InPntr_REL,1,FromLabel);
        break;
		
      case Delay:
	if(From==PEQ) { //connect PEQ to Delay
                 FromLabel[0]=ADSP_X_Eq_OutFL_REL;  //Connect Delay_F_InPntr to ADSP_X_Eq_OutFL
                 retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_Delay_F_InPntr_REL,1,FromLabel);

	         FromLabel[0]=ADSP_X_Eq_OutRL_REL;  //Connect Delay_R_InPntr to ADSP_X_Eq_OutRL
                 retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_Delay_R_InPntr_REL,1,FromLabel);

		 FromLabel[0]=ADSP_X_Eq_OutSwL_REL;  //Connect Delay_F_InPntr to ADSP_X_Eq_OutFL
                 retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_Delay_C_InPntr_REL,1,FromLabel);
				 
		 FromLabel[0]=ADSP_X_Eq_OutSwR_REL;  //Connect Delay_R_InPntr to ADSP_X_Eq_OutRL
                 retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_Delay_Sw_InPntr_REL,1,FromLabel);
	 }
	 else {
                 FromLabel[1] = FromLabel[0];/* PEQ Front and Rear point to same location */
                 retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_Delay_F_InPntr_REL/* Front */,2,FromLabel);
                 retVal = WriteDataToHero(I2CDRV_X_MEM,ADSP_X_Delay_Sw_InPntr_REL/* SubWoofer */,2,FromLabel);
	}	
	break;
      }
     return retVal;
}

/*-----------------------------------------------------------------------
Function name:	void Audio_DrvPEQEnaDisable(void)
Input:			EnaDisable
                            ENABLE   -----
                            DISABLE
Output:			NULL
Description:	
               Enable or disable PEQ
------------------------------------------------------------------------*/
void Audio_DrvPEQEnaDisable(U8 EnaDisable)
{
   if(EnaDisable==ENABLE) {  /*PEQ Enable	*/
           Audio_DrvPathConnect(Loudness,PEQ);
	   Audio_DrvPathConnect(PEQ, Delay);
   }
   else{ /*disable*/   	
           Audio_DrvPathConnect(Loudness, Delay);
   }

}

/*-----------------------------------------------------------------------
Function name:	void Audio_DrvSetPEQScale( U8 channel, S8 Gain )
Input:			channel
                               font 
                               rear
                               center
                               subwoof                               
                            Gain
                            
Output:			NULL
Description:	
               set channel PEQ pre_scaler
------------------------------------------------------------------------*/
U8 Audio_DrvSetPEQScale( eAudioChannel channel, S8 Gain )
{
	U32 Label;
	U16 ret;
	S32 data;
	
	switch(channel)
		{
            case Front:
			Label = ADSP_Y_Vol_ScalF;	
			break;			
	     case Rear:
			Label = ADSP_Y_Vol_ScalR;	
			break;
	     case Center:
			Label = ADSP_Y_Vol_ScalSwL;	
			break;
	     case SubWoofer:
			Label = ADSP_Y_Vol_ScalSwR;	
			break;
	     case Secondary:
			Label = ADSP_Y_Vol_ScalS;	
			break;

	}	
	data = (S32)table_dB2Lin[-Gain];
	ret = WriteDataToHero(I2CDRV_X_MEM,Label,1,&data);
	return( ret );
}

static char IIRFilterOrder=0;
/* Parametric equalizer                     */
/* Used to compensate for car acoustics     */
/*----------------------------------------------------------------------------
Function       : 
    U8 Audio_DrvPEQSet(eAudioChannel Channel, U32 Band, U32* Coefs)

Input        :
    Channel
    FrontLeft   Primary audio stream front left channel
    FrontRight  Primary audio stream front right channel
    RearLeft    Primary audio stream rear left channel
    RearRight   Primary audio stream rear right channel
    SubWoofer   Primary audio stream subwoofer channel
    Center      Primary audio stream center channel

    Band
    BAND1    Band 1
    BAND2    Band 2
    BAND3    Band 3
    BAND4    Band 4
    BAND5    Band 5, only applicable for front and rear channels
    BAND6    Band 6, only applicable for front and rear channels
    BAND7    Band 7, only applicable for front and rear channels

    Coefs
    Coefs[0]    B0 L
    Coefs[1]    B0 H
    Coefs[2]    B1 L
    Coefs[3]    B1 H
    Coefs[4]    B2 L
    Coefs[5]    B2 H
    Coefs[6]    A1 L
    Coefs[7]    A1 H
    Coefs[8]    A2 L
    Coefs[9]    A2 H

Output        :
    None

Return        :
    ret

Description    :
    Change the coefficients of the selected band in the selected channel of the parametric equalizer.

    Default:
    Center channel:    Pass through.
    SubWoofer channel:    6th order.
    Coefs:    Flat response.

*---------------------------------------------------------------------------------*/
U8 Audio_DrvPEQSet(eAudioChannel Channel, U32 Band, U32* Coefs)
{
   
    U16 Label = 0;
    U8 filterCoefficients = 10;
    U8 ret=ERROR;

    switch (Channel)
    {
       case FrontLeft:/*front left*/
              switch (Band)
                {
                      case BAND1:
                              Label = (U16)(ADSP_Y_EqFL_b10L_REL);
                              filterCoefficients = 7;
                              break;

                      case BAND2:
                              Label = (U16)(ADSP_Y_EqFL_b20L_REL);
                              filterCoefficients = 7;
                              break;

                      case BAND3:
                              Label = (U16)(ADSP_Y_EqFL_b30L_REL);
                              filterCoefficients = 7;
                              break;
                      case BAND4: 
                              Label = (U16)(ADSP_Y_EqFL_b40L_REL);
                              filterCoefficients = 7;
                              break;

                     case BAND5:
                             Label = (U16)(ADSP_Y_EqFL_b50L_REL);
                             break;

                    case BAND6:
                            Label = (U16)(ADSP_Y_EqFL_b60L_REL);
                            break;

                   case BAND7:
                           Label = (U16)(ADSP_Y_EqFL_b70L_REL);
                           break;
        
                  default:
                          filterCoefficients = 0;
                          break;
            }
            break;

     case FrontRight:/*front right*/
             switch (Band)
               {
                  case BAND1:
                          Label = (U16)(ADSP_Y_EqFR_b10L_REL);
                          filterCoefficients = 7;
                          break;

                  case BAND2:
                          Label = (U16)(ADSP_Y_EqFR_b20L_REL);
                          filterCoefficients = 7;
                          break;

                 case BAND3:
                         Label = (U16)(ADSP_Y_EqFR_b30L_REL);
                         filterCoefficients = 7;
                         break;

                case BAND4: 
                        Label = (U16)(ADSP_Y_EqFR_b40L_REL);
                        filterCoefficients = 7;
                        break;

               case BAND5:
                       Label = (U16)(ADSP_Y_EqFR_b50L_REL);
                       break;

               case BAND6:
                       Label = (U16)(ADSP_Y_EqFR_b60L_REL);
                       break;

              case BAND7:
                      Label = (U16)(ADSP_Y_EqFR_b70L_REL);
                      break;
   
              default:
                      filterCoefficients = 0;
                      break;
            }
            break;

    case RearLeft:/*rear left*/
           switch (Band)
              {
               case BAND1:
                       Label = (U16)(ADSP_Y_EqRL_b10L_REL);
                       filterCoefficients = 7;
                       break;

               case BAND2:
                       Label = (U16)(ADSP_Y_EqRL_b20L_REL);
                       filterCoefficients = 7;
                       break;

               case BAND3:
                       Label = (U16)(ADSP_Y_EqRL_b30L_REL);
                       filterCoefficients = 7;
                       break;

              case BAND4: 
                      Label = (U16)(ADSP_Y_EqRL_b40L_REL);
                      filterCoefficients = 7;
                      break;

              case BAND5:
                      Label = (U16)(ADSP_Y_EqRL_b50L_REL);
                      break;

              case BAND6:
                       Label = (U16)(ADSP_Y_EqRL_b60L_REL);
                       break;

              case BAND7:
                      Label = (U16)(ADSP_Y_EqRL_b70L_REL);
                      break;        
              default:
                       filterCoefficients = 0;
                      break;
              }
             break;

    case RearRight:
          switch (Band)
            {
              case BAND1:
                      Label = (U16)(ADSP_Y_EqRR_b10L_REL);
                      filterCoefficients = 7;
                      break;

              case BAND2:
                      Label = (U16)(ADSP_Y_EqRR_b20L_REL);
                      filterCoefficients = 7;
                      break;

              case BAND3:
                      Label = (U16)(ADSP_Y_EqRR_b30L_REL);
                      filterCoefficients = 7;
                      break;

             case BAND4: 
                      Label = (U16)(ADSP_Y_EqRR_b40L_REL);
                      filterCoefficients = 7;
                      break;

             case BAND5:
                      Label = (U16)(ADSP_Y_EqRR_b50L_REL);
                      break;

             case BAND6:
                     Label = (U16)(ADSP_Y_EqRR_b60L_REL);
                     break;

             case BAND7:
                     Label = (U16)(ADSP_Y_EqRR_b70L_REL);
                     break;
          
             default:
                     filterCoefficients = 0;
                     break;
             }
            break;

   case SubWoofer: /* SubWoofer pointer is located at 'Center pointer' + Center Order * 5 */
                        /* 'IIRFilterOrder' is the order of the IIR filter
                             e.g. if the order of Center = 2 the SubWoofer pointer is at 
                             2 * 5 = 10 addresses further                          */
          switch (Band)
             {
               case BAND1:
                       Label = (U16)(ADSP_Y_EqSwC_b00L_REL + (IIRFilterOrder * 5));
                       break;

               case BAND2:
                       Label = (U16)(ADSP_Y_EqSwC_b10L_REL + (IIRFilterOrder * 5));
                      break;

               case BAND3:
                       Label = (U16)(ADSP_Y_EqSwC_b20L_REL + (IIRFilterOrder * 5));
                       break;

              case BAND4: 
                      Label = (U16)(ADSP_Y_EqSwC_b30L_REL + (IIRFilterOrder * 5));
                      break;

              default:
                     filterCoefficients = 0;
                     break;
          }
         break;

    case Center:
           switch (Band)
            {
              case BAND1:
                     Label = (U16)(ADSP_Y_EqSwC_b00L_REL);
                     break;

             case BAND2:
                     Label = (U16)(ADSP_Y_EqSwC_b10L_REL);
                     break;

             case BAND3:
                     Label = (U16)(ADSP_Y_EqSwC_b20L_REL);
                     break;

            case BAND4: 
                    Label = (U16)(ADSP_Y_EqSwC_b30L_REL);
                    break;

            default:
                   filterCoefficients = 0;
                   break;
           }
          break;

   default:
          filterCoefficients = 0;
          break;
    }
/*write data to hero*/
  if(filterCoefficients)
        ret=WriteScratchDataToHero(I2CDRV_Y_MEM,Label,filterCoefficients,Coefs);

    return ret;
}

/* Choose filter order for selected channel */
/*------------------------------------------------------------------------------
Function       : 
    void Audio_DrvPEQConfig(UInt32 CenterOrder,
                                      UInt32 SubWooferOrder)

Input        :
    CenterOrder
        0    Pass through configuration
        2    2nd Order IIR with 6dB headroom
        4    4th Order IIR with 6dB headroom
        6    6th Order IIR with 6dB headroom
        8    8th Order IIR with 6dB headroom

    SubWooferOrder
        0    Pass through configuration
        2    2nd Order IIR with 6dB headroom
        4    4th Order IIR with 6dB headroom
        6    6th Order IIR with 6dB headroom
        8    8th Order IIR with 6dB headroom

Output        :
    None

Return        :
    None

Description    :
    Choose filter order for center and subwoofer channel (max 8 together).
-------------------------------------------------------------------------------*/
U8 Audio_DrvPEQConfig(U32 CenterOrder, U32 SubWooferOrder)
{
    U8 Instruction = 0;
    U8 retVal=ERROR;
    IIRFilterOrder = CenterOrder;/* IIRFilterOrder is required in PeqSet() */

    switch (CenterOrder)
    {
	case 0:/*pass though configuration*/
	       Instruction = (U8)ADSP_EASYP_PEQ_Center_IIR_passthrough;
	       break;

	case 2:/* 2nd Order IIR with 6dB headroom*/
	       Instruction = (U8)ADSP_EASYP_PEQ_Center_IIR2_headroom;
	       break;

	case 4:/* 4nd Order IIR with 6dB headroom*/
	       Instruction = (U8)ADSP_EASYP_PEQ_Center_IIR4_headroom;
	       break;

	case 6:/* 6nd Order IIR with 6dB headroom*/
	       Instruction = (U8)ADSP_EASYP_PEQ_Center_IIR6_headroom;
	       break;

	case 8:/* 8nd Order IIR with 6dB headroom*/
	       Instruction = (U8)ADSP_EASYP_PEQ_Center_IIR8_headroom;
	       break;

	default:
	      break;
    }
    /*write data to hero*/
    if (Instruction != 0)
    {
        retVal = WriteScratchDataToHero(I2CDRV_X_MEM,
                                        ADSP_X_EasyP_Index_REL,
                                        Instruction,
                                        NULL);
    }

   switch (SubWooferOrder)
    {
	case 0:/*pass though configuration*/
	    Instruction = (U8)ADSP_EASYP_PEQ_Sw_IIR_passthrough;
	    break;

	case 2:/* 2nd Order IIR with 6dB headroom*/
	    Instruction = (U8)ADSP_EASYP_PEQ_Sw_IIR2_headroom;
	    break;

	case 4:/* 4nd Order IIR with 6dB headroom*/
	    Instruction = (U8)ADSP_EASYP_PEQ_Sw_IIR4_headroom;
	    break;

	case 6:/* 6nd Order IIR with 6dB headroom*/
	    Instruction = (U8)ADSP_EASYP_PEQ_Sw_IIR6_headroom;
	    break;

	case 8:/* 8nd Order IIR with 6dB headroom*/
	    Instruction = (U8)ADSP_EASYP_PEQ_Sw_IIR8_headroom;
	    break;

	default:
	   break;
    }
 /*write data to hero*/
    if (Instruction != 0)
    {
        retVal = WriteScratchDataToHero(I2CDRV_X_MEM,
                                        ADSP_X_EasyP_Index_REL,
                                        Instruction,
                                        NULL);
    }

    return retVal;
}


/* Preset values                                              */
/*----------------------------------------------------------------------------
Function       : 
void Audio_DrvBalanceSet(Audio_Stream_t Stream,
                                      eAudioChannel Channel, U32 Preset)

Input        :
    Stream =>   Primary - Balance on Primary stream, adapt SubWoofer and Center channels also.

    Channel =>  Left - Left channel(s) attenuated, balance goes to right.
                Right -    Right channel(s) attenuated, balance goes to left.
    Preset =>   0 to 63    (6 bits coding)

Output        :
    None

Return        :
    None

Description    :
    Balance in Primary stream. Sets balance from left to right,
    by attenuating other side Step size = 1/63 for left and right (64 presets).
    For Primary stream, Center and SubWoofer channels are updated simultaneously.
    The balance is controlled by attenuating the left or the right channel(s) in a stream

    For multichannel streams, the API routine also updates the SubWoofer and Center attenuation,
    according to the setting for Left or right.

    Associated with the gain for left or right channel will be the following gain for SubWoofer and Center channel

    Note that when sending an attenuation value to the left or right channels, the other side will be reset to full-scale.

    Default: No attenuation on either channel: balance in center.
------------------------------------------------------------------------------*/
void Audio_DrvBalanceSet(Audio_Stream_t Stream,eAudioChannel Channel, U32 Level)
{

    S32 Att[2] = { 0x0, 0x0 } , Temp, DoWrite = 1;

    Temp = (Level * 0x41) >> 1; /* temp is used in order to save code space */

    switch (Channel)
    {
    case Left:
        Att[0] = Temp;   /* Left */
        Att[1] = 0x7FF;  /* Right */
        break;

    case Right:
        Att[0] = 0x7FF;  /* Left */
        Att[1] = Temp;   /* Right */
        break;

    default:
        DoWrite = 0;
        break;
    }
    if (DoWrite != 0)
    {
        if(PrimaryStr == Stream)
        {
           WriteDataToHero(I2CDRV_Y_MEM,
                                        ADSP_Y_Vol_BalPL_REL,
                                        2,
                                        Att);
            /* Sw and Center update */
            Temp = (Temp >> 1) + 0x400;/* Sw and Center */
            Att[0] = Temp;
            Att[1] = Temp;

            WriteDataToHero(I2CDRV_Y_MEM,ADSP_Y_Vol_BalSwL_REL,2,Att);
        }
    }
    return;
}

/*--------------------------------------------------------------------------------
Function       : 
    void Audio_DrvFaderSet(eAudioChannel Channel, U32 Level)

Input        :
    Channel => Front    Front channels attenuated, sound goes to rear.
                        Center and Subwoofer updated also.
                Rear    Rear channels attenuated, sound goes to front.
                        Subwoofer update also.

    Preset => 0 - 63    6 bits coding

Output        :
    None

Return        :
    None

Description    :
    The fader positions the audio between front and rear by attenuating the rear or front channels.
    Also the (center and) subwoofer attenuation has to be adapted.
    Subwoofer always, center only in case front channels are attenuated.
    
    The fader preset is equal to the balance preset, but only applicable to the primary audio stream.
    Faderin Primary stream
        Sets audio from front to rear, by attenuating other "side" 
        Step size = 1/63 (64 presets).
        Center and SubWoofer channels are updated simultaneously, according to needs.

    When sending an attenuation value to front or rear channels, the other channels will be reset to full-scale.
    SubWoofer attenuation is equal to the Balance calculations.
    Center attenuation value is equal to front channel attenuation value (including full scale when rear channels are attenuated).

    Default: No attenuation, fader set to mid between front and rear.
---------------------------------------------------------------------------------*/
void Audio_DrvFaderSet(eAudioChannel Channel, U32 Level)
{

    S32 Att[2], Temp;

    Temp = (Level * 0x41) >> 1; /* temp is used in order to save code space */

    switch (Channel)
    {
    case Front:
        /* Front update to 'Temp', Rear to 'Max' */
        Att[0] = Temp;
        Att[1] = 0x7FF;
        break;

    case Rear:
        /* Front update to 'Max', Rear to 'Temp' */
        Att[0] = 0x7FF;
        Att[1] = Temp;
        break;

    default:
        Att[0] = 0;
        Att[1] = 0;
        break;

    }
     WriteDataToHero(I2CDRV_Y_MEM,ADSP_Y_Vol_FadF,2,Att);          

    /* Sw and Center = Att[0] (= same value as front) update */
    Temp = (Temp >> 1) + 0x400;/* Sw update */
    Att[1] = Temp;
    WriteDataToHero(I2CDRV_Y_MEM,ADSP_Y_Vol_FadSwL,2,Att);
}

 

/*----------------------------------------------------------------------------
Function       : void Audio_DrvDCFilterSet(Audio_Stream_t Stream, U32 Value)

Input        :
    Stream =>   PrimaryStr   Primary audio stream

    OnOff  =>   AUDIO_OFF    DC filter off
                   AUDIO_ON    DC filter on

Output        :
    None

Return        :
    None

Description    :
    Switches DC filter on and off in selected stream.
    When switching DC filter ON, the default filter coefficients will be downloaded.

    Default: DC filter on in primary stream.
------------------------------------------------------------------------------*/
void Audio_DrvDCFilterSet(Audio_Stream_t Stream, U32 OnOff)
{

    U32 Coeffs[3];
    U16 Label = 0;

    if(PrimaryStr == Stream) {/*primary stream*/
        Label = ADSP_Y_DCfilt_a1A_REL;
    }
    else if(SecondaryStr==Stream) {/*second stream*/
       Label=ADSP_Y_DCfilt_a1B_REL;
    }

    if (Label != 0)  { 
        if (OnOff ==AUDIO_ON) {   /* on */	
             /* Filter characteristics: fc = 3.5 Hz @ 44.1kHz
	      if user need to change this parameter,you need to calculate the Coefficients acording
	      to the formula(see UM_Audio_TEF6638_Rev.2.3) OR You can  get flowing data by Hero Control GUI's Debug Console  */   		
            Coeffs[0] = 0x7FF;
            Coeffs[1] = 0x801;
            Coeffs[2] = 0x801;
        }
        else 
        { 
            /* off */
            Coeffs[0] = 0x000;
            Coeffs[1] = 0x000;
            Coeffs[2] = 0x800;
        }
       WriteScratchDataToHero(I2CDRV_Y_MEM,
                                        Label,
                                        3,
                                        Coeffs);
    }
    return;
}




/*-----------------------------------------------------------------------
Function       : 
  void Audio_DrvNavPhoneFilterSet(eAudio_Feature_t Feature, U32 Section, U32* Coefs)

Input        :
    Feature
    PhoneFilter        Select voice filter of phone channel
    NavigationFilter    Select voice filter of navigation channel

    Section
    0    Select first section of filter
    1    Select second section of filter

    Coefs
        Coefs[0]    B0 L
    Coefs[1]    B0 H
    Coefs[2]    B1 L
    Coefs[3]    B1 H
    Coefs[4]    B2 L
    Coefs[5]    B2 H
    Coefs[6]    A1 L
    Coefs[7]    A1 H
    Coefs[8]    A2 L
    Coefs[9]    A2 H

Output        :
    None

Return        :
    None

Description    :
    Update coefficients of voice filters. Address will be determined by API implementation from filter and section parameters.
    
    Default: Flat response
----------------------------------------------------------------------------*/
void Audio_DrvNavPhoneFilterSet(eAudio_Feature_t Feature, U32 Section, U32* Coefs)
{
    U16 Label = 0;

    switch (Feature)
    {
    case NavigationFilter:/*nav filter*/
        switch (Section)
         {
          case 0:
                 Label = ADSP_Y_EqN_b00L_REL;
                 break;
          case 1:
                 Label = ADSP_Y_EqN_b10L_REL;
                 break;   
           default:
                 break;   
        }
        break;   

    case PhoneFilter:/*phone filter*/
        switch (Section)
        {
           case 0:
                 Label = ADSP_Y_EqT_b00L_REL;
                 break;

          case 1:
                 Label = ADSP_Y_EqT_b10L_REL;                   
                 break;   
          default:
                 break;   
        }
        break; 

    default:
        break;   
    }
    /*write data to hero*/
    if(Label != 0)
    {
        WriteScratchDataToHero(I2CDRV_Y_MEM,
                                        Label,
                                        10,
                                        Coefs);
    }
    return;
}




/*-------------------------------------------------------------------------
Function :Audio_DrvExpanderSet(S8 Threshold)
input:
     Threshold :expander threshold value(-66   ,  -34)
output:
      none
 description:
      set expander modual parameter

---------------------------------------------------------------------------*/
void Audio_DrvExpanderSet(S8 Threshold)
{
  S32 Temp1,Temp2;
  /*parameters check*/
  if(Threshold>-34)
	Threshold=-34;
  if(Threshold<-66)
	Threshold=-66;

  /*calc Temp1=ADSP_Y_CompExp_ExpL*2048*/
  Temp1=table_dB2Lin[-Threshold];

  /*calc ADSP_Y_CompExp_GExp	   */
  /*ADSP_Y_CompExp_GExp= (power(2,-12)/ADSP_Y_CompExp_ExpL)*2048= power(2,-12)*2048/ADSP_Y_CompExp_ExpL
                                    =0.5/ADSP_Y_CompExp_ExpL=(0.5*2048)/(ADSP_Y_CompExp_ExpL*2048)=1024/Temp1
  */
   Temp2=1024/Temp1;

 /*set ADSP_Y_CompExp_GExp register*/
    WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_CompExp_GExp ,Temp2);
    WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_CompExp_ExpL ,Temp1); 
}

/*--------------------------------------------------------------------------
* Fuction :Audio_DrvCompressorSet										        
*                                                                                                              
* Input       :
*              CureType:
*                  Curve Type:CompCurve_High,CompCurve_Mid,CompCurve_Low,CompCurve_User
*               
*              Ratio *10      if CurType!=CompCurve_User,  Parameter [Ratio] is  useless                                                                                     
*                  [ Compressor Ratio]    [10  100]                                                             
*              Gain              if CurType!=CompCurve_User, Parameter [Gain] is  useless                                                                              
*                  [ Boost Gain ]            [0  MGMax]                                                              
*              Pone             if CurType!=CompCurve_User,  Parameter [Pone]  is  useless                                                                              
*                  [ Point of on Effect ]   [-30  0]                                                              
*                                                                                                             
*                                                                                                              
* Output      :null                                                                                         
*Description : set compressor                                                                       
*--------------------------------------------------------------------------*/
void Audio_DrvCompressorSet(eAudioComp_Curve CurveType,U16 Ratio, U16 Gain, U16 Pone)
{
	double MGMax, MG, k3, k4;
	S32 Reg_CompExp_K3,Reg_CompExp_K4,Scal, PonE;
	switch(CurveType)
	{
	case CompCurve_High:/*set compressor curve is high*/
		WriteHero_MEM_1Word(I2CDRV_EP_MEM, ADSP_X_EasyP_Index, ADSP_EASYP_CompExp_CompExpHigh);
		break;
	case CompCurve_Mid:/*set compressor curve is mid*/
		WriteHero_MEM_1Word(I2CDRV_EP_MEM, ADSP_X_EasyP_Index, ADSP_EASYP_CompExp_CompExpMid);
		break;
	case CompCurve_Low:/*set compressor curve is low*/
		WriteHero_MEM_1Word(I2CDRV_EP_MEM, ADSP_X_EasyP_Index, ADSP_EASYP_CompExp_CompExpLow);
		break;
	case CompCurve_User:/*set compressor curve  is user parameters*/		
		/*calcute max gain*/
		MGMax = ( 60.0 + ((double)Pone) ) * ( 1.0 - ( 10.0 / ((double) Ratio ) ) );
		if( MGMax > 24.0 ){
			MGMax = 24.0;
		}
		MG = (double) Gain;
		if( MG > MGMax ){
			MG = MGMax;
			Gain = (U16) MGMax;
		}

		k3 = ( ( 10.0 / ((double) Ratio ) ) - 1.0 );
		k4 = ( ( ( ((double) Pone ) * k3 ) + MG ) / -60.0 );

		Hero_TransDouble2XYHexData(HERO_YDATA,k3,&Reg_CompExp_K3);
		Hero_TransDouble2XYHexData(HERO_YDATA,k4,&Reg_CompExp_K4);	 
		/******************************************************
		calc Scal:
		Scal=power(10,MG/20)/16 *2048
		 =2048/(power(10,-MG/20)*16)=128/power(10,-MG/20)
		 =128*2048/table_dB2Lin[MG]
		**************************************************** */	 
		Scal=(2048/table_dB2Lin[Pone])*128;
		/******************************************************
		calc Pone:
		Scal=power(10,Pone/20) *8388608
		 =power(10,Pone/20)*2048*4096
		 =table_dB2Lin[-Pone]*4096
		**************************************************** */	 
		PonE=(table_dB2Lin[-Pone]+1)*4096;
		if(PonE>0x7FFFFF)
			PonE=0x7FFFFF;
		//write data to coeff register
		WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_UpdatC_Coeff0, Reg_CompExp_K3);
		WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_UpdatC_Coeff1, Reg_CompExp_K4);
		WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_UpdatC_Coeff2, Scal);
		WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_UpdatC_Coeff3, PonE&0xffff);
		WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_UpdatC_Coeff4, PonE>>16);	
		WriteHero_MEM_1Word(I2CDRV_EP_MEM, ADSP_X_EasyP_Index, ADSP_EASYP_CompExp_Update);	
		break;

	}

}
/*----------------------------------------------------------------------------
* Function :Audio_DrvCompExpControl										
* Input       :comp_onoff                                                                                      
*                   compressor enable / disable                                                      
*              exp_onoff                                                                                  
*                   expander enable / disable                                                        
* Output      :null                                                                                        
* FUNCTION    :Compressor/Expander Enable or Disable                                    
*----------------------------------------------------------------------------*/
void Audio_DrvCompExpControl( U16 comp_onoff, U16 exp_onoff )
{
		
	if( comp_onoff == AUDIO_ON ){/*enable compressor*/
	    WriteHero_MEM_1Word( I2CDRV_X_MEM,ADSP_X_CompExp_switch,0x000000 );
       }
	else{ /*disable compressor*/
	    WriteHero_MEM_1Word( I2CDRV_X_MEM,ADSP_X_CompExp_switch,0x800000 );
	}
	if( exp_onoff == AUDIO_ON ){/*enable expander*/
	    WriteHero_MEM_1Word( I2CDRV_Y_MEM,ADSP_Y_CompExp_ExpL,0x0001 );
	}
	else{ /*disable expander*/
	    WriteHero_MEM_1Word( I2CDRV_Y_MEM,ADSP_Y_CompExp_ExpL,0x0000 );		
	}	
}

/*-----------------------------------------------------------------------------
* Function :Audio_DrvLimiterConfigurationSel										
* Input       :config                                                                                                         
*                      1:configuration 1 
*                      2:configuration 2                                                                                
* Output      :null                                                                               
* FUNCTION    :Limiter configuration select                                          
*---------------------------------------------------------------------------*/
void Audio_DrvLimiterConfigurationSel(U8 config)
{
  if(1==config) {/*  configuration 1 */ 	
        TEF663x_EasyProgrammWrite(ADSP_EASYP_Limit_LimitRSOnSec);
  }
  else if(2==config){  /*configuration 2 */
        TEF663x_EasyProgrammWrite(ADSP_EASYP_Limit_LimitRSOnRear);
  }
}
/*-----------------------------------------------------------------------------
* Function :Audio_DrvLimiterSet							
* Input       :Section                                                                                                         
*                      Front                                              
*                      Rear                                                                                
*                      SubLeft/Center                                         
*                      SubRight                                         
*                      Secondary                                                
*                   gain:                                                      
*                      Limter mini gain   [-66  0] 
*                   onoff:                                 
*                       AUDIO_ON :enable    
*                      AUDIO_ON:disable    
* Output      :null                                                                                        
* FUNCTION    :set Limiter  parametric                                      
*----------------------------------------------------------------------------*/
void Audio_DrvLimiterSet(eAudioChannel section,S8 gain,U8 onoff)
{
 S32 MinGain;
 /*Calac Mininum gain */
 /*MinGain=-(power(10,gain/20)*8388608)     */
 /*            =-(power(10,gain/20)* 2048*4096  */
 /*            =~(table_dB2Lin[-gain]*4096)    */
 MinGain=~((table_dB2Lin[-gain]+1)*4096);
 
 if(MinGain>0x7FFFFF)
	 MinGain=0x7FFFFF;
 
  switch(section)
  	{
		case Front:
			if(AUDIO_OFF==onoff)
				WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainFR, 0x800000);
			else	{
				WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainFR, MinGain);
			}
			break;
		case Rear:
			if(AUDIO_OFF==onoff)
				WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainRS, 0x800000);
			else	{
				WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainRS, MinGain);
			}
			break;
		case SubCenter:
			if(AUDIO_OFF==onoff)
				WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainSwL, 0x800000);
			else	{
				 WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainSwL, MinGain);
			}
			break;
		case SubWoofer:
			if(AUDIO_OFF==onoff)
				WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainSwR, 0x800000);
			else	{
				 WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainSwR, MinGain);
			}
			break;
		case Secondary:
			if(AUDIO_OFF==onoff)
				WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainRS, 0x800000);
			else	{
				WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_MinGainRS, MinGain);
			}
			break;
  }

}
/*----------------------------------------------------------------------------
* Function :Audio_DrvLimiterFlagGenerSet								            
* Input       :Section                                                                                                       
*                      Front                                                                                                   
*                      Rear                                                                                                    
*                      SubLeft/Center                                                                                    
*                      SubRight                                                                                                 
*                      Secondary                                                                                               
*                   clipdetectMask:                                                                                        
*                           LimiterFlag_Mask_SCD0  ,LimiterFlag_Mask_SCD1..                                                
*                   polarity:                                                                                                  
*                           GPIO polarity                                                                                  
* Output      :null                                                                                        
* FUNCTION    :set Limiter  parametric     
*例如:要设置FRONT通道Limiter出触发源为GPIO_0 高电平触发
*       Audio_DrvLimiterFlagGenerSet(Front,LimiterFlag_Mask_GPIO,0);
*---------------------------------------------------------------------------*/
void Audio_DrvLimiterFlagGenerSet(eAudioChannel section,S8 clipdetectMask,U32 polarity)
{
   S32 Mask=clipdetectMask;
   switch(section)
  	{
         case Front:	
		 WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_FlgGenMaskFR, Mask);	
		 break;
	 case Rear:
	 	 WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_FlgGenMaskRS, Mask);	
	 	 break;
	case SubCenter:	
		WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_FlgGenMaskSwL, Mask);	
		break;
	case SubWoofer:	
		WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_FlgGenMaskSwR, Mask);	
	        break;
	case Secondary:
		WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_FlgGenMaskRS, Mask);	
		break;	
     
       }
    WriteHero_MEM_1Word(I2CDRV_X_MEM, ADSP_X_FCLim_FlgGenDSPIO_Polarity, polarity);
}

/*--------------------------------------------------------------------------
* Function :Audio_DrvSoftClipDetectSet								
*                   ThreGain:                                                       
*                      Soft Clip detector treshold gain   [-80  0]    
* Output      :null                                                                                        
* FUNCTION    :set soft clip parametric                                      
*--------------------------------------------------------------------------*/
void Audio_DrvSoftClipDetectSet(U8 section,S8 ThreGain)
{ 
   U16 addr;
   S32 Threshold=(table_dB2Lin[-ThreGain]+1)*4096; 
   if(Threshold>0x7FFFFF)
  	Threshold=0x7FFFFF;
   switch(section)
     {
       case SCD0:
	      addr=ADSP_X_SCD_Thresh0;
	      break;
	case SCD1:
	      addr=ADSP_X_SCD_Thresh1;
	      break;
        case SCD2:
	      addr=ADSP_X_SCD_Thresh2;
	      break;
	case SCD3:
	      addr=ADSP_X_SCD_Thresh3;
	      break;
      }
   WriteHero_MEM_1Word(I2CDRV_X_MEM, addr, Threshold);
}


/*-----------------------------------------------------------------------
		  
Input:	NULL		
Output:	NULL		
Description: 	enable hero chime generator
------------------------------------------------------------------------*/
void Audio_DrvEnableChime(void)
{
	TEF663x_EasyProgrammWrite(ADSP_EASYP_Chime_Enable); //see the para in UM
}
/*-----------------------------------------------------------------------
		  
Input:	NULL		
Output:	NULL		
Description: 	disable hero chime generator
------------------------------------------------------------------------*/
void Audio_DrvDisableChime(void)
{
	Audio_DrvStopChime();//must stop the chime generator before disable it
	TEF663x_EasyProgrammWrite(ADSP_EASYP_ChimePDC_Disable); //see the para in UM,and at the same time the PDC 1 will be also disable
}
/*-----------------------------------------------------------------------
Input:	NULL		
Output:	NULL		
Description: 	stop hero chime generator
------------------------------------------------------------------------*/
void Audio_DrvStopChime(void)
{
	WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_Tsq_SeqStat,0);//stop the chime generator
}
/*-----------------------------------------------------------------------
Input:	vol:   chime master volume -66~ 0 dB
Output:	NULL		
Description: 	set the chime master volume 
------------------------------------------------------------------------*/
void Audio_DrvSetChimeMasterVol(S8 vol)
{
	S32 Data;
	if(vol>0)//check the volume range
		vol=0;
	if(vol<-66)
		vol=-66;
	Data=table_dB2Lin[-vol];                    //chime volume set
	WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_Tsq_ChimeVol_Bk,Data);
}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: enable the pdc generator, PDC1=chime	 
------------------------------------------------------------------------*/
void Audio_DrvEnablePDC(void)
{
	Audio_DrvEnableChime();//PDC 1 use the same switch with chime
}
/*-----------------------------------------------------------------------
Input:	
Output:			
Description: disable the pdc generator, PDC1=chime	 
------------------------------------------------------------------------*/
void Audio_DrvDisablePDC(void)
{
	Audio_DrvDisableChime();//PDC 1 use the same switch with chime
}

/*-----------------------------------------------------------------------
Input:	OnOff: 0=off, others=on	
Output:	NULL		
Description: enable or disable the sin noise generator	 
------------------------------------------------------------------------*/
void Audio_DrvSinGenSwitch(U8 OnOff)
{
	if(OnOff)//enable the sin generator
	{
		TEF663x_EasyProgrammWrite(ADSP_EASYP_SinGen_Enable);
	}
	else//disable the sin generator, also with the noise generator and checksum function
	{
		TEF663x_EasyProgrammWrite(ADSP_EASYP_NoiseGen_SinGen_CheckSum_Disable);
	}
}
/*-----------------------------------------------------------------------
Input:	OnOff: 0=off, others=on	
Output:	NULL		
Description: enable or disable the sin noise generator	 
------------------------------------------------------------------------*/
void Audio_DrvNoiseGenSwitch(U8 OnOff)
{
    if(OnOff)//enable the noise generator
    {
		TEF663x_EasyProgrammWrite(ADSP_EASYP_NoiseGen_Enable);
    }
    else//disable the noise generator, also with the sin generator and checksum function
    {
		TEF663x_EasyProgrammWrite(ADSP_EASYP_NoiseGen_SinGen_CheckSum_Disable);
    }
}
/*-----------------------------------------------------------------------
Input:	Type: 0=white, others=pink	 
Output:			
Description: set the noise type	 
------------------------------------------------------------------------*/
void Audio_DrvSetNoiseType(U8 Type)
{
	S32Y Buffer[9];

	if(Type)	//pink noise see the para calcute of filter on UM
	{
		Buffer[0]=0x0820;	//ADSP_Y_NG_a11                             
		Buffer[1]=0x07FF;	//ADSP_Y_NG_a01                             
		Buffer[2]=0x07F7;	//ADSP_Y_NG_b11                             
		Buffer[3]=0x0954;	//ADSP_Y_NG_a12                              
		Buffer[4]=0x07FF;	//ADSP_Y_NG_a02                              
		Buffer[5]=0x0795;	//ADSP_Y_NG_b12                              
		Buffer[6]=0x0FEA;	//ADSP_Y_NG_a13                              
		Buffer[7]=0x006D;	//ADSP_Y_NG_a03                              
		Buffer[8]=0x0046;	//ADSP_Y_NG_b13                              
	}
	else   //white noise  see the para calcute of filter on UM
	{
		Buffer[0]=0x0333;	//ADSP_Y_NG_a11                              
		Buffer[1]=0x07FF;	//ADSP_Y_NG_a01                             
		Buffer[2]=0x0814;	//ADSP_Y_NG_b11                            
		Buffer[3]=0x04CD;	//ADSP_Y_NG_a12                             
		Buffer[4]=0x07FF;	//ADSP_Y_NG_a02                             
		Buffer[5]=0x099A;	//ADSP_Y_NG_b12                             
		Buffer[6]=0x0000;	//ADSP_Y_NG_a13                             
		Buffer[7]=0x0150;	//ADSP_Y_NG_a03                             
		Buffer[8]=0x0000;	//ADSP_Y_NG_b13                             
	}
	WriteDataToHero(I2CDRV_Y_MEM,ADSP_Y_NG_a11,9,Buffer);
}
/*-----------------------------------------------------------------------
Input:	Freq:20~20000Hz	 
Output:	PARA_ERROR=input para error
		SUCCESS=set ok
Description: set the sin generator freq
------------------------------------------------------------------------*/
S8 Audio_DrvSetSinFreq(U32 Freq)
{
	S32Y Buffer[3];
	double 	Data01,Data02;
	 
	if((Freq<20)||(Freq>20000))//check the input freq range
	{
	    return PARA_ERROR;
	}
	Audio_DrvSinGenSwitch(1);//enable the sin generator

	//calculate the data
	Data01=DOUBLE_PI*((double)(2*Freq));
	Data01/=(double)TEF663x_ADSP_SAMPLEFREQ;
	Data02=cos(Data01);

	if(Hero_TransDouble2XYHexData(HERO_2YDATA,Data02,Buffer)==SUCCESS)	//trans the float data to hero data
	{
		//Buffer[0]:ADSP_Y_SinGen_aHbak          this data has been updated with Hero_TransDouble2XYHexData   function          
		//Buffer[1]:ADSP_Y_SinGen_aLbak           this data has been updated with Hero_TransDouble2XYHexData   function             
	    Buffer[2]=0x0001;	  //ADSP_Y_SinGen_Init                      
	    WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_SinGen_aHbak,Buffer[0]);
	    WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_SinGen_aLbak,Buffer[1]);
	    WriteHero_MEM_1Word(I2CDRV_Y_MEM, ADSP_Y_SinGen_Init,Buffer[2]);
	}
	return SUCCESS;
} 
/*-----------------------------------------------------------------------
Input:	Channel: see define for detail
        	OnOff: 0=mute off, others=mute on	 
Output:	no use		
Description: 	set channel softmute switch	 
------------------------------------------------------------------------*/
S8 Audio_DrvSoftMuteSwitch(U16 Channel,U8 OnOff)
{
	S32  MuteData;

	MuteData = OnOff? 0x000/*mute on*/ : 0x07FF/*mute off*/;//0x0800 is no right audio output reverse phase
	if(Channel&TEF663x_PCHANNEL)		//overall primary channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Mute_P,MuteData);
	}
	if(Channel&TEF663x_PFLCHANNEL)		//front left channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_MuteSix_FL,MuteData);
	}
	if(Channel&TEF663x_PFRCHANNEL)		//front right channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_MuteSix_FR,MuteData);
	}
	if(Channel&TEF663x_PRLCHANNEL)		//rear left channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_MuteSix_RL,MuteData);
	}
	if(Channel&TEF663x_PRRCHANNEL)		//rear right channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_MuteSix_RR,MuteData);
	}
	if(Channel&TEF663x_SWLCHANNEL)		//subwoofer left channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_MuteSix_SwL,MuteData);
	}
	if(Channel&TEF663x_SWRCHANNEL)		//subwoofer right channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_MuteSix_SwR,MuteData);
	}
	if(Channel&TEF663x_SCHANNEL)		//overall secondary channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Mute_S,MuteData);
	}
	if(Channel&TEF663x_NCHANNEL)		//overall navigation channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Mute_N,MuteData);
	}
	if(Channel&TEF663x_TCHANNEL)		//overall phone channel
	{
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Mute_T,MuteData);
	}
	return SUCCESS;
}
/*-----------------------------------------------------------------------
Input:	Channel: see define for detail
        	Ta:Mute Fall Time,Tr:Mute Rase Time	 
Output:	no use		
Description: 	config softmute fade in /fade out 
------------------------------------------------------------------------*/
void Audio_DrvSoftMuteConfig(U16 Channel,U16 Ta,U16 Tr)
{
	double Data1;
	S32 Data_A,Data_R;
     Data1=0.8*2.0/(double)(44.1*Ta);
	Hero_TransDouble2XYHexData(HERO_XDATA,Data1,&Data_A);	//trans the float data to hero data
	
	Data1=0.8*2.0/(double)(44.1*Tr);
	Hero_TransDouble2XYHexData(HERO_XDATA,Data1,&Data_R);	//trans the float data to hero data
	
	switch(Channel)
	{
	case TEF663x_PCHANNEL:

		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpAttP,Data_A); 
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpRelP,Data_R); 
		break;
	case TEF663x_PFLCHANNEL:
	case TEF663x_PFRCHANNEL:
	case TEF663x_PRRCHANNEL:	
	case TEF663x_PRLCHANNEL:	
	case TEF663x_SWLCHANNEL:
	case TEF663x_SWRCHANNEL:
		Data_A=Data_A<<1;
		Data_R=Data_R<<1;
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_MuteSix_StpAtt,Data_A); 
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_MuteSix_StpRel,Data_R); 
		break;
	case TEF663x_SCHANNEL:
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpAttS,Data_A); 
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpRelS,Data_R); 
		break;
	case TEF663x_NCHANNEL:
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpAttN,Data_A); 
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpRelN,Data_R); 
		break;
	case TEF663x_TCHANNEL:
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpAttT,Data_A); 
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpRelT,Data_R); 
		break;
	default:
		break;
	}


}

/* coefs are dependant on (fs, f_cutoff)            */
/* Table Construction: a1t, a2t, b0t, b1t, b2t */
static const U16 table_Loudness_TrebleCoeffs[5] =
{ 
	//0x230, 0x000, 0x318, 0xce8, 0x000, /* fc = 4000, fs = SAMP_FREQ_44KHZ */
	//0x0536, 0x0DD0, 0x00E8, 0x0000, 0x0F18/*fc=4k,q=1*/
	0x0C5, 0xEB7, 0x15C, 0x000, 0xEA4
};

/* coefs are dependant on (fs, f_cutoff)            */
/* b1b does not exist */
/* Table Construction: a1bL, a1bH, a2bL, a2bH, b0bL, b0bH, b2bL, b2bH */
static const U16 table_Loudness_BassCoeffs[8] =
{ 
	//0x01f, 0x7cb, 0x0a1, 0xc34, 0x3a0, 0x000, 0x3a0, 0x000, /* fc = 150, fs = SAMP_FREQ_44KHZ */
	//0x0711, 0x07E9, 0x053B, 0x0C15, 0x069D, 0x000A, 0x0162, 0x0FF5/*fc=150,q=1*/
	0x0E6, 0x7F6, 0x571, 0xC09, 0x6B9, 0x004, 0x147, 0xFFB
};
/*-----------------------------------------------------------------------
		  
Input:	OnOff: 0=off,others=on
Output:			
Description: 	set the loudness work state 
------------------------------------------------------------------------*/
static const sMemAddr_Data LoudnessSwitchTab[] = {
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_MaxBstB,	/*0x0096*/0x022a},/*max bass gain 12.04db*/
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_MaxBstT,	/*0x012B*/0x012b},/*max treble gain 4.43db*/
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KpBL,		0x02B6},
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KpBH,		0x0809},
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KmBL,		0x0273},
	{I2CDRV_Y_MEM,	ADSP_Y_Loudf_KmBH,		0x0012},
	//{I2CDRV_Y_MEM,	ADSP_Y_Loudf_StatLev,		0x0000}, 
 	
	//end flag
	{I2CDRV_INVALID,	0,			0},
};
S8 Audio_DrvLoudnessSwitch(U8 OnOff)
{
	U8 i;

	U32 Buffer[8];

	if(OnOff)//the loudness will on
	{
		//treble config, see define for more information
		for(i=0;i<5;i++)
		{
			Buffer[i]=table_Loudness_TrebleCoeffs[i];
		}
		WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_Loudf_a1t_REL,ADSP_EASYP_UpdateCoeff_Coeff5,Buffer);

		//bass config, see define for more information
		for(i=0;i<8;i++)
		{
			Buffer[i]=table_Loudness_BassCoeffs[i];
		}

		WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_Loudf_a1bL_REL,ADSP_EASYP_UpdateCoeff_Coeff8,Buffer);

		//update the global config
		WriteHero_MEM_Tab(LoudnessSwitchTab);
	}
	else//loudness off,stop the loudness
	{
		//coupled mode ,just one para
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Loudf_StatLev,0x07FF);	
	}
	///DispFlag_LOUD(OnOff);//display of the loudness icon
	return SUCCESS;
}

/*-----------------------------------------------------------------------
Input:	BarID:gsa bar
		BufferP:output data
Output:	PARA_ERROR:input para error
		SUCCESS=read ok
Description: 	read the current gsa data  
------------------------------------------------------------------------*/
S8 Audio_DrvReadGSABarData(eHero_GSABarIDDef BarID,U8 *BufferP)
{
	U8 Buffer;

	if(BufferP==NULL)//check the input pointer 
	{
		return PARA_ERROR;
	}

	if(BarID==Total)//read all bars data
	{
		Buffer=(U8)Bar1;
		I2C1_WriteData(TEF663x_SlaveAddr,&Buffer,1);
		I2C1_ReadData(TEF663x_SlaveAddr,BufferP,9);
	}
	else//read one bar data
	{
		Buffer=(U8)BarID;
		I2C1_WriteData(TEF663x_SlaveAddr,&Buffer,1);
		I2C1_ReadData(TEF663x_SlaveAddr,BufferP,1);	
	}
	return SUCCESS;
}
/*-----------------------------------------------------------------------
Input:	OnOff:0=off, others=on
Output:			
Description: 	  GSA Switch
------------------------------------------------------------------------*/
S8 Audio_DrvGSASwitch(U8 OnOff)
{
	if(OnOff)		 //enable
	{
		TEF663x_EasyProgrammWrite(ADSP_EASYP_GSA_Enable);
	}
	else			 //disable
	{
		TEF663x_EasyProgrammWrite(ADSP_EASYP_GSA_Disable);
	}
	return SUCCESS;
}
/*-----------------------------------------------------------------------
Input:			
Output:			
Description: shut down  the click-clack	 
------------------------------------------------------------------------*/
void Audio_DrvDisableClickClack(void)
{
	TEF663x_EasyProgrammWrite(ADSP_EASYP_ClickClack_Disable); //see the para in UM
}
/*-----------------------------------------------------------------------
Input:	ADNum:  0~3, AD input channel
		Mode:0~5,     Dif 2V=0,	Dif 1V=1, HCM 2V=2,	HCM 1V=3, HCM 0.5V=4, IPod=5
Output:			
Description: set the AD input mode	 
------------------------------------------------------------------------*/
U8 Audio_DrvSetADInMode(U8 ADNum, U8 Mode)
{
	U8 Buffer[3];    

	if((ADNum>3)||(Mode>5))
	{
		return PARA_ERROR;
	}

	Buffer[0]=0xA9;  //subaddr of the peripheral
	Buffer[1]=ADNum;
	Buffer[2]=Mode; 
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,3);	 //start to send data to i2c1 bus
	return SUCCESS;		    
}
/*-----------------------------------------------------------------------
Input:	I2SNum:  0~3, I2S0~I2S2,Host I2S 
		Mode:0~5,     Philips=0, LSB 16bits=1, LSB 18bits=2, LSB 20bits=3, LSB 24bits=4, off=5
Output:			
Description: set the I2S input mode	 
------------------------------------------------------------------------*/
U8 Audio_DrvSetI2SInMode(U8 I2SNum, U8 Mode)
{
	U8 Buffer[3];    

	if((I2SNum>3)||(Mode>5))
	{
		return PARA_ERROR;
	}

	Buffer[0]=0xA9;  //subaddr of the peripheral
	if(I2SNum==3)
	{ 
		Buffer[1]=0x14;
	}
	else
	{
		Buffer[1]=0x0A+I2SNum;
	}
	Buffer[2]=Mode; 
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,3);	 //start to send data to i2c1 bus
	return SUCCESS;		    
}
/*-----------------------------------------------------------------------
Input:	Mode:0~2,     Analog=0, digital=1, off=2
Output:			
Description: set the SPDIF input mode	 
------------------------------------------------------------------------*/
U8 Audio_DrvSetSPDIFInMode(U8 Mode)
{
	U8 Buffer[3];    

	if(Mode>2)
	{
		return PARA_ERROR;
	}

	Buffer[0]=0xA9;  //subaddr of the peripheral
	Buffer[1]=0x1E;
	Buffer[2]=Mode; 
	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,3);	 //start to send data to i2c1 bus
	return SUCCESS;		    
}
/*-----------------------------------------------------------------------
Input:	GPIO_Num: 0~8 ref to the part num 
        FeatureID: see um for the pin feature ID
Output:			
Description: 	set the gpio to be the function selected  
------------------------------------------------------------------------*/
S8 Hero_PinConnect(U8 GPIO_Num,U32 FeatureID)
{
	U8 Buffer[4];

	if(GPIO_Num>8)
	{
		return PARA_ERROR;
	}
	Buffer[0]=0xC0;  //subaddr of the GPIO config
	Buffer[1]=GPIO_Num;
	Buffer[2]=(U8)(FeatureID>>8); 
	Buffer[3]=(U8)FeatureID;

	I2C1_WriteData(TEF663x_SlaveAddr,Buffer,4);	 //start to send data to i2c1 bus
	return SUCCESS;		  
}

/*-----------------------------------------------------------------------
Input:	 Pin: 0~8 ref to the part num 
		 Polarity: 0=low, others=high
		 FallTime: 0.1ms~1000mS	   //attach time
		 RiseTime: 0.1mS~10000mS   //release time
Output:			
Description: 	set the fast mute config  
------------------------------------------------------------------------*/
S8 Audio_DrvFastMuteConfig(U8 Pin,U8 Polarity,double FallTime,double RiseTime)
{
	double Data1;
	S32 Data;

	Hero_PinConnect(Pin,GPIO_FUNID_FASTMUTE);
	if(Polarity)
	{
		Data1=0.8*4.0/(44.1*FallTime);
		Hero_TransDouble2XYHexData(HERO_XDATA,Data1,&Data);	//trans the float data to hero data
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpAttDac,Data);
		Data1=0.8*4.0/(44.1*RiseTime);
		Hero_TransDouble2XYHexData(HERO_XDATA,Data1,&Data);	//trans the float data to hero data
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpRelDac,Data); 
	}
	else
	{
		Data1=0.0-0.8*4.0/(44.1*FallTime);
		Hero_TransDouble2XYHexData(HERO_XDATA,Data1,&Data);	//trans the float data to hero data
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpAttDac,Data);
		Data1=0.0-0.8*4.0/(44.1*RiseTime);
		Hero_TransDouble2XYHexData(HERO_XDATA,Data1,&Data);	//trans the float data to hero data
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Mute_StpRelDac,Data);	
	}
	return SUCCESS; 
}


/*+12db ~ -36db*/
const S32 tabHero_PhonNavVolumeValue[49]=
{
/*12 ~3db*/
0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0805, 0x08E3, 0x09A9, 0x0A5A,

/*+2 ~-7db*/
0x0AE7, 0x0B83, 0x0C00, 0x0C6F, 0x0CD3, 0x0D2B, 0x0D7A, 0x0DC0, 0x0DFF, 0x0E37,

/*-8 ~-17db*/
0x0E68, 0x0E95, 0x0EBC, 0x0EDF, 0x0EFF, 0x0F1B, 0x0F34, 0x0F4A, 0x0F5E, 0x0F6F,

/*-18 ~-27db*/
0x0F7F, 0x0F8D, 0x0F9A, 0x0FA5, 0x0FAF, 0x0FB8, 0x0FBF, 0x0FC6, 0x0FCD, 0x0FD2,

/*-28 ~-36db*/
 0x0FD7, 0x0FDC, 0x0FE0, 0x0FE3, 0x0FE6, 0x0FE9, 0x0FEC, 0x0FEE, 0x0FF0,
};
/*-----------------------------------------------------------------------
Input:	Channel: see define
		Volume: the volume value, real value dB
Output:	not use		
Description: 	 
------------------------------------------------------------------------*/
S8 Audio_DrvSetVolume(Audio_Stream_t Channel,double Volume)
{
	S32Y Volume_Data01,Volume_Data02;
	//double ExpData;
	U8 Step;
     U32 addr;
	switch(Channel)
 	{
      case PrimaryStr:
		  addr=ADSP_Y_Vol_Main1P;
		  break;
	  case SecondaryStr:
	  	  addr=ADSP_Y_Vol_Main1S;
		  break;
	  case NavigationStr:
	  	  addr=ADSP_Y_Vol_Nav;
	  	  break;
	  case PhoneStr:
	  	  addr=ADSP_Y_Vol_Phon;
	  	  break;
	  default:
          return ERROR;

	}
	if((Channel==PrimaryStr)||(Channel==SecondaryStr))		 //set primary channel volume 
	{
		if(Volume>=0.0)			 //see volume calcute  
		{
		     /*ADSP_Y_Vol_Main1P(S) calcute 
			 ADSP_Y_Vol_Main1P(S)=1  */
			Volume_Data01=0x07FF;
			 
			/*ADSP_Y_Vol_Main2P(S) calcute
                 ADSP_Y_Vol_Main2P(S)=power(10,(Voldb-MainVolMax)/20)*2048  
                                     =table_dB2Lin[TEF663X_MAINVOLUMEMAX-Volume]
			*/
			Step=(U8)(TEF663X_MAINVOLUMEMAX-Volume);
			Volume_Data02=table_dB2Lin[Step];
		}
		else if(Volume>=(0.0-TEF663x_LOUDBOOSTMAX))//see volume calcute  
		{
		     /*ADSP_Y_Vol_Main1P(S) calcute 
			 ADSP_Y_Vol_Main1P(S)=power(10,(Voldb)/20)*2048  
			                     =table_dB2Lin[-Volume]*/
			//ExpData=Volume;
			Step=(U8)(0-Volume);
			Volume_Data01=table_dB2Lin[Step];
			
               /*ADSP_Y_Vol_Main2P(S) calcute 
			 ADSP_Y_Vol_Main2P(S)=power(10,(-MainVolMax)/20)*2048  
			                     =table_dB2Lin[TEF663X_MAINVOLUMEMAX]*/
			//ExpData=0.0-TEF663X_MAINVOLUMEMAX;
			//Step=0-ExpData;
			Volume_Data02=table_dB2Lin[TEF663X_MAINVOLUMEMAX];
		}
		else if(Volume>=(TEF663X_MAINVOLUMEMAX-TEF663X_FIXEDBOOST-TEF663x_LOUDBOOSTMAX))//see volume calcute  
		{
		     /*ADSP_Y_Vol_Main1P(S) calcute 
			 ADSP_Y_Vol_Main1P(S)=power(10,(-MaxLoudBoost)/20)*2048  
			                     =table_dB2Lin[TEF663x_LOUDBOOSTMAX]*/
			//ExpData=0.0-TEF663x_LOUDBOOSTMAX;
			//Step=0-ExpData;
			Volume_Data01=table_dB2Lin[TEF663x_LOUDBOOSTMAX];

                /*ADSP_Y_Vol_Main2P(S) calcute 
			 ADSP_Y_Vol_Main2P(S)=power(10,(Voldb+MaxLoudBoost-MainVolMax)/20)*2048  
			                     =table_dB2Lin[TEF663X_MAINVOLUMEMAX-Volume-TEF663x_LOUDBOOSTMAX]*/
			//ExpData=Volume+TEF663x_LOUDBOOSTMAX-TEF663X_MAINVOLUMEMAX;
			Step=(U8)(TEF663X_MAINVOLUMEMAX-TEF663x_LOUDBOOSTMAX-Volume);
			Volume_Data02=table_dB2Lin[Step];
		}
		else//see volume calcute  
		{
		       /*ADSP_Y_Vol_Main1P(S) calcute 
			 ADSP_Y_Vol_Main1P(S)=256*power(10,(Voldb-MaxLoudBoost)/20)*2048  
			                     =table_dB2Lin[TEF663x_LOUDBOOSTMAX]*/
			//ExpData=Volume-TEF663X_MAINVOLUMEMAX;
			Step=(U8)(TEF663X_MAINVOLUMEMAX-Volume);
			Volume_Data01=table_dB2Lin[Step];
			Volume_Data01*=256;

			 /*ADSP_Y_Vol_Main2P(S) calcute 
			 ADSP_Y_Vol_Main2P(S)=2048/256*/
			Volume_Data02=0x008;
		}
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,addr,Volume_Data01); //the primary volume will be divided to 2 prara
		WriteHero_MEM_1Word(I2CDRV_Y_MEM,addr+1,Volume_Data02);//the primary volume will be divided to 2 prara
	}
	else if((Channel==NavigationStr)||(Channel==PhoneStr))
	{
	    
	    /*UM_Audio_TEF6635_TEF6638_Rev.3.3.pdf P61 The Calculations is wrong?
           the table tabHero_PhonNavVolumeValue[] is got from GUI
		*/
	    Step=(12-Volume);
         Volume_Data01=tabHero_PhonNavVolumeValue[Step];
	    WriteHero_MEM_1Word(I2CDRV_Y_MEM,addr,Volume_Data01); 
	}
	return SUCCESS;

}
/*-----------------------------------------------------------------------
Input:	Band: 0~4,the geq band num
		Geq_DescaleData: geq descale data, Y memory data format
Output:			
Description: 	write one band geq data to hero 
------------------------------------------------------------------------*/
S8 Audio_DrvConfigGeqBand(U8 Band,S32 Geq_DescaleData)
{
	U32 Buffer[7];
	U8 i;
	S32 Data;

	if(Band>=GEQ_MAXBAND)//check the band range
	{
		return PARA_ERROR;
	}

	if(Band<3) //double presicion
	{
		for(i=0;i<6;i++)
		{
			Buffer[i]=Geq_UserModeTable[Band][i+1];//get the band data Y memory format
		}
		Buffer[6]=Geq_UserModeTable[Band][0];//get the  band data Y memory format
	}
	else //single presicion
	{
		for(i=0;i<3;i++)
		{
			Buffer[i]=Geq_UserModeTable[Band][i*2+1];//get the band data Y memory format
		}
		Buffer[3]=Geq_UserModeTable[Band][0];      //get the band data Y memory format
	}
	switch(Band)
	{

		case 0:
		{
			WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b10L_REL, ADSP_EASYP_UpdateCoeff_Coeff7, Buffer);
			Data=Geq_DescaleData; //write the descale data
			WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data); 			
			break;
		}
		case 1:
		{
			WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b20L_REL, ADSP_EASYP_UpdateCoeff_Coeff7, Buffer);
			Data=Geq_DescaleData; //write the descale data
			WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data); 				
			break;
		}
		case 2:
		{
			WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b30L_REL, ADSP_EASYP_UpdateCoeff_Coeff7, Buffer);
			Data=Geq_DescaleData; //write the descale data
			WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data);  				
			break;
		}		
		case 3:
		{
			WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b40_REL, ADSP_EASYP_UpdateCoeff_Coeff4, Buffer);
			Data=Geq_DescaleData; //write the descale data
			WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data);  				
			break;
		}		
		case 4:
		{
			WriteScratchDataToHero(I2CDRV_Y_MEM,ADSP_Y_GEq_b50_REL, ADSP_EASYP_UpdateCoeff_Coeff4, Buffer);
			Data=Geq_DescaleData; //write the descale data
			WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_DesScalGEq,Data); 				
			break;
		}		
	}
	return SUCCESS;
}
/*-----------------------------------------------------------------------
Input:	Geq_Band: 0~4,the geq band num
		pGeqPara: pointer to Geq_Band`s para struct,see define for paras
Output:			
Description: 	calcute the geq data and trans to the hero data  
------------------------------------------------------------------------*/
S8 Audio_DrvCalcuteGeq(U8 Geq_Band,sGeq_ParaDef *pGeqPara)
{								 
	double G,t,D,a2,a1,b0,Data01,Data02,Data03;
	S32Y YData,DYData[2];
	U8 Geq_TableRel;

	//see the UM for the calcute formula
	Data01=pGeqPara->Gain;
	Data01/=20.0;
	Data02=pow(10,Data01);
	Data02-=1.0;
	G=Data02/4.0;

	Data01=pGeqPara->Fc/pGeqPara->Fs;
	t=Data01*DOUBLE_PI*2.0;	

	if(pGeqPara->Gain>=0)
	{
		D=1.0;	
	}
	else
	{
		Data01=G*4.0+1.0;
		D=1.0/Data01;
	}

	Data01=2.0*pGeqPara->Q;	  
	Data02=t/Data01;
	Data01=D*Data02;
	Data03=Data01+1.0;
	Data02=1.0-Data01;
	a2=(0.0-0.5)*Data02/Data03;

	Data01=cos(t);
	Data02=0.5-a2;
	a1=Data01*Data02;

	b0=(0.5+a2)/2.0;

	if(Hero_TransDouble2XYHexData(HERO_YDATA,G,&YData)==SUCCESS)	//trans the float data to hero data
	{
		Geq_TableRel=0;
		Geq_UserModeTable[Geq_Band][Geq_TableRel]=YData;
	}


	if((Geq_Band==0)||(Geq_Band==1)||(Geq_Band==2))//double precision
	{
		if(Hero_TransDouble2XYHexData(HERO_2YDATA,b0,DYData)==SUCCESS)	//trans the float data to hero data
		{
			Geq_TableRel=1;
			Geq_UserModeTable[Geq_Band][Geq_TableRel]=DYData[1];
			Geq_TableRel=2;
			Geq_UserModeTable[Geq_Band][Geq_TableRel]=DYData[0];
		}
		if(Hero_TransDouble2XYHexData(HERO_2YDATA,a2,DYData)==SUCCESS)	//trans the float data to hero data
		{
			Geq_TableRel=3;
			Geq_UserModeTable[Geq_Band][Geq_TableRel]=DYData[1];
			Geq_TableRel=4;
			Geq_UserModeTable[Geq_Band][Geq_TableRel]=DYData[0];
		}
		if(Hero_TransDouble2XYHexData(HERO_2YDATA,a1,DYData)==SUCCESS)	//trans the float data to hero data
		{
			Geq_TableRel=5;
			Geq_UserModeTable[Geq_Band][Geq_TableRel]=DYData[1];
			Geq_TableRel=6;
			Geq_UserModeTable[Geq_Band][Geq_TableRel]=DYData[0];
		}	
	}
	else if((Geq_Band==3)||(Geq_Band==4))//single precision
	{
		if(Hero_TransDouble2XYHexData(HERO_YDATA,b0,&YData)==SUCCESS)	//trans the float data to hero data
		{
			Geq_TableRel=1;
			Geq_UserModeTable[Geq_Band][Geq_TableRel]=YData;
		}
		if(Hero_TransDouble2XYHexData(HERO_YDATA,a2,&YData)==SUCCESS)	//trans the float data to hero data
		{
			Geq_TableRel=3;
			Geq_UserModeTable[Geq_Band][Geq_TableRel]=YData;
		}
		if(Hero_TransDouble2XYHexData(HERO_YDATA,a1,&YData)==SUCCESS)	//trans the float data to hero data
		{
			Geq_TableRel=5;
			Geq_UserModeTable[Geq_Band][Geq_TableRel]=YData;
		}	
	}
	else
	{
		return PARA_ERROR;
	}
	return SUCCESS;
}
/*-----------------------------------------------------------------------
Function name:			  
Input:	 Channel: see head file for data define
         	ScalingdB: dB of source scaling, -60~6.02dB
Output:			
Description: 	set the input source scaling  
------------------------------------------------------------------------*/
S8 Audio_DrvSetSourceScaling(Audio_Stream_t Channel, double ScalingdB)
{
	double Data1;
	S32 Data;


	if((ScalingdB>6.02)||(ScalingdB<-60.0))//check the source scaling data  range
	{
		return PARA_ERROR;
	}

	//calcute the data
	Data1=ScalingdB-6.02;
	Data1/=20.0;
	ScalingdB=pow(10,Data1);
	Hero_TransDouble2XYHexData(HERO_YDATA,ScalingdB,&Data);	//trans the float data to hero data

	switch(Channel)
	{
		case PrimaryStr://primary channel
		{
			WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_SrcScalP,Data); 
			break;
		}
		case SecondaryStr://secondary channel
		{
			WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_SrcScalS,Data);
			break;
		}
		case NavigationStr://navigation channel
		{
			WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_SrcScalN,Data);
			break;
		}
		case PhoneStr://phone channel
		{
			WriteHero_MEM_1Word(I2CDRV_Y_MEM,ADSP_Y_Vol_SrcScalT,Data);
			break;
		}
		default:
		{
			return PARA_ERROR;
		}
	}
	return SUCCESS;
}
struct
{
	double PFLChannel;
	double PFRChannel;
	double PRLChannel;
	double PRRChannel;
	double SWLChannel;
	double SWRChannel;
}Hero_DelayTime;
/*-----------------------------------------------------------------------
Input:	Channel: can set several channel once but with the same delay time
			TEF663x_PFLCHANNEL 0x0001   //front left channel     
			TEF663x_PFRCHANNEL 0x0002	//front right channel
			TEF663x_PRLCHANNEL 0x0004	//rear left channel
			TEF663x_PRRCHANNEL 0x0008	//rear right channel
			TEF663x_SWLCHANNEL 0x0010	//subwoofer left channel
			TEF663x_SWRCHANNEL 0x0020	//subwoofer right channel
			TEF663x_SLCHANNEL 0x0040	//secondary left channel
			TEF663x_SRCHANNEL 0x0080	//secondary right channel
 		Time: 0.0~20.0ms, and Total<20ms
Output:	PARA_ERROR=the input para is wrong
		SUCCESS=set ok
Description: 	 se the  channel delay time
------------------------------------------------------------------------*/
S8 Audio_DrvSetDelay(U16 Channel, double Time)
{	
	double Data1;
	U32 Data;
	double MaxDelayTime;

	if((Time<0)||(Time>20)||(Channel>=0x40))
	{
		return PARA_ERROR;
	}

	MaxDelayTime=0;
	Data1=0;
	if(Hero_DelayTime.PFLChannel>MaxDelayTime)
	{
		MaxDelayTime=Hero_DelayTime.PFLChannel;
	}
	if(Hero_DelayTime.PFRChannel>MaxDelayTime)
	{
		MaxDelayTime=Hero_DelayTime.PFRChannel;
	}
	if(Hero_DelayTime.PRLChannel>MaxDelayTime)
	{
		MaxDelayTime=Hero_DelayTime.PRLChannel;
	}
	if(Hero_DelayTime.PRRChannel>MaxDelayTime)
	{
		MaxDelayTime=Hero_DelayTime.PRRChannel;
	}
	if(Hero_DelayTime.SWLChannel>MaxDelayTime)
	{
		MaxDelayTime=Hero_DelayTime.SWLChannel;
	}
	if(Hero_DelayTime.SWRChannel>MaxDelayTime)
	{
		MaxDelayTime=Hero_DelayTime.SWRChannel;
	}

	//check if the total time exceeds 20ms
	Data1=Hero_DelayTime.PFLChannel+Hero_DelayTime.PFRChannel+Hero_DelayTime.PRLChannel
		+Hero_DelayTime.PRRChannel+Hero_DelayTime.SWLChannel+Hero_DelayTime.SWRChannel;

	if(Channel&&TEF663x_PFLCHANNEL)
	{
		Data1-=Hero_DelayTime.PFLChannel;
		Data1+=Time;
	}
	if(Channel&&TEF663x_PFRCHANNEL)
	{
		Data1-=Hero_DelayTime.PFRChannel;
		Data1+=Time;	    
	}
	if(Channel&&TEF663x_PRLCHANNEL)
	{
		Data1-=Hero_DelayTime.PRLChannel;
		Data1+=Time;	    
	}
	if(Channel&&TEF663x_PRRCHANNEL)
	{
		Data1-=Hero_DelayTime.PRRChannel;
		Data1+=Time;	    
	}
	if(Channel&&TEF663x_SWLCHANNEL)
	{
		Data1-=Hero_DelayTime.SWLChannel;
		Data1+=Time;	    
	}
	if(Channel&&TEF663x_SWRCHANNEL)
	{
		Data1-=Hero_DelayTime.SWRChannel;
		Data1+=Time;	    
	}
	if(Data1>20)
	{
		return PARA_ERROR;
	}

	//calcute the data
	Data1=Time*44.1;
	Data1+=0.5;
	Data=(U32)Data1;
	Data+=1;

	///p_FastMute_High();    //correspond with your hero gpio config       //mute to void the audible
	if(Channel&&TEF663x_PFLCHANNEL)
	{
		Hero_DelayTime.PFLChannel=Time;
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Delay_Length1_In,Data);
	}
	if(Channel&&TEF663x_PFRCHANNEL)
	{
		Hero_DelayTime.PFRChannel=Time;
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Delay_Length2_In,Data);
	}
	if(Channel&&TEF663x_PRLCHANNEL)
	{
		Hero_DelayTime.PRLChannel=Time;	    
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Delay_Length3_In,Data);
	}
	if(Channel&&TEF663x_PRRCHANNEL)
	{
		Hero_DelayTime.PRRChannel=Time;	    
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Delay_Length4_In,Data);
	}
	if(Channel&&TEF663x_SWLCHANNEL)
	{
		Hero_DelayTime.SWLChannel=Time;	    
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Delay_Length5_In,Data);
	}
	if(Channel&&TEF663x_SWRCHANNEL)
	{
		Hero_DelayTime.SWRChannel=Time;	    
		WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Delay_Length6_In,Data);
	}
	//must write this to update the delay para
	WriteHero_MEM_1Word(I2CDRV_X_MEM,ADSP_X_Delay_InitFlag,0x7FFFFF);
	TimerDelayMs(MaxDelayTime);//must delay the max delay time of all channels to avoid audible clicks
	///p_FastMute_Low();    //correspond with your hero gpio config   mute off
	return SUCCESS;
}

