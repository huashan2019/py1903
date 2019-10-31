#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#ifdef   SETTINGS_ROOT
#define  SET_EXT
#else
#define  SET_EXT     extern
#endif

#define PCLK_SEL_PCLK		0
#define PCLK_SEL_PLL		1
#define SPI_CK_SEL_27M		0
#define SPI_CK_SEL_32K		1
#define SPI_CK_SEL_PCLK		2
#define SPI_CK_DIV_1P0		0
#define SPI_CK_DIV_1P5		1
#define SPI_CK_DIV_2P0		2
#define SPI_CK_DIV_2P5		3


SET_EXT void SspllSetFreqAndPll(DWORD _PPF);
SET_EXT BYTE McuSpiClkToPclk(BYTE divider);
SET_EXT void McuSpiClkRestore(void);
SET_EXT void McuSpiClkSet(BYTE PCLK_SEL,BYTE SPI_CK_SEL,BYTE SPI_CK_DIV); 

#endif
