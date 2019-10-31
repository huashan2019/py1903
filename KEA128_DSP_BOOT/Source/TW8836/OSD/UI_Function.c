#include "include.h"
//#include "Uart_Command.h"
extern BYTE 	SpiFlash4ByteAddr;			//4Byte mode for big SPIFLASH	 1 = 32bit adr     0 = 24bit adr	//for TW8836
extern BYTE SPI_Buffer[SPI_BUFFER_SIZE];

//================================OSD=====================================


/*
显示图片，用原厂软件生成的MRLE图片
*/
#if 0
void	Rec_Download_BMP_MRLE(BYTE winno, WORD x, WORD y, DWORD spi_addr, BYTE Offset)
{
	winno=0; x=0; y=0; spi_addr=0xffffff; Offset=0;
#if 0
	struct image_item_info_s *image;	 

extern	void MenuPrepareImageHeader(struct image_item_info_s *image);
extern	image_header_info_t header_table;

	image_header_info_t *header = &header_table;
	image_item_info_t	disp_img	= {1, 0x000000, NULL,	0xff};    // Intersil-Techwell
	WORD	lut_offset;

	disp_img.loc = spi_addr;
	image = &disp_img;

	if(Offset==1)
	lut_offset = 0x100;
	else
	lut_offset = 0x000;

//	SpiOsdSetDeValue();
	//prepare header
	MenuPrepareImageHeader(image);	//update header_table

	SpiOsdWinImageLoc(winno, header->image_loc); 
	SpiOsdWinImageSizeWH( winno, header->dx, header->dy );
	SpiOsdWinScreen( winno,  x,  y, header->dx, header->dy );
	if(winno==0) {
		SpiOsdWin0ImageOffsetXY( 0, 0 );
		SpiOsdWin0Animation( 1, 0, 0, 0);
	}
	SpiOsdWinPixelAlpha( winno, ON );
	//SpiOsdWinGlobalAlpha( winno, 0);							////////
	SpiOsdWinPixelWidth(winno, header->bpp);
	SpiOsdWinLutOffset(winno, lut_offset /* menu_item->osd_s.lut */);

	SpiOsdWinBuffEnable( winno, ON );
//write to HW
	WaitVBlank(1);
	if(header->rle) {	//need RLE ?
		SpiOsdEnableRLC(ON);
		SpiOsdRLC( winno, header->bpp,header->rle);
	}	
	else {
		//BK110203
		//We using RLE only on the background.
		//if(item == 0) 
		WriteTW88Page(0X04);
		if(winno==(ReadTW88(PAGE4_SOSD|0x06)&0x0f))
		{
			SpiOsdEnableRLC(OFF);		//disable RLE
			//SpiOsdEnableRLC is not enough. So, assign win0
			SpiOsdRLC( 0,0,0); //BK110217
		}
	}
	
	//update HW
	SOsdWinBuffWrite2Hw(winno, winno);

	//Load Palette
	//SpiOsdLoadLUT(header->lut_type, lut_offset, header->lut_size, header->lut_loc);
	SpiOsdLoadLUT(winno,header->lut_type, lut_offset, header->lut_size, header->lut_loc);
#endif
}

#endif

/*
	加载图片 (非压缩)
	spi_addr    图片在flash中地址
	Offset(0-1):所使用256色表的偏移量
	En: 0x01 = 加载图片的同时打开窗口 ； 0x00 = 只加载图片，不打开窗口
*/
extern void OsdImgLoad1(BYTE sosd_win,WORD x, WORD y, WORD w, WORD h, DWORD spi_addr, BYTE Offset, WORD alpha);

