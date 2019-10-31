/*
 *  OSDSPI.c - 
 *
 *  Copyright (C) 2011 Intersil Corporation
 *
 */
//*****************************************************************************
//
//								OSD.c
//
//*****************************************************************************
//
//
#include "include.h"
#define XDATA			 
#define wPrintf(...)	 
#define dPrintf(...)	 
#define ePrintf(...)	 

//==========================================
//win0: 0x420	animation
//win1: 0x440	background. low priority
//win2: 0x430
//..
//win8: 0x4B0	focused.    high priority
//----------------------------------------
 BYTE SpiWinBuff[10*0x10];
//extern  BYTE SpiWinBuff[10*0x10];

BYTE 	SpiFlash4ByteAddr = 0;			//4Byte mode for big SPIFLASH	 1 = 32bit adr     0 = 24bit adr	//for TW8836


//TW8835 have 9 windows
 BYTE	SpiOsdWinBase[9] = { SPI_WIN0_ST, 
		SPI_WIN1_ST, SPI_WIN2_ST, SPI_WIN3_ST, SPI_WIN4_ST,
		SPI_WIN5_ST, SPI_WIN6_ST, SPI_WIN7_ST, SPI_WIN8_ST
		};


//=============================================================================
//		OSD Window Functions
//=============================================================================


//BYTE SPICMD_READ        = 0x03;
//BYTE SPICMD_PROGRAM     = 0x02;

//BYTE SPICMD_BYTES		= 5;

 //BYTE SPI_Buffer[SPI_BUFFER_SIZE];
 //BYTE SPI_CmdBuffer[8];

//=============================================================================
//		OSD Window Setup
//=============================================================================
 BYTE	SPI_MODE_OFFSET[] = {30,21,26,38,22,28,37};
#define LUT_TYPE_FONT	0x00
#define LUT_TYPE_LUTS	0x80
#define LUT_TYPE_LUT	0xC0


#define DMA_TYPE_FONT	0	//x00
#define DMA_TYPE_CHIP	1	//x40
#define DMA_TYPE_SPIOSD	2	//x80
#define DMA_TYPE_MCU	3	//xC0


//desc: wait VBlank
void WaitVBlank(BYTE cnt)
{
	XDATA	BYTE i;
	WORD loop;
//	BYTE page;

	//page = 
	ReadTW88(0xff);
	//WriteTW88Page( PAGE0_GENERAL );

	for ( i=0; i<cnt; i++ ) {
		WriteTW88( PAGE0_GENERAL|0x02, 0xff );
		loop = 0;
		while (!( ReadTW88( PAGE0_GENERAL|0x02 ) & 0x40 ) ) {
			// wait VBlank
			loop++;
			if(loop > 0xFFFE) {
				//ePrintf("\nERR:WaitVBlank");
				break;
			}
		}	
		//Printf("\nWaitVBlank:%d",loop);
	}
	//WriteTW88Page(page);
}



//desc: Set SpiOsd DE value
//How to calculate DE value 
//	HDE = REG(0x210[7:0])
//	PCLKO = REG(0x20d[1:0]) {0,1,2,2}
//	PCLKO = REG(0x20d[1:0]) {1,1,1,1}  new
//	result = HDE + PCLKO - 17
void SpiOsdSetDeValue(void)
{
		WORD wTemp;
	BYTE HDE,PCLKO;

	WriteTW88Page( 0x02 );
	HDE = ReadTW88( PAGE2_SCALER|0x10 );				// HDE
	PCLKO = ReadTW88(PAGE2_SCALER|0x0d) & 0x03;
	//if(PCLKO == 3)
	//	PCLKO = 2;
	PCLKO = 1;

	wTemp = (WORD)HDE + PCLKO - 17;

	WriteTW88Page( 0x04 );
	WriteTW88( PAGE4_SOSD|0x0E, (BYTE)(wTemp>>8) );		// write SPI OSD DE value(high nibble)
	WriteTW88( PAGE4_SOSD|0x0F, (BYTE)wTemp );   		// write SPI OSD DE value(low byte)
	//dPrintf("\nSpiOsdDe:%04x",wTemp);		
}

//desc: Enable SpiOsd. HW function
void SpiOsdEnable(BYTE en)
{
	BYTE dat;
	WriteTW88Page( 0x04 );
	dat = ReadTW88(0x00);
	if( en ) 
	{
		//McuSpiClkSet(PCLK_SEL_PLL,SPI_CK_SEL_PCLK,SPI_CK_DIV_1P5);	//select MCU/SPI Clock.
		//McuSpiClkSet(PCLK_SEL_PCLK,SPI_CK_SEL_PCLK,SPI_CK_DIV_1P5);	//select MCU/SPI Clock.
		WriteTW88(PAGE4_SOSD|0x00, dat | 0x04);
	}
	else 
	{
	    WriteTW88(PAGE4_SOSD|0x00, dat & 0xfb);
		//McuSpiClkSet(PCLK_SEL_PLL,SPI_CK_SEL_27M,SPI_CK_DIV_1P5);
		//McuSpiClkSet(PCLK_SEL_PCLK,SPI_CK_SEL_27M,SPI_CK_DIV_1P5);
	}
}

//===========================================================================================
// Windows Finctions
//===========================================================================================
//
//desc: Enable SpiOsd Window. HW function.
void SpiOsdWinHWEnable(BYTE winno, BYTE en)
{
		BYTE index;
		BYTE dat;

	index = SpiOsdWinBase[winno] + SPI_OSDWIN_ENABLE;

	WriteTW88Page( 0x04 );
	dat = ReadTW88(PAGE4_SOSD|index);
	if( en ) {
		WriteTW88(PAGE4_SOSD|index, dat | 0x01);
	}
	else     WriteTW88(PAGE4_SOSD|index, dat & 0xfe);
}

