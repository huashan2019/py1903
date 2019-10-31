#define	TFT_ROOT
#define __EXTERN_H__

#include	"include.h"
#include	"Tw8836.txt"
#define uchar	SCH_U8

#if	_UartDebug
#define	_DebugTw8816Comm		0
#define	_DebugTw8816Video		0
#define	_DebugTw8816Init		0
#define	_DebugTw8816I2C			0
#define	_DebugStd     		  	0
#define	_DebugBar     			0
#endif

const	UNCHAR	*pRangRom;
UNCHAR		rOSDDispAddr;
UNCHAR		rOSDDispNum;
UNCHAR		rOSDCharAddr;

UNCHAR	  tw8836[0x0f];
UNCHAR	  tw8836_temp;
extern	void OsdImgLoad(BYTE sosd_win, DWORD spi_addr, WORD alpha);
extern	void I2CDeviceInitialize(BYTE *RegSet, BYTE delay);
extern	void	Init_OSD(void);
extern	void	Init_Trace(void);
extern	void	Refresh_BackCar_Status(void);
extern	void	OSDWinGlobalAlpha( BYTE winno, BYTE alpha );
extern	SCH_U8 I2C1In8Bit(void);


///===================SCL1,SDA1============================      
#define GPIO_SCL1_CTL      	   GPIO_PTH4//////
#define T8836On_SCL        	   GPIO_PinSet(GPIO_SCL1_CTL)
#define T8836Off_SCL       	   GPIO_PinClear(GPIO_SCL1_CTL)

#define GPIO_SDA1_CTL      	   GPIO_PTH3//////
#define T8836On_SDA        	   GPIO_PinSet(GPIO_SDA1_CTL)
#define T8836Off_SDA       	   GPIO_PinClear(GPIO_SDA1_CTL)

#define SetTw8816I2CBusIn	GPIO_Init(GPIOB,0x08000000,GPIO_PinInput_InternalPullup)
#define SetTw8816I2CBusOut	GPIO_Init(GPIOB,0x08000000,GPIO_PinOutput)

#define iTw8816_Sda		GPIO_Read(GPIOB) & 0x08000000
//#define _ENABLE_8836_I2C 0



extern	void I2C1Start(void);

extern	void I2C1Stop(void);
extern SCH_U8 I2C1Out8Bit(SCH_U8 outbyte);




void delay2us(U8 d)
{
	U8 i = 0;
	for(i=0;i<6*d;i++)
	;
}
void delay1ms(U8 cnt_1ms)
{
	U32 i = 0;
	for(i=0;i<cnt_1ms;i++)
		for(i=0;i<10000;i++)
			;
}



/*SDA falling edge when SCL is high will start I2C bus*/
void    StartTw8816I2CBus(void)
{
#if _ENABLE_8836_I2C
	T8836On_SDA;
	delay2us(1);
	T8836On_SCL; 
	delay2us(1);
	T8836Off_SDA;
	delay2us(1);
	T8836Off_SCL;
#else
	I2C1Start();
#endif
}

/*---------------------------------------------------------------------------------------*/
/* rising edge when SCL is high will stop I2C bus */
void    StopTw8816I2CBus(void)
{
#if _ENABLE_8836_I2C
	T8836Off_SDA;
	delay2us(1);
	T8836On_SCL;
	delay2us(1);
	T8836On_SDA;
	delay2us(1);
	T8836Off_SCL;  
#else
	I2C1Stop();
#endif
}

#if _DebugTw8816I2C

/*******************************************************************************\
* Function   : wait ACK.                   										*
*  Input     : none					                                            *
* Output     : none                                                        		*
\*-----------------------------------------------------------------------------*/
void	WaitTw8816I2CACK(void)
{
#if _ENABLE_8836_I2C
	uchar	c;

	// start wait ACK bit from slave
	SetTw8816I2CBusIn();
	c = 20;
	T8836On_SCL;
	delay2us(1);
	for( ; ; )
	{
		if(!iTw8816_Sda)	break;
		--c;
		if(c == 0)	break;
	}
	delay2us(1);
	T8836Off_SCL;// ACK bit clock
	SetTw8816I2CBusOut();
    
   // if(!c)  UartOutputString("Tw8816 Err\n");
	//else
		//UartOutputString("8ok\n");
#else

#endif
}
#endif

