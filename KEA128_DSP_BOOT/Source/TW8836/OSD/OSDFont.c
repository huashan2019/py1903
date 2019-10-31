/*
 *  OSDFont.c - 
 *
 *  Copyright (C) 2011 Intersil Corporation
 *
 */
//*****************************************************************************
//
//								OSDFont.c
//
//*****************************************************************************
//
//
#include "include.h"

//set font info
struct FontInfo_s {
	char name[16];	//consolas
	BYTE w,h;
	BYTE bpp;
	BYTE loc;		//fontram location

	BYTE bppm,bpp2,bpp3,bpp4;	//start loc. bppm[3]:bpp4[8] bppm[2]:bpp3[8] bppm[1]:bpp2[8]  

	BYTE bpp2_attr;		//for MultiBPP	(LUT index >> 2)
	BYTE bpp3_attr;		//for MultiBPP	(LUT index >> 2)
	BYTE bpp4_attr;		//for MultiBPP	(LUT index >> 2)
};

struct FontWinInfo_s {
	WORD osdram;		//osdram start offset, 0~511.
	WORD sx,sy;			//pixel base
	BYTE column,line;  	//char base
	BYTE x,y;			//last position. char base.
	//BYTE zoom;		//[3:2] Horizontal, [1:0] Vertical
	//BYTE alpha_color; 	//for 1bpp
	//WORD fgcolor,bgcolor;	//for 1bpp


	BYTE bpp1_attr;		//(bgColor << 4) | fgColor
};


struct FontOsdInfo_s {
	struct FontInfo_s font;
	struct FontWinInfo_s win[4];
} FontOsdInfo;


//R307[7:0]R308[7:0]	OSD RAM Data Port
//R309					Font RAM Address
//R30A					Font RAM Data Port
//R30B					Multi-Color Start Position
//R30C					Font OSD Control
//R30D[7:0]R30E[7:0]	Character Color Look-up table data port

#define	FONTWIN1_ST		0x10
#define	FONTWIN2_ST		0x20
#define	FONTWIN3_ST		0x30
#define	FONTWIN4_ST		0x40
#define	FONTWIN5_ST		0x50
#define	FONTWIN6_ST		0x60
#define	FONTWIN7_ST		0x70
#define	FONTWIN8_ST		0x80

#define	FONTWIN_ENABLE	0X00
#define	FONT_ALPHA		0x01

	BYTE	FontOsdWinBase[] = { FONTWIN1_ST, FONTWIN2_ST, FONTWIN3_ST, FONTWIN4_ST, FONTWIN5_ST, FONTWIN6_ST, FONTWIN7_ST, FONTWIN8_ST};

//========================================================================================
// Initialize Functions
//========================================================================================
/**
* set FOSD OsdRam FIFO
*
* OsdRam uses a PCLK domain.
*
* If FIFO is ONed, HW has a 8 FIFO, and updata OsdRam on HBlank(Horizontal).
* If FW wants to update the OsdRam related registers more than 8,
* FW has to wait one HBlank.
*
* If FIFO is OFFed, FW has to execute the updata on VBlank(Vertical).
* If FW turns on FIFO before HW updates a OsdRam related registers,
* HW can write data|attr at the ramdom position.
* So, FW has to wait 1 VBlank.
*
* oldname:FOsdRamFifoBypass
* 
* HW default: FIFO ON
* SW prefer: FIFO OFF
*
*	register
*	r300[0]	1:Bypass OSD RAM FIFO.
*/
//bypass OSD RAM FIFO
void FOsdOsdRamFifoBypass(BYTE fOn)
{
	WriteTW88Page( PAGE3_FOSD );
	if(fOn)	WriteTW88(PAGE3_FOSD|0x00, ReadTW88(PAGE3_FOSD|0x00) | 0x01);
	//DO NOT TURN OFF OsdRamFifoBypass. BUGLIST110801
	//else	WriteTW88(PAGE3_FOSD|0x00, ReadTW88(PAGE3_FOSD|0x00) & ~0x01);
}

//------------------------------------------------------------------------
//		void FontOsdWinEnable(BYTE winno, BYTE en)
//------------------------------------------------------------------------
/*	Font OSD DE delay calculation 
HDE = REG(0x210[7:0])
PCLKO = REG(0x20d[1:0]) {0,1,1,1}
Mixing = REG(0x400[1:1])

result = HDE + PCLKO - (Mixing*2 + 36)*/ 