void SpiOsdWinBuffEnable(BYTE winno, BYTE en)
{
	 BYTE  *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	if(en) *data_p |= 0x01;
	else   *data_p &= 0xfe;
}

//---------------------------------------------------------------------------------
//desc: clear all SpiWinBuff[]
void SOsdWinBuffClean(BYTE hw)
{
	BYTE i,j;

	if(hw) {
		SpiOsdWinHWOffAll(1);
	 	SpiOsdRLC(0,0,0);	//disable RLE
	}

	for(i=0; i < 10; i++) {
		for(j=0; j < 0x0E; j++)
			SpiWinBuff[i*16+j]=0;
	}
}
//--------------------------
//desc: write SpiWinBuff to HW registers
//	start address for ecah window
//	WIN		0	1	2	3	4	5	6	7	8
//	addr	420 440 450 460 470 480 490 4A0 4B0
//parameter
//	start: start window. between 0 to 8
//	end:   end window. between 0 to 8
void SOsdWinBuffWrite2Hw(BYTE start, BYTE end)
{
	 BYTE i; //,j;
	 BYTE reg_i;
	 BYTE  *data_p;

	//dPrintf("\nSOsdWinBuffWrite2Hw(%bd,%bd)",start,end);

	if(start)	start++;
	if(end)		end++;

	//WaitVBlank(1);
	WriteTW88Page( 0x04 );
	data_p = &SpiWinBuff[start << 4];
//EA = 0;
	for(i=start; i <= end; i++) {
		reg_i = (i << 4) + 0x20;
		//if(*data_p & 0x01) {
		//	dPrintf(" %bd:%bx", i-1, *data_p);
		//}
		//dPrintf("\nR:%bx ",reg_i);	
		//for(j=0; j < 16; j++) {
		//	WriteTW88(reg_i++, *data_p++);
		//	Printf("%bx ",SpiWinBuff[i*16+j]);	
		//}
		//dPrintf("::%bx",*data_p);	
		if(i==0)
		WriteTW88(PAGE4_SOSD|reg_i++, (*data_p++)|0x02 );	//0
		else
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//0

		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//1
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//2
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//3
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//4
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//5
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//6
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//7
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//8
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//9
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//A
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//B
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//C
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//D
		WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//E
		if(i) {
			data_p++;						//F
		}
		else {
			WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//0F
			i++;
			WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//10
			WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//11
			WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//12
			WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//13
			WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//14
			WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//15
			WriteTW88(PAGE4_SOSD|reg_i++, *data_p++);	//16
			reg_i+=9;
			data_p+=9;
		}
	}
//EA = 1;
}

void SpiOsdWinHWOffAll(BYTE wait)
{
	BYTE i;
	if(wait)
		WaitVBlank(wait);
	//SpiOsdEnableRLC(OFF);		//disable RLE
	for(i=0; i<= 8; i++)
		SpiOsdWinHWEnable(i, 0);
}


//WINx Image Location on SpiFlash	 
void SpiOsdWinImageLoc(BYTE winno, DWORD start)
{	
	 BYTE  *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	data_p += SPI_OSDWIN_BUFFERSTART;

	*data_p++ =  (BYTE)(start>>16);				//+0x07
	*data_p++ =  (BYTE)(start>>8);				//+0x08
	*data_p++ =  (BYTE)start;					//+0x09
}

#if 0
void SpiOsdWinImageLoc_TW88(BYTE winno, DWORD start)
{
	BYTE	index;
	
	index = SpiOsdWinBase[winno] + SPI_OSDWIN_BUFFERSTART;

	WriteTW88Page( 0x04 );
	WriteTW88(PAGE4_SOSD|index, (BYTE)(start>>16));
	index++;
	WriteTW88(PAGE4_SOSD|index, (BYTE)(start>>8));
	index++;
	WriteTW88(PAGE4_SOSD|index, (BYTE)(start));
}


//win0 win1       win2 
//N/A  0x44A[7:6] 0x45A[7:6].,,,
void SpiOsdWinImageLocBit(BYTE winno, BYTE start)
{
	 BYTE  *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	data_p += SPI_OSDWIN_BUFFERSTART_BIT;
	*data_p &= 0x3F;
	*data_p |= (start << 6);
}
#endif

//WINBUFF
void SpiOsdWinImageSizeW(BYTE winno, WORD w)
{
	 BYTE  *data_p;
	BYTE value;

	data_p = &SpiWinBuff[(winno+1) << 4];	//No WIN0
	data_p += SPI_OSDWIN_DISPSIZE;

 	value = *data_p & 0xC0;

	*data_p++ = (BYTE)(w>>8 | value);		//+0x0A
	*data_p++ = (BYTE)w;					//+0x0B
}

//WINx buff size
void SpiOsdWinImageSizeWH (BYTE winno, WORD w, WORD h)
{
	 BYTE  *data_p;
	BYTE value;

	//WIN1to8 need only Width.
	if(winno) {
		SpiOsdWinImageSizeW(winno,w);
		return; 
	}

	//now only for WIN0
	data_p = SpiWinBuff;				   //Only WIN0
	data_p += SPI_OSDWIN_DISPSIZE;

	value = (BYTE)(h >> 8);
	value <<= 4;
	value |= (BYTE)( w>>8 );
	*data_p++ = value; 		//42A
	*data_p++ = (BYTE)w;	//42B
	*data_p++ = (BYTE)h;	//42C
}

