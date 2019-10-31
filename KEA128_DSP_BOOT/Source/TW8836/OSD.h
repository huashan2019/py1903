#ifndef __OSD_H__
#define __OSD_H__


//=============================================================================
//		FontOSD Definition
//=============================================================================
//new FontOsd feature
//18Bytes
#if 0
typedef struct FONT_INFO_s {
	BYTE type;				//??
	BYTE width,height;
	BYTE dummy;
	WORD max;				//number of fonts
	WORD loc_2bpp;
	WORD loc_3bpp;
	WORD loc_4bpp;
	DWORD loc;				//Font location on SpiFlash.
	WORD size;				//max 64K. TW8835 use 10K.
} FONT_INFO_t;
#endif

typedef struct FONT_INFO_s {
	BYTE name[4+2];
	BYTE width,height;
	WORD max;
	WORD bpp2;		//start of 2BPP
	WORD bpp3;		//start of 3BPP
	WORD bpp4;		//start of 4BPP
	BYTE lut2, lut3, lut4;	//(LUT offset*4)
	BYTE dummy;
} FONT_INFO_t;

typedef struct FONT_SPI_INFO_s {
	DWORD loc;
	WORD size;				//curr max 10*1024 = 2800h
	BYTE width,height;
	WORD max;
	WORD bpp2;
	WORD bpp3;
	WORD bpp4;
} FONT_SPI_INFO_t;

typedef struct FONT_WIN_INFO_s {
	BYTE win;
	BYTE osdram;			//osdram start offset

} FONT_WIN_INTO_t;


//=============================================================================
//		RLE Type Definition
//=============================================================================

struct _RLE_HEADER {
    BYTE id[2];     			// id='TW' 
    BYTE c;  					// LUT | bits/pixel
    BYTE cp; 					// compression info = data bits | count bits      
	WORD w;                		// width (little endian)
    WORD h;				   		// height (little endian)
    DWORD size;					// size (little endian)
    BYTE *bmp;   		//
} ;
//=============================================================================
//		Animation Structure
//=============================================================================

#define MAX_ANIMATION	4

struct st_animation {
	BYTE active;
	BYTE current;
	BYTE speed;					// fixable
	BYTE vcnt;
	WORD x, y;					// fixable
	WORD w, h;					// fixable
	BYTE max;					// fixable
	WORD srcx[10], srcy[10];	// fixable
};


extern BYTE string_zoom_x;
extern BYTE string_zoom_y;


//=============================================================================
//		OSD Register Definition
//=============================================================================
//#define OSD_PAGE			0x07	  pls remove

#define OSD_ST				0x00
#define WIN0_ST				0x80
#define WIN1_ST				0xa0
#define WIN4_ST				0xc0

#define OSD_MODE			0x00
#define OSD_START			0x01
#define OSD_DATAPORT		0x02
#define OSD_SPECIAL			0x03
#define OSD_RLECTRL			0x04
#define OSD_RLEDATACNT		0x05
#define OSD_BITBLTCOLOR		0x0e

#define	OSD_SOURCECOLOR		0x40
#define	OSD_TARGETCOLOR		0x48
#define	OSD_NOWRITECOLOR	0x50

#define OSD_BITBLTSOURCE	0x60
#define OSD_BITBLTSOURCEW64	0x63
#define OSD_BITBLTSOURCEXY	0x64	
#define OSD_BITBLTSIZE		0x68
#define OSD_BITBLTDEST		0x70
#define OSD_BITBLTDESTW64	0x73
#define OSD_BITBLTDESTXY	0x74

#define OSD_LUT_WINNO		0x7a
#define OSD_LUT_INDEX		0x7b
#define OSD_LUT_ATTR		0x7c		
#define OSD_LUT_R			0x7d
#define OSD_LUT_G			0x7e
#define OSD_LUT_B			0x7f

#ifdef MODEL_TW8836
//#define SPIOSD_PAGE			0x04  	//use CPU.h

#define	SPI_OSDWIN_ENABLE		0x00
#define SPI_OSDWIN_HZOOM		0x00

#define	SPI_OSD_ST				0x00
#define SPI_WIN0_ST				0x20
#define SPI_WIN1_ST				0x40
#define SPI_WIN2_ST				0x50
#define SPI_WIN3_ST				0x60
#define SPI_WIN4_ST				0x70
#define SPI_WIN5_ST				0x80
#define SPI_WIN6_ST				0x90
#define SPI_WIN7_ST				0xA0
#define SPI_WIN8_ST				0xB0