/**
* calculate and set FontOSD DE value
*
* Font OSD DE delay calculation 
* HDE = REG(0x210[7:0])
* PCLKO = REG(0x20d[1:0]) {0,1,1,1}
* Mixing = REG(0x400[1:1])
*	
* result = HDE + PCLKO - (Mixing*2 + 36)
*
* NOTE: minimum DE value.
*	- 1BPP: 3
*	- 2BPP: 4
*	- 3BPP: 5
*	- 4BPP: 6
*
*	r303[7:0]
*/ 

void FontOsdSetDeValue(void)
{
	 BYTE temp;
	BYTE HDE,PCLKO,Mixing;

	WriteTW88Page( PAGE2_SCALER );
	HDE = ReadTW88( PAGE2_SCALER|0x10 );				// HDE
	PCLKO = ReadTW88(PAGE2_SCALER|0x0d) & 0x03;
	//if(PCLKO == 3)
		PCLKO = 1;

	WriteTW88Page( PAGE4_SOSD );
	Mixing = (ReadTW88(PAGE4_SOSD|0x00) & 0x02) ? 1 : 0;

	if((HDE + PCLKO) < (Mixing*2 + 36) ) { //I don't want a negative value.
		//temp = 0;								//mnm
		temp = (Mixing*2 + 36) - HDE - PCLKO + 1;
	}
	else
		temp = HDE + PCLKO - (Mixing*2 + 36);

	WriteTW88Page( PAGE3_FOSD );
	WriteTW88( PAGE3_FOSD|0x03, temp );   				// write FONT OSD DE value
	//dPrintf("\nFontOsdDE:%02bx",temp);

	//Printf_Ian("\nFontOsdDE:%02bx",temp);

}

//=========================================================================
// Windos Functions
//=========================================================================
//=======================================================
//
//=======================================================
void FontOsdWinEnable(BYTE winno, BYTE en)
{
	 	BYTE index;
		BYTE dat;

	index = FontOsdWinBase[winno] + FONTWIN_ENABLE;

	WriteTW88Page( PAGE3_FOSD );
	dat = ReadTW88(PAGE3_FOSD|index);
	if( en ) WriteTW88(PAGE3_FOSD|index, dat | 0x80);
	else     WriteTW88(PAGE3_FOSD|index, dat & 0x7F);
}
//=========================================================================
//
//=========================================================================
#if 0
void FontOsdWinMulticolor(BYTE winno, BYTE en)
{
	 	BYTE index;
		BYTE dat;

	index = FontOsdWinBase[winno] + FONTWIN_ENABLE;

	WriteTW88Page( PAGE3_FOSD );
	dat = ReadTW88(PAGE3_FOSD|index);
	if( en ) WriteTW88(PAGE3_FOSD|index, dat | 0x40);
	else     WriteTW88(PAGE3_FOSD|index, dat & 0xBF);
}
#endif
	 
//------------------------------------------------------------------------
//		void FontOsdWinAlphaPixel(BYTE winno, BYTE color, BYTE alpha)
//------------------------------------------------------------------------
void FontOsdWinAlphaPixel(BYTE winno, BYTE color, BYTE alpha)
{
	BYTE	index = FontOsdWinBase[winno] + FONT_ALPHA;

	WriteTW88Page( PAGE3_FOSD );
	WriteTW88( PAGE3_FOSD|0x92,  color );	 		// first select color index		 //0x392 
	WriteTW88( PAGE3_FOSD|index, alpha );			// second write alpha value
}


//------------------------------------------------------------------------
//		void FontOsdWinScreen(BYTE winno, WORD x, WORD y, BYTE w, BYTE h, BYTE zoomH, BYTE zoomV)
//------------------------------------------------------------------------

void FontOsdWinScreen(BYTE winno, WORD x, WORD y, BYTE w, BYTE h)
{
		BYTE index, temp;

	index = FontOsdWinBase[winno];

	WriteTW88Page( PAGE3_FOSD );

	temp = x >> 8;
	temp <<= 4;
	temp += ( y >> 8 );
	WriteTW88( PAGE3_FOSD|index+2,  temp );			// upper bit for position x, y
	WriteTW88( PAGE3_FOSD|index+3, x );				// position x
	WriteTW88( PAGE3_FOSD|index+4, y );				// position y
	WriteTW88( PAGE3_FOSD|index+5, h );
	WriteTW88( PAGE3_FOSD|index+6, w );

}

