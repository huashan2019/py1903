/*****************************************************************************/
/*                                                                           */
/*              Grobal Variables                                             */
/*                                                                           */
/*****************************************************************************/
#ifndef __EXTERN_H__
#define __EXTERN_H__

#include	"const.h"



//-----------------Rev
//extern	BYTE	g_TW8836_Rev;


//extern BYTE SPICMD_READ;
extern BYTE SPICMD_PROGRAM;

extern BYTE SPICMD_BYTES;
//---------------------------------- CPU --------------------------------------
extern	 	BYTE   	tm001;
extern	 	WORD   	tm01;
extern	 	BYTE   	tic01;
extern	 	WORD   	tic_pc;
extern	 	WORD   	tic_task;
extern	 	DWORD	SystemClock;


//extern			bit		RS0_Xbusy;
//extern			bit		RS1_Xbusy;





//extern		BYTE	X *  regTW88;

extern		BYTE	INT_STATUS;
extern		BYTE	INT_STATUS2;
extern		BYTE	EXINT_STATUS;
extern  	WORD	MCU_INT_STATUS;
extern		WORD	VH_Loss_Changed;
extern		BYTE	INT_STATUS_ACC;
extern		BYTE	INT_STATUS2_ACC;

//extern			bit		RM_get;

//-------------------------- SPI ----------------------------------------------
extern   BYTE SPI_Buffer[128];
//-------------------------- FontOsd Message ----------------------------------------------
extern BYTE FOsdMsgBuff[40];
//-------------------------- Debugging & Monitor ------------------------------
extern		BYTE	MonAddress;	
//extern	X	BYTE	MonIndex;
extern		BYTE	MonR, MonW;
extern		BYTE	monstr[];		// buffer for input string
extern		BYTE 	*argv[];
extern		BYTE	argc;
//extern			bit		echo;

extern			BYTE 	*MonString;

//extern	struct st_animation Animation[];
//extern          bit 	AnimationON;
//extern			bit		access;


//extern		BYTE	HANNSTAR_1280_CVBS_NTSC[];
//extern		BYTE	HANNSTAR_1280_DVD_RGB[];
//extern		BYTE	HANNSTAR_1280_DTV_GPS[];

extern		BYTE	PANELDATA_CVBS_ALL[];
#if (CUR_PANEL == HANNSTAR_PANEL_7INCH_1024)
//extern		BYTE	PANELDATA_CVBS_ALL_REVB[];
extern  	BYTE	PANELDATA_VGA_RGB_REVB[];
#endif
extern		BYTE	PANELDATA_CVBS_NTSC[];
extern		BYTE	PANELDATA_CVBS_PAL[];	
extern		BYTE	PANELDATA_DVD_YUV[];
extern		BYTE	PANELDATA_VGA_RGB[];
extern		BYTE	PANELDATA_DTV_RGB[];

extern		BYTE	PANELDATA_VGA_640X480_PATCH[];
extern		BYTE	PANELDATA_VGA_800X480_PATCH[];
extern		BYTE	PANELDATA_VGA_800X600_PATCH[];
extern		BYTE	PANELDATA_VGA_1024X600_PATCH[];
extern		BYTE	PANELDATA_VGA_1024X768_PATCH[];
extern		BYTE	PANELDATA_VGA_1280X720_PATCH[];
extern		BYTE	PANELDATA_VGA_1280X800_PATCH[];
extern		BYTE	PANELDATA_VGA_1280X1024_PATCH[];
extern		BYTE	PANELDATA_VGA_1366X768_PATCH[];	

extern		BYTE	PANELDATA_HDMI_640X480_PATCH[];
extern		BYTE	PANELDATA_HDMI_1280X720_PATCH[];
extern		BYTE	PANELDATA_HDMI_1920X1080_PATCH[];
extern		BYTE	PANELDATA_HDMI_1920X1080i_PATCH[];

extern		struct	BMP_RleData BackCar_LeftTrack[];
extern		struct	BMP_RleData BackCar_RightTrack[];


extern	WORD	g_Video_OSD_delay;
//extern	BYTE	g_TraceValue;
//extern	WORD	g_Trace_delay;
//extern	WORD	g_Radar_delay;

//extern	BYTE	g_CVBS_For_PAL;