#if 0
void	SpiOsd_Win0_Buf_Offset(WORD buf_w, WORD buf_h, WORD offset_w, WORD offset_h)
{
	 BYTE  *data_p;
	BYTE value,temp;

	//only for WIN0
	data_p = SpiWinBuff;				   //Only WIN0
	data_p += SPI_OSDWIN_DISPSIZE;

	value = (BYTE)(buf_h >> 8);
	value <<= 4;
	value |= (BYTE)( buf_w>>8 );
	*data_p++ = value; 		//42A
	*data_p++ = (BYTE)buf_w;	//42B
	*data_p++ = (BYTE)buf_h;	//42C

	temp = (BYTE)(offset_h >> 8);
	temp <<= 4;
	temp |= (BYTE)( offset_w>>8 );
	*data_p++ = temp; 			//42D
	*data_p++ = (BYTE)offset_w;	//42E
	*data_p++ = (BYTE)offset_h;	//42F
		
}


void	SpiOsd_Win0_OffsetWH_TW88(WORD offset_w, WORD offset_h)
{
	BYTE	temp;
	
	temp = (BYTE)(offset_h >> 8);
	temp <<= 4;
	temp |= (BYTE)( offset_w>>8 );
	

	WriteTW88Page( 0x04 );
	WriteTW88( PAGE4_SOSD|0x2D, temp );
	WriteTW88( PAGE4_SOSD|0x2E, (BYTE)offset_w );
	WriteTW88( PAGE4_SOSD|0x2F, (BYTE)offset_h );
}


void	SpiOsd_Win0_OffsetW_TW88(WORD offset_w)
{
	BYTE	temp;
	BYTE	r88;

	WriteTW88Page( 0x04 );

	temp = (BYTE)( offset_w>>8 );
	r88 = ReadTW88(PAGE4_SOSD|0x2D) & 0xF8;
	
	WriteTW88( PAGE4_SOSD|0x2D, temp | r88);
	WriteTW88( PAGE4_SOSD|0x2E, (BYTE)offset_w );
	
}

void	SpiOsd_Win0_OffsetH_TW88(WORD offset_h)
{
	BYTE	temp;
	BYTE	r88;

	WriteTW88Page( 0x04 );

	temp = (BYTE)( offset_h>>8 );
	temp <<= 4;
	r88 = ReadTW88(PAGE4_SOSD|0x2D) & 0x8F;
	
	WriteTW88( PAGE4_SOSD|0x2D, temp | r88);
	WriteTW88( PAGE4_SOSD|0x2F, (BYTE)offset_h );
}

#endif


//WINx Screen(win) Pos & Size
void SpiOsdWinScreen(BYTE winno, WORD x, WORD y, WORD w, WORD h)
{
	 BYTE  *data_p;
	BYTE value;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	data_p += SPI_OSDWIN_SCREEN;
	value = (y >> 8);
	value <<= 4;
	value |= (x >> 8);
	*data_p++ = value;		//421	441...
	*data_p++ = (BYTE)x;	//422	442... 	
	*data_p++ = (BYTE)y;	//423	443...
	
	value = (h >> 8);
	value <<= 4;
	value |= (w >> 8);
	*data_p++ = value;		//424	444...
	*data_p++ = (BYTE)w;	//425	445...	 	
	*data_p++ = (BYTE)h;	//426	446...	 
}

#if 0
void	SpiOsdWinScreen_XW_TW88(BYTE winno, WORD x, WORD w)
{
	BYTE index,dat;

	WriteTW88Page( 0x04 );
	//x
	index = SpiOsdWinBase[winno] + SPI_OSDWIN_SCREEN;
	dat = ReadTW88(PAGE4_SOSD|index) & 0xF8;
	WriteTW88(PAGE4_SOSD|index, dat| (x >> 8));
	index++;
	WriteTW88(PAGE4_SOSD|index,  (BYTE)x);
	//w
	index += 2;
	dat = ReadTW88(PAGE4_SOSD|index) & 0xF0;
	WriteTW88(PAGE4_SOSD|index, dat| (w >> 8));
	index++;
	WriteTW88(PAGE4_SOSD|index,  (BYTE)w);	
}
#endif
//=============================================================================
//		Load LUT
//=============================================================================
//LUT offset use 5bit & 16 unit
void SpiOsdWinLutOffset( BYTE winno, WORD table_offset )
{
	 BYTE  *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];
	data_p += SPI_OSDWIN_LUT_PTR;
	if(!winno) data_p += 4;
	
	//LUT offset use 5bit & 16 unit
	*data_p = table_offset >> 4;
}

#if 0
void	OsdWinLutOffset_TW88( BYTE winno, WORD offset )
{
		BYTE index;
		BYTE dat;

	if(winno==0)
	index = SpiOsdWinBase[winno] + 0x11;	
	else
	index = SpiOsdWinBase[winno] + SPI_OSDWIN_LUT_PTR;
	
	dat = (BYTE)(offset >> 4) ;
	WriteTW88Page( 0x04 );

	WriteTW88(PAGE4_SOSD|index, dat);
}
#endif
	
