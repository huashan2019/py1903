/*
 *  spi.c - a device driver for the spi-bus interface 
 *
 *  Copyright (C) 2011 Intersil Corporation
 *
 */
//*****************************************************************************
//
//									SPI.c
//
//*****************************************************************************
/*
	ToDo List

	o. Implement Data Corruption Check and Recovery
	o. Improve EE_Format() by minimizing SPI Programming count->Done by BKang 110224
*/

#include "include.h"

#define dPrintf(...)	 
#define dPuts(...)	 
#define Puts(...)	 
#define  XDATA		 
//----------------- SPI Mode Definition ---------------------

BYTE DMAMODE_RW_FIX_REG = 0x64;
BYTE DMAMODE_RW_INC_REG = 0x44;
BYTE DMAMODE_R_XDATA    = 0xc4;
BYTE DMAMODE_W_XDATA    = 0xc4;

BYTE SPICMD_READ        = 0x03;
BYTE SPICMD_PROGRAM     = 0x02;

BYTE SPICMD_BYTES		= 5;

XDATA BYTE SPI_Buffer[SPI_BUFFER_SIZE];
XDATA BYTE SPI_CmdBuffer[8];

BYTE SpiFlashVendor;


//internal prototype
BYTE EE_CheckMoveDoneBank(BYTE block, BYTE bank);
void EE_WriteBlock(BYTE block, BYTE *buf, BYTE *mask);
void EE_WriteMoveDone(BYTE block,BYTE bank);
BYTE EE_CheckBlankBank(BYTE block, BYTE bank);
void EE_ReadBlock(BYTE block, BYTE *buf, BYTE *mask);

BYTE EE_CleanBlock(BYTE block, BYTE fSkipErase);


//=============================================================================
//
//=============================================================================
BYTE SPI_SetModeByRegister( void )
{
	BYTE mod;

	WriteTW88Page( PAGE4_SPI );
	mod = ReadTW88( PAGE4_SPI|0xc0 ) & 0x07;

	switch( mod ) {
		case 0:	//--- Slow
			DMAMODE_RW_FIX_REG = 0x64;
			DMAMODE_RW_INC_REG = 0x44;
			DMAMODE_R_XDATA    = 0xc4;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0x03;	SPICMD_BYTES = 4;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 1:	//--- Fast
			DMAMODE_RW_FIX_REG = 0x65;
			DMAMODE_RW_INC_REG = 0x45;
			DMAMODE_R_XDATA    = 0xc5;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0x0b;	SPICMD_BYTES = 5;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 2:	//--- Dual
			DMAMODE_RW_FIX_REG = 0x65;
			DMAMODE_RW_INC_REG = 0x45;
			DMAMODE_R_XDATA    = 0xc5;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0x3b;	SPICMD_BYTES = 5;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 3:	//--- Quad
			DMAMODE_RW_FIX_REG = 0x65;
			DMAMODE_RW_INC_REG = 0x45;
			DMAMODE_R_XDATA    = 0xc5;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0x6b;	SPICMD_BYTES = 5;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 4:	//--- Dual-IO
			DMAMODE_RW_FIX_REG = 0x65;
			DMAMODE_RW_INC_REG = 0x45;
			DMAMODE_R_XDATA    = 0xc5;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0xbb;	SPICMD_BYTES = 5;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 5:	//--- Quad-IO
			DMAMODE_RW_FIX_REG = 0x67;
			DMAMODE_RW_INC_REG = 0x47;
			DMAMODE_R_XDATA    = 0xc7;	
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0xeb;	 SPICMD_BYTES = 7;
			SPICMD_PROGRAM     = 0x02;
			break;
 	}
	return mod;
}


//=============================================================================
//
//=============================================================================
/**
* SpiFlash command routine that using a ChipRegister 0x4D0.
*
* use SPI_Buffer[] for Write & Read.
* so, Do not use it with a real Read/Write routines that using SPI_Buffer[].
*
*	SpiFlashChipRegCmd(SPICMD_RDID,0,3)
*	SpiFlashChipRegCmd(SPICMD_WREN,0,0)
*
* need REG4C1[0] before you use.
* SPICMD_4READ can read only 1 byte. See TW8835 Errata.
*/
BYTE SpiFlashChipRegCmd(BYTE cmd, BYTE w_len, BYTE r_len, BYTE vblank)
{
	BYTE dma_option;
	BYTE i;
	volatile BYTE vdata;

	WriteTW88Page(PAGE4_SPI);
	WriteTW88(REG4C3,(DMA_DEST_CHIPREG << 6) | (1+w_len));
	WriteTW88(REG4C6, DMA_BUFF_REG_ADDR_PAGE);
	WriteTW88(REG4C7, DMA_BUFF_REG_ADDR_INDEX);
	WriteTW88(REG4C8, 0x00 );							// data Buff count middle
	WriteTW88(REG4C9, r_len );							// data Buff count Lo
	WriteTW88(REG4CA, cmd );
	for(i=0; i < w_len; i++)
		WriteTW88(REG4CB+i, SPI_CmdBuffer[i]);
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


//	if(cmd==SPICMD_SE)
//		return 0;

	//wait done
	for(i=0; i < 200; i++) {
		vdata = ReadTW88(REG4C4);
		if((vdata & 0x01)==0)
			break;
		delay1ms(10);
	}
	if(i==200) {
		Printf("\nSpiFlashChipRegCmd Busy cmd:%bx",cmd);
		return 2;	//fail:busy
	}
	//read
	for(i=0; i < r_len; i++) 
		SPI_CmdBuffer[i] = ReadTW88(REG4D0+i);		

	return 0;	//success
}

