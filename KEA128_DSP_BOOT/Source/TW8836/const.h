#ifndef		_CONST_H_
#define		_CONST_H_

typedef enum{
	e_PANELDATA_CVBS_ALL = 0,
	e_PANELDATA_CVBS_N = 1,
	e_PANELDATA_CVBS_P,
	e_PANELDATA_YUV_DVD,
	e_PANELDATA_ARGB_VGA,
	e_PANELDATA_ARGB_GPS,
	e_PANELDATA_DRGB_HDMI,
}E_PNLDATA;

struct	BMP_RleData
{
	unsigned long  adr;
	unsigned char  rlc;
	
};

struct Radar_Data
{
	unsigned char FrontLeft;
	unsigned char FrontMidLeft;
	unsigned char FrontRight;
	unsigned char FrontMidRight;
	unsigned char RearLeft;
	unsigned char RearMidLeft;
	unsigned char RearRight;
	unsigned char RearMidRight;	
};

//-----------------------------------------------------------------------------
//		Chip Access Mode
//-----------------------------------------------------------------------------
#define		ACCESS_8BIT			0
#define		ACCESS_16BIT		1

//-----------------------------------------------------------------------------
//		Panel Vendor Specific
//-----------------------------------------------------------------------------
#define		HANNSTAR_PANEL_7INCH_1280		0
#define		HANNSTAR_PANEL_7INCH_1024		1
#define		HANNSTAR_PANEL_7INCH_800		2
#define		TM_PANEL_62INCH					3

//-----------------------------------------------------------------------------
//		Printf Com
//-----------------------------------------------------------------------------
#define		PRINT_UART0			1
#define		PRINT_UART1			0

//-----------------------------------------------------------------------------
//		Demo PCB Version 
//-----------------------------------------------------------------------------
#define		DEMO_INTERSIL		0
#define		DEMO_AT				1

//-----------------------------------------------------------------------------
//		TW8836 Version
//-----------------------------------------------------------------------------
#define		REV_A					0x00
#define		REV_B					0x11


#define 	ERR_SUCCESS		0
#define 	ERR_FAIL		1

//#define 	ON				1
//#define 	OFF				0


//#define	TRUE	1
//#define	FALSE	0
#ifndef NULL
 #define NULL ((void *) 0)
#endif


//VInputStdDetectMode
#define AUTO	0

//VInputStd
#define NTSC	0			
#define PAL		1
#define SECAM	2
#define NTSC443	3
#define PALM	4
#define PAL_CN	5
#define PAL60	6

//g_Group 
#define			G_MAIN							1
#define			G_RADIO							2
#define			G_BT							3
#define			G_DVD							4
#define			G_USB							5
#define			G_IPOD							6
#define			G_TV							7
#define			G_SD							8
#define			G_SETUP							9
#define			G_GPS							10
#define			G_AUX							11

#define			G_VGA							12
#define			G_HDMI							13
#define			G_TRACE							14
#define			G_OSDDEMO						15

//LUT
#define		BG_LUT_OFFSET		0
#define		BUTTON_LUT_OFFSET	1

//BG WIN
#define		ANIMATION_WIN0	0
#define		BG_WIN1			1		//Main,BT etc  no video's background  	  1280X800
#define		BG_WIN2			2		//DVD Aux etc  video's menu           not 1280X800

//Press WIN
#define		PRESS_WIN		8
#define		PRESS_WIN2		2

//Trace WIN	
#define		TRACE_WIN				2
#define		RADAR_MODEL_WIN			0
#define		RADAR_FRONTLEFT_WIN		3
#define		RADAR_FRONTRIGHT_WIN	4
#define		RADAR_REARLEFT_WIN		5
#define		RADAR_REARLEFTM_WIN		6
#define		RADAR_REARRIGHTM_WIN		7
#define		RADAR_REARRIGHT_WIN		8




#define		STATUS_MENU_HIDE					0
#define		STATUS_MENU_SHOW					1

#define		SEMITRANSPARENT_VALUE				0x26	

#define MCUSPI_CLK_27M		0
#define MCUSPI_CLK_32K		1
#define MCUSPI_CLK_PCLK		2
#define CLKPLL_SEL_PCLK		0
#define CLKPLL_SEL_PLL		1
#define CLKPLL_DIV_1P0		0
#define CLKPLL_DIV_1P5		1
#define CLKPLL_DIV_2P0		2
#define CLKPLL_DIV_2P5		3
#define CLKPLL_DIV_3P0		4
#define CLKPLL_DIV_5P0		7


#define	RGB_NOSIGNAL		0
#define	YUV_NOSIGNAL		0
#define	CVBS_NOSIGNAL		0
#define	HDMI_NOSIGNAL		0

#define	HDMI_HAVESIGNAL		2	

#define	VGA_RESOLUTION_640X480		 5
#define	VGA_RESOLUTION_800X480		50
#define	VGA_RESOLUTION_1024X600		51
#define	VGA_RESOLUTION_800X600		10	
#define	VGA_RESOLUTION_1024X768		18
#define	VGA_RESOLUTION_1280X720		24
#define	VGA_RESOLUTION_1280X800		32
#define	VGA_RESOLUTION_1280X1024	39
#define	VGA_RESOLUTION_1366X768		42


#define	HDMI_RESOLUTION_640X480		 5
#define	HDMI_RESOLUTION_1280X720	24
#define	HDMI_RESOLUTION_1920X1080	49
#define	HDMI_RESOLUTION_1920X1080i	52


#endif