//=============================================================================
//		Pixel Width
//=============================================================================
//bpp
//	0:4bit, 1:6bit others:8bit
//
void SpiOsdWinPixelWidth(BYTE winno, BYTE bpp)
{
	 BYTE  *data_p;
	BYTE mode;

	if(bpp==4)	mode=0;
	else if(bpp==6) mode=1;
	else mode=2;	//7 and 8 use mode 2

	if(winno) 	winno++;
	data_p = &SpiWinBuff[winno << 4];

	*data_p &= 0x3f;
	*data_p |= (mode <<6);
}
//=============================================================================
//		SpiOsdWinFillColor( BYTE winno, BYTE color )
//=============================================================================
//color will be an offset from the LUT location that Window have. 
//If window start LUT from 80, the color value means color+80 indexed color.
void	SpiOsdWinFillColor( BYTE winno, BYTE color )
{
	BYTE	index;

	index = SpiOsdWinBase[winno];
	WriteTW88Page( 0x04 );

	if ( color ) {
		WriteTW88( PAGE4_SOSD|index, (ReadTW88( PAGE4_SOSD|index ) | 0x04));				// en Alpha & Global
	}
	else {
		WriteTW88( PAGE4_SOSD|index, ( ReadTW88( PAGE4_SOSD|index ) & 0xFB ) );				// dis Alpha & Global
	}
	index = SpiOsdWinBase[winno] + SPI_OSDWIN_FILLCOLOR;
	if(!winno)	index += 8;
	WriteTW88( PAGE4_SOSD|index, color );
}

#if 0
//=============================================================================
//		SpiOsdWinGlobalAlpha( BYTE winno, BYTE alpha )
//=============================================================================
//alpha: 0 to 7F
void	SpiOsdWinGlobalAlpha( BYTE winno, BYTE alpha )
{
	 BYTE  *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	*data_p &= 0xCF;
	if(alpha) *data_p |= 0x10;

	data_p += SPI_OSDWIN_ALPHA;
	if(!winno)	data_p += 4;
	*data_p = alpha;
}

void	SpiOsdWinGlobalAlpha_TW88( BYTE winno, BYTE alpha )
{
	BYTE index,dat;
	
	WriteTW88Page( 0x04 );
	
	index = SpiOsdWinBase[winno];
	dat = ReadTW88( PAGE4_SOSD|index )&0xcf;
	
	WriteTW88(PAGE4_SOSD|index, dat|0x10);
		
	if(winno==0)
		index = SpiOsdWinBase[winno] + 0x10;
	else
		index = SpiOsdWinBase[winno] + SPI_OSDWIN_ALPHA;

	WriteTW88(PAGE4_SOSD|index, alpha);

}
#endif
//=============================================================================
//		SpiOsdWinGlobalAlpha( BYTE winno, BYTE alpha )
//=============================================================================
void	SpiOsdWinPixelAlpha( BYTE winno, BYTE alpha )
{
	 BYTE  *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	if(alpha)	*data_p |= 0x30;
	else		*data_p &= 0xCF;

	data_p += SPI_OSDWIN_ALPHA;
	if(!winno)	data_p += 4;
	*data_p = alpha;
}

#if 0
void	SpiOsdWinPixelAlpha_TW88( BYTE winno, BYTE alpha )
{
	BYTE index,dat;

	WriteTW88Page( 0x04 );

	index = SpiOsdWinBase[winno];
	if(alpha)		{	dat = ReadTW88(PAGE4_SOSD|index)| 0x30;	}
	else		 	{	dat = ReadTW88(PAGE4_SOSD|index)& 0xCF;	}

	WriteTW88(PAGE4_SOSD|index, dat);
	
	if(winno==0)
		index = SpiOsdWinBase[winno] + 0x10;
	else
		index = SpiOsdWinBase[winno] + SPI_OSDWIN_ALPHA;
	
	WriteTW88(PAGE4_SOSD|index, alpha);
}
#endif

//=============================================================================
//		Animation
//=============================================================================
//parameter
//	mode	
//		0:display one time of the loop and then disappear
//		1:display one time of the loop and then stay at the last frame
//		2:Enable looping 
//		3:static. Show the frame pointed by (0x431 and 0x432)
//	Duration
//		duration time of each frame (in unit of VSync)
//		0: infinite
//		1: One VSync period
//		max 0xFF: 255 VSync period		
//
void SpiOsdWin0Animation(BYTE mode, BYTE FrameH, BYTE FrameV, BYTE Duration)
{
	 BYTE  *data_p;

	data_p = SpiWinBuff;	  			//Only WIN0
	data_p += SPI_OSDWIN_ANIMATION;

	*data_p++ = FrameH;
	*data_p++ = FrameV;
	*data_p++ = Duration;

	*data_p &= 0x3f;
	*data_p |= (mode << 6);
}

//WINx buff offset
void SpiOsdWin0ImageOffsetXY (WORD x, WORD y)
{
	BYTE value;
	 BYTE  *data_p;

	data_p = SpiWinBuff;			//Only WIN0
	data_p += SPI_OSDWIN_DISPOFFSET;

	value  = (BYTE)(y >> 8);
	value <<=4;
	value |= (BYTE)(x >> 8);
	*data_p++ = value;
	*data_p++ = (BYTE)x;
	*data_p++ = (BYTE)y;
}


BYTE temp_SPI_Buffer[64];	//
//without DMA.
//LutOffset: 0~511(0x00~0x1FF)
//parameter
//	type:	
//		1:Byte pointer - LUTS type
//		0:Address pointer - LUT type