void SendTw8816I2CByte(uchar cData)
{
#if _ENABLE_8836_I2C
	uchar i;

	for(i=0; i<8; i++)
	{
		if(cData & 0x80)	T8836On_SDA;
		else	T8836Off_SDA;
		cData <<= 1;
		delay2us(1);
		T8836On_SCL;
		delay2us(1);
		//delay1ms(1);
		T8836Off_SCL;
	}
	//ninth clock for request ACK bit
	//oI2CData = 0;
#if _DebugTw8816I2C
	WaitTw8816I2CACK( );
#else
	delay2us(1);
	T8836On_SCL;
	delay2us(1);
	T8836On_SDA;
	delay2us(1);
	T8836Off_SCL;
	delay2us(1);
#endif
#else
	I2C1Out8Bit(cData);
#endif
}

uchar ReadTw8816I2CByte(void)
{
	uchar d;
#if _ENABLE_8836_I2C
	uchar i, d;
	SetTw8816I2CBusIn;
	delay2us(1);
	d = 0;
	for(i = 8; ; )
	{
		T8836On_SCL;
    		d <<= 1;
		if(GPIO_Read(GPIOB) & 0x08000000)	
		{
			d |= 0x01;
		}
		else
		{
			d |= 0x00;
		}
		delay2us(1);			
		T8836Off_SCL;
		--i;
		if(i == 0)   break;
		delay2us(1);
	}
	SetTw8816I2CBusOut;
	return d;
#else
	d = I2C1In8Bit();
	return d;
#endif
}


void WriteTw8816Reg(uchar cReg, uchar cData)
{
	StartTw8816I2CBus();
	SendTw8816I2CByte(WrTw8816Addr);
	SendTw8816I2CByte(cReg); 
	SendTw8816I2CByte(cData); 
	StopTw8816I2CBus();
}
void WriteTw8816Regs(uchar cReg, uint cNum, uchar *cData)
{
#if _ENABLE_8836_I2C
	StartTw8816I2CBus();

	SendTw8816I2CByte(WrTw8816Addr);
	SendTw8816I2CByte(cReg); 
	while(cNum--)
	{
		SendTw8816I2CByte(*cData);
		cData++;
	}

	StopTw8816I2CBus();
#else
	I2C1_Tx_A(WrTw8816Addr,cReg,cData,cNum);
#endif
}


uchar ReadTw8816Reg(uchar cReg)
{
#if 1
	uchar  result;
	StartTw8816I2CBus();
	SendTw8816I2CByte(WrTw8816Addr);
	SendTw8816I2CByte(cReg);
	//StopTw8816I2CBus();

	StartTw8816I2CBus();
	SendTw8816I2CByte((uchar)(WrTw8816Addr | 0x01));
	result = ReadTw8816I2CByte();
	StopTw8816I2CBus();
#else
	uchar *result;
	ReadTw8816Regs(WrTw8816Addr,cReg, &result, 1);
#endif
	return (result);
}

void ReadTw8816Regs(uchar cDevAddr, uchar cReg, uchar *cData, uchar cNum)
{
#if _ENABLE_8836_I2C
	StartTw8816I2CBus();
	SendTw8816I2CByte(cDevAddr);
	SendTw8816I2CByte(cReg);

	StartTw8816I2CBus();
	SendTw8816I2CByte(cDevAddr | 0x01);
	while (cNum--)
	{
		*cData = ReadTw8816I2CByte();
		cData++;
	}

	StopTw8816I2CBus();
#else
	I2C1_Rx_A(cDevAddr,cReg,cData,cNum);
#endif
}

