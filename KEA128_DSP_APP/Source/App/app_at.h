///=========================
#ifndef __APP_AT_H
#define __APP_AT_H
///===============================================
#define AT_START_END_S       0x41//0x0D
#define AT_START_END_E       0x54//0x0A

#define AT_START_END_A       0x41//A
#define AT_START_END_T       0x54//T
#define AT_START_END_SM      0x2D//-
#define AT_START_END_B       0x42//B
#define AT_START_END_SPACE   0x20// 
#define AT_START_END_END     0x0D//END
///=======================================================
#define BT_VER_GET       	"AT+B GVER\r"
#define BT_BLEADV_SET       "AT+B BLEADV 1\r"
#define BT_NAME_SET         "AT+B SLDN DSP_8CH1\r"
#define BT_NAME_GET         "AT+B GLDN\r"
#define BT_FACTORYRESET     "AT+FACTORYRESET\r"
#define BT_LBDADDR_GET      "AT+B GLBD\r"
#define BT_GOTA_STAT_GET      "AT+B GOTA\r"


#define BT_HFDISC_SET      "AT+B HFDISC\r"

#define BT_A2DP_PLAY       "AT+B AVRCPPLAY\r"
#define BT_A2DP_PAUSE      "AT+B AVRCPPAUSE\r"

extern SCH_U8 BtSPPCONFlag;
extern SCH_U8 bGUKAIBLE;

extern SCH_U8 BtGATTCONFlag;
extern SCH_U8 BtPHFCONFlag;

static SCH_U8 PCSTATFlag;

extern SCH_U8 BtTemplag;
extern SCH_U8 BtTemplag1;

typedef struct
{
	SCH_U8 Name[31];
	SCH_U8 Ble_Init;
}Rx_BLE_DATA;
extern Rx_BLE_DATA      BtRxBLE;

///======================================================
extern SCH_U8 BT_Addr[12+1];/*add one byte for HPF connect stat*/
extern SCH_U8 BLE_GVER[30];

extern void AtDataAnalyse(SCH_U8 *Data);
#endif