void SpiOsdIoReadLUT(BYTE winno,BYTE type, WORD LutOffset, WORD size, DWORD spiaddr)
{
		//DWORD spiaddr;
		WORD lut_offset;
		BYTE r410;
		volatile BYTE rdata0, rdata1;
		//BYTE errno;
		WORD nRead;
		BYTE i;
//		int j = 0;
		BYTE bgra;
	
		Printf("\n\rSpiOsdCheckLut(%d, %d, %d, %d) \n", winno, type, LutOffset, size);
		//spiaddr = address;
		lut_offset = LutOffset;
		WriteTW88(0xff, 0x04);
	
		//errno = 0;
		while(size) {
			nRead = 64; //min. color is 16.
		
	
			for(i=0; i < (nRead >> 2); i++) {
				r410 = SOSD_LUT_WEN;
				if(winno==1 || winno==2)	  r410 |= SOSD_LUT_BGRP;
				if(lut_offset >> 8) 	r410 |= SOSD_LUT_HIGH256;
	
				for(bgra=0; bgra < 4; bgra++) {
					r410 &= 0xFC;
					r410 |= bgra;
					WriteTW88(REG410, r410);
					WriteTW88(REG411, (BYTE)lut_offset);
	
					rdata0 = ReadTW88(REG412);
					rdata1 = ReadTW88(REG412);	//read twice
	
					Printf(" %02x  \t",rdata1);
					if((i*4+bgra) < 0xff)
					temp_data[i*4+bgra] = rdata1;

				}
				lut_offset++;
				Printf("\n");
			}
			if(size >= nRead)	size -= nRead;
			else				size = 0;
	
		//	spiaddr += nRead;
		}
	
	   // return errno;
		
}


void SpiOsdIoLoadLUT(BYTE winno,BYTE type, WORD LutOffset, WORD size, DWORD spiaddr)
{

	BYTE i,j,k;
	BYTE R410_data;
	//dPrintf("\nSpiOsdIoLoadLUT%s LutLoc:%d size:%d 0x%06lx", type ? "S":" ", LutOffset, size, spiaddr);

	WriteTW88Page( 0x04 );

	//--- SPI-OSD config
	if(type==0)	R410_data = 0xC0;			// LUT Write Mode, En & address ptr inc.
	else		R410_data = 0xA0;			// LUT Write Mode, En & byte ptr inc.
	if(LutOffset >> 8)
		R410_data |= 0x04;
	
	if(type==0) {
		//
		//ignore size. it is always 0x400.(256*4)
		//		
		for(i=0; i < 4; i++) {	 
			WriteTW88(PAGE4_SOSD|0x10, R410_data | i );	//assign byte ptr	
			WriteTW88(PAGE4_SOSD|0x11, (BYTE)LutOffset);	//reset address ptr.
			for(j=0; j<(256/64);j++) {
				SPI_ReadData2xdata( spiaddr + i*256 + j*64, temp_SPI_Buffer, 64 );
				for(k=0; k < 64; k++) {
					WriteTW88(PAGE4_SOSD|0x12, temp_SPI_Buffer[k]);		//write data
				}
			}
		}
	}
	else {
		WriteTW88(PAGE4_SOSD|0x10, R410_data);			//assign byte ptr. always start from 0.
		WriteTW88(PAGE4_SOSD|0x11, (BYTE)LutOffset);	//reset address ptr.

		for(i=0; i < (size / 64); i++ ) {	//min size is a 64(16*4)
			SPI_ReadData2xdata( spiaddr + i*64, temp_SPI_Buffer, 64 );
			for(k=0; k < 64; k++) {
				WriteTW88(PAGE4_SOSD|0x12, temp_SPI_Buffer[k]);		//write data
			}
		}
	}
}



//parameter
//	type:	
//		1:Byte pointer - LUTS type
//		0:Address pointer - LUT type
//			if 0, use LutOffset:0, size:0x400
//NOTE BK110330:after we support 512 palette, we donot support the address method.
// We need a width and a height info. but RTL only supports a size info.
// So, if you want to use the address method, use a PIO method.
//NOTE Do not add WaitVBlank() here


#if 0  										//For 8835
void SpiOsdLoadLUT(BYTE type, WORD LutOffset, WORD size, DWORD address)
{
	volatile BYTE B0,B;
	//BYTE win_lut_debug;
	BYTE reg;

	if(type==0) {
		//SpiOsdIoLoadLUT(type,LutOffset,size,address);
		//dPrintf("\nSpiOsdLoadLUT convert LutOffset:%d->0, LutSize:0x%03x->0x400",LutOffset,size);
		SpiOsdIoLoadLUT(type,0,0x400,address);
		return;
	}	
	//dPrintf("\nSpiOsdLoadLUT%s LutLoc:0x%x size:%d 0x%06lx", type ? "S":" ", LutOffset, size, address);


	//save clock mode & select PCLK	
	McuSpiClkToPclk(0x02);	//with divider 1=1.5(72MHz)	try 2

	//win_lut_debug = 0;
	if(LutOffset) {
		WriteTW88Page( 0x04 );
		WriteTW88(PAGE4_SOSD|0x10,0x80/*0xa0*/);	//read B
		WriteTW88(PAGE4_SOSD|0x11,0);		//addr 0
		B0=ReadTW88(PAGE4_SOSD|0x12);		//read twice
		B0=ReadTW88(PAGE4_SOSD|0x12);		
		//win_lut_debug = 1;
	}

//EA = 0;
//P1_3 =1;
//P1_4 = !P1_4;
	WriteTW88Page( 0x04 );

	//--- SPI-OSD config
	reg = 0;	//ReadTW88(0x10) ;
	if(LutOffset & 0x100) reg = 0x08;
	if(type==0)	WriteTW88( PAGE4_SOSD|0x10, reg | 0xc0 );    		// LUT Write Mode, En & address ptr inc.
	else		WriteTW88( PAGE4_SOSD|0x10, reg | 0xa0 );    		// LUT Write Mode, En & byte ptr inc.
	WriteTW88( PAGE4_SOSD|0x11, (BYTE)LutOffset ); 			// LUT addr. set 0 on DMA


	//Spi Flash DMA
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x04, 0x00 );	// DMA stop	
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x03, 0x80 | SPICMD_BYTES ); //LUT,Increase, 0x0B with 5 commands, 0xeb with 7 commands
	           
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x0a, SPICMD_READ ); 			// SPI Command=R
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x0b, (BYTE)(address>>16) ); 	// SPI Addr
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x0c, (BYTE)(address>>8) );
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x0d, (BYTE)(address) ); 		//////00

	//d		h		addr	 addr  byte
	//0    0x00     0x000		0	0 	
	//128  0x80		0x200	   80   0
	//192  0xC0		0x300	   c0   0
	if(type==0) {
		//if use addrss ptr inc.
		//addr_ptr = LutOffset;
		//byte_ptr  0;
		WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x06, (BYTE)(LutOffset >> 8));	//LutOffset[8]  ->R4C6[0]
		WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x07, (BYTE)LutOffset);			//LutOffset[7:0]->R4C7[7:0]
	}
	else {
		//if use byte ptr inc.
		WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x06, (BYTE)(LutOffset >> 6) ); 	//LutOffset[8:6] -> R4C6[2:0]
		WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x07, (BYTE)(LutOffset << 2) );	//LutOffset[5:0] -> R4C7[7:2] 
		                                                        //R4C7[1:0]  start of byteptr
	}
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x1a, 0x00 ); // DMA size
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x08, (BYTE)(size >> 8) );
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x09, (BYTE)size );
	
	WriteTW88( PAGE4_SOSD|TWBASE_SPI+0x04, 0x01 ); // DMA Start