int tw8836_write_page_reg(unsigned short reg, unsigned char value)
{
//    int ret = -1; 
    WriteTw8816Reg(0xff,reg >>8);// switch page
   // return 
   WriteTw8816Reg( reg&0xff,value);
   return 0;
}

unsigned char tw8836_read_page_reg(unsigned short reg)
{
    WriteTw8816Reg(0xff,reg >>8);// switch page
    return ReadTw8816Reg(reg&0xff);
}

uchar	DetSignalInTw8836(void)
{
	WriteTw8816Reg(0xff, 0x00);
	return (ReadTW88(0x04) & 0x01);
}

void SelectOsdWindow(uchar win)	
{

}

void OSDDisable(void)
{

}





extern BYTE	PANELDATA_CVBS_ALL[];
#if (CUR_PANEL == HANNSTAR_PANEL_7INCH_1024)
extern BYTE	PANELDATA_CVBS_ALL_REVB[];
extern BYTE	PANELDATA_VGA_RGB_REVB[];
#endif
extern BYTE	PANELDATA_CVBS_NTSC[];
extern BYTE	PANELDATA_CVBS_PAL[];
extern BYTE	PANELDATA_DVD_YUV[];
extern BYTE	PANELDATA_VGA_RGB[];
extern BYTE	PANELDATA_DTV_RGB[];


void	BackLight( BYTE on )
{
	//PIN_BACKLIGHT = on;
	//Write_GPIO(PIN_BACKLIGHT,on);
}

void	Hide_Video(BYTE on)
{
 	if( on == 1 )
 	{ 		
		WriteTW88(0xff,2);
		WriteTW88(PAGE2_IMAGE|0x84,0);
		WriteTW88(PAGE2_IMAGE|0x85,0);
		WriteTW88(PAGE2_IMAGE|0x86,0);
		WriteTW88(PAGE2_IMAGE|0x8a,0);
 	}
 	else
 	{
		WriteTW88(0xff,2);
		WriteTW88(PAGE2_IMAGE|0x84,0x80);
		WriteTW88(PAGE2_IMAGE|0x85,0x80);
		WriteTW88(PAGE2_IMAGE|0x86,0x80);
		WriteTW88(PAGE2_IMAGE|0x8a,0x80);
 	}
}



BYTE MeasStartMeasure(void)
{
	volatile BYTE val;
	BYTE i;
	BYTE	page;

	page = ReadTW88( 0xff );

	//Puts("\nMeasStart..");

	// StartMeasurement
	WriteTW88Page( 0x05 );					// Measurement page : 5

	WriteTW88( REG508, ReadTW88(REG508) | 1 );						// Measure start

	// WaitMeasurementDataReady
	for(i=0; i<200; i++) {			// make more delay
		delay1ms(10);	   			// 10ms delay 
		val = ReadTW88(REG508);
		if((val & 0x01) == 0) {
			Printf("%bd_OK",i);
			WriteTW88( 0xff, page );
			return 0;
		}
	}
	//dPrintf("-->fail");
	//Puts("\n Fail!!!");
	WriteTW88( 0xff, page);
	return 1;						// means timeout
}

void InputSetHStart( WORD x)
{
	WriteTW88Page( PAGE0_INPUT );

	WriteTW88(REG040, (ReadTW88(REG040) & 0x3F) | ((x & 0xF00)>>2) );
	WriteTW88(REG045, (BYTE)x);
	Printf("\nInput Crop Window: x = %d", x);
}

WORD	GetHActive( void )
{
	WORD	start,period, end;
	BYTE	page;

	page = ReadTW88( 0xff );

	WriteTW88Page( 0x05 );

	Read2TW88(REG52E, REG52F, period);
	Read2TW88(REG530, REG531, start);
	if ( start < period )	//select big
		start = period;

	Read2TW88(REG532, REG533, period);
	Read2TW88(REG534, REG535, end);
	if ( end > period )		//select small
		end = period;

	period = end - start + 1;

	WriteTW88( 0xff, page );
	return( period );
}