#define SPI_OSDWIN_SCREEN				0x01
#define	SPI_OSDWIN_BUFFERSTART			0x07
#define SPI_OSDWIN_BUFFERSTART_BIT		0x0A
#define SPI_OSDWIN_DISPSIZE				0x0A
#define SPI_OSDWIN_DISPOFFSET			0x0D
#define SPI_OSDWIN_ALPHA				0x0C	//430(42C+4), 44C
#define SPI_OSDWIN_LUT_PTR				0x0D	//431(42D+4), 44D
#define SPI_OSDWIN_FILLCOLOR			0x0E	//436(42E+8), 44E
#define	SPI_OSDWIN_ANIMATION			0x12    //432
//---------------------------------------------
#endif






//---------- Window Definition -------------
#define OSDWIN_ENABLE		0x00
#define OSDWIN_BUFFERSTART	0x09
#define OSDWIN_DISPOFFSET	0x0e
#define OSDWIN_SCREEN		0x01

//---------- OSD Output Path Selection -------------
#define OSD_OUT_SEL			0xf8
//=============================================================================
//		Bit Definition in OSD Registers
//=============================================================================

//=== [7:6] Write Mode
#define	CPUWRITE			0x00	// CPU
#define BLOCKTRANS			0x40	// Block Transfer
#define BLOCKFILL			0x80	// Block Fill
#define BLOCKTRANSL			0xc0	// Block Transfer with Linear

//=== [5:4] BitBlt Mode
#define BLT_SELECTIVE		0x00	// Selective No Write
#define BLT_SOURCE			0x10	// Mask from Source
#define BLT_REG				0x20	// Mask from register
#define BLT_NO				0x30	// No

//=== [3] Enable Color Conversion
#define CONV_EN				0x08	// Enable Color Conversion
#define CONV_DIS			0x00	// Disable

//=== [2] Window Pixel Unit
#define PIXEL16				0x00	// 16 bit
#define PIXEL8				0x04	// 8 bit

//=== [1:0] Source Data bit/pixel
#define SOURCE0				0x00	// Source Data = same as display
#define SOURCE1BIT			0x01	// 1 bit
#define SOURCE2BIT			0x02	// 2 bit
#define SOURCE4BIT			0x03	// 4 bit
#define SOURCE_				0xfc	// mask source data

//=============================================================================
//		OSD Window Setup
//=============================================================================
#define OSD16_YCBCR422		0x00
#define OSD16_RGB565		0x02
#define OSD16_RGB4444		0x04
#define OSD16_RGB1555		0x06



void OsdWinDisplay			(BYTE mode);

BYTE OsdWinBase 			(BYTE winno);
void OsdWinEnable 			(BYTE winno, BYTE en);
void OsdWinBufferMem 		(BYTE winno, DWORD start);
void OsdWinBufferWH 		(BYTE winno, BYTE w, BYTE h);
void OsdWinBuffer 			(BYTE winno, DWORD start, BYTE w, BYTE h);
void OsdWinBufferOffsetXY 	(BYTE winno, WORD x, WORD y);
void OsdWinScreen 			(BYTE winno, WORD x, WORD y, WORD w, WORD h);
void Osd16Format			(BYTE format);
void OsdWriteMode			(BYTE mode);
void OsdSpecialExpansion	( BYTE mode );
//=============================================================================
//
//=============================================================================
void OsdLoadDefaultLUT (BYTE winno);
//=============================================================================
//		OSD BitBlit Memory Setup
//=============================================================================
void OsdBltSourceMemoryStart(DWORD start);
void OsdBltSourceMemory   	(DWORD start, BYTE w64);
void OsdBltSourceXY       	(WORD x, WORD y);
void OsdBltSourceMemoryW64	(BYTE w64);

void OsdBltSize           	(WORD w, WORD h);

void OsdBltDestMemoryStart	(DWORD start);
void OsdBltDestMemoryW64  	(BYTE w64);
void OsdBltDestXY         	(WORD x, WORD y);

void OsdBltColor          	(WORD color);
void OsdSourceColor			( BYTE index, WORD color);
void OsdTargetColor			( BYTE index, WORD color);
void OsdNoWriteColor		( BYTE index, WORD color);