void	Download_OSD_BMP(BYTE winno, WORD x, WORD y, WORD w, WORD h, DWORD spi_addr, BYTE Offset, BYTE En)
{
	WORD	temp_lut_offset;
	
	if(Offset==1)
		temp_lut_offset = 0x100;
	else
		temp_lut_offset = 0x000;

//	SpiOsdSetDeValue();	

	SpiOsdWinImageLoc(winno, spi_addr/16); 
	SpiOsdWinImageSizeWH( winno, w, h );
	SpiOsdWinScreen( winno, x, y, w, h );

	if(winno==0) {
		SpiOsdWin0ImageOffsetXY( 0, 0 );
		SpiOsdWin0Animation( 1, 0, 0, 0);
	}
	SpiOsdWinPixelAlpha( winno, ON );
	//SpiOsdWinGlobalAlpha( winno, 0);
	SpiOsdWinPixelWidth(winno, 8);

	
	SpiOsdWinLutOffset(winno, temp_lut_offset);

	SpiOsdWinBuffEnable( winno, En );
	//SOsd_SetRlc( sosd_win, header->bpp,header->rle);
	//SOsd_UpdateRlc();
	
	WaitVBlank(1);
	//WriteTW88Page(/*PAGE4_SOSD*/0x04);
	if(winno != 0)
	{
		if(winno==(ReadTW88(PAGE4_SOSD|0x06)&0x0f))
		{
			//SpiOsdEnableRLC(ON);		//disable RLE
			//SpiOsdEnableRLC is not enough. So, assign win0
			SpiOsdRLC( 0,0,0); //BK110217
			//SpiOsdRLC( winno,8,3); //BK110217
		}
	}
	//update HW
	SOsdWinBuffWrite2Hw(winno, winno);

	SpiOsdWinFillColor(winno,0);
	SpiOsdWinHZoom(winno, 0);

	SpiOsdSetDeValue();

	
	//SpiOsdIoReadLUT(winno,1,0,0x400, 0x10);
//	#endif
}
/*
	加载压缩图片
	Offset(0-1):所使用256色表的偏移量，35可以同时显示两个256色表
	En: 0x01 = 加载图片的同时打开窗口 ； 0x00 = 只加载图片，不打开窗口
*/
void	Download_OSD_BMP_RLE(BYTE winno, WORD x, WORD y, WORD w, WORD h, DWORD spi_addr,BYTE rlc, BYTE Offset, BYTE En)
{
	WORD	temp_lut_offset;
	
	if(Offset==1)
	temp_lut_offset = 0x100;
	else
	temp_lut_offset = 0x000;

//	SpiOsdSetDeValue();

	SpiOsdWinImageLoc(winno, spi_addr/16); 
	SpiOsdWinImageSizeWH( winno, w, h );
	SpiOsdWinScreen( winno, x, y, w, h );

	if(winno==0) {
		SpiOsdWin0ImageOffsetXY( 0, 0 );
		SpiOsdWin0Animation( 1, 0, 0, 0);
	}
	SpiOsdWinPixelAlpha( winno, ON );
	//SpiOsdWinGlobalAlpha( winno, 0);
	SpiOsdWinPixelWidth(winno, 8);

	
	SpiOsdWinLutOffset(winno, temp_lut_offset);

	SpiOsdWinBuffEnable( winno, En );
	
	WaitVBlank(1);
		WriteTW88Page(0X04);
		if(winno != 0)
		{
			///SpiOsdEnableRLC(ON);
			SpiOsdRLC( winno, rlc>>4,rlc&0x0F);
		}
	//update HW
	SOsdWinBuffWrite2Hw(winno, winno);

	SpiOsdWinFillColor(winno,0);
	SpiOsdWinHZoom(winno, 0);
}

//打开动态图
void	Download_OSD_BMP_Animation(WORD x, WORD y, WORD w, WORD h, DWORD spi_addr, BYTE Offset,BYTE mode, BYTE FrameH, BYTE FrameV, BYTE Duration, BYTE En)
{
	WORD	temp_lut_offset;
	
	if(Offset==1)
		temp_lut_offset = 0x100;
	else
		temp_lut_offset = 0x000;

	SpiOsdSetDeValue();	

	SpiOsdWinImageLoc(ANIMATION_WIN0, spi_addr/16); 
	SpiOsdWinImageSizeWH( ANIMATION_WIN0, w, h );
	SpiOsdWinScreen( ANIMATION_WIN0, x, y, w, h );

	SpiOsdWin0ImageOffsetXY( 0, 0 );
	SpiOsdWin0Animation( mode, FrameH, FrameV, Duration);

	SpiOsdWinPixelAlpha( ANIMATION_WIN0, ON );
	//SpiOsdWinGlobalAlpha( winno, 0);
	SpiOsdWinPixelWidth(ANIMATION_WIN0, 8);

	
	SpiOsdWinLutOffset(ANIMATION_WIN0, temp_lut_offset);

	SpiOsdWinBuffEnable( ANIMATION_WIN0, En );
	
	WaitVBlank(1);
	WriteTW88Page(0X04);

	//update HW
	SOsdWinBuffWrite2Hw(ANIMATION_WIN0, ANIMATION_WIN0);

	SpiOsdWinFillColor(ANIMATION_WIN0,0);
	SpiOsdWinHZoom(ANIMATION_WIN0, 0);

	//SpiOsdSetDeValue();
}