//return
//	0: fail or MX. default
//	1: EON
//
#if 1
BYTE SPI_QUADInit(void)
{
	BYTE dat0,vid,cid;
	BYTE ret,temp;
							 
	ret = SpiFlashChipRegCmd(SPICMD_RDID,0,3, 0);
	if(ret)
		Puts("\nSPICMD_RDID fail");
	vid  = SPI_CmdBuffer[0];	
	dat0 = SPI_CmdBuffer[1];	
	cid  = SPI_CmdBuffer[2];

	Printf("\nSPI JEDEC ID: %02bx %02bx %02bx", vid, dat0, cid );
	//C2 20 19 - MX 256

	if(vid == 0x1C) 
	{
		SpiFlashVendor = SFLASH_VENDOR_EON;
		if(dat0==0x70 && cid==0x19)
			SpiFlashVendor = SFLASH_VENDOR_EON_256;
	}
	else if(vid == 0xC2) 
	{
	 	SpiFlashVendor = SFLASH_VENDOR_MX;
		if(dat0==0x20 && cid==0x19)
			SpiFlashVendor = SFLASH_VENDOR_MX_256;
	} 
	else if(vid == 0xEF)	
		SpiFlashVendor = SFLASH_VENDOR_WB;
	else if(vid == 0x20) 
	{
		if(cid != 0x18) 
		{
			Printf(" UNKNOWN MICRON SPIFLASH !!");			
			return 0;
		}
		SpiFlashVendor = SFLASH_VENDOR_MICRON; //numonyx
	}
	else if(vid == 0xF8)
	{
		SpiFlashVendor = SFLASH_VENDOR_FDX;
		//Printf( "\nFidelix Flash !");	
	}
	else 
	{
		Printf(" UNKNOWN SPIFLASH !!");
		return 0;
	}

	//----------------------------
	//read status register
	//----------------------------

	if (vid == 0xC2 || vid == 0x1C) 					//C2:MX 1C:EON
	{ 							
		ret=SpiFlashChipRegCmd(SPICMD_RDSR,0, 1, 0);
		temp = SPI_CmdBuffer[0] & 0x40;							
		//if 0, need to enable quad
	}
	else if (vid == 0xEF) 							  	// WB
	{				
		//if(cid == 0x18) {				//Q128 case different status read command
			ret=SpiFlashChipRegCmd(SPICMD_RDSR2,0, 1, 0);
			temp = SPI_CmdBuffer[0];							//dat0[1]:QE
			dPrintf("\nStatus2 before QUAD: %02bx", temp);
			//if 0, need to enable quad
		//}
		//else {
		//	SpiFlashCmd(SPICMD_RDSR, 1);
		//	SpiFlashDmaReadLenByte(2);
		//	SpiFlashDmaStart(SPIDMA_READ,0, __LINE__);
		//	dat0 = SPIDMA_READDATA(0);
		//	dat1 = SPIDMA_READDATA(1);
		//	dPrintf("\nStatus before QUAD: %02bx, %02bx", dat0, dat1 );	
		//	temp = dat1;
		//}
	}
	else if(vid == SFLASH_VENDOR_MICRON) 			     //STM
	{
		ret=SpiFlashChipRegCmd(SPICMD_RDVREG,0, 1, 0);	//cmd, read Volatile register
		temp = SPI_CmdBuffer[0];
		Printf("\nVolatile Register: %02bx", temp );
		if(temp != 0x6B)
			temp = 0; //need an enable routine
	}
	else if( vid == 0xF8 )
	{
		ret=SpiFlashChipRegCmd(SPICMD_RDSR_2,0, 1, 0);
		temp = SPI_CmdBuffer[0] & 0x02;							
		//if 0, need to enable quad	
	}

	if(temp)
		return SpiFlashVendor;

	//----------------------------
	// enable quad
	//----------------------------
	Puts("\nEnable quad mode" );
	if (vid == 0xC2 || vid == 0x1c) 
	{
		SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);
#ifdef DEBUG_SPIFLASH	
		SpiFlashChipRegCmd(SPICMD_RDSR, 0, 1, 0);
		Printf("\nStatus after Write enable %02bx", SPI_CmdBuffer[0] );
#endif	
		SPI_CmdBuffer[0] = 0x40;	//en QAUD mode
		SpiFlashChipRegCmd(SPICMD_WRSR, 1, 0, 0);
		Puts("\nQUAD ENABLED" );
	
#ifdef DEBUG_SPIFLASH	
		SpiFlashChipRegCmd(SPICMD_RDSR, 0, 1, 0);
		Printf("\nStatus after Write enable %02bx", SPI_CmdBuffer[0] );
#endif
		SpiFlashChipRegCmd(SPICMD_WRDI, 0, 0, 0);
	}
	else if(vid == 0xEF) 
	{
		SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);
		SPI_CmdBuffer[0] = 0x00;	//cmd, en QAUD mode			  //0x02?
		SPI_CmdBuffer[1] = 0x02;	
		SpiFlashChipRegCmd(SPICMD_WRSR, 2, 0, 0);
		dPuts("\nQUAD ENABLED" );
#ifdef DEBUG_SPIFLASH
		//if(cid == 0x18) {				//Q128 case different status read command
			SpiFlashChipRegCmd(SPICMD_RDSR2, 0, 1, 0);
			Printf("\nStatus2 after Write enable %02bx", SPI_CmdBuffer[0] );
		//}
		//else {
		//	SpiFlashCmd(SPICMD_RDSR, 1);
		//	SpiFlashDmaReadLenByte(2);
		//	SpiFlashDmaStart(SPIDMA_READ,0, __LINE__);
		//	dat0 = SPIDMA_READDATA(0);
		//	dat1 = SPIDMA_READDATA(1);
		//	dPrintf("\nStatus before QUAD: %02bx, %02bx", dat0, dat1 );	
		//}
#endif
		SpiFlashChipRegCmd(SPICMD_WRDI, 0, 0, 0);
	}
	else if(vid == SFLASH_VENDOR_MICRON) 
	{
		SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);

		SPI_CmdBuffer[0] = 0x6B;						// cmd, write Volatile. set 6 dummy cycles
		SpiFlashChipRegCmd(SPICMD_WDVREG, 1, 0, 0);
		Puts("\nVolatile 6 dummy SET" );
		SpiFlashChipRegCmd(SPICMD_WRDI, 0, 0, 0);

		// set non-Volatile
#if 0
		SpiFlashChipRegCmd(SPICMD_RDNVREG, 0, 2, 0);	//cmd, read Non-Volatile register
		dat0 = SPI_CmdBuffer[0];
		temp = SPI_CmdBuffer[1];
		Printf("\nNon-Volatile Register: %02bx, %02bx", dat0, temp );
		if ( temp != 0x6F ) {
			SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);
	
			SPI_CmdBuffer[0] = 0xFF;							// B7~B0, B15~B8, set 6 dummy cycles
			SPI_CmdBuffer[1] = 0x6F;
			SpiFlashChipRegCmd(SPICMD_WDNVREG, 2, 0, 0);	// cmd, write Non-Volatile.
			Puts("\nnon-Volatile 6 dummy SET" );
	
			SpiFlashChipRegCmd(SPICMD_WRDI, 0, 0, 0);
		}
#endif
	}
	else if ( vid == 0xF8 ) 
	{
		SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);
