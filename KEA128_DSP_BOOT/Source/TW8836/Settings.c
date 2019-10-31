/*
 *  settings.c -  
 *
 *  Copyright (C) 2011 Intersil Corporation
 *
 */
//input			
//	CVBS	YIN0
//	SVIDEO	YIN1, CIN0
//	aRGB	G:YIN2 B:CIN0 R:VIN0 
//	aYUV	G:YIN2 B:CIN0 R:VIN0
#define SETTINGS_ROOT

#include "include.h"

/*
result  register        stepA	stepB	stepC	stepD	stepE
------	--------		-----	-----	-----	-----	-----
fail	PCLK_SEL  		PLL				 		PCLK
		SPI_CK_SEL		PCLK	27M						PCLK
		SPI_CK_DIV		1		   		0				1

OK		PCLK_SEL  		PLL				 		PCLK
		SPI_CK_SEL		PCLK	27M						PCLK
		SPI_CK_DIV		1		   		 				1

OK		PCLK_SEL  		PLL				 		PCLK
		SPI_CK_SEL		PCLK
		SPI_CK_DIV		1
---------------------------------------------------------------
*/
BYTE shadow_r4e0;
BYTE shadow_r4e1;

BYTE McuSpiClkToPclk(BYTE divider)
{
	WriteTW88Page(PAGE4_CLOCK);
	shadow_r4e0=ReadTW88(PAGE4_CLOCK|0xE0);
	if(shadow_r4e0 & 0x01) {
		shadow_r4e1 = ReadTW88(PAGE4_CLOCK|0xE1);
		WriteTW88(PAGE4_CLOCK|0xE0, shadow_r4e0 & 0xFE);
		WriteTW88(PAGE4_CLOCK|0xE1, 0x20 | divider);
		return 1;
	}
	return 0;
}

void McuSpiClkRestore(void)
{
	if(shadow_r4e0 & 0x01) {
		WriteTW88Page(PAGE4_CLOCK);
		WriteTW88(PAGE4_CLOCK|0xE0, shadow_r4e0);
		WriteTW88(PAGE4_CLOCK|0xE1, shadow_r4e1);
	}
}


void McuSpiClkSet(BYTE PCLK_SEL,BYTE SPI_CK_SEL,BYTE SPI_CK_DIV) 
{
	WriteTW88Page(PAGE4_CLOCK);
	if(PCLK_SEL)	WriteTW88(PAGE4_CLOCK|0xE0, ReadTW88(PAGE4_CLOCK|0xE0) |  0x01);
	else			WriteTW88(PAGE4_CLOCK|0xE0, ReadTW88(PAGE4_CLOCK|0xE0) & ~0x01); 
	WriteTW88(PAGE4_CLOCK|0xE1, (SPI_CK_SEL << 4) | SPI_CK_DIV);
}

//desc
// PLL = 108MHz *FPLL / 2^17
// FPLL = PLL * 2^17 / 108MHz
//		= PLL * 131072 / 108MHz
//		= PLL * 131072 / (108 * 100000 * 10)Hz
//      = (PLL / 100000) * (131072 / 108)* (1/10)
//      = (PLL / 100000) * (1213.6296) * (1/10)
//      = (PLL / 100000) * (1213.6296 *2 ) * (1/10*2) 
//      = (PLL / 100000) * (2427.2592) / 20 
//      = (PLL / 100000) * (2427) / 20 
//parameter
//	_PPF: input pixel clock
//oldname:ChangeInternPLL@TW8816
//nickname: SetPclk
//description
//	set PCLKO divider and CLK polarity
//parameter
//	pol - Pixel clock output polarity
//		0:	no inversion
//		1:	inversion
//		0xFF: do not change it. Use previous value
//	div - Pixel clock output frequency division
//		0:div 1,	1:div 2,	2:div 3,	3:div 4.
void PclkoSetDiv(BYTE pol, BYTE div)
{
	BYTE value;
	WriteTW88Page(PAGE2_SCALER);
	value = ReadTW88(PAGE2_SCALER|0x0D);
	if(pol != 0xFF) {
		value &= 0xEC;
		if(pol) value |= 0x10;	
	}
	else {
		value &= 0xFC;
	}
	//dPrintf("\nPclkoSetDiv(%bx,%bx) value:0x%bx|0x%bx",pol,div,value,div);
	WriteTW88(PAGE2_SCALER|0x0D, value | div);
}