void	MeasGetAndSetHStart( BYTE force )				//测量得出HStart的值并且设置之
{
//	BYTE val;
	WORD hstart,period;
	static WORD hstart_old = 0xFFFF;
	
	//val = 
	MeasStartMeasure();
	
	//Read2TW88(REG52E, REG52F, hstart);
	hstart = ReadTW88(REG52E)<<8|ReadTW88(REG52F);
	
	Printf("\nhstart = %x",hstart);
	
	hstart += 6;	
	hstart -= 1;

	if( g_Cur_PanelData == e_PANELDATA_DRGB_HDMI )										//仅HDMI 特殊处理，VGA不处理
	{
		period = GetHActive();		 
		if( (period >= (g_H_Resolution + 80)) || (period <= (g_H_Resolution - 80)))		//means 竖屏 
			hstart -= 0x290;															//竖屏居中处理 (特殊处理)
		Printf("\ng_H_Resolution = %d,period = %d",g_H_Resolution,period);
	}
	
	Printf("\nhstart = %x",hstart);
	
	if( hstart != hstart_old || force == 1 )
	{
		hstart_old = hstart;
		InputSetHStart(hstart);	
	}	
}


//*****************************************************************************
//*      CVBS NTSC ->  CVBS
//*****************************************************************************			
void 	PanelData_To_CVBS(void)
{
	BackLight(OFF);
	delay1ms(30);
	//Enable_LVDS_TX(0);
 	//PanelMute(1);
	if( g_TW8836_Rev == REV_A )	
		I2CDeviceInitialize(PANELDATA_CVBS_ALL,3);
	else	//RevB
	{
	#if (CUR_PANEL == HANNSTAR_PANEL_7INCH_1024)
		//I2CDeviceInitialize(PANELDATA_CVBS_ALL_REVB,3);
	#else
		I2CDeviceInitialize(PANELDATA_CVBS_ALL,3);
	#endif
	}
	//PanelMute(0);
	//Enable_LVDS_TX(1);
	delay1ms(70);
	BackLight(ON);
	
	
}

void 	PanelData_To_CVBS_NTSC(void)
{
 	//PanelMute(1);
	I2CDeviceInitialize(PANELDATA_CVBS_NTSC,0);
}

void 	PanelData_To_CVBS_PAL(void)
{
 	//PanelMute(1);
	I2CDeviceInitialize(PANELDATA_CVBS_PAL,0);
}

//*****************************************************************************
//*      CVBS ->  ARGB
//*****************************************************************************			
void 	PanelData_To_YUV(void)
{
	BackLight(OFF);
	delay1ms(30);
	//Enable_LVDS_TX(0);
	//PanelMute(1);
	I2CDeviceInitialize(PANELDATA_DVD_YUV,0);
	//PanelMute(0);
	//Enable_LVDS_TX(1);
	delay1ms(70);
	BackLight(ON);
}

//*****************************************************************************
//*      CVBS ->  ARGB
//*****************************************************************************			
void 	PanelData_To_ARGB(void)
{
	BackLight(OFF);
	delay1ms(30);
	//Enable_LVDS_TX(0);
	//PanelMute(1);
	if( g_TW8836_Rev == REV_A )	
		I2CDeviceInitialize(PANELDATA_VGA_RGB,3);
	else
	{
		#if (CUR_PANEL == HANNSTAR_PANEL_7INCH_1024)
		I2CDeviceInitialize(PANELDATA_VGA_RGB_REVB,3);
		#else
		I2CDeviceInitialize(PANELDATA_VGA_RGB,3);
		#endif
	}
	delay1ms(100);
	MeasGetAndSetHStart(1);
	InputVGA_Resolution = VGA_RESOLUTION_1366X768;
	//PanelMute(0);
	//Enable_LVDS_TX(1);
	delay1ms(70);
	BackLight(ON);	
}