//#ifdef DEBUG_SPIFLASH	
		SpiFlashChipRegCmd(SPICMD_RDSR, 0, 1, 0);
		Printf("\nStatus after Write enable %02bx", SPI_CmdBuffer[0] );
		SpiFlashChipRegCmd(SPICMD_RDSR_2, 0, 1, 0);
		Printf("\nStatus 2 after Write enable %02bx", SPI_CmdBuffer[0] );
//#endif	
		SPI_CmdBuffer[0] = 0x02;	
		SPI_CmdBuffer[1] = 0x02;	//en QAUD mode
		SpiFlashChipRegCmd(SPICMD_WRSR, 2, 0, 0);
		delay1ms(10);
		Printf("\nQUAD ENABLED" );
	
//#ifdef DEBUG_SPIFLASH	
		SpiFlashChipRegCmd(SPICMD_RDSR, 0, 1, 0);
		Printf("\nStatus after Write Status %02bx", SPI_CmdBuffer[0] );
//#endif
		SpiFlashChipRegCmd(SPICMD_WRDI, 0, 0, 0);
	}
	return SpiFlashVendor;
}
#else

	
BYTE SPI_QUADInit(void)	  						//mnm
{
	BYTE	temp;
	BYTE ret=0;


	ret = SpiFlashChipRegCmd(SPICMD_RDID,0,3, 0);

	WriteTW88Page( PAGE4_SPI );

	WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
	WriteTW88( PAGE4_SPI|0xca, 0x9f );					// cmd, read Jedec ID
	WriteTW88( PAGE4_SPI|0xc6, 0x04 );
	WriteTW88( PAGE4_SPI|0xc7, 0xd0 );						// read buffer size 3

	WriteTW88( PAGE4_SPI|0xda, 0 );
	WriteTW88( PAGE4_SPI|0xc8, 0 );
	WriteTW88( PAGE4_SPI|0xc9, 3 );						// read buffer size 3
	WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start

	temp = ReadTW88(PAGE4_SPI|0xd0);
	Printf("\nSPI JEDEC ID: %02bx %02bx %02bx", temp, ReadTW88(PAGE4_SPI|0xd1), ReadTW88(PAGE4_SPI|0xd2) );


	if ( (temp == 0xC2) || ( temp == 0x1c) ){		// for MX	&& EON  && STM
		if(temp ==0x1C)
			ret = SFLASH_VENDOR_EON;
		else
			ret = SFLASH_VENDOR_MX;

		WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( PAGE4_SPI|0xca, 0x05 );					// cmd, read Status
		WriteTW88( PAGE4_SPI|0xc9, 1 );						// read buffer size 1
		WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
		temp = ReadTW88( PAGE4_SPI|0xd0 );					// read status regisiter value

		if ( temp == 0 ) {
			if(ret== SFLASH_VENDOR_EON)
				Puts("\nEnable EON quad mode" );
			else 
				Puts("\nEnable MX quad mode" );
			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x06 );					// cmd, write enable
			WriteTW88( PAGE4_SPI|0xc9, 0 );						// read buffer size 1
			WriteTW88( PAGE4_SPI|0xc4, 0x03 );					// start

			Puts("\nWrite ENABLED" );

			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x05 );					// cmd, read Status
			WriteTW88( PAGE4_SPI|0xc9, 1 );						// read buffer size 1
			WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
			temp = ReadTW88( PAGE4_SPI|0xd0 );					// read status regisiter value
			Printf("\nStatus after Write enable %02bx", temp );

			WriteTW88( PAGE4_SPI|0xc3, 0x42 );					// mode, cmd-len 2
			WriteTW88( PAGE4_SPI|0xca, 0x01 );					// cmd, write Status
			WriteTW88( PAGE4_SPI|0xcb, 0x40 );					// cmd, en QAUD mode
			WriteTW88( PAGE4_SPI|0xc9, 0 );						// read buffer size 1
			WriteTW88( PAGE4_SPI|0xc4, 0x07 );					// start + write + busycheck

			Puts("\nQUAD ENABLED" );

			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x05 );					// cmd, read Status
			WriteTW88( PAGE4_SPI|0xc9, 1 );						// read buffer size 1
			WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
			temp = ReadTW88( PAGE4_SPI|0xd0 );					// read status regisiter value
			Printf("\nStatus after QUAD enable %02bx", temp );

			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x04);//0x06 );					// cmd, write disable
			WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
		}
	}
	else if ( temp == 0xEF ) {				// for WB
		ret = SFLASH_VENDOR_WB;
		WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( PAGE4_SPI|0xca, 0x05 );					// cmd, read Status
		WriteTW88( PAGE4_SPI|0xc9, 2 );						// read buffer size 1
		WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
		temp = ReadTW88( PAGE4_SPI|0xd1 );					// read status regisiter value
		if ( temp == 0 ) {
			Puts("\nEnable WB quad mode" );
			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x06 );					// cmd, write enable
			WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start

			WriteTW88( PAGE4_SPI|0xc3, 0x43 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x01 );					// cmd, write Status
			WriteTW88( PAGE4_SPI|0xcb, 0x00 );					// cmd, en QAUD mode
			WriteTW88( PAGE4_SPI|0xcc, 0x02 );					// cmd, en QAUD mode
			WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
		}
	}
	else if(temp == 0x20 ) {				// for STM
		ret = SFLASH_VENDOR_NUM;
		Puts("\nEnable Numonyx quad mode" );

		WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( PAGE4_SPI|0xca, 0x06 );					// cmd, write enable
		WriteTW88( PAGE4_SPI|0xc9, 0 );						// read buffer size 1
		WriteTW88( PAGE4_SPI|0xc4, 0x03 );					// start

		WriteTW88( PAGE4_SPI|0xc3, 0x43 );					// mode, cmd-len 1
		WriteTW88( PAGE4_SPI|0xca, 0xB1 );					// cmd, write enable
		WriteTW88( PAGE4_SPI|0xcb, 0x6F );
		WriteTW88( PAGE4_SPI|0xcc, 0xFF );
		WriteTW88( PAGE4_SPI|0xc9, 0 );						// read buffer size 1
		WriteTW88( PAGE4_SPI|0xc4, 0x07 );					// start

		WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( PAGE4_SPI|0xca, 0x06 );					// cmd, write enable
		WriteTW88( PAGE4_SPI|0xc9, 0 );						// read buffer size 1
		WriteTW88( PAGE4_SPI|0xc4, 0x03 );					// start

		WriteTW88( PAGE4_SPI|0xc3, 0x42 );					// mode, cmd-len 1
		WriteTW88( PAGE4_SPI|0xca, 0x81 );					// cmd, write enable
		WriteTW88( PAGE4_SPI|0xcb, 0x6B );
		WriteTW88( PAGE4_SPI|0xc9, 0 );						// read buffer size 1
		WriteTW88( PAGE4_SPI|0xc4, 0x07 );					// start

		Puts("--DONE");
	}

	else if ( temp == 0xF8 ){								// for Fidelix
		ret = SFLASH_VENDOR_FDX;

		WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( PAGE4_SPI|0xca, 0x05 );					// cmd, read Status
		WriteTW88( PAGE4_SPI|0xc9, 1 );						// read buffer size 1
		WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
		temp = ReadTW88( PAGE4_SPI|0xd0 );					// read status regisiter value

		Printf("\nGR Read Status = %02bx",temp);

		if ( temp == 0x00 ) { 
			Puts("\nEnable FM quad mode" );

			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x06 );					// cmd, write enable
			WriteTW88( PAGE4_SPI|0xc9, 0 );						// read buffer size 1
			WriteTW88( PAGE4_SPI|0xc4, 0x03 );					// start

			Puts("\nWrite ENABLED" );

			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x05 );					// cmd, read Status
			WriteTW88( PAGE4_SPI|0xc9, 1 );						// read buffer size 1
			WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
			temp = ReadTW88( PAGE4_SPI|0xd0 );					// read status regisiter value
			Printf("\nStatus after Write enable %02bx", temp );