//P1_3 =0;
//P1_4 = !P1_4;
//	while(ReadTW88(0xff) != PAGE4_SPI);			//trick. check DONE. BusyWait
//EA = 1;

	if(LutOffset) {
		WriteTW88Page( 0x04 );
		WriteTW88(PAGE4_SOSD|0x10,0x80/*0xa0*/);	//read B
		WriteTW88(PAGE4_SOSD|0x11,0);		//addr 0
		B=ReadTW88(PAGE4_SOSD|0x12);		//read twice
		B=ReadTW88(PAGE4_SOSD|0x12);		
		if(B0 != B) {
			WriteTW88(PAGE4_SOSD|0x11, 0);			//addr 0
//BK110809			WriteTW88(0x12, B0);	//overwrite
			ePrintf("\n***BUGBUG*** B0 %bx->%bx",B, B0); //--pls, use without EA
		}
	}
		//restore clock mode
	McuSpiClkRestore();
}
#else					 	//For 8836

void SpiOsdLoadLUT(BYTE _winno, BYTE type, WORD LutOffset, WORD size, DWORD address)
{

	BYTE reg;

	BYTE winno;
	winno = _winno;
	if(type==0) {
		SpiOsdIoLoadLUT(_winno,type,0,0x400,address);
		return;
	}	

	//save clock mode & select PCLK	
	McuSpiClkToPclk(CLKPLL_DIV_3P0);	//with divider 1=2(54MHz)	try 3

	//WriteTW88Page(0x04);

	//--- SPI-OSD config
	reg = 0;	//ReadTW88(REG410);
	if(LutOffset & 0x100) reg = 0x08;
	reg |= 0x80;									// LUT Write Mode.
	if(type==0)	reg |= 0x40;						// address ptr inc						
	else		reg |= 0x20;						// byte ptr inc.

	if(winno == 1 || winno == 2)					// if win1 or win2, 
		reg |= 0x04;								//	select group B LUT

	if(LutOffset > 0xff)   							
		reg |= 0x08;								
	WriteTW88(REG410, reg);
	WriteTW88(REG411, (BYTE)LutOffset ); 			// LUT addr. set 0 on DMA

	//Spi Flash DMA
	WriteTW88(REG4C0_SPIBASE+0x04, 0x00 );	// DMA stop	

	WriteTW88(REG4C0_SPIBASE+0x03, 0x80 | SPICMD_BYTES + SpiFlash4ByteAddr); //LUT,Increase, 0x0B with 5 commands, 0xeb with 7 commands	           
	WriteTW88(REG4C0_SPIBASE+0x0a, SPICMD_READ ); 			// SPI Command=R

	if(SpiFlash4ByteAddr) {
		WriteTW88(REG4C0_SPIBASE+0x0b, (BYTE)(address>>24) );
		WriteTW88(REG4C0_SPIBASE+0x0c, (BYTE)(address>>16) );
		WriteTW88(REG4C0_SPIBASE+0x0d, (BYTE)(address>>8) );
		WriteTW88(REG4C0_SPIBASE+0x0e, (BYTE)(address) );
	}
	else {
		WriteTW88(REG4C0_SPIBASE+0x0b, (BYTE)(address>>16) ); 	// SPI Addr
		WriteTW88(REG4C0_SPIBASE+0x0c, (BYTE)(address>>8) );
		WriteTW88(REG4C0_SPIBASE+0x0d, (BYTE)(address) ); 		//////00
	}
	//d		h		addr	 addr  byte
	//0    0x00     0x000		0	0 	
	//128  0x80		0x200	   80   0
	//192  0xC0		0x300	   c0   0
//	if(type==0) {
//		//if use addrss ptr inc.
//		//addr_ptr = LutOffset;
//		//byte_ptr  0;
//		WriteTW88(REG4C0_SPIBASE+0x06, (BYTE)(LutOffset >> 8));	//LutOffset[8]  ->R4C6[0]
//		WriteTW88(REG4C0_SPIBASE+0x07, (BYTE)LutOffset);			//LutOffset[7:0]->R4C7[7:0]
//		
//	}
//	else {
		//if use byte ptr inc.
		WriteTW88(REG4C0_SPIBASE+0x06, (BYTE)(LutOffset >> 6) ); 	//LutOffset[8:6] -> R4C6[2:0]
		WriteTW88(REG4C0_SPIBASE+0x07, (BYTE)(LutOffset << 2) );	//LutOffset[5:0] -> R4C7[7:2] 
		                                                        	//					R4C7[1:0]  start of byteptr
//	}

	WriteTW88(REG4C0_SPIBASE+0x1a, 0x00 ); // DMA size
	WriteTW88(REG4C0_SPIBASE+0x08, (BYTE)(size >> 8) );
	WriteTW88(REG4C0_SPIBASE+0x09, (BYTE)size );

	if(winno==1 || winno==2) {
		WriteTW88(REG410, ReadTW88(REG410) | 0x04);	//indicate GROUP_B palette table. MAX size 256.
	}

	WriteTW88(REG4C0_SPIBASE+0x04, 0x01 ); // DMA Start
//P1_3 =0;
//P1_4 = !P1_4;
//	while(ReadTW88Page() != PAGE4_SPI);			//trick. check DONE. BusyWait
//EA = 1;

	//restore clock mode
	McuSpiClkRestore();
}