//*****************************************************************************
//*      CVBS ->  DRGB
//*****************************************************************************			
void 	PanelData_To_DRGB(void)
{
	BackLight(OFF);
	delay1ms(30);
 	//PanelMute(1);
	I2CDeviceInitialize(PANELDATA_DTV_RGB,0);
	delay1ms(100);
	MeasGetAndSetHStart(1);

	InputHDMI_Resolution = HDMI_RESOLUTION_1280X720;

	delay1ms(70);
	BackLight(ON);
}



//*****************************************************************************
//*      Convert Panel data from one to another
//*****************************************************************************
void	PanelData_Switch(E_PNLDATA to)			   
{
	if(g_Cur_PanelData == to)
		return;

   g_Cur_PanelData = to;
	
	switch(to)
	{
		case e_PANELDATA_CVBS_ALL:		PanelData_To_CVBS();	g_CVBS_For_PAL = 0;		break;	  //default for NTSC
		case e_PANELDATA_CVBS_N:		PanelData_To_CVBS_NTSC();						break;
		case e_PANELDATA_CVBS_P:		PanelData_To_CVBS_PAL();						break;
		case e_PANELDATA_YUV_DVD:  		PanelData_To_YUV();								break;
		case e_PANELDATA_ARGB_VGA:  	PanelData_To_ARGB();							break;
		case e_PANELDATA_DRGB_HDMI:   	PanelData_To_DRGB();							break;

		default:																		break;
	}
	
 
}

void	GobalAlpha_GradualProcess( BYTE winno , BYTE on )		//Process of Gradual Change          //渐变处理
{
	BYTE i;

	if( on == 1 )
	{
		OSDWinGlobalAlpha(winno,0x7F);
		SpiOsdWinHWEnable(winno,ON);
	
		for( i = 0x7F; i > 0; i -- )
		{
			OSDWinGlobalAlpha(winno,i);
			delay1ms(5);
		}	
	}
	else
	{
		OSDWinGlobalAlpha(winno,0x00);
		SpiOsdWinHWEnable(winno,ON);
		for( i = 0; i < 0x7F; i ++ )
		{
			OSDWinGlobalAlpha(winno,i);
			delay1ms(5);
		}
		//SpiOsdWinHWEnable(winno,OFF);	
	}	
	
}	

BYTE	Get_TW8836_Version( void )
{
	BYTE Rev;
	WriteTW88Page(PAGE0_GENERAL);
	Rev = ReadTW88(PAGE0_GENERAL|0x01);

	if( Rev == REV_A )
		Printf("TW8836 Version: A");
	else if( Rev == REV_B )
		Printf("TW8836 Version: B");
	else
		Printf("TW8836 Version: Unknown");
	return Rev;
}

void	Set_UB927_Reg( void )
{
	WriteTw8816Reg(0x03,0xda);
	WriteTw8816Reg(0x07,0x58);
	WriteTw8816Reg(0x08,0x58);
}
//=============================================================================
//			                                               
//=============================================================================
void	InitCore(void)
{
	//Puts("\nInitCore");	
	//----- Set SPI mode
	SPI_QUADInit();
	SPI_Init();

	//----- Enable Chip Interrupt
	WriteTW88Page( PAGE0_GENERAL );
	WriteTW88( PAGE0_GENERAL|0x02, 0xFF );	// Clear Pending Interrupts				//??
	WriteTW88( PAGE0_GENERAL|0x03, 0xEE );	// enable SW. enable SW INTR7FF			//??

}

void	InitClock(void)
{
	// init clock
 	//SspllSetFreqAndPll(108000000L);
 	SspllSetFreqAndPll(72000000L);
	//McuSpiClkSet(PCLK_SEL_PLL,SPI_CK_SEL_27M,SPI_CK_DIV_1P5); 
	//McuSpiClkSet(PCLK_SEL_PCLK,SPI_CK_SEL_27M,SPI_CK_DIV_1P5);
	McuSpiClkSet(PCLK_SEL_PCLK,SPI_CK_SEL_PCLK,SPI_CK_DIV_1P0);
		temp_data[0] = ReadTW88(0xe1);

}