//== test
			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x35 );					// cmd, read Status
			WriteTW88( PAGE4_SPI|0xc9, 2 );						// read buffer size 1
			WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
			temp = ReadTW88( PAGE4_SPI|0xd0 );					// read status regisiter value
			Printf("\nStatus after QUAD enable %02bx", temp );
			temp = ReadTW88( PAGE4_SPI|0xd1 );					// read status regisiter value
			Printf("\nStatus after QUAD enable2 %02bx", temp );
//==
			WriteTW88( PAGE4_SPI|0xc3, 0x43 );					// mode, cmd-len 2
			WriteTW88( PAGE4_SPI|0xca, 0x01 );					// cmd, write Status
			WriteTW88( PAGE4_SPI|0xcb, 0x02 );					// cmd, en QAUD mode
			WriteTW88( PAGE4_SPI|0xcc, 0x02 );					// cmd, en QAUD mode
			WriteTW88( PAGE4_SPI|0xc9, 0 );						// read buffer size 1
			WriteTW88( PAGE4_SPI|0xc4, 0x01);//0x07 );					// start + write + busycheck

			delay1ms(100);
			Puts("\nQUAD ENABLED" );

			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x35 );					// cmd, read Status
			WriteTW88( PAGE4_SPI|0xc9, 1 );						// read buffer size 1
			WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
			temp = ReadTW88( PAGE4_SPI|0xd0 );					// read status regisiter value
			Printf("\nStatus after QUAD enable %02bx", temp );

			WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( PAGE4_SPI|0xca, 0x04);//0x06 );					// cmd, write disable
			WriteTW88( PAGE4_SPI|0xc4, 0x01 );					// start
		}
	}
	return ret;
}
#endif
//=============================================================================
//
//=============================================================================
void SPI_Init(void)						//mnm
{
	WriteTW88Page( PAGE4_SPI );

	WriteTW88( PAGE4_SPI|TWBASE_SPI, SPI_READ_MODE );		// SPI Mode:	0=Slow, 1=fast, 2=dual, 3=quad, 4=dual-io, 5=quad-io
												// R4C0	SPI mode QuadIO
//	WriteTW88( PAGE4_SPI|0xc1, 0x06 );					// MCU Clock:	27MHz
//	WriteTW88( PAGE4_SPI|0xc2, 0x06 );					// SPI Clock:	27MHz

//	dat = ReadTW88( PAGE4_SPI|0x20 ) | 0x2c;				//----- Enable OSD-DMA, Cache
//	WriteTW88( PAGE4_SPI|0x20, dat );

	SPI_SetModeByRegister();					// Match DMA mode with SPI-read

}

//=============================================================================
//		SPI DMA (SPI --> Incremental XDATA)
//=============================================================================
//#include <intrins.h>
//	_nop_();

void SPI_ReadData2xdata( DWORD spiaddr, BYTE *ptr, DWORD cnt )
{
	WORD xaddr;

	xaddr = (WORD)ptr;

	WriteTW88Page( PAGE4_SPI );			// Set Page=4

	WriteTW88( PAGE4_SPI|0xc3, DMAMODE_R_XDATA );			// Mode = SPI -> incremental xdata
	WriteTW88( PAGE4_SPI|0xca, SPICMD_READ );				// Read Command
	WriteTW88( PAGE4_SPI|0xcb, spiaddr>>16 );				// SPI address
	WriteTW88( PAGE4_SPI|0xcc, spiaddr>>8 );				// SPI address
	WriteTW88( PAGE4_SPI|0xcd, spiaddr );					// SPI address
 	WriteTW88( PAGE4_SPI|0xc6, xaddr>>8 );				// Buffer address
	WriteTW88( PAGE4_SPI|0xc7, xaddr );					// Buffer address

	WriteTW88( PAGE4_SPI|0xda, cnt>>16 );					// Read count
 	WriteTW88( PAGE4_SPI|0xc8, cnt>>8 );					// Read count
	WriteTW88( PAGE4_SPI|0xc9, cnt );						// Read count

	WriteTW88( PAGE4_SPI|0xC4, 0x01);						//DMA-Read start
}

#if 0
//=============================================================================
//		SPI Write Enable
//=============================================================================
void SPI_WriteEnable(void)
{
	WriteTW88Page( PAGE4_SPI );			// Set Page=4

	WriteTW88( PAGE4_SPI|0xc3, 0x41 );					// Mode = command write, Len=1
	WriteTW88( PAGE4_SPI|0xca, 0x06 );					// SPI Command = WRITE_ENABLE
 	WriteTW88( PAGE4_SPI|0xc8, 0x00 );					// Read count
	WriteTW88( PAGE4_SPI|0xc9, 0x00 );					// Read count

	WriteTW88( PAGE4_SPI|0xc4, 0x03 );					// DMA-Write start
}
//=============================================================================
//		SPI Sector Erase
//=============================================================================