#endif


void SpiOsdPixelAlphaAttr(BYTE win, WORD lutloc, BYTE value)
{
	//BYTE val;
	WriteTW88Page(0X04 );

	//--- SPI-OSD config
	//WriteTW88(REG410, 0xc0 );    		// LUT Write Mode, En & address ptr inc.
	//WriteTW88(REG410, 0xa0 );    		// LUT Write Mode, En & byte ptr inc.

	if(win==1 || win==2) {
		WriteTW88(REG410, 0xc3 | 0x04);    		// LUT Write Mode, En & byte ptr inc.
		WriteTW88(REG410, ReadTW88(REG410) & 0xF7);
		WriteTW88(REG411, (BYTE)lutloc ); // LUT addr
		WriteTW88(REG412, value ); // LUT addr
	}
	else {
		WriteTW88(REG410, 0xc3 );    		// LUT Write Mode, En & byte ptr inc.
		if(lutloc >> 8)	WriteTW88(REG410, ReadTW88(REG410) | 0x08);	//support 512 palette
		else            WriteTW88(REG410, ReadTW88(REG410) & 0xF7);
		WriteTW88(REG411, (BYTE)lutloc ); // LUT addr
		WriteTW88(REG412, value ); // LUT addr
	}

	delay1ms(1);					//Printf("\n    Lut Index = %bx",(BYTE)lutloc);
	//val = ReadTW88(REG411);		Printf("\n    Lut Index = %bd",val);
	//val = ReadTW88(REG412);		Printf("      Lut Alpha = %bd",val);

//	WriteTW88(REG411, 0 );    			// LUT addr
//	WriteTW88(REG4C0_SPIBASE+0x04, 0x01 ); // DMA Start
}



//-----------------------------------
// RLE functions
//
//-----------------------------------

//desc
//parameter
//	winno: win number.
//		winno 0 means disable.
//	dcnt:Data BPP
//		4:4bit, 6:6bit, others:8bit
//	ccnt: counter value.
//		4:4bit,5:5bit,..15:16bit, others:16bit
void SpiOsdRLC(BYTE winno,BYTE dcnt, BYTE ccnt)
{
	BYTE val;
	val = ( winno<<4 );
	//7 means 8BPP with 128 color.
	if(dcnt==7)
		dcnt++;

	WriteTW88Page( 0x04 );
	//WriteTW88(PAGE4_SOSD|0x05, (dcnt << 4) | (ccnt));
	//WriteTW88(PAGE4_SOSD|0x06, winno);
	
	if( winno == 1 || winno == 2 )				//Group B
	{
		WriteTW88(PAGE4_SOSD|0x07, (dcnt << 4) | (ccnt));
		WriteTW88(PAGE4_SOSD|0x06, val); 		
	}
	else if( winno >= 3 && winno <= 8 )			//Group A
	{	
		WriteTW88(PAGE4_SOSD|0x05, (dcnt << 4) | (ccnt));	
		WriteTW88(PAGE4_SOSD|0x04, val); 	
	}
}

#if 0
void SpiOsdResetRLC(BYTE reset)
{
	WriteTW88Page( 0x04 );
	if(reset)	WriteTW88(PAGE4_SOSD|0x04, ReadTW88(PAGE4_SOSD|0x04) | 0x02);
	else		WriteTW88(PAGE4_SOSD|0x04, ReadTW88(PAGE4_SOSD|0x04) & 0xFD);
}

void SpiOsdEnableRLC(BYTE en)
{
	WriteTW88Page( 0x04 );
	if(en)		WriteTW88(PAGE4_SOSD|0x04, ReadTW88(PAGE4_SOSD|0x04) | 0x01);
	else		WriteTW88(PAGE4_SOSD|0x04, ReadTW88(PAGE4_SOSD|0x04) & 0xFE);
}
#endif


//Zoom TW8836 only
void SpiOsdWinHZoom(BYTE winno, BYTE en)
{
	 WORD index;
		BYTE dat;

	index = SpiOsdWinBase[winno] + SPI_OSDWIN_HZOOM;

	WriteTW88Page(0X04 );
	dat = ReadTW88(PAGE4_SOSD|index);
	if( en ) 	WriteTW88(PAGE4_SOSD|index, dat | 0x02);
	else     	WriteTW88(PAGE4_SOSD|index, dat & ~0x02);

}