//LVDS TX
void	Enable_LVDS_TX( BYTE en )
{

	WriteTW88Page( PAGE6_LVDS );
	if( en )	
	{
		WaitVBlank(1);
		WriteTW88( PAGE6_LVDS | 0x40 , 0x0C);//ReadTW88( PAGE6_LVDS | 0x40 ) | 0x04 );	 //Enable  LVDS TX
	}
	else
	{
		WaitVBlank(1);		
		WriteTW88( PAGE6_LVDS | 0x40 , 0x08);//ReadTW88( PAGE6_LVDS | 0x40 ) & 0xFB );	 //Disable LVDS TX
	}
		
}


void MeasSetWindow( WORD x, WORD y, WORD w, WORD h )
{	
	//TW8835 needs minimum 2 Horizontal start position
	if(x < 2)
		x = 2;

	WriteTW88Page(0x05 );
	Write2TW88(REG500_MEAS_HSTART_MSB, REG501_MEAS_HSTART_LSB, x);		
	Write2TW88(REG502_MEAS_HLEN_MSB,   REG503_MEAS_HLEN_LSB,   w);

	Write2TW88(REG505_MEAS_VSTART_MSB, REG505_MEAS_VSTART_LSB, y);		
	Write2TW88(REG506_MEAS_VLEN_MSB,   REG507_MEAS_VLEN_LSB,   h);
}

void MeasSetField(BYTE field)
{
	WriteTW88Page(0x05 );
	WriteTW88(REG508, ReadTW88(REG508) & 0xF3 | (field << 2) );	// Note:DO not turn on the start
}

void	InitMeasure(void)
{
	MeasSetWindow( 32, 32, 1600, 1200 );
	MeasSetField(2);					//both	
}


void	LCDOnOff(BYTE on)
{
	//PIN_LCD = on;
	//Write_GPIO(PIN_LCD,on);
}

void	Initialize(void)
{
	
	g_TW8836_Rev = Get_TW8836_Version(); 		//Read TW8836 Version

	//Enable_LVDS_TX(1);							//Enable LVDS Output				
	
	InitCore();
	InitClock();				 					
	//FP Data Out
#if ( PCB_VER == DEMO_INTERSIL )
	OutputEnablePin(ON,ON);						//Output enable. FP data: enable  	LCD On
#else
	LCDOnOff(ON);					 
#endif

	delay1ms(15);
	Hide_Video(ON);
	//g_Cur_PanelData = 0xff;
	//PanelData_Switch(e_PANELDATA_CVBS_ALL);			//Init PanelData											    	
	//DownLoadGamma(); 								//DownLoad Gamma	

	Init_OSD();
									
	//InitTouch();
	//InitMeasure();

	//delay1ms(200);
	//BackLight(ON);									//Backlight	
	Init_Trace();

	//InitLogo();										//Logo
	
	//Init_Main();									//Main
	
}
extern void osd_test(void);