#if 0
void FontOsdWinScreenWH(BYTE winno, BYTE w, BYTE h)
{
		BYTE index;

	index = FontOsdWinBase[winno];

	WriteTW88Page( PAGE3_FOSD );
	WriteTW88( PAGE3_FOSD|index+5, h );
	WriteTW88( PAGE3_FOSD|index+6, w );

}
#endif

void FontOsdWinZoom(BYTE winno, BYTE zoomH, BYTE zoomV)
{
		BYTE index, temp;

	index = FontOsdWinBase[winno];

	WriteTW88Page( PAGE3_FOSD );

	temp = zoomH*4 + zoomV;
	temp += (ReadTW88( PAGE3_FOSD|index ) & 0xf0);
	WriteTW88( PAGE3_FOSD|index, temp );				// write Zoom value

}

//
//parameter
//	winno	from 0 to 3
//	OsdRamAddr	from 0 to 511
void FontOsdWinInit(BYTE winno, WORD OsdRamAddr)		  		//mnm
{
	BYTE index;

	WriteTW88Page( PAGE3_FOSD );

	FontOsdWinEnable(winno, OFF);
	index = FontOsdWinBase[winno];

	//init all fontosd attributes
	WriteTW88( PAGE3_FOSD|index+0x07, (OsdRamAddr>>8) ? 0x10 : 0x00 );
	WriteTW88( PAGE3_FOSD|index+0x08, 0x00 );
	WriteTW88( PAGE3_FOSD|index+0x09, 0x00 );
	WriteTW88( PAGE3_FOSD|index+0x0A, 0x00 );
	WriteTW88( PAGE3_FOSD|index+0x0B, 0x00 );
	WriteTW88( PAGE3_FOSD|index+0x0C, 0x00 );
	WriteTW88( PAGE3_FOSD|index+0x0D, 0x00 );
	WriteTW88( PAGE3_FOSD|index+0x0E, 0x00 );
	WriteTW88( PAGE3_FOSD|index+0x0F, (BYTE)(OsdRamAddr) );

	//clear alpha
	FontOsdWinAlphaPixel(winno,1,0);		//bgcolor 1 alpha 8
}

#if 0
//add 110124
//R3X7[4],R3XF[7:0], WIN1:R317, WIN2:R327, WIN3:R337 WIN4:R347
void FontOsdWinSetOsdRamStart(BYTE winno, WORD addr)
{
	BYTE index;
	BYTE value;

	WriteTW88Page( PAGE3_FOSD );
	index = FontOsdWinBase[winno];

	value = ReadTW88(PAGE3_FOSD|index+0x07) & 0xEF;
	if(addr >> 8) value |= 0x10;
	WriteTW88(PAGE3_FOSD|index+0x07, value );
	WriteTW88(PAGE3_FOSD|index+0x0F, (BYTE)addr);

}

//==================================================================================
// Palette Functions
//==================================================================================
void FontOsdSetPaletteColorArray(BYTE index, WORD *array, BYTE size)
{
	BYTE i;
	BYTE r30c;


	McuSpiClkToPclk(0x02);	//with divider 1=1.5(72MHz)	try 2

	WriteTW88Page( PAGE3_FOSD );
	r30c = ReadTW88(PAGE3_FOSD|0x0C) & 0xC0;
	dPrintf("\nFontOsdSetPaletteColorArray index:%bd",index);
	for(i=0; i < size; i++) {
		WriteTW88(PAGE3_FOSD|0x0C, (index+i) | r30c); //index 2 is for normal foreground	   Now 6bit.(64).
		WriteTW88(PAGE3_FOSD|0x0D, (BYTE)(array[i] >> 8));
		WriteTW88(PAGE3_FOSD|0x0E, (BYTE)array[i]);
		dPrintf(" 0x%02bx%02bx(%04x)",(BYTE)(array[i] >> 8),(BYTE)array[i],array[i]);
	}
	McuSpiClkRestore();
}
#endif