#define OSD_WMODE_CPU				0x00
#define OSD_WMODE_BLOCKTRANSFER		0x40
#define OSD_WMODE_BLOCKFILL			0x80
#define OSD_WMODE_BLOCKTRANSFERL	0xc0









//void OsdWriteMode(BYTE mode);
void OsdStart(BYTE en);
BYTE OsdBusy(void);
	
//=============================================================================
//
//=============================================================================
void OsdLoadBmpXY            ( struct _RLE_HEADER *p, WORD sx, WORD sy );
void OsdLoadBmpXYFromSPI     ( struct _RLE_HEADER *p, DWORD spiaddr, WORD sx, WORD sy );
void OsdLoadBmpLinear        ( struct _RLE_HEADER *p, DWORD ddraddr );
//void OsdLoadBmpLinearFromSPI0( BYTE wbits, struct _RLE_HEADER *p, DWORD spiaddr, DWORD ddraddr );
void OsdLoadBmpLinearFromSPI ( struct _RLE_HEADER *p, DWORD spiaddr, DWORD ddraddr );
void OsdLoadTransBmpXYFromSPI ( struct _RLE_HEADER *p, DWORD spiaddr, WORD sx, WORD sy  );
void OsdLoadFont             ( BYTE wbits );
void OsdLoadFontMV           ( BYTE wbits );
void OsdLoadFontTransparent	 ( BYTE wbits ) ;
void Osd16bitTransparent	 ( BYTE wbits );
void OsdDisplayString        ( BYTE kind, WORD x, WORD y, char *str, BYTE fore, BYTE back );
void OsdDisplayStringT       ( BYTE kind, WORD x, WORD y, char *str, BYTE fore );
void OsdDisplayStringLinear  ( WORD x, WORD y, char *str, BYTE fore, BYTE back );
void OsdDisplayStringTLinear ( WORD x, WORD y, char *str, BYTE fore );

void OsdLoadLUT              ( BYTE winno, BYTE *ptr );
void OsdLoadLUT16            ( BYTE winno, BYTE *ptr );
void OsdLoadTranparentLUT	 (BYTE winno, BYTE red, BYTE green, BYTE blue);
void OsdLoadTranparentLUT2	 (BYTE winno, BYTE red, BYTE green, BYTE blue, BYTE cred, BYTE cgreen, BYTE cblue);
#if 1
//#define OsdWaitVBlank(x)	WaitVBlank(x)
#define OsdWaitVBlank(x)
#else
void OsdWaitVBlank           ( BYTE cnt );
#endif
void OsdWaitDisplayBlank     ( BYTE cnt );
void OsdWaitOSDBlank		 (BYTE cnt);
void OsdWaitWindowBlank		 (BYTE winno, BYTE cnt);
void OsdBlockFill            ( WORD dx, WORD dy, WORD w, WORD h, WORD color );
void OsdBlockTransfer        ( WORD sx, WORD sy, WORD dx, WORD dy, WORD w, WORD h );
void OsdBlockTransferLinear  ( DWORD addr, WORD dx, WORD dy, WORD w, WORD h );

void OsdMirror( BYTE on );
void OsdFlip( BYTE on );

//void Animation(void);

//*****************************************************************************
//				Initialize OSD
//*****************************************************************************
void OsdInit(BYTE winno);

//============================
//FontOSD
//============================
#define TEXT_ALIGN_LEFT		0
#define TEXT_ALIGN_MIDDLE	1
#define TEXT_ALIGN_RIGHT	2

struct font_info_s {
	BYTE width;		//12 or 16
	BYTE height;	//18
	BYTE align;		//0:Left, 1:middle 2:right
	BYTE attr;		//high: bgcolor index low:fgcolor
					//if high & low have a same color is a special.
					//we can have a 16 special color
};
//default color index
#define FOSD_COLOR_IDX_BLACK		0
#define FOSD_COLOR_IDX_BLUE			1
#define FOSD_COLOR_IDX_LIME			2
#define FOSD_COLOR_IDX_CYAN			3
#define FOSD_COLOR_IDX_RED			4
#define FOSD_COLOR_IDX_MAGENTA		5
#define FOSD_COLOR_IDX_YELLOW		6
#define FOSD_COLOR_IDX_SILVER		7
#define FOSD_COLOR_IDX_GRAY			8
#define FOSD_COLOR_IDX_DARKBLUE		9
#define FOSD_COLOR_IDX_DLIME		10
#define FOSD_COLOR_IDX_DCYAN		11
#define FOSD_COLOR_IDX_DRED			12
#define FOSD_COLOR_IDX_DMAGENTA		13
#define FOSD_COLOR_IDX_DYELLOW		14
#define FOSD_COLOR_IDX_WHITE		15