/*
打开/关闭OSD窗口
*/
#if 0
void	Rec_OSDWin_Enable(BYTE winno, BYTE En)
{
	WaitVBlank(1);
 	SpiOsdWinHWEnable(winno , En);
}
#endif
/*
打开/关闭多个OSD窗口(0-7)
*/
#if 0
void	Rec_OSDWins_ONOFF(BYTE wins, BYTE En)
{
	WaitVBlank(1);
	if(wins & WIN0_BIT)	{	SpiOsdWinHWEnable(0 , En);	/*Printf_Ian("\nRec_OSDWins_ONOFF_Win0");*/}
	if(wins & WIN1_BIT)	{	SpiOsdWinHWEnable(1 , En);	/*Printf_Ian("\nRec_OSDWins_ONOFF_Win1");*/}
	if(wins & WIN2_BIT)	{	SpiOsdWinHWEnable(2 , En);	/*Printf_Ian("\nRec_OSDWins_ONOFF_Win2");*/}
	if(wins & WIN3_BIT)	{	SpiOsdWinHWEnable(3 , En);	/*Printf_Ian("\nRec_OSDWins_ONOFF_Win3");*/}
	if(wins & WIN4_BIT)	{	SpiOsdWinHWEnable(4 , En);	/*Printf_Ian("\nRec_OSDWins_ONOFF_Win4");*/}
	if(wins & WIN5_BIT)	{	SpiOsdWinHWEnable(5 , En);	/*Printf_Ian("\nRec_OSDWins_ONOFF_Win5");*/}
	if(wins & WIN6_BIT)	{	SpiOsdWinHWEnable(6 , En);	/*Printf_Ian("\nRec_OSDWins_ONOFF_Win6");*/}
	if(wins & WIN7_BIT)	{	SpiOsdWinHWEnable(7 , En);	/*Printf_Ian("\nRec_OSDWins_ONOFF_Win7");*/}

}
#endif



BYTE SpiFlashChipRegCmd_ReadData(BYTE cmd, BYTE w_len, DWORD addr, BYTE r_len, BYTE *buffer, BYTE vblank)
{
	BYTE dma_option;
	BYTE i;
	volatile BYTE vdata;

	WriteTW88Page(0x04);
	WriteTW88(REG4C3,(DMA_DEST_CHIPREG << 6) | (1+w_len)); //cmd+cmd_buff_len
	WriteTW88(REG4C6, DMA_BUFF_REG_ADDR_PAGE);
	WriteTW88(REG4C7, DMA_BUFF_REG_ADDR_INDEX);
	WriteTW88(REG4C8, 0x00 );							// data Buff count middle
	WriteTW88(REG4C9, r_len );							// data Buff count Lo
	WriteTW88(REG4CA, cmd );
//	for(i=0; i < w_len; i++)
//		WriteTW88(REG4CB+i, SPI_CmdBuffer[i]);


	if(1/*SpiFlash4ByteAddr*/) {
		WriteTW88(REG4CB, (BYTE)(addr >> 24));
		WriteTW88(REG4CC, (BYTE)(addr >> 16));
		WriteTW88(REG4CD, (BYTE)(addr >> 8));
		WriteTW88(REG4CE, (BYTE)(addr));
	}
	else {
		WriteTW88(REG4CB, (BYTE)(addr >> 16));
		WriteTW88(REG4CC, (BYTE)(addr >> 8));
		WriteTW88(REG4CD, (BYTE)(addr));
	}
	WriteTW88(REG4DA, 0x00 );							// data Buff count high
	//vblank wait
	if(vblank)
		WaitVBlank(vblank);

	//dma option
	dma_option = 0x00;
	if(cmd==SPICMD_PP
	|| cmd==SPICMD_SE
	|| cmd==SPICMD_BE
	|| cmd==SPICMD_BE32K
	|| cmd==SPICMD_CE
	)
		dma_option |= 0x02;	//DMA Buff Write Mode

	if(cmd==SPICMD_WDVEREG 
	|| cmd==SPICMD_WDVREG 
	|| cmd==SPICMD_WDNVREG

	|| cmd==SPICMD_READ
	|| cmd==SPICMD_FASTREAD
	|| cmd==SPICMD_4READ

	|| cmd==SPICMD_WRSR

	|| cmd==SPICMD_SE
	|| cmd==SPICMD_BE
	|| cmd==SPICMD_BE32K
	|| cmd==SPICMD_CE
	)
		dma_option |= 0x04;	//BUSY CHECK

	// DMA-start
	WriteTW88(REG4C4, 0x01 | dma_option);	

	if(cmd==SPICMD_EX4B) SpiFlash4ByteAddr = 0;
	if(cmd==SPICMD_EN4B) SpiFlash4ByteAddr = 1;


	//wait done
	for(i=0; i < 200; i++) {
		vdata = ReadTW88(REG4C4);
		if((vdata & 0x01)==0)
			break;
		delay1ms(10);
	}
	if(i==200) {
		Printf("\n\rSpiFlashChipRegCmd Busy cmd:%bx",cmd);
		return 2;	//fail:busy
	}
	//read
	if(cmd==SPICMD_PP)
		r_len = 0;
	for(i=0; i < r_len; i++) 
		buffer[i] = ReadTW88(REG4D0+i);		

	return 0;	//success
}

