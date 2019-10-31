///=========================
#ifndef __APP_BT_H
#define __APP_BT_H
///=====================================================
#define HEAD_ADDRESS_BT      0xAA
#define HEAD_ADDRESS_MCU     0x55
///=====================================================
#define B2M_DSP_DATA     0x0A
#define M2B_DSP_DATA     0x0A

#define B2M_DSP_UPDATA     0x0B
#define M2B_DSP_UPDATA     0x0B

#define B2M_SPDIF_INFO     0x0C
#define M2B_SPDIF_INFO     0x0C

#define COMM_NACK_NG              0xF0
#define COMM_NACK_NO_SUPPORT      0xF3
#define COMM_NACK_BUSY            0xFC
#define COMM_ACK                  0xFF

#define BtRx_Head1     BtRxModuel.RxData[0]
#define BtRx_Head2     BtRxModuel.RxData[1]
#define BtRx_Length    BtRxModuel.RxData[2]
#define BtRx_Sequence  BtRxModuel.RxData[3]
#define BtRx_GroupID   BtRxModuel.RxData[4]
#define BtRx_SubID     BtRxModuel.RxData[5]
#define BtRx_Data      BtRxModuel.RxData[6]
#define BtTx_Head1     BtTxModuel.TxData[10]
#define BtTx_Head2     BtTxModuel.TxData[11]
#define BtTx_Length    BtTxModuel.TxData[12]
#define BtTx_Sequence  BtTxModuel.TxData[13]
#define BtTx_GroupID   BtTxModuel.TxData[14]
#define BtTx_SubID     BtTxModuel.TxData[15]
#define BtTx_Data      BtTxModuel.TxData[16]


typedef enum
{
	A2M_IDLE_CMD=0x00,
	A2M_SYS_CMD=0x01,
	A2M_UICC_CMD,
	A2M_RADIO_CMD,
	A2M_CAN_CMD,
	A2M_STUDY_KEY_CMD,
	A2M_AUDIO_CMD,
	A2M_USART_CMD,
	A2M_IAP_CMD,
	A2M_AUTO_TEST_CMD,
	A2M_BT_IAP_CMD = 0x0F,
	A2M_ACK = 0xff
}A2M_GROUP_ID;

typedef enum
{
	A2M_SYS_OK=0x01,
	A2M_SETUP,
	A2M_SETTING,
	A2M_MCU_VER,
	A2M_CAN_VER,
	A2M_PANEL_TYPE,
	A2M_SOURCE,
	A2M_BT_STATE,
	A2M_G3_STATE,
	A2M_WORK_STATE
}A2M_SYS_SUBID;


typedef enum
{
	M2A_SYS_CMD=0x01,
	M2A_UICC_CMD,
	M2A_RADIO_CMD,
	M2A_CAN_CMD,
	M2A_STUDY_KEY_CMD,
	M2A_AUDIO_CMD,
	M2A_USART_CMD,
	M2A_IAP_CMD,
	M2A_AUTO_TEST_CMD,
    M2A_END=0xff
}M2A_GROUP_ID;

typedef enum
{
	M2A_CAN_DATA=0x02,
	M2A_CAN_AAC=0x03,
	M2A_CAN_LIN=0x04,
	M2A_CAN_RADAR=0x05
}M2A_CAN_SUBID;


typedef enum
{
	RADIO_REQUEST=0x01,
	RADIO_INFO=0x02,
	RADIO_FREQLIST=0x03,
	RADIO_PTY=0x04,
	RADIO_PS=0x05,
	RADIO_TEXT=0x06
}M2A_RADIO_SUBID;

typedef enum
{
	M2A_REQUEST=0x01,
	M2A_POWER,
	M2A_MCU_VER,
	M2A_CAN_VER,
	M2A_VOLUME,
	M2A_PARK,
	M2A_REVERSE,
	M2A_VOLTAGE,
	M2A_SETUP,
	M2A_SOURCE,
	M2A_SCREEN,
	M2A_WORK_STATE
}M2A_SYS_SUBID;

typedef enum
{
	ASP_REQUEST=0x01,
	ASP_SETUP=0x02,
	ASP_EQ=0x03,
	ASP_BAND=0x04
}M2A_EQ_SUBID;

typedef enum
{
	M2A_IAP_REQ_C_L=0x01,    ///请求长度校验
	M2A_IAP_REQ_DATA=0x02,   ///请求数据
	M2A_IAP_REQ_RESET=0x03,  ///请求复位
}M2A_IAP_D;

///======================================================
extern void BtDataAnalyse(void);
extern void TASK_Bt_Pro(void);
extern SCH_BOOL Is_PASSWORD(SCH_U8 *Data);
extern SCH_BOOL Set_PASSWORD(SCH_U8 *Data);
#endif
