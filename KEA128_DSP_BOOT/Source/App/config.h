






/************************************************************************/
/*tw8836 config.h*/
/************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include	"const.h"	

//-----------------------------------------------------------------------------
//		Firmware Version
//-----------------------------------------------------------------------------
#define		FW_VERSION				17			//Firmware Version V1.7

//-----------------------------------------------------------------------------
//#define INTERNAL_MCU -- defined by Project Compiler session
//-----------------------------------------------------------------------------
#define 	MODEL_TW8836

//-----------------------------------------------------------------------------
//		Chip Access Mode
//-----------------------------------------------------------------------------
#define		CHIP_ACCESS_MODE	ACCESS_16BIT
		

#define		SUPPORT_HDMI

//-----------------------------------------------------------------------------
//		Options for Debugging/Release
//-----------------------------------------------------------------------------
#define DEBUG_TOUCH
#define		UART0_ON				
#define		UART1_ON

#define		PRINT_COM	PRINT_UART0

//-----------------------------------------------------------------------------
//		Panel Vendor Specific
//-----------------------------------------------------------------------------

#define		CUR_PANEL						HANNSTAR_PANEL_7INCH_1024


#if (CUR_PANEL == HANNSTAR_PANEL_7INCH_1280)
#include	"UI\Ver_1280\Bmp_Adr.h"
#include	"UI\Ver_1280\btn_xywh.h"
#include	"UI\Ver_1280\user_btn_id.h"
#include	"UI\Ver_1280\Bmp_rle.h"
#elif (CUR_PANEL == HANNSTAR_PANEL_7INCH_1024)
#include	"UI\Ver_1024\Bmp_Adr.h"
#include	"UI\Ver_1024\btn_xywh.h"
#include	"UI\Ver_1024\user_btn_id.h"
#include	"UI\Ver_1024\Bmp_rle.h"
#else
#include	"UI\Ver_800\Bmp_Adr.h"
#include	"UI\Ver_800\btn_xywh.h"
#include	"UI\Ver_800\user_btn_id.h"
#include	"UI\Ver_800\Bmp_rle.h"
#endif


//-----------------------------------------------------------------------------
//		Panel Resolution
//-----------------------------------------------------------------------------
#if (CUR_PANEL == HANNSTAR_PANEL_7INCH_1280)
	#define		PANEL_W		1280
	#define 	PANEL_H		800
#elif (CUR_PANEL == HANNSTAR_PANEL_7INCH_1024)
    #define		PANEL_W		1024
	#define 	PANEL_H		600
#else
	#define		PANEL_W		800
	#define 	PANEL_H		480
#endif

//-----------------------------------------------------------------------------
//		Demo PCB Version 
//-----------------------------------------------------------------------------
#define		PCB_VER				DEMO_AT	



#define USE_SFLASH_EEPROM		//FEE(FlashEepromEmulator)

//#define SUPPORT_UART1			//connot share with BT656 output(ExtCVBS)

#define SUPPORT_TOUCH





#define	REG_START_ADDRESS	0xc000		// register start 0xc000, for page0, 0xc100-page1 
//-----------------------------------------------------------------------------
//		Panel Resolution
//-----------------------------------------------------------------------------
//#define PANEL_WVGA

//-----------------------------------------------------------------------------
//		I2C Device Access Address Mapping
//-----------------------------------------------------------------------------

#define TW88I2CAddress		0x8a	// TW8836
#define ADDRESS_EEPROM		0xA0	// 24C16 2Kx8bit		




#endif // __CONFIG_H__










#define WDOG_EN

#define MCU_VERSION         "BOOT.0.200506.0"