void SPI_SectorErase( DWORD spiaddr )
{
	dPrintf("\nSPI_SectorErase %06lx",spiaddr);

	SPI_WriteEnable();

	WriteTW88Page( PAGE4_SPI );			// Set Page=4

	WriteTW88( PAGE4_SPI|0xc3, 0x44 );					// Mode = command write, Len=4
	WriteTW88( PAGE4_SPI|0xca, 0x20 );					// SPI Command = SECTOR_ERASE
	WriteTW88( PAGE4_SPI|0xcb, spiaddr>>16 );				// SPI address
	WriteTW88( PAGE4_SPI|0xcc, spiaddr>>8 );				// SPI address
	WriteTW88( PAGE4_SPI|0xcd, spiaddr );					// SPI address
 	WriteTW88( PAGE4_SPI|0xc8, 0x00 );					// Read count
	WriteTW88( PAGE4_SPI|0xc9, 0x00 );					// Read count

	WriteTW88( PAGE4_SPI|0xc4, 0x07 );					// DMA-Write start, Busy check
}
//=============================================================================
//		SPI Block Erase
//=============================================================================
void SPI_BlockErase( DWORD spiaddr )
{
	SPI_WriteEnable();

	WriteTW88Page( PAGE4_SPI );			// Set Page=4

	WriteTW88( PAGE4_SPI|0xc3, 0x44 );					// Mode = command write, Len=4
	WriteTW88( PAGE4_SPI|0xca, 0xd8 );					// SPI Command = BLOCK_ERASE
	WriteTW88( PAGE4_SPI|0xcb, spiaddr>>16 );				// SPI address
	WriteTW88( PAGE4_SPI|0xcc, spiaddr>>8 );				// SPI address
	WriteTW88( PAGE4_SPI|0xcd, spiaddr );					// SPI address
 	WriteTW88( PAGE4_SPI|0xc8, 0x00 );					// Read count
	WriteTW88( PAGE4_SPI|0xc9, 0x00 );					// Read count

	WriteTW88( PAGE4_SPI|0xc4, 0x07 );					// DMA-Write start, Busy check
}
//=============================================================================
//		SPI PageProgram
//=============================================================================
void SPI_PageProgram( DWORD spiaddr, BYTE *ptr, WORD cnt )
{
	WORD xaddr = (WORD)ptr;

	SPI_WriteEnable();

	WriteTW88Page( PAGE4_SPI );			// Set Page=4

	WriteTW88( PAGE4_SPI|0xc3, DMAMODE_W_XDATA );			// Mode = xdata -> SPI, Len=4
	WriteTW88( PAGE4_SPI|0xca, SPICMD_PROGRAM );			// SPI Command = PAGE_PROGRAM
	WriteTW88( PAGE4_SPI|0xcb, spiaddr>>16 );				// SPI address
	WriteTW88( PAGE4_SPI|0xcc, spiaddr>>8 );				// SPI address
	WriteTW88( PAGE4_SPI|0xcd, spiaddr );					// SPI address
 	WriteTW88( PAGE4_SPI|0xc6, xaddr>>8 );				// Buffer address
	WriteTW88( PAGE4_SPI|0xc7, xaddr );					// Buffer address
 	WriteTW88( PAGE4_SPI|0xc8, cnt>>8 );					// Write count
	WriteTW88( PAGE4_SPI|0xc9, cnt );						// Write count

	WriteTW88( PAGE4_SPI|0xc4, 0x07 );					// DMA-Write start, Busy check
}

//*****************************************************************************
//
//		EEPROM Emulation
//
//*****************************************************************************
//		Format: For each 4 bytes [Index] [Index^FF] [Data] [Data^FF]
//
//

BYTE EE_CurrBank[EE_BLOCKS];
WORD EE_WritePos[EE_BLOCKS];
BYTE EE_buf[EE_INDEX_PER_BLOCK];
BYTE EE_mask[(EE_INDEX_PER_BLOCK+7)/8];

//print Block info.
void EE_PrintCurrInfo(void)
{
	BYTE block;
	DWORD sector_addr;

	for(block=0; block<EE_BLOCKS; block++) {
		sector_addr = EE_SPI_SECTOR0 + SPI_SECTOR_SIZE*((DWORD)block*EE_SPI_BANKS+EE_CurrBank[block]);
		dPrintf("\n\tBlock:%bx Bank%bx WritePos:%x Sector:%06lx", 
			block, EE_CurrBank[block],
			EE_WritePos[block],
			sector_addr
			);
	}
}

//=============================================================================
//
//=============================================================================
void EE_Format(void)
{
	BYTE  block,j;
	DWORD spi_addr;

	dPrintf("\nEE_Format start");

	//select the default SPI mode
	WriteTW88Page(PAGE4_SPI);
	//..	

	for(block=0; block<EE_BLOCKS; block++) {
		spi_addr = EE_SPI_SECTOR0 + SPI_SECTOR_SIZE*(DWORD)block*EE_SPI_BANKS;
		for(j=0; j < EE_SPI_BANKS; j++) {
			SPI_SectorErase( spi_addr );
			spi_addr += SPI_SECTOR_SIZE;
		}
		EE_CurrBank[block] = 0;
		EE_WritePos[block] = 0;
	}

	dPrintf("\nEE_Format end - please call 'EE find'");
}

//=============================================================================
//
//=============================================================================
//description
//	read eeprom index data
//	Work only on current bank.
//return
//	indexed eeprom data.
//	if no data, return 0.
BYTE EE_Read(WORD index)
{
	int i; //NOTE
	BYTE block;
	BYTE sindex;	//sub index
	DWORD sector_addr;
	WORD remain;
	BYTE read_cnt;

	block  = index / EE_INDEX_PER_BLOCK;
	sindex = index % EE_INDEX_PER_BLOCK;		//index in block. max 0xFF
	sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + EE_CurrBank[block]) * SPI_SECTOR_SIZE;

	remain = EE_WritePos[block];
	while( remain ) {
		if( remain >= EE_BUF_SIZE ) read_cnt = EE_BUF_SIZE;
		else                        read_cnt = remain;
		SPI_ReadData2xdata( sector_addr+remain-read_cnt, SPI_Buffer, read_cnt );
		remain -= read_cnt;

		for(i=read_cnt-4; i>=0; i-=4) {
			if(	SPI_Buffer[i] != sindex )
				continue;
			if( ((SPI_Buffer[i]+SPI_Buffer[i+1])==0xff) && ((SPI_Buffer[i+2]+SPI_Buffer[i+3])==0xff) )
				return SPI_Buffer[i+2];
		}
	}

	dPrintf("\nCannot find EEPROM index %x data in block%bx bank%bx", index, block,EE_CurrBank[block]);

	//BKTODO try other bank

	return 0;
}


