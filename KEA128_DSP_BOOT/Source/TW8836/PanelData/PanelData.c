//*****************************************************************************
//
//
//#pragma NOAREGS
#define	CODE	 
#include "include.h"

#if (CUR_PANEL == HANNSTAR_PANEL_7INCH_1280)
	CODE	BYTE	PANELDATA_CVBS_ALL[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1280_CVBS_All.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_CVBS_NTSC[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1280_CVBS_N.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_CVBS_PAL[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1280_CVBS_P.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_DVD_YUV[] = {						  	//YUV
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1280_DVD_RGB_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_RGB[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1280_VGA_RGB_All_Firm.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_DTV_RGB[] = {							//Digital RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1280_HDMI_RGB_ALL_Firm.txt"
		0xFF, 0xFF,
	};
#elif (CUR_PANEL == HANNSTAR_PANEL_7INCH_1024)
	CODE	BYTE	PANELDATA_CVBS_ALL[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_CVBS_All.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_CVBS_ALL_REVB[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_CVBS_All_RevB.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_CVBS_NTSC[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_CVBS_N.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_CVBS_PAL[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_CVBS_P.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_DVD_YUV[] = {						  	//YUV
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_DVD_RGB_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_RGB[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_All_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_RGB_REVB[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_All_Firm_RevB.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_DTV_RGB[] = {							//Digital RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_HDMI_RGB_ALL_Firm.txt"
		0xFF, 0xFF,
	};


	CODE	BYTE	PANELDATA_VGA_640X480_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_640X480_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_800X480_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_800X480_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_800X600_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_800X600_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_1024X600_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_1024X600_Firm.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_VGA_1024X768_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_1024X768_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_1280X720_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_1280X720_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_1280X800_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_1280X800_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_1280X1024_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_1280X1024_Firm.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_VGA_1366X768_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_VGA_RGB_1366X768_Firm.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_HDMI_640X480_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_HDMI_RGB_640X480_Firm.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_HDMI_1280X720_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_HDMI_RGB_1280X720_Firm.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_HDMI_1920X1080_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_HDMI_RGB_1920X1080_Firm.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_HDMI_1920X1080i_PATCH[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_1024_HDMI_RGB_1920X1080i_Firm.txt"
		0xFF, 0xFF,
	};	
#else
	CODE	BYTE	PANELDATA_CVBS_ALL[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_800_CVBS_All.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_CVBS_NTSC[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_800_CVBS_N.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_CVBS_PAL[] = {
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_800_CVBS_P.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_DVD_YUV[] = {						  	//YUV
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_800_DVD_RGB_Firm.txt"
		0xFF, 0xFF,
	};

	CODE	BYTE	PANELDATA_VGA_RGB[] = {						  	//Analog RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_800_VGA_RGB_All_Firm.txt"
		0xFF, 0xFF,
	};
	
	CODE	BYTE	PANELDATA_DTV_RGB[] = {							//Digital RGB
	  	0x00, 0x00,	//c 8a
		#include	"PanelData\Hanstar_800_HDMI_RGB_ALL_Firm.txt"
		0xFF, 0xFF,
	};
#endif