/*
下载色表
spi_addr为色表在flash的起始地址
Offset (0-1),色表偏移
*/
void	Download_OSD_Lut(BYTE winno,DWORD spi_addr, BYTE Offset)
{
	WORD	lut_offset;	

	if(Offset==1)
		lut_offset = 0x100;
	else
		lut_offset = 0x000;

	WaitVBlank(1);
	//SpiOsdLoadLUT(winno,1,0,0x400, 0x10);
	SpiOsdLoadLUT(winno,1,lut_offset,0x400, spi_addr);
	//SpiOsdLoadLUT(winno,1,lut_offset,0x400, spi_addr);

	//SpiOsdIoLoadLUT1(winno,1,lut_offset,0x400, spi_addr,0);
	//SpiOsdIoReadLUT(winno,1,0,0x400, 0x10);
	//SOsd_SetLut(winno,1,lut_offset, 0x400, 0x10, 0xff);
   	//SOsd_UpdateLut(winno, 1);

}
/*
OSD窗口所在的色表偏移
*/
#if 0
void	Rec_OSD_LutOffset(BYTE winno, BYTE Offset)
{
	WORD	lut_offset;	

	if(Offset==1)
	lut_offset = 0x100;
	else
	lut_offset = 0x000;

	WaitVBlank(1);
 	OsdWinLutOffset_TW88(winno,lut_offset);
}

/*
改变多个OSD窗口的色表偏移量
*/
void	Rec_Wins_LutOffset_Set(BYTE wins, BYTE offset)
{
	WORD	lut_offset;	
	
	if(offset==1)
	lut_offset = 0x100;
	else
	lut_offset = 0x000;
	
	WaitVBlank(1);
	if(wins & WIN0_BIT)	{	OsdWinLutOffset_TW88(0 , lut_offset);	}
	if(wins & WIN1_BIT)	{	OsdWinLutOffset_TW88(1 , lut_offset);	}
	if(wins & WIN2_BIT)	{	OsdWinLutOffset_TW88(2 , lut_offset);	}
	if(wins & WIN3_BIT)	{	OsdWinLutOffset_TW88(3 , lut_offset);	}
	if(wins & WIN4_BIT)	{	OsdWinLutOffset_TW88(4 , lut_offset);	}
	if(wins & WIN5_BIT)	{	OsdWinLutOffset_TW88(5 , lut_offset);	}
	if(wins & WIN6_BIT)	{	OsdWinLutOffset_TW88(6 , lut_offset);	}
	if(wins & WIN7_BIT)	{	OsdWinLutOffset_TW88(7 , lut_offset);	}
}
#endif

//================================Font=====================================