//=============================================================================
//				   void FontDMA( void )
//=============================================================================
//description
//	download font set
//	the dest font index starts from 0.
//parameter
//	dest: start addr on FontRam.(0~10*1024) ..Need to Verify
//	src_loc: location on SpiFlash.
//	width: 12 or 16
//	height: 2~32
void DownloadFontByDMA(WORD dest_font_index, DWORD src_loc, WORD size, BYTE width, BYTE height)
{
	BYTE value;
	WORD address;

	//save clock mode & select PCLK
	WaitVBlank(1);	
	McuSpiClkToPclk(0x02);	//with divider 1=1.5(72MHz). try 2

	WriteTW88Page( PAGE3_FOSD );
	value = ReadTW88(PAGE3_FOSD|0x00);	
	if(width==16)	value |= 0x10;	   				//width 16
	else			value &= 0xEF;					//   or 12
	value |= 0x02;									//Enable Bypass FontRam FIFO
	WriteTW88( PAGE3_FOSD|0x00, value );  

	WriteTW88( PAGE3_FOSD|0x04, 0x0d ); 						//FontRAM access
	WriteTW88( PAGE3_FOSD|0x09, (BYTE)dest_font_index ); 		//Font Addr
	WriteTW88( PAGE3_FOSD|0x90, height >> 1 ); 				//Font height(2~32)
	WriteTW88( PAGE3_FOSD|0x91, (width >> 2) * (height >> 1));	//sub-font total count.


	WriteTW88Page( PAGE4_SPI );
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x03, 0x00 | SPICMD_BYTES ); //e4	FontRAM,Inc.7Commands		 //remove or call SPI_SetModeByRegister

	address = dest_font_index * (width >> 2) * (height >> 1);
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x06, (BYTE)(address >> 8) ); //03	DMA Page
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x07, (BYTE)address ); //0a	DMA index

	//set SPI command
	//READ
	//with source address
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x0a, SPICMD_READ ); 			//0x0b	
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x0b, (BYTE)(src_loc>>16) ); 	// SPI Addr
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x0c, (BYTE)(src_loc>>8) );
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x0d, (BYTE)(src_loc) ); 		
	// set DMA size        
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x1a, 0x00 );
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x08, (BYTE)(size>>8) );
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x09, (BYTE)size );

	WriteTW88Page( PAGE4_SPI );
	WriteTW88( PAGE4_SPI|TWBASE_SPI+0x04, 0x01); 				//DMA Start
#if ( CHIP_ACCESS_MODE == ACCESS_8BIT )
	while(ReadTW88(PAGE4_SPI|0xff) != PAGE4_SPI);			//trick. check DONE. BusyWait
#endif
	FOsdOsdRamFifoBypass(OFF);

	WriteTW88Page( PAGE3_FOSD );
	//WriteTW88( 0x00, ReadTW88(0x00) & 0xFD ); 		//remove FontRam FIFO Bypass mode
	WriteTW88( PAGE3_FOSD|0x04, ReadTW88(PAGE3_FOSD|0x04) & 0xFE ); 		//OsdRam Access Mode

	//restore clock mode
	McuSpiClkRestore();
} 

void	OSDFont_MultiStart_Set(WORD start_2bpp, WORD start_3bpp, WORD start_4bpp)
{
	BYTE	hight_bit;
	hight_bit = 0;
	if(start_2bpp>0xff)	hight_bit |= 0x02;
	if(start_3bpp>0xff)	hight_bit |= 0x04;
	if(start_4bpp>0xff)	hight_bit |= 0x08;
	
	WriteTW88Page( PAGE3_FOSD );
	WriteTW88( PAGE3_FOSD |0x05, ReadTW88(0x05)|hight_bit);
	WriteTW88( PAGE3_FOSD |0x0b, (BYTE)start_2bpp ); 			// 2bit-multi-font start
	WriteTW88( PAGE3_FOSD |0x93, (BYTE)start_3bpp ); 			// 3bit-multi-font start
	WriteTW88( PAGE3_FOSD |0x94, (BYTE)start_4bpp ); 			// 4bit-multi-font start	
}

void	OSDFont_Init(void)
{	
	DownloadFontByDMA(0x0000, 0x040000, 0x2800, 12, 18);		
	OSDFont_MultiStart_Set(0x1ff, 0x1ff, 0x1ff);
}