//=============================================================================
//
//=============================================================================
//description
//	write index & data with index+^index+data+^data format
//
#if 0
void EE_Write(WORD index, BYTE dat)
{
	BYTE block;
	BYTE sindex;
	DWORD sector_addr; //, sector_addr_new;
	BYTE ret;

	block = index / EE_INDEX_PER_BLOCK;
	sindex = index % EE_INDEX_PER_BLOCK;		//index in block. max 0xFF

	if(EE_WritePos[block] >= SPI_SECTOR_SIZE) {
		ret=EE_CleanBlock(block, 1);
		//BKFYI: EE_CurrBank[block] & EE_WritePos[block] will be updated.
		if(ret) {
			wPrintf("\nWarning:");
			if(ret & 0xF0) wPrintf("BankMove ");
			if(ret & 0x0F) wPrintf("SectorErase ");
			wPrintf(" in EE_Write");
		}
	}
	sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + EE_CurrBank[block]) * SPI_SECTOR_SIZE;

	//----- Write data
	SPI_Buffer[0] = (BYTE)sindex;
	SPI_Buffer[1] = 0xff^(BYTE)sindex;
	SPI_Buffer[2] = dat;
	SPI_Buffer[3] = 0xff^dat;
	SPI_PageProgram( sector_addr + EE_WritePos[block], SPI_Buffer, 4L );

	EE_WritePos[block] += 4;
}
#endif



//=============================================================================
//
//=============================================================================
//description
//	Find EE_CurrBank[] and EE_WritePos[] per block.
//	method 3. you can use all blank sector
//return
//	0:OK
//	1:Found broken banks. Need repair.
BYTE EE_FindCurrInfo(void)		
{
	BYTE i, j, k;
	DWORD sector_addr;
	BYTE ret;

	ePrintf("\nEE_FindCurrInfo");
	ret = 0;
	//----- Check EEPROM corruption -------------------------
	//
	//

	//----- Find EE_CurrBank and EE_WritePos -------------

	for(i=0; i<EE_BLOCKS; i++) {
		//
		//get EE_CurrBank[]
		//
		EE_CurrBank[i] = EE_SPI_BANKS;  //start from garbage.
		for(j=0; j < EE_SPI_BANKS; j++) {
			sector_addr = EE_SPI_SECTOR0 + SPI_SECTOR_SIZE*((DWORD)i*EE_SPI_BANKS+j);
			SPI_ReadData2xdata( sector_addr, SPI_Buffer, 4L );

			//check Blank Bank
			if((SPI_Buffer[0]==0xFF) 		
			&& (SPI_Buffer[1]==0xFF)   		
			&& (SPI_Buffer[2]==0xFF)   		
			&& (SPI_Buffer[3]==0xFF) ) {	
				//If you already have a used bank, stop here.
				//If it is a first blank bank after used one, stop here.
				if(EE_CurrBank[i] != EE_SPI_BANKS)
					break;
				//keep search
			}
			else {
				//found used bank, keep update bank number.
				//EE_CurrBank[i] = j;
				//continue check.	
				
				//check MoveDone flag at end of secotr.
				//sector_addr = EE_SPI_SECTOR0 + SPI_SECTOR_SIZE*((DWORD)i*EE_SPI_BANKS+j);
				SPI_ReadData2xdata( sector_addr + SPI_SECTOR_SIZE-4, SPI_Buffer, 4L );
				if((SPI_Buffer[0]==0x00) 		
				&& (SPI_Buffer[1]==0x00)   		
				&& (SPI_Buffer[2]==0x00)   		
				&& (SPI_Buffer[3]==0x00) ) {
					//found MoveDone bank
					; //skip this bank	
				}
				else {
					if(EE_CurrBank[i] != EE_SPI_BANKS) {
						//we found two used banks, maybe it is a broken bank.
						//But, we will use this broken bank.
						//and, I am sure, this broken bank have a small items.(less then 64)
						//so, we don't need to clean yet.
						wPrintf("\nFound broken bank at block%bx. %bx and %bx", i, EE_CurrBank[i],j);
						ret = 1 << i;	//found broken
					}
					if(EE_CurrBank[i]==0 && j==(EE_SPI_BANKS-1)) {
						//bank0 is a corrent one. do not update bank3(last bank)
						;
					} 
					else {
						EE_CurrBank[i] = j;
					}
				}
			}	 
		}
		//if no used bank, start from 0.
		if(EE_CurrBank[i] == EE_SPI_BANKS)
			EE_CurrBank[i]=0;
			
		//	
		//get EE_WritePos[]
		//
		sector_addr = EE_SPI_SECTOR0 + SPI_SECTOR_SIZE*(EE_SPI_BANKS*(DWORD)i+EE_CurrBank[i]);
		for(j=0; j<SPI_SECTOR_SIZE/EE_BUF_SIZE; j++) {
			SPI_ReadData2xdata( sector_addr + j*EE_BUF_SIZE, SPI_Buffer, EE_BUF_SIZE );

			EE_WritePos[i] = SPI_SECTOR_SIZE;
			for(k=0; k<EE_BUF_SIZE; k+=4) {
				if( SPI_Buffer[k]==0xff && SPI_Buffer[k+1]==0xff ) {
					EE_WritePos[i] = j*EE_BUF_SIZE + k;
					j=254; //next will be 0xFF, the max BYTE number. So, we can stop.
					break;
				}
			}
		}
	}

	EE_PrintCurrInfo();

	if(ret)
		wPrintf("\ntype EE repair");

	return ret;
}