/*
初始化一个Font窗口
*/
void	Rec_FontWin_Init(BYTE winno, WORD start_addr, WORD x, WORD y,BYTE w, BYTE h, BYTE zoomH, BYTE zoomV)
{
//	FontOsdSetDeValue();

	FontOsdWinEnable(winno, OFF);

	FontOsdWinInit(winno, start_addr);
	FontOsdWinScreen(winno, x, y, w, h);
	FontOsdWinZoom(winno, zoomH, zoomV);

	Rec_FontRam_Disp(start_addr,' ',FOSD_COLOR_IDX_CYAN, FOSD_COLOR_IDX_CYAN, w*h);
	FontOsdWinAlphaPixel(winno, FOSD_COLOR_IDX_CYAN, 10);
}
/*
在某一地址显示字符
*/
void	Rec_FontRam_Disp(WORD start_addr, WORD ch, BYTE fcolor, BYTE bcolor, WORD len)
{
	BYTE attr;
	WORD i, cnt;
	BYTE need_VBlank;

	if(len>50)
		need_VBlank = 1;
	else
		need_VBlank = 0;
	
	WriteTW88Page( PAGE3_FOSD );
	
	WriteTW88( PAGE3_FOSD|0x00, ReadTW88(PAGE3_FOSD|0x00)&0xfe ); // no bypass OSD RAM
	WriteTW88( PAGE3_FOSD|0x04, ReadTW88(PAGE3_FOSD|0x04)&0xf2 ); // no Auto Inc.
	
	attr = (bcolor<<4)|fcolor;
	len += start_addr;
	i = start_addr;

	if(ch<256)
	WriteTW88( PAGE3_FOSD|0x04,ReadTW88(PAGE3_FOSD|0x04)&0xdf);
	else
	WriteTW88( PAGE3_FOSD|0x04,ReadTW88(PAGE3_FOSD|0x04)|0x20);

	if(need_VBlank != 0)
	WaitVBlank(1);

	WriteTW88( PAGE3_FOSD|0x05, ReadTW88(PAGE3_FOSD|0x05)&0xfe ); // OSD RAM High bit clear
	for (; (i<len)&&(i<256); i++ ) {
		WriteTW88( PAGE3_FOSD|0x06, (BYTE)i );	// Addr
		WriteTW88( PAGE3_FOSD|0x07, (BYTE)ch );		// Data
		WriteTW88( PAGE3_FOSD|0x08, attr );	// Attr
		for(cnt=0; cnt<100; cnt++);

	
	}

	WriteTW88( PAGE3_FOSD|0x05, ReadTW88(PAGE3_FOSD|0x05)|0x01 ); // OSD RAM High bit set
	for (; (i<len)&&(i>=256); i++ ) {
		WriteTW88( PAGE3_FOSD|0x06, (BYTE)i );	// Addr
		WriteTW88( PAGE3_FOSD|0x07, (BYTE)ch );		// Data
		WriteTW88( PAGE3_FOSD|0x08, attr );	// Attr
		for(cnt=0; cnt<100; cnt++);
	}
	
	WriteTW88( PAGE3_FOSD|0x05, ReadTW88(PAGE3_FOSD|0x05)&0xfe ); // OSD RAM High bit clear		
}

#if 0
/*
打开/关闭多个Font窗口(0-3)
*/
void	Rec_FontWins_ONOFF(BYTE wins, BYTE En)
{
	WaitVBlank(1);
	if(wins & WIN0_BIT)	{	FontOsdWinEnable(0 , En);	}
	if(wins & WIN1_BIT)	{	FontOsdWinEnable(1 , En);	}
	if(wins & WIN2_BIT)	{	FontOsdWinEnable(2 , En);	}
	if(wins & WIN3_BIT)	{	FontOsdWinEnable(3 , En);	}
}