void SetTw8816VideoSrcProc( )
{
	//extern menu_image_header_t header_table;
	//menu_image_header_t *header = &header_table;

	uchar  i;

	switch(rTw8816CtrlSubSt)
	{
		case    0:
			 return;
			
		case    1:
			bDispNoSignalset =0;
			rTw8816CtrlSubSt++;	
			g_Cur_PanelData = 0xfe;
			Initialize();
			break;
			
	    case    2:
	    		Tw8816Ctrl_X4ms = 0/4;
			if(rARMPage == PAGE_CAM)
			{
	           		 if(rImageMode == _NTSC) rSrcStdIdx = 1;  			// CVBS NTSC				    
	             		else rSrcStdIdx = 2;  					   			// CVBS PAL			
			}
			else
			{
				rSrcStdIdx = 4; 	// RGB 888
			}
           		 if(rSrcStdIdx == 1)	rTw8816CtrlSubSt = 3;
			else if(rSrcStdIdx == 2)	rTw8816CtrlSubSt = 4;
			else	rTw8816CtrlSubSt = 5;
	 		rSrcStdIdxBck = rSrcStdIdx;
			g_Cur_PanelData = 0xFF;
			//osd_test();
			
			//OsdImgLoad(2,0x0,0,&header_table);

			//OsdImgLoad(1,0x0000,0xff);
			bTw8816CtrlSt = 0;
		        break;

		case	3:
			 if(!bTw8816CtrlSt)
			{
				bTw8816CtrlSt = 1;
				pRangRom = &cvbs_ntsc_tab[0];
			}
			else
			{
			#if 0
				PanelData_Switch(e_PANELDATA_CVBS_ALL); 		//Init PanelData	
				
				bTw8816CtrlSt = 0;						
				rTw8816CtrlSubSt = 0xfe;
				break;
			#else
				for (i=0; i<15; i++)
				{
					WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
					pRangRom += 2;
					if(pRangRom >= &cvbs_ntsc_tab[0] + sizeof(cvbs_ntsc_tab))
					{
						bTw8816CtrlSt = 0;						
						rTw8816CtrlSubSt = 0xfe;
						//osd_test();	
						//Set_UB927_Reg();
						Initialize();
						break;
					}
				}
			#endif
			}
           		break;
			
		case	4:
			if(!bTw8816CtrlSt)
			{
			    bTw8816CtrlSt = 1;
			    pRangRom = &cvbs_pal_tab[0];
			}
			else
			{
			#if 1
				//PanelData_Switch(e_PANELDATA_CVBS_P); 		//Init PanelData		
				Initialize();
				bTw8816CtrlSt = 0;						
				rTw8816CtrlSubSt = 0xfe;
				break;
			#else
				WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
				pRangRom += 2;
				if(pRangRom >= &cvbs_pal_tab[0] + sizeof(cvbs_pal_tab))
				{
					bTw8816CtrlSt = 0;						
					rTw8816CtrlSubSt = 0xfe;
					break;
				}
			#endif
			}
            		break;	

		case    0x05:	
			if(!bTw8816CtrlSt)
			{
			    bTw8816CtrlSt = 1;
			    pRangRom = &Init_TW8816Page[0];
				SpiOsdEnable(OFF);
			}
			else
			{
				for (i=0; i<15; i++)
				{
					WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
					pRangRom += 2;
					if(pRangRom >= &Init_TW8816Page[0] + sizeof(Init_TW8816Page))
					{
						bTw8816CtrlSt = 0;						
						rTw8816CtrlSubSt = 0xff;
						// I2C1_SCL_DDR_0;
						// I2C1_SDA_DDR_0;
						//Initialize();
						break;
					}
				}
			}
			break;

		case    0xfe:
			//if(Tw8816Ctrl_X4ms)	return;
			//Tw8816Ctrl_X4ms = 500/4;
			Refresh_BackCar_Status();	//Refresh BackCar Trace & Radar
			break;
			
		default:
			//WriteTw8816Reg(0xff,0);/*test wr/rd reg is OK or NG?*/
			//for(i=0;i<16;i)
			//{
				// ReadTw8816Regs(WrTw8816Addr,i,&temp_data[i],0x8);
				 //i+=8;
		//	}
			if(bCarReverseOn != Get_REV_Flag)	
			{
				//if(Get_REV_Flag)
				//rTw8816CtrlSubSt = 0x03;
				//else rTw8816CtrlSubSt = 0x05;
				//bCarReverseOn = Get_REV_Flag;
			}
			break;
	}	
}