void	OSDWinGlobalAlpha( BYTE winno, BYTE alpha )
{
	BYTE	index,val;

	WriteTW88Page( 0x04 );
	index = SpiOsdWinBase[winno];

	val =  ReadTW88(PAGE4_SOSD | index);
	val &= 0xCF;	
	if(alpha)
	{
		val |= 0x10;
		WriteTW88( PAGE4_SOSD | index, val );
	}	

	index = SpiOsdWinBase[winno] + SPI_OSDWIN_ALPHA;
	if(!winno)	index += 8;
	WriteTW88( PAGE4_SOSD | index, alpha );
}

void	OSDMixOrder( BYTE font_first )				 //0 = spi first   1 = font first
{
	BYTE val;
	
	WriteTW88Page( 0x04 );
	val =  ReadTW88( PAGE4_SOSD | 0x00 );
	if( font_first == 0 )
		WriteTW88( PAGE4_SOSD | 0x00, val & 0xfd  );
	else
		WriteTW88( PAGE4_SOSD | 0x00, val | 0x02  );				
}

#if 0
void	SpiOsdPixelAlpha( BYTE winno, BYTE coloridx, BYTE alpha ,BYTE lut_offset)			//TW8835 Version
{
	
	if( lut_offset )
		WriteTW88( PAGE4_SOSD | 0x10, 0x88 );    	/*// LUT Write Mode, En*/
	else
		WriteTW88( PAGE4_SOSD | 0x10, 0x80 );    	/*// LUT Write Mode, En*/
	WriteTW88( PAGE4_SOSD | 0x11, coloridx );    /*// LUT Write Addr*/
	
	if( lut_offset )
		WriteTW88( PAGE4_SOSD | 0x10, 0x8B );    	
	else
		WriteTW88( PAGE4_SOSD | 0x10, 0x83 ); 
	WriteTW88( PAGE4_SOSD | 0x12, alpha & 0x7f );/*// LUT Write Attr*/
	  	
	if( lut_offset )				/*// LUT Write Mode, Dis*/
		WriteTW88( PAGE4_SOSD | 0x10, 0x08 );    	
	else
		WriteTW88( PAGE4_SOSD | 0x10, 0x00 ); 
	
	if ( alpha & 0x7f ) {
		WriteTW88( PAGE4_SOSD | SpiOsdWinBase[winno], ( ReadTW88( PAGE4_SOSD | SpiOsdWinBase[winno] ) & 0xCF ) | 0x30 );		/*// en Alpha & Pixel*/
	}else {
		WriteTW88( PAGE4_SOSD | SpiOsdWinBase[winno], ( ReadTW88( PAGE4_SOSD | SpiOsdWinBase[winno] ) & 0xEF ) );				/*// dis Alpha*/
	}

}
#endif

void	SpiOsdPixelAlpha( BYTE winno, BYTE coloridx, BYTE alpha ,BYTE lut_offset)			  //TW8836 Version
{
	WriteTW88( 0xFF, 0x04 );    								 	  // LUT Write Mode, En
	WriteTW88( PAGE4_SOSD | 0x10, 0x80 );    								 	  // LUT Write Mode, En
	
	if( winno == 1 || winno == 2 )		//Group B								  //Lut Group A/B
		WriteTW88( PAGE4_SOSD | 0x10, ReadTW88(PAGE4_SOSD | 0x10) | 0x04 );		  //Reg[410] bit2  Set 1 
	else							    //Group A
		WriteTW88( PAGE4_SOSD | 0x10, ReadTW88(PAGE4_SOSD | 0x10) & 0xFB );		  //Reg[410] bit2  Clr 0

	if( lut_offset )															  //Lut Offset 0/256
		WriteTW88( PAGE4_SOSD | 0x10, ReadTW88(PAGE4_SOSD | 0x10) | 0x08 );		  //Reg[410] bit3  Set 1     	
	else
		WriteTW88( PAGE4_SOSD | 0x10, ReadTW88(PAGE4_SOSD | 0x10) & 0xF7 );		  //Reg[410] bit3  Clr 0
		
	WriteTW88( PAGE4_SOSD | 0x10, ReadTW88(PAGE4_SOSD | 0x10) | 0x03 );			  //Set Color Mode Attr
		    	
	WriteTW88( PAGE4_SOSD | 0x11, coloridx );    								  // LUT Write Addr
/*	
	if( lut_offset )
		WriteTW88( PAGE4_SOSD | 0x10, 0x8B );    	
	else
		WriteTW88( PAGE4_SOSD | 0x10, 0x83 ); 
*/
	WriteTW88( PAGE4_SOSD | 0x12, alpha & 0x7f );								  // LUT Write Attr

#if 0	  	
	if( lut_offset )				/*// LUT Write Mode, Dis*/
		WriteTW88( PAGE4_SOSD | 0x10, 0x08 );    	
	else
		WriteTW88( PAGE4_SOSD | 0x10, 0x00 );
#endif

	WriteTW88( PAGE4_SOSD | 0x10, ReadTW88(PAGE4_SOSD | 0x10) & 0xEF );			// LUT Write Mode, Dis		 
	
	if ( alpha & 0x7f ) {
		WriteTW88( PAGE4_SOSD | SpiOsdWinBase[winno], ( ReadTW88( PAGE4_SOSD | SpiOsdWinBase[winno] ) & 0xCF ) | 0x30 );		/*// en Alpha & Pixel*/
	}else {
		WriteTW88( PAGE4_SOSD | SpiOsdWinBase[winno], ( ReadTW88( PAGE4_SOSD | SpiOsdWinBase[winno] ) & 0xEF ) );				/*// dis Alpha*/
	}

}