#define FOSD_COLOR_VALUE_BLACK		0x0000
#define FOSD_COLOR_VALUE_RED		0xF800
#define FOSD_COLOR_VALUE_LIME		0x07E0
#define FOSD_COLOR_VALUE_BLUE		0x001F
#define FOSD_COLOR_VALUE_WHITE		0xFFFF
#define FOSD_COLOR_VALUE_ORANGE		0xFC00
#define FOSD_COLOR_VALUE_OLIVE		0x8400

#define FOSD_COLOR_USER0		FOSD_COLOR_VALUE_OLIVE
#define FOSD_COLOR_USER1		FOSD_COLOR_VALUE_ORANGE
#define FOSD_COLOR_USER(num)	FOSD_COLOR_USER##num

//#define FONTOSD_COLOR_RED		0xF800
//#define FONTOSD_COLOR_LIME		0x07E0
//#define FONTOSD_COLOR_BLUE		0x001F
//#define FONTOSD_COLOR_BLACK		0x0000
//#define FONTOSD_COLOR_WHITE		0xFFFF
//#define FONTOSD_COLOR_ORANGE    0xFC00

#define	FOSD_SETUP_VERSION_ADR	0x00

#define	FOSD_SETUP_BRI_ADR		0x10
#define	FOSD_SETUP_CONT_ADR		(FOSD_SETUP_BRI_ADR + 15)
#define	FOSD_SETUP_HUE_ADR		(FOSD_SETUP_BRI_ADR + 30)

#define	FOSD_DEMO_NUM_ADR			0x00
#define	FOSD_DEMO_SMALLLETTER_ADR	0x0A
#define	FOSD_DEMO_BIGLETTER_ADR		0x24




extern BYTE BPP3_alpha_table[8];


void FontOsdSetDeValue(void);
void FontOsdWinEnable				(BYTE winno, BYTE en);
void FontOsdWinMulticolor			(BYTE winno, BYTE en);
void FontOsdWinOffAll(void);
void FontOsdWinAlphaPixel			(BYTE winno, BYTE color, BYTE alpha);
//void FontOsdWinScreen				(BYTE winno, WORD x, WORD y, BYTE w, BYTE h, BYTE zoomH, BYTE zoomV);
void FontOsdWinScreen				(BYTE winno, WORD x, WORD y, BYTE w, BYTE h);
void FontOsdWinScreenWH				(BYTE winno, BYTE w, BYTE h);
void FontOsdWinZoom					(BYTE winno, BYTE zoomH, BYTE zoomV);

void FontOsdSetPaletteColor(BYTE start, WORD color, BYTE size);
void FontOsdSetPaletteColorArray(BYTE index, WORD *array, BYTE size);
//void FontDMA( void );
void FontDMA( DWORD	address, WORD size );
void FontOsdInit(void);
void FontOsdWinPuts(BYTE winno, WORD sx, WORD sy, BYTE align, BYTE *str);

void FontOsdWinSetOsdRamStart(BYTE winno, WORD addr);


//---------------------
//FontSet
//location, size, width,height, start of 2bpp, start of 3bpp, start of 4bpp.
//LUT values(16*16)

void InitFontRam(WORD dest_font_index,FONT_SPI_INFO_t *font);
void InitFontRam_consolas(WORD dest_font_index);
void FontOsdWinInit(BYTE winno, WORD OsdRamAddr);
void DownloadExtFont(WORD dest_font_index, DWORD src_loc, WORD size, WORD bpp2, WORD bpp3, WORD bpp4);

void FontOsdPrintIndexedString(BYTE winno, WORD x, WORD y, BYTE str_n);

#define FOSD_STR0_GOOD			0
#define FOSD_STR1_TW8835		1
#define FOSD_STR2_NOSIGNAL		2
#define FOSD_STR3_OUTRANGE		3
#define FOSD_STR4_INIT_EEPROM	4
#define FOSD_STR5_INPUTMAIN		5