void InitTw8816Proc(void)
{
    uchar   i;
	
	switch(rTw8816CtrlSubSt)
	{
		case	1:
		if(!bTw8816CtrlSt)
		{
			TFTLamp_X16ms = 2000/16;
			bTw8816CtrlSt = 1;
			Tw8816Ctrl_X4ms = 200/4;
		}
		else
		{
			if(Tw8816Ctrl_X4ms)   return;
			bTw8816CtrlSt = 0;
			rTw8816CtrlSubSt++; 
		}
		break;
			
        case    2:
		if(!bTw8816CtrlSt)
		{
			bTw8816CtrlSt = 1;
			pRangRom = &Init_TW8816Page0[0];
		}
		else
		{
			for (i = 0; i < 16; i++)
			{
				if(*pRangRom == 0xFD)
				{
					WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
					bTw8816CtrlSt = 0;						
				    	rTw8816CtrlSubSt++;
					break;
				}
				else    
				{
					WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
					//tw8836[i] = ReadTw8816Reg(*pRangRom); //test2017
				}
				pRangRom += 2;
			}
		}
		break;
		
	case	3:
		if(!bTw8816CtrlSt)
		{
		    bTw8816CtrlSt = 1;
		    pRangRom = &Init_TW8816Page1[0];
		}
		else
		{
			for (i = 0; i < 16; i++)
			{
			    if(*pRangRom == 0xFA)
			    {
					WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
						bTw8816CtrlSt = 0;						
					rTw8816CtrlSubSt++;
						break;
			    }				
				else    WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
					pRangRom += 2;
			}
		}
        break;

	case 4:
		if(!bTw8816CtrlSt)
		{
		    bTw8816CtrlSt = 1;
		    pRangRom = &Init_TW8816Page2[0];
		}
		else
		{
			for (i = 0; i < 16; i++)
			{
			    if(*pRangRom == 0xF9)
			    {
					WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
						bTw8816CtrlSt = 0;						
					rTw8816CtrlSubSt++;
						break;
			    }				
				else    WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
					pRangRom += 2;
			}
		}
	break;

	case 5:
		if(!bTw8816CtrlSt)
		{
		    bTw8816CtrlSt = 1;
		    pRangRom = &Init_TW8816Page4[0];
		}
		else
		{
			for (i = 0; i < 16; i++)
			{
				if(*pRangRom == 0xFE)
				{
					WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
					bTw8816CtrlSt = 0;						
					rTw8816CtrlSubSt++;
					break;
				}				
				else    WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
				pRangRom += 2;
			}
		}
	break;

	case 6:
		if(!bTw8816CtrlSt)
		{
		    bTw8816CtrlSt = 1;
		    pRangRom = &Init_TW8816Page6[0];
		}
		else
		{
			for (i = 0; i < 16; i++)
			{
			    if(*pRangRom == 0x4E)
			    {
					WriteTw8816Reg(*pRangRom, *(pRangRom + 1));

						bTw8816CtrlSt = 0;						
					rTw8816CtrlSubSt++;


					//SOsd_init();
					//SpiOsdSetDeValue();
					//SOsd_CleanReg();
					//SOsd_CleanLut();
					//SOsd_CleanRlc();
					//SOsd_UpdateReg(0,8);
					//SOsd_UpdateRlc();
					//SpiOsdEnable(ON);
					//InitLogo1();
					//proc_SOsd_demo1_start();

					//WaitVBlank(1);
					//MenuEnd();	
					//SOsdDemo1();
					//InitInputAsDefault();

					//RoseDemo(  );
					//proc_pigeon_demo_start();
					break;
			    }				
				else    WriteTw8816Reg(*pRangRom, *(pRangRom + 1));
					pRangRom += 2;
			}
		}
	break;


      }
}

void	Tw8816CtrlProc( )
{	
	//if(I2C0_IsFindAddr(WrTw8816Addr))
	{
		//Printf("tw8836 check OK --- %x \n",WrTw8816Addr);
	}
	//else
	{
		//Printf("tw8836 check err --- %x \n",WrTw8816Addr);
	}
	if(g_Radar_delay)		g_Radar_delay--;
	if(g_Trace_delay)		g_Trace_delay--;

	if(Tw8816Ctrl_X4ms)	--Tw8816Ctrl_X4ms;
	if(rTw8816CtrlSt == _InitTw8816 )
	{
		InitTw8816Proc( );
	}
	else if(rTw8816CtrlSt == _SetTw8816Src )
		SetTw8816VideoSrcProc( );	
}