//=============================================================================
//
//=============================================================================
//description
//	Read index+data on buff[]
//	each block have max 64 index+data pair. 
//	each block have max 1024 items(4*1024 / 4).
//	this function gather the valid 64 index+data pairs on current bank. 
//
//note: buf[] size have to be EE_INDEX_PER_BLOCK
//      mask[] size have to be (EE_INDEX_PER_BLOCK/8)
//
// if we have a item, the bitmap mask will have a "1".
// if we donot have a item, the bitmap mask will have a "0".
void EE_ReadBlock(BYTE block, BYTE *buf, BYTE *mask)
{
	BYTE i, j, ch0, ch1, ch2, ch3;
	DWORD sector_addr;
	WORD remain;
	BYTE read_cnt;

	//clear buffer and mask bitmap
	for(i=0; i < EE_INDEX_PER_BLOCK; i++)
		buf[i]=0x00;
	for(i=0; i < ((EE_INDEX_PER_BLOCK+7)/8); i++)
		mask[i]=0x00;

	sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + EE_CurrBank[block]) * SPI_SECTOR_SIZE;
	remain = EE_WritePos[block];

	for(j=0; j<SPI_SECTOR_SIZE/EE_BUF_SIZE; j++) {
		if(remain==0)
			break;
		if( remain >= EE_BUF_SIZE ) read_cnt = EE_BUF_SIZE;
		else                        read_cnt = remain;

		SPI_ReadData2xdata( sector_addr + j * EE_BUF_SIZE, SPI_Buffer, read_cnt );
		remain -= read_cnt;

		for(i=0; i<read_cnt; i+=4) {
			ch0 = SPI_Buffer[i];		//index
			ch1 = SPI_Buffer[i+1];		//^index
			ch2 = SPI_Buffer[i+2];		//data
			ch3 = SPI_Buffer[i+3];		//^data

			if( ((ch0^ch1)==0xff) && ((ch2^ch3)==0xff) ) {
				mask[ch0>>3] |= (1 << (ch0 & 0x07));
				buf[ch0] = ch2;
			}
		}
	}
}

//description
//	fill out the lost item data from bank(other)
//	If we already have a valid item, skip the update.
void EE_FillLostItems(BYTE block, BYTE bank, BYTE *buf, BYTE *mask)
{
	DWORD sector_addr;
	WORD remain;
	BYTE i, j;
	BYTE read_cnt;
	BYTE ch0,ch1,ch2,ch3;

	sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + bank) * SPI_SECTOR_SIZE;
	remain = SPI_SECTOR_SIZE;

	for(j=0; j<SPI_SECTOR_SIZE/EE_BUF_SIZE; j++) {
		if(remain==0)
			break;
		if( remain >= EE_BUF_SIZE ) read_cnt = EE_BUF_SIZE;
		else                        read_cnt = remain;
		SPI_ReadData2xdata( sector_addr + j * EE_BUF_SIZE, SPI_Buffer, read_cnt );
		remain -= read_cnt;

		for(i=0; i<read_cnt; i+=4) {
			ch0 = SPI_Buffer[i];		//index
			ch1 = SPI_Buffer[i+1];		//^index
			ch2 = SPI_Buffer[i+2];		//data
			ch3 = SPI_Buffer[i+3];		//^data

			if( ((ch0^ch1)==0xff) && ((ch2^ch3)==0xff) ) {
				//found valid item

				//now, check mask.
				if(mask[ch0>>3] & (1<<(ch0&0x07))) {
					//we already have a valid item. just skip.
				}
				else {
					mask[ch0>>3] |= (1 << (ch0 & 0x07));
					buf[ch0] = ch2;
				}
			}
		}
	}
}

//Used only in EE_MoveBank and EE_RepairMoveDone
//so, I assume, we have enough space.
void EE_WriteBlock(BYTE block, BYTE *buf, BYTE *mask)
{
	DWORD sector_addr;
	BYTE idx;
	BYTE i, j;
	BYTE wptr,bptr;

	//dPrintf("\nEE_WriteBlock(%bd,,)",block);
	sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + EE_CurrBank[block]) * SPI_SECTOR_SIZE;

	//write buf to new bank.
	wptr=0;		//write pointer
	for(i=0; i<(EE_INDEX_PER_BLOCK*4) / SPI_BUFFER_SIZE; i++) {
		bptr=0;		//SPI_Buffer pointer
		for(j=0; j<SPI_BUFFER_SIZE/4; j++) {
			idx = SPI_BUFFER_SIZE/4*i + j;

			if(mask[idx>>3] & (1<<(idx&0x07))) {
				//found valid data
				SPI_Buffer[bptr++] = idx;
				SPI_Buffer[bptr++] = idx^0xff;
				SPI_Buffer[bptr++] = buf[idx];
				SPI_Buffer[bptr++] = buf[idx] ^ 0xFF;
			}
		}
		if(bptr==0) {
			//dPrintf("\n0byte. skip %bd",i);
			continue;
		}
		SPI_PageProgram( sector_addr + wptr, SPI_Buffer, bptr );
		wptr+= bptr;
	}
	EE_WritePos[block] = wptr;
}


//If you found a broken bank, call it to repair.
//After this routine, call the EE_CleanBank.
//we assume, we have a enough space on current bank.  ===>WRONG
//When we have a broken bank, the item number of current bank is alwasy less then 64.
void EE_RepairMoveDone(void) //need new name
{
	BYTE block;
	BYTE prev_bank;
	BYTE ret;

	dPrintf("\nEE_RepairBank");

	for(block=0; block < EE_SPI_BANKS; block++) {
		dPrintf("\nblock%bx",block);
		prev_bank = (EE_CurrBank[block] + EE_SPI_BANKS -1) % EE_SPI_BANKS;
		ret = EE_CheckMoveDoneBank(block, prev_bank);
		ret += EE_CheckBlankBank(block, prev_bank);
		if(ret==0) {
			dPrintf(" repair %bx->%bx", prev_bank, EE_CurrBank[block]); 
			//prev_bank is not a MoveDone bank.
			//we need a repair.
			EE_ReadBlock(block, EE_buf, EE_mask);					//read items from current bank
			EE_FillLostItems(block, prev_bank, EE_buf, EE_mask);	//read the lost items from prev bank
			EE_WriteBlock(block, EE_buf, EE_mask);				//update items.
			EE_WriteMoveDone(block, prev_bank);
		}
		else {
			dPrintf("->skip");
		}
	}
}



//internal
BYTE EE_CheckBlankBank(BYTE block, BYTE bank)
{
	DWORD sector_addr;
	sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + bank) * SPI_SECTOR_SIZE;
	SPI_ReadData2xdata( sector_addr, SPI_Buffer, 4L );
	if( (SPI_Buffer[0]==0xff) && (SPI_Buffer[1]==0xff) && (SPI_Buffer[2]==0xff) && (SPI_Buffer[3]==0xff) )
		return 1;	 //TRUE
	return 0;
}