void FOsdMsgSetInfo(BYTE winno, WORD sx,WORD sy);
BYTE FOsdMsgGetWin(void);
WORD FOsdMsgGetSx(void);
WORD FOsdMsgGetSy(void);
void FOsdCopyMsgBuff2Osdram(BYTE fOn);
void FOsdIndexMsgPrint(BYTE index);
void FOsdDumpPalette(BYTE winno);
void FOsdWriteAllPalette(WORD color);
void FOSDIntersil(BYTE winno);

void OSDFont_Init(void);

//============================
//SpiOSD
//============================
void SOsdWinBuffClean(BYTE hw);
void SOsdWinBuffWrite2Hw(BYTE start, BYTE end);


void WaitVBlank						(BYTE cnt);
void SpiOsdSetDeValue(void);
void SpiOsdEnable(BYTE en);


void SpiOsdWinHWEnable				(BYTE winno, BYTE en);
void SpiOsdWinBuffEnable			(BYTE winno, BYTE en);
void SpiOsdWinHWOffAll(BYTE wait);
void SpiOsdWinBuffSynchEnable(void);
void SpiOsdWinBuffOffAll(void);
void SpiOsdWinHWOff(BYTE start, BYTE end);


void SpiOsdWinImageLoc		(BYTE winno, DWORD start);
void SpiOsdWinImageLoc_TW88(BYTE winno, DWORD start);
void SpiOsdWinImageLocBit	(BYTE winno, BYTE start);
void SpiOsdWinImageSizeW	(BYTE winno, WORD w);
void SpiOsdWinImageSizeWH 	(BYTE winno, WORD x, WORD y);
void	SpiOsd_Win0_Buf_Offset(WORD buf_w, WORD buf_h, WORD offset_w, WORD offset_h);
void	SpiOsd_Win0_OffsetWH_TW88(WORD offset_w, WORD offset_h);
void	SpiOsd_Win0_OffsetW_TW88(WORD offset_w);
void	SpiOsd_Win0_OffsetH_TW88(WORD offset_h);

void SpiOsdWinScreen		(BYTE winno, WORD x, WORD y, WORD w, WORD h);
void	SpiOsdWinScreen_XW_TW88(BYTE winno, WORD x, WORD w);
void SpiOsdWinLutOffset		(BYTE winno, WORD table_offset );
void	OsdWinLutOffset_TW88( BYTE winno, WORD table_offset );
void SpiOsdWinPixelWidth	(BYTE winno, BYTE bpp);
void SpiOsdWinFillColor		(BYTE winno, BYTE color );
void SpiOsdWinGlobalAlpha	(BYTE winno, BYTE alpha );
void	SpiOsdWinGlobalAlpha_TW88( BYTE winno, BYTE alpha );
void SpiOsdWinPixelAlpha	(BYTE winno, BYTE alpha );
void	SpiOsdWinPixelAlpha_TW88( BYTE winno, BYTE alpha );
void SpiOsdWin0SetPriority	(BYTE high);
void SpiOsdWin0Animation	(BYTE mode, BYTE FrameH, BYTE FrameV, BYTE Duration);
void SpiOsdWin0ImageOffsetXY(WORD x, WORD y);

#define LUTTYPE_BYTE	1
#define LUTTYPE_ADDR	0
void SpiOsdIoLoadLUT(BYTE winno,BYTE type, WORD LutOffset, WORD size, DWORD spiaddr);
//void SpiOsdLoadLUT(BYTE type, WORD LutOffset, WORD size, DWORD address);
void SpiOsdLoadLUT(BYTE _winno, BYTE type, WORD LutOffset, WORD size, DWORD address);
void SpiOsdPixelAlphaAttr(BYTE win, WORD lutloc, BYTE value);

void SpiOsdRLC(BYTE winno,BYTE dcnt, BYTE ccnt);
void SpiOsdResetRLC(BYTE reset);
void SpiOsdEnableRLC(BYTE en);


//call from main.
void InitLogo(void);

void SOsdHwBuffClean(void);
void SOsdHwBuffSetLut(BYTE win, /*BYTE type,*/  WORD LutOffset, WORD size, DWORD address);
void SOsdHwBuffSetRle(BYTE win, BYTE bpp, BYTE count);
void SOsdHwBuffSetAlpha(WORD alpha);
void SOsdHwBuffWrite2Hw(void);

void SpiOsdWinHZoom(BYTE winno, BYTE en);

void Init_OSD(void);
#endif