void SspllSetFreqAndPll(DWORD _PPF)
{
	BYTE	ppf, CURR, VCO, POST, i;
	DWORD	FPLL;
	
//	BYTE	val;	


	//dPrintf("\nSspllSetFreqAndPll(%ld)",_PPF);
	ppf = _PPF/1000000L;		//base:1Hz

	//----- Frequency Range --------------------
	if     ( ppf < 27 )  { VCO=2; CURR=0; POST=2; }		// step = 0.5MHz
	else if( ppf < 54 )  { VCO=2; CURR=1; POST=1; }		// step = 1.0MHz
	else if( ppf < 108 ) { VCO=2; CURR=2; POST=0; }		// step = 1.0MHz
	else                 { VCO=3; CURR=3; POST=0; }		// step = 1.0MHz

	CURR = VCO+1;	//BK110721. Harry Suggest.

	//----- Get FBDN
	
	FPLL = _PPF/1000L;	//base:10Hz

	i = POST;
	for(; i>0; i-- )
		FPLL *= 2;

	FPLL <<= 12;
	
	FPLL = FPLL / 3375L;		//Base:1Hz

	//----- Setting Registers : below is different with 8806
	WriteTW88Page(PAGE0_SSPLL);
	WriteTW88( PAGE0_SSPLL|TW88_FPLL0, (FPLL>>16));
	WriteTW88( PAGE0_SSPLL|TW88_FPLL1, (BYTE)(FPLL>>8));
	WriteTW88( PAGE0_SSPLL|TW88_FPLL2, (BYTE)FPLL );
	WriteTW88( PAGE0_SSPLL|TW88_SSPLL_ANALOG, (VCO<<4) | (POST<<6) |CURR );

//	val =  
	ReadTW88(PAGE0_SSPLL|0xFC);

	//dPrintf("\nTW8835 0x0fC: %02bx ", ReadTW88(PAGE0_SSPLL|0xFC) );
	//dPrintf("\nTW8835 0x0fC&~0x80: %02bx ", ReadTW88(PAGE0_SSPLL|0xFC) & ~0x80 );

	WriteTW88( PAGE0_SSPLL|0xFC, ReadTW88(PAGE0_SSPLL|0xFC) & ~0x80 );		// OFF PD_SSPLL(means PowerUP)

	//adjust clock divider depend on the clock. see SetDefaultPClock()
	PclkoSetDiv(1, 0/*(ppf+39) / 40 - 1*/); //0:div1, 1:div2, 2:div3	
}
//=============================================================================
/**
* initialize registers with text array
*
*	format
*		0xII, 0x00	//start. If II is not 00, use WriteI2CByte.  
*		0xff, 0xXX	//assign page
*		0xRR, 0xDD	//register & data
*		...
*		0xff, 0xXX	//assign page
*		0xRR, 0xDD	//register & data
*		...
*		0xff, 0xff	//end
*/                                                                           
//=============================================================================
void I2CDeviceInitialize(BYTE *RegSet, BYTE delay)
{
	BYTE index, val;
	WORD page = 0;

	RegSet+=2;

	while (( RegSet[0] != 0xFF ) || ( RegSet[1]!= 0xFF )) {			// 0xff, 0xff is end of data
		index = *RegSet;
		val = *(RegSet+1);

		if(index == 0xFF)	page = val << 8;

		//Printf("\n %bx,%bx",index,val);

		WriteTW88(page|index, val);		

		if(delay)	delay1ms(delay);

		RegSet+=2;
	}
}


//Add TW8836
#if ( PCB_VER == DEMO_INTERSIL )
void	BackLight( BYTE on )
{
	BYTE bTemp;

	if( on == ON )
	{
		WriteI2CByte( I2CID_SX1504, 1, 0 );		// output enable
		WriteI2CByte( I2CID_SX1504, 0, ReadI2CByte(I2CID_SX1504, 0) & 0xFD );		// FPBIAS enable.
	
		WriteI2CByte(I2CID_ISL97671A, 0x01, 0x05);
		bTemp = ReadI2CByte(I2CID_ISL97671A,0x01);
	
		Printf(" ID:58 Idx:1 W:0x05 R:%bx", bTemp);
		if ((bTemp & 0x05) != 0x05)	
		{
			Printf(" ID:58 Idx:1 W:0x05 R:%bx", bTemp);
	
			WriteI2CByte(I2CID_ISL97671A, 0x01, 0x05);
			bTemp = ReadI2CByte(I2CID_ISL97671A,0x01);
			if ((bTemp & 0x05) != 0x05)	{
				Printf(" W:0x05 R:%bx", bTemp);
			}
		}
	}
	else
	{
		WriteI2CByte( I2CID_SX1504, 1, 0 );		// output enable
		WriteI2CByte( I2CID_SX1504, 0, ReadI2CByte(I2CID_SX1504, 0) | 0x02 );		// FPBIAS disable
	}
		
}

void OutputEnablePin(BYTE fFPDataPin, BYTE fOutputPin)
{
	BYTE value;

	Printf("\nFP_Data:%s OutputPin:%s",fFPDataPin ? "On" : "Off",fOutputPin ? "On" : "Off");

	WriteTW88Page(PAGE0_GENERAL);
//	WriteTW88(REG008, 0x80 | (ReadTW88(REG008) & 0x0F));	//Output enable......BUGBUG
	value = ReadTW88(0x008) & ~0x30;
	if(fFPDataPin==0) 		value |= 0x20;
	if(fOutputPin==0)		value |= 0x10;
	WriteTW88(0x008,  value);
}

#else 		//DEMO_AT

#endif