BYTE EE_CheckMoveDoneBank(BYTE block, BYTE bank)
{
	DWORD sector_addr;
	sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + bank) * SPI_SECTOR_SIZE;
	SPI_ReadData2xdata( sector_addr + SPI_SECTOR_SIZE - 4, SPI_Buffer, 4L );
	if( (SPI_Buffer[0]==0x00) && (SPI_Buffer[1]==0x00) && (SPI_Buffer[2]==0x00) && (SPI_Buffer[3]==0x00) )
		return 1;	//TRUE
	return 0;
}

//internal
void EE_WriteMoveDone(BYTE block,BYTE bank)
{
	DWORD sector_addr;
	sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + bank) * SPI_SECTOR_SIZE;

	//mark it as done.
	SPI_Buffer[0] = 0;
	SPI_Buffer[1] = 0;
	SPI_Buffer[2] = 0;
	SPI_Buffer[3] = 0;
	SPI_PageProgram( sector_addr + SPI_SECTOR_SIZE - 4, SPI_Buffer, 4L );
}

//return
//	1:SectorErase happen
BYTE EE_MoveBank(BYTE block)
{
	DWORD sector_addr;
	BYTE prev_bank;
	BYTE ret = 0;

	//read Block data to buf.
	EE_ReadBlock(block, EE_buf, EE_mask);

	//move to next bank
	prev_bank = EE_CurrBank[block];
	EE_CurrBank[block] = (EE_CurrBank[block] +1) % EE_SPI_BANKS;
	EE_WritePos[block] = 0;

	ret=EE_CheckBlankBank(block, EE_CurrBank[block]);
	if(ret==0) {
		sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + EE_CurrBank[block]) * SPI_SECTOR_SIZE;
		SPI_SectorErase( sector_addr );
		ret = 1; 
	}

	EE_WriteBlock(block,EE_buf,EE_mask);

	EE_WriteMoveDone(block,prev_bank);

	return ret;
}


//description
//	clean block. 
//	If item is bigger then threshold, move to next bank.
//	If no blank bank when it is moving, do SectorErase first.
//	If fSkipErase is 0, do SectorErase for garbage banks.
//parameter
//	fSkipErase- to reduce SectorErase. 
//	if 2, do not check the threshold.		
//
//If we donot have a blank bank when we move, we do SectorErase. 
//	
//return
//	low-nibble:	moving occur.
//	high-nibble: SectorErase occur
#define EE_INDEX_THRESHOLD	(64*4)

BYTE EE_CleanBlock(BYTE block, BYTE fSkipErase)
{
	DWORD sector_addr;
	BYTE i, j;

	BYTE ret=0;

	//Do you need to move a bank ?
	if(fSkipErase==2 || EE_WritePos[block] >= EE_INDEX_THRESHOLD) {
		ret++;

		if(EE_MoveBank(EE_CurrBank[block]))
			ret+= 0x10;
	}

	if(fSkipErase)
		//done.
		return ret;	//it can be 0 or 1, or 0x11.

 	//erase the used other banks
	for(i=1; i < EE_SPI_BANKS;i++) {   //note: start from 1
		j = (EE_CurrBank[block] + i) % EE_SPI_BANKS;		//get target bank
 		sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + j) * SPI_SECTOR_SIZE;

		SPI_ReadData2xdata( sector_addr, SPI_Buffer, 4L );
		if( (SPI_Buffer[0]==0xff) && (SPI_Buffer[1]==0xff) && (SPI_Buffer[2]==0xff) && (SPI_Buffer[3]==0xff) )
			//next is blank.
			continue;
		SPI_SectorErase( sector_addr );
		ret+=0x10;
	}

	return ret;
}



void EE_CleanBlocks(void)
{
	BYTE block;
	BYTE ret;

	dPrintf("\nEE_CleanBlocks ");
	for(block=0; block < EE_BLOCKS; block++) {
		dPrintf(" Block:%02bx-",block);
		ret=EE_CleanBlock(block, 0);	//normal
		if(ret)	dPrintf("clean");
		else	dPrintf("skip");
	}
}



void EE_DumpBlocks(void)
{
	BYTE block;
	BYTE i,j;

	for(block=0; block < EE_BLOCKS; block++) {
		Printf("\nBlock:%02bx Bank%d WritePos:%x", block, (WORD)EE_CurrBank[block], EE_WritePos[block]);

		EE_ReadBlock(block, EE_buf, EE_mask);
		for(i=0; i < (EE_INDEX_PER_BLOCK / 16); i++) {
			Printf("\n%03x:",(WORD)block*EE_INDEX_PER_BLOCK+i*16);
			for(j=0; j < 16; j++) {
				if(EE_mask[(i*16+j)>>3] & (1<<(j&0x07)))
					Printf("%02bx ",EE_buf[i*16+j]);
				else
					Printf("-- ");
			}
		}
	}
}
//=============================================================================
//
//=============================================================================
//EEPROM check routine
void EE_Check(void)
{
	BYTE block,bank;
	BYTE ret;
	WORD j;
	DWORD sector_addr;

	//print summary
	for(block=0; block < EE_BLOCKS; block++) {
		Printf("\nblock%bx ",block);
		for(bank=0; bank < EE_SPI_BANKS; bank++) {
			ret=EE_CheckBlankBank(block,bank);
			if(ret) {
				Printf("_");			//blank
			}
			else {
				ret=EE_CheckMoveDoneBank(block, bank);
				if(ret) Printf("X");	//done	  	
				else	Printf("U");	//used
			}
		}
	}
	//dump
	EE_DumpBlocks();
	
	//check corruptted items
	for(block=0; block < EE_BLOCKS; block++) {
		sector_addr = EE_SPI_SECTOR0 + ((DWORD)block*EE_SPI_BANKS + EE_CurrBank[block]) * SPI_SECTOR_SIZE;
		//read
		for(j=0; j < SPI_SECTOR_SIZE; j+=4) {

			if(j >= EE_WritePos[block])
				break;	

			//BKTODO:Use more big buffer size. Max SPI_BUFFER_SIZE(128)
			SPI_ReadData2xdata( sector_addr+j, SPI_Buffer, 4L );
			//check corruption
			if((SPI_Buffer[0]^SPI_Buffer[1]) != 0xFF || (SPI_Buffer[2]^SPI_Buffer[3]) != 0xFF) {
				Printf("\ncorrupted ?? Block%bx Bank%bx addr:%06lx [%02bx %02bx %02bx %02bx]",
					block,EE_CurrBank[block],
					sector_addr+j,
					SPI_Buffer[0], SPI_Buffer[1], SPI_Buffer[2], SPI_Buffer[3]);
			}
		}
   	}
}

#endif