extern	BYTE	g_PanelMute;

//extern		BYTE	InputVGA_Resolution;
extern	WORD	g_VGA_CheckMode_Delay;

//extern		BYTE	InputHDMI_Resolution;
extern	WORD	g_HDMI_CheckMode_Delay;
extern	WORD	g_HDMI_CheckSignal_Delay;

//extern	WORD	g_H_Resolution;	

//Function
extern	void	PanelMute( BYTE on );

//extern	void	Panel_Switch(E_PNL to);


extern	void	OSDWinGlobalAlpha( BYTE winno, BYTE alpha );
extern	void	SpiOsdPixelAlpha( BYTE winno, BYTE coloridx, BYTE alpha ,BYTE lut_offset);

//Font
extern	void	OSDMixOrder( BYTE font_first );
extern	void	Rec_FontWin_Init(BYTE winno, WORD start_addr, WORD x, WORD y,BYTE w, BYTE h, BYTE zoomH, BYTE zoomV);
extern	void	Rec_FontRam_Disp(WORD start_addr, WORD ch, BYTE fcolor, BYTE bcolor, WORD len);
extern	void	Draw_Font_with_AutoInc(WORD addr, WORD font, BYTE fcolor, BYTE bcolor, WORD length);
extern	void	FontOsdWinOffAll(void);

extern	void	Init_Setup_VideoVar_Bar(void);
extern	void	Init_Setup_Version_Bar(void);

//Touch
extern	void	Touch_Scan(void);
extern	void	Touch_Main(void);
extern	void	Touch_Aux(void);
extern	void	Touch_DVD(void);
extern	void	Touch_VGA(void);
extern	void	Touch_HDMI(void);
extern	void	Touch_Trace(void);
extern	void	Touch_Setup(void);
extern	void	Touch_OSDDemo(void);

extern	BYTE	Draw_When_Button_First_Press(void);

//I2C
//extern	void I2CDeviceInitialize(BYTE *RegSet, BYTE delay);

//extern	void	BackLight( BYTE on );
extern	void 	OutputEnablePin(BYTE fFPPin, BYTE fOutputPin);
//extern	void	LCDOnOff(BYTE on);

//Group
extern	void	Init_Main(void);
extern	void	Init_Aux(void);
extern	void	Init_DVD(void);
extern	void	Init_VGA(void);
extern	void	Init_HDMI(void);
extern	void	Init_Trace(void);
extern	void	Init_Setup(void);
extern	void	Init_OSDDemo(void);

extern	BYTE	If_Display_Aux_OSD(void);
extern	BYTE	If_Display_DVD_OSD(void);
extern	BYTE	If_Display_VGA_OSD(void);
extern	BYTE	If_Display_HDMI_OSD(void);
extern	void	If_Hide_Video_OSD(void);

extern	void	Refresh_BackCar_Trace(void);
extern	void	Refresh_BackCar_Radar(void);
extern	void	Refresh_BackCar_Status(void);

//Image
extern	void	Hide_Video(BYTE on);

extern	void	Check_Change_CVBS(void);

extern	void	Change_Video_Bright(char flag);
extern	void	Change_Video_Contrast(char flag);
extern	void	Change_Video_Hue(char flag);


//Measure
extern	WORD	MeasGetHActive( WORD *start );
extern	void	MeasGetAndSetHStart(BYTE force );

extern	void	CheckAndSet_VGAMode(void);
extern	void	CheckAndSet_HDMIMode(void);

extern	BYTE 	MeasFindInputMode(BYTE fDTV);
extern	BYTE	CheckAndSetPC( void );
extern	BYTE	CheckVGAMode( void );

//Check Video
extern	BYTE	Check_DVD_YUV(void);
extern	BYTE	Check_VGA_RGB(void);
extern	void	Check_Video(void);
extern	BYTE	Check_HDMI_Signal(void);

//LVDS TX
extern	void	Enable_LVDS_TX( BYTE en );


//
extern	void	Set_Panel_HVTotal( WORD htotal, WORD vtotal ,BYTE mode );

//HDMI
extern	BYTE 	CheckHDMIMode(void);
extern	BYTE 	CheckAndSetHDMI(void);

extern	WORD	GetHActive( void );


#endif // __EXTERN_H__