#endif
/*
生成一个字符型方块（可作按键或进度条用）
*/
extern		 BYTE	FontOsdWinBase[]; 
#if 0
void	Rec_FontBorder_Greate(BYTE winno, WORD x, WORD y, BYTE w, BYTE h, BYTE color)
{
	WriteTW88Page( PAGE3_FOSD );
	//zoom 0x0
	WriteTW88( PAGE3_FOSD|FontOsdWinBase[winno], ReadTW88(PAGE3_FOSD|FontOsdWinBase[winno])&0xf0);
	//Border
	WriteTW88( PAGE3_FOSD|FontOsdWinBase[winno]+0x09, w );
	WriteTW88( PAGE3_FOSD|FontOsdWinBase[winno]+0x0A, h );
	WriteTW88( PAGE3_FOSD|FontOsdWinBase[winno]+0x0E, (ReadTW88(PAGE3_FOSD|FontOsdWinBase[winno]+0x0E)&0x0F)|(color<<4) );
	//Ch.Space
	WriteTW88( PAGE3_FOSD|FontOsdWinBase[winno]+0x0C, 0x00 );
	WriteTW88( PAGE3_FOSD|FontOsdWinBase[winno]+0x0D, 0x00 );	
	//addr = 0
	WriteTW88( PAGE3_FOSD|FontOsdWinBase[winno]+0x07, ReadTW88(PAGE3_FOSD|FontOsdWinBase[winno]+0x07)&0xef );
	WriteTW88( PAGE3_FOSD|FontOsdWinBase[winno]+0x0f, 0);

	FontOsdWinScreen(winno, x, y, 1, 1);
	FontOsdWinAlphaPixel(winno, color, 0x03);
	Rec_FontRam_Disp(0, 0, color, color, 1);

//	FontOsdSetDeValue();
	FontOsdWinEnable(winno , ON);
}

/*
使用AutoInc写字符
*/
extern	void FOsdOsdRamFifoBypass(BYTE fOn);

void	Draw_Font_with_AutoInc(WORD addr, WORD font, BYTE fcolor, BYTE bcolor, WORD length)
{
	BYTE attr;
	WORD i,cnt;

	EA = 0;
	
	attr = (bcolor<<4)|fcolor;
	
	WriteTW88Page( PAGE3_FOSD );
	
	WriteTW88( PAGE3_FOSD|0x00, ReadTW88(PAGE3_FOSD|0x00)&0xfe ); // no bypass OSD RAM
	WriteTW88( PAGE3_FOSD|0x04, ReadTW88(PAGE3_FOSD|0x04)|0x0C ); //  Auto Inc.
	
	if(addr<256)
	WriteTW88( PAGE3_FOSD|0x05, ReadTW88(PAGE3_FOSD|0x05)&0xfe ); // OSD RAM High bit clear
	else
	WriteTW88( PAGE3_FOSD|0x05, ReadTW88(PAGE3_FOSD|0x05)|0x01 ); // OSD RAM High bit set
	WriteTW88( PAGE3_FOSD|0x06, (BYTE)addr); // Addr

	WriteTW88( PAGE3_FOSD|0x08, attr ); // Attr
	
	if(font<256)
	WriteTW88( PAGE3_FOSD|0x04,ReadTW88(PAGE3_FOSD|0x04)&0xdf);
	else
	WriteTW88( PAGE3_FOSD|0x04,ReadTW88(PAGE3_FOSD|0x04)|0x20);

	//if(length>50)
	//WaitVBlank(1);

	FOsdOsdRamFifoBypass(ON);
	for ( i=0; i<length; i++ )
	{
		WriteTW88( PAGE3_FOSD|0x07,(BYTE)font); // Display RAM
		for(cnt=0; cnt<100; cnt++);
	}
	WriteTW88( PAGE3_FOSD|0x00, ReadTW88(PAGE3_FOSD|0x00)&0xfe ); // no bypass OSD RAM
	
	WriteTW88( PAGE3_FOSD|0x04, ReadTW88(PAGE3_FOSD|0x04)&0xf2 ); // no Auto Inc.

	EA = 1;
}
#endif
/*
移动字符窗口到XY
*/
#if 0
void	Rec_FontOSD_Screen_XY(BYTE winno, WORD x, WORD y)
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
}
#endif


void	FontOsdWinOffAll(void)
{
	BYTE winno;
	for(winno = 0;winno < 8; winno ++)
		FontOsdWinEnable(winno , OFF);
}


void	Init_OSD(void)
{
	//OSDFont_Init();
	//FontOsdSetDeValue();

	SOsdWinBuffClean(0);
	SpiOsdSetDeValue(); 		
	//SpiOsdWinHWOffAll(0); //without wait			//mnm
	SpiOsdEnable(ON);
	
	OSDMixOrder(0);	
}




