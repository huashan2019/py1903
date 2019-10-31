#ifndef        _BMP_ADR_H
#define        _BMP_ADR_H

#define      LUT_Main                                                                0x048400
#define      OFFSET_Main                                                             0x050000
#define      START_Main_IB8_Main_Style1_Aux_Sel_K02                                  (OFFSET_Main + 0x000000)
#define      START_Main_IB8_Main_Style1_BG_K01                                       (OFFSET_Main + 0x009610)
#define      START_Main_IB8_Main_Style1_DVD_Sel_K02                                  (OFFSET_Main + 0x09FA10)
#define      START_Main_IB8_Main_Style1_GPS_Sel_K02                                  (OFFSET_Main + 0x0A9020)
#define      START_Main_IB8_Main_Style1_HDMI_Sel_K02                                 (OFFSET_Main + 0x0B2630)
#define      START_Main_IB8_Main_Style1_OSD_Sel_K02                                  (OFFSET_Main + 0x0BBC40)
#define      START_Main_IB8_Main_Style1_Setup_Sel_K02                                (OFFSET_Main + 0x0C5250)
#define      START_Main_IB8_Main_Style1_Trace_Sel_K02                                (OFFSET_Main + 0x0CE860)
#define      START_Main_IB8_Main_Style1_VGA_Sel_K02                                  (OFFSET_Main + 0x0D7E70)

#define      LUT_DVD                                                                 0x049000
#define      OFFSET_DVD                                                              0x140000
#define      START_DVD_IB8_Aux_Style1_Menu_BG_K01                                    (OFFSET_DVD + 0x000000)
#define      START_DVD_IB8_DVD_Back_Sel_K02                                          (OFFSET_DVD + 0x00F400)
#define      START_DVD_IB8_DVD_Style1_Menu_BG_K01                                    (OFFSET_DVD + 0x011200)
#define      START_DVD_IB8_HDMI_Style1_Menu_BG_K01                                   (OFFSET_DVD + 0x020600)
#define      START_DVD_IB8_VGA_Style1_Menu_BG_K01                                    (OFFSET_DVD + 0x02FA00)

#define      LUT_Setup                                                               0x04A400
#define      OFFSET_Setup                                                            0x180000
#define      START_Setup_IB8_Setup_Back_Sel_K02                                      (OFFSET_Setup + 0x000000)
#define      START_Setup_IB8_Setup_Sys_BriMinus_Sel_K02                              (OFFSET_Setup + 0x001E00)
#define      START_Setup_IB8_Setup_Sys_BriPlus_Sel_K02                               (OFFSET_Setup + 0x003CA0)
#define      START_Setup_IB8_Setup_Sys_ContMinus_Sel_K02                             (OFFSET_Setup + 0x005B40)
#define      START_Setup_IB8_Setup_Sys_ContPlus_Sel_K02                              (OFFSET_Setup + 0x0079E0)
#define      START_Setup_IB8_Setup_Sys_HueMinus_Sel_K02                              (OFFSET_Setup + 0x009880)
#define      START_Setup_IB8_Setup_Sys_HuePlus_Sel_K02                               (OFFSET_Setup + 0x00B720)
#define      START_Setup_IB8_Setup_Sys_Style1_BG_K01                                 (OFFSET_Setup + 0x00D5C0)

#define      LUT_Welcome                                                             0x04B000
#define      OFFSET_Welcome                                                          0x230000
#define      START_Welcome_IB8_Logo_TechWell                                         (OFFSET_Welcome + 0x000000)

#define      LUT_OtherPage2                                                          0x04BC00
#define      OFFSET_OtherPage2                                                       0x2D0000
#define      START_OtherPage2_IB8_OSDDemo_ShowBird_K01                               (OFFSET_OtherPage2 + 0x000000)

#define      LUT_OtherPage3                                                          0x000010//0x04C000
#define      OFFSET_OtherPage3                                                       0x000000//0x300000
#define      START_OtherPage3_IB8_CarRadar_CarModel_K01                              (OFFSET_OtherPage3 + 0x000000)/*car*/

#define      START_OtherPage3_IB8_CarRadar_FrontLeft00_K01                           (OFFSET_OtherPage3 + 0x0098D0)/*FL*/
#define      START_OtherPage3_IB8_CarRadar_FrontLeft01_K01                           (OFFSET_OtherPage3 + 0x00B5A0)
#define      START_OtherPage3_IB8_CarRadar_FrontLeft02_K01                           (OFFSET_OtherPage3 + 0x00D270)
#define      START_OtherPage3_IB8_CarRadar_FrontLeft03_K01                           (OFFSET_OtherPage3 + 0x00EF40)

#define      START_OtherPage3_IB8_CarRadar_FrontRight00_K01                          (OFFSET_OtherPage3 + 0x0128E0)//FR
#define      START_OtherPage3_IB8_CarRadar_FrontRight01_K01                          (OFFSET_OtherPage3 + 0x0145B0)// (OFFSET_OtherPage3 + 0x116840)
#define      START_OtherPage3_IB8_CarRadar_FrontRight02_K01                          (OFFSET_OtherPage3 + 0x016280)//(OFFSET_OtherPage3 + 0x11AAE0)
#define      START_OtherPage3_IB8_CarRadar_FrontRight03_K01                          (OFFSET_OtherPage3 + 0x017F50)

#define      START_OtherPage3_IB8_CarRadar_RearLeft00_K01                            (OFFSET_OtherPage3 + 0x008410)/*RL*/
#define      START_OtherPage3_IB8_CarRadar_RearLeft01_K01                            (OFFSET_OtherPage3 + 0x00A500)
#define      START_OtherPage3_IB8_CarRadar_RearLeft02_K01                            (OFFSET_OtherPage3 + 0x00C5F0)
#define      START_OtherPage3_IB8_CarRadar_RearLeft03_K01                            (OFFSET_OtherPage3 + 0x00E6E0)

#define      START_OtherPage3_IB8_CarRadar_RearLeft10_K01                            (OFFSET_OtherPage3 + 0x0107D0)/*RL*/
#define      START_OtherPage3_IB8_CarRadar_RearLeft11_K01                            (OFFSET_OtherPage3 + 0x0128C0)
#define      START_OtherPage3_IB8_CarRadar_RearLeft12_K01                            (OFFSET_OtherPage3 + 0x0149B0)
#define      START_OtherPage3_IB8_CarRadar_RearLeft13_K01                            (OFFSET_OtherPage3 + 0x016AA0)

#define      START_OtherPage3_IB8_CarRadar_RearLeft20_K01                            (OFFSET_OtherPage3 + 0x018B90)/*RL*/
#define      START_OtherPage3_IB8_CarRadar_RearLeft21_K01                            (OFFSET_OtherPage3 + 0x01AC80)
#define      START_OtherPage3_IB8_CarRadar_RearLeft22_K01                            (OFFSET_OtherPage3 + 0x01CD70)
#define      START_OtherPage3_IB8_CarRadar_RearLeft23_K01                            (OFFSET_OtherPage3 + 0x01EE60)

#define      START_OtherPage3_IB8_CarRadar_RearLeft30_K01                            (OFFSET_OtherPage3 + 0x020F50)/*RL*/
#define      START_OtherPage3_IB8_CarRadar_RearLeft31_K01                            (OFFSET_OtherPage3 + 0x023040)
#define      START_OtherPage3_IB8_CarRadar_RearLeft32_K01                            (OFFSET_OtherPage3 + 0x025130)
#define      START_OtherPage3_IB8_CarRadar_RearLeft33_K01                            (OFFSET_OtherPage3 + 0x027220)

#define      START_OtherPage3_IB8_CarRadar_RearLeftM00_K01                            (OFFSET_OtherPage3 + 0x00FD50)/*RL_M*/
#define      START_OtherPage3_IB8_CarRadar_RearLeftM01_K01                            (OFFSET_OtherPage3 + 0x011560)
#define      START_OtherPage3_IB8_CarRadar_RearLeftM02_K01                            (OFFSET_OtherPage3 + 0x012D70)
#define      START_OtherPage3_IB8_CarRadar_RearLeftM03_K01                            (OFFSET_OtherPage3 + 0x014580)

#define      START_OtherPage3_IB8_CarRadar_RearRightM00_K01                           (OFFSET_OtherPage3 + 0x015D90)/*RR_M*/
#define      START_OtherPage3_IB8_CarRadar_RearRightM01_K01                           (OFFSET_OtherPage3 + 0x0175A0)
#define      START_OtherPage3_IB8_CarRadar_RearRightM02_K01                           (OFFSET_OtherPage3 + 0x018DB0)
#define      START_OtherPage3_IB8_CarRadar_RearRightM03_K01                           (OFFSET_OtherPage3 + 0x01A5C0)


#define      START_OtherPage3_IB8_CarRadar_RearRight00_K01                           (OFFSET_OtherPage3 + 0x029310)/*RR*/
#define      START_OtherPage3_IB8_CarRadar_RearRight01_K01                           (OFFSET_OtherPage3 + 0x02B400)
#define      START_OtherPage3_IB8_CarRadar_RearRight02_K01                           (OFFSET_OtherPage3 + 0x02D4F0)
#define      START_OtherPage3_IB8_CarRadar_RearRight03_K01                           (OFFSET_OtherPage3 + 0x02F5E0)

#define      START_OtherPage3_IB8_CarRadar_RearRight10_K01                           (OFFSET_OtherPage3 + 0x0316D0)/*RR*/
#define      START_OtherPage3_IB8_CarRadar_RearRight11_K01                           (OFFSET_OtherPage3 + 0x0337C0)
#define      START_OtherPage3_IB8_CarRadar_RearRight12_K01                           (OFFSET_OtherPage3 + 0x0358B0)
#define      START_OtherPage3_IB8_CarRadar_RearRight13_K01                           (OFFSET_OtherPage3 + 0x0379A0)

#define      START_OtherPage3_IB8_CarRadar_RearRight20_K01                           (OFFSET_OtherPage3 + 0x039A90)/*RR*/
#define      START_OtherPage3_IB8_CarRadar_RearRight21_K01                           (OFFSET_OtherPage3 + 0x03BB80)
#define      START_OtherPage3_IB8_CarRadar_RearRight22_K01                           (OFFSET_OtherPage3 + 0x03DC70)
#define      START_OtherPage3_IB8_CarRadar_RearRight23_K01                           (OFFSET_OtherPage3 + 0x03FD60)

#define      START_OtherPage3_IB8_CarRadar_RearRight30_K01                           (OFFSET_OtherPage3 + 0x041E50)/*RR*/
#define      START_OtherPage3_IB8_CarRadar_RearRight31_K01                           (OFFSET_OtherPage3 + 0x043F40)
#define      START_OtherPage3_IB8_CarRadar_RearRight32_K01                           (OFFSET_OtherPage3 + 0x046030)
#define      START_OtherPage3_IB8_CarRadar_RearRight33_K01                           (OFFSET_OtherPage3 + 0x048120)



//BMP Size
#define      W_IB8_Main_Style1_Aux_Sel_K02                                     0x00C4
#define      H_IB8_Main_Style1_Aux_Sel_K02                                     0x00C4

#define      W_IB8_Main_Style1_BG_K01                                          0x0400
#define      H_IB8_Main_Style1_BG_K01                                          0x0259

#define      W_IB8_Main_Style1_DVD_Sel_K02                                     0x00C4
#define      H_IB8_Main_Style1_DVD_Sel_K02                                     0x00C4

#define      W_IB8_Main_Style1_GPS_Sel_K02                                     0x00C4
#define      H_IB8_Main_Style1_GPS_Sel_K02                                     0x00C4

#define      W_IB8_Main_Style1_HDMI_Sel_K02                                    0x00C4
#define      H_IB8_Main_Style1_HDMI_Sel_K02                                    0x00C4

#define      W_IB8_Main_Style1_OSD_Sel_K02                                     0x00C4
#define      H_IB8_Main_Style1_OSD_Sel_K02                                     0x00C4

#define      W_IB8_Main_Style1_Setup_Sel_K02                                   0x00C4
#define      H_IB8_Main_Style1_Setup_Sel_K02                                   0x00C4

#define      W_IB8_Main_Style1_Trace_Sel_K02                                   0x00C4
#define      H_IB8_Main_Style1_Trace_Sel_K02                                   0x00C4

#define      W_IB8_Main_Style1_VGA_Sel_K02                                     0x00C4
#define      H_IB8_Main_Style1_VGA_Sel_K02                                     0x00C4

#define      W_IB8_Aux_Style1_Menu_BG_K01                                      0x0400
#define      H_IB8_Aux_Style1_Menu_BG_K01                                      0x003D

#define      W_IB8_DVD_Back_Sel_K02                                            0x0080
#define      H_IB8_DVD_Back_Sel_K02                                            0x003C

#define      W_IB8_DVD_Style1_Menu_BG_K01                                      0x0400
#define      H_IB8_DVD_Style1_Menu_BG_K01                                      0x003D

#define      W_IB8_HDMI_Style1_Menu_BG_K01                                     0x0400
#define      H_IB8_HDMI_Style1_Menu_BG_K01                                     0x003D

#define      W_IB8_VGA_Style1_Menu_BG_K01                                      0x0400
#define      H_IB8_VGA_Style1_Menu_BG_K01                                      0x003D

#define      W_IB8_Setup_Back_Sel_K02                                          0x0080
#define      H_IB8_Setup_Back_Sel_K02                                          0x003C

#define      W_IB8_Setup_Sys_BriMinus_Sel_K02                                  0x0070
#define      H_IB8_Setup_Sys_BriMinus_Sel_K02                                  0x0046

#define      W_IB8_Setup_Sys_BriPlus_Sel_K02                                   0x0070
#define      H_IB8_Setup_Sys_BriPlus_Sel_K02                                   0x0046

#define      W_IB8_Setup_Sys_ContMinus_Sel_K02                                 0x0070
#define      H_IB8_Setup_Sys_ContMinus_Sel_K02                                 0x0046

#define      W_IB8_Setup_Sys_ContPlus_Sel_K02                                  0x0070
#define      H_IB8_Setup_Sys_ContPlus_Sel_K02                                  0x0046

#define      W_IB8_Setup_Sys_HueMinus_Sel_K02                                  0x0070
#define      H_IB8_Setup_Sys_HueMinus_Sel_K02                                  0x0046

#define      W_IB8_Setup_Sys_HuePlus_Sel_K02                                   0x0070
#define      H_IB8_Setup_Sys_HuePlus_Sel_K02                                   0x0046

#define      W_IB8_Setup_Sys_Style1_BG_K01                                     0x0400
#define      H_IB8_Setup_Sys_Style1_BG_K01                                     0x0259

#define      W_IB8_Logo_TechWell                                               0x0400
#define      H_IB8_Logo_TechWell                                               0x0259

#define      W_IB8_OSDDemo_ShowBird_K01                                        0x01E0
#define      H_IB8_OSDDemo_ShowBird_K01                                        0x0118

#define      W_IB8_CarRadar_CarModel_K01                                      126// 0x0086
#define      H_IB8_CarRadar_CarModel_K01                                       260//0x00F0

#define      W_IB8_CarRadar_FrontLeft00_K01                                    64//0x0068
#define      H_IB8_CarRadar_FrontLeft00_K01                                    80//0x00A4

#define      W_IB8_CarRadar_FrontLeft01_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft01_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft02_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft02_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft03_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft03_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft04_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft04_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft05_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft05_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft06_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft06_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft07_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft07_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft08_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft08_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft09_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft09_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft10_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft10_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft11_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft11_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft12_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft12_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft13_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft13_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft14_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft14_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft15_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft15_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft16_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft16_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft17_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft17_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft18_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft18_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft19_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft19_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft20_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft20_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft21_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft21_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft22_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft22_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft23_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft23_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft24_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft24_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft25_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft25_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft26_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft26_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft27_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft27_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft28_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft28_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft29_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft29_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft30_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft30_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft31_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft31_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft32_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft32_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft33_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft33_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft34_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft34_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft35_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft35_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft36_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft36_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft37_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft37_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft38_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft38_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft39_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft39_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft40_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft40_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft41_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft41_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft42_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft42_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft43_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft43_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft44_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft44_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft45_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft45_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft46_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft46_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft47_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft47_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft48_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft48_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft49_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft49_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft50_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft50_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft51_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft51_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft52_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft52_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft53_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft53_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft54_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft54_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft55_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft55_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft56_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft56_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft57_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft57_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft58_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft58_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft59_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft59_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft60_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft60_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft61_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft61_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft62_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft62_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontLeft63_K01                                    0x0068
#define      H_IB8_CarRadar_FrontLeft63_K01                                    0x00A4

#define      W_IB8_CarRadar_FrontRight00_K01                                  64// 0x0068
#define      H_IB8_CarRadar_FrontRight00_K01                                  80// 0x00A4

#define      W_IB8_CarRadar_FrontRight01_K01                                   64// 0x0068
#define      H_IB8_CarRadar_FrontRight01_K01                                  80// 0x00A4

#define      W_IB8_CarRadar_FrontRight02_K01                                     64// 0x0068
#define      H_IB8_CarRadar_FrontRight02_K01                                   80// 0x00A4

#define      W_IB8_CarRadar_FrontRight03_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight03_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight04_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight04_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight05_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight05_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight06_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight06_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight07_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight07_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight08_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight08_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight09_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight09_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight10_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight10_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight11_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight11_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight12_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight12_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight13_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight13_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight14_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight14_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight15_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight15_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight16_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight16_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight17_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight17_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight18_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight18_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight19_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight19_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight20_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight20_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight21_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight21_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight22_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight22_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight23_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight23_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight24_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight24_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight25_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight25_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight26_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight26_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight27_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight27_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight28_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight28_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight29_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight29_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight30_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight30_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight31_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight31_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight32_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight32_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight33_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight33_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight34_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight34_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight35_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight35_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight36_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight36_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight37_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight37_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight38_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight38_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight39_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight39_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight40_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight40_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight41_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight41_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight42_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight42_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight43_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight43_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight44_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight44_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight45_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight45_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight46_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight46_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight47_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight47_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight48_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight48_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight49_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight49_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight50_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight50_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight51_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight51_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight52_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight52_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight53_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight53_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight54_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight54_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight55_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight55_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight56_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight56_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight57_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight57_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight58_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight58_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight59_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight59_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight60_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight60_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight61_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight61_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight62_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight62_K01                                   0x00A4

#define      W_IB8_CarRadar_FrontRight63_K01                                   0x0068
#define      H_IB8_CarRadar_FrontRight63_K01                                   0x00A4

#define      W_IB8_CarRadar_RearLeft00_K01                                     90//0x0068
#define      H_IB8_CarRadar_RearLeft00_K01                                     82//0x00A4

#define      W_IB8_CarRadar_RearLeft01_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft01_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft02_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft02_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft03_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft03_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft04_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft04_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft05_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft05_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft06_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft06_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft07_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft07_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft08_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft08_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft09_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft09_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft10_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft10_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft11_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft11_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft12_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft12_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft13_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft13_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft14_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft14_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft15_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft15_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft16_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft16_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft17_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft17_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft18_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft18_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft19_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft19_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft20_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft20_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft21_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft21_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft22_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft22_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft23_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft23_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft24_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft24_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft25_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft25_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft26_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft26_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft27_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft27_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft28_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft28_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft29_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft29_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft30_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft30_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft31_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft31_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft32_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft32_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft33_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft33_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft34_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft34_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft35_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft35_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft36_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft36_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft37_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft37_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft38_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft38_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft39_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft39_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft40_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft40_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft41_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft41_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft42_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft42_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft43_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft43_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft44_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft44_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft45_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft45_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft46_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft46_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft47_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft47_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft48_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft48_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft49_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft49_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft50_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft50_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft51_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft51_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft52_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft52_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft53_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft53_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft54_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft54_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft55_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft55_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft56_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft56_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft57_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft57_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft58_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft58_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft59_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft59_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft60_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft60_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft61_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft61_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft62_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft62_K01                                     0x00A4

#define      W_IB8_CarRadar_RearLeft63_K01                                     0x0068
#define      H_IB8_CarRadar_RearLeft63_K01                                     0x00A4

#define      W_IB8_CarRadar_RearRight00_K01                                   90// 0x0068
#define      H_IB8_CarRadar_RearRight00_K01                                   82// 0x00A4

#define      W_IB8_CarRadar_RearRight01_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight01_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight02_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight02_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight03_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight03_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight04_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight04_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight05_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight05_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight06_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight06_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight07_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight07_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight08_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight08_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight09_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight09_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight10_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight10_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight11_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight11_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight12_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight12_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight13_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight13_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight14_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight14_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight15_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight15_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight16_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight16_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight17_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight17_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight18_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight18_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight19_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight19_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight20_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight20_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight21_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight21_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight22_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight22_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight23_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight23_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight24_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight24_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight25_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight25_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight26_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight26_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight27_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight27_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight28_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight28_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight29_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight29_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight30_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight30_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight31_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight31_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight32_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight32_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight33_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight33_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight34_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight34_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight35_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight35_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight36_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight36_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight37_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight37_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight38_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight38_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight39_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight39_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight40_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight40_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight41_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight41_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight42_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight42_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight43_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight43_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight44_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight44_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight45_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight45_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight46_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight46_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight47_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight47_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight48_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight48_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight49_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight49_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight50_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight50_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight51_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight51_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight52_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight52_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight53_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight53_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight54_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight54_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight55_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight55_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight56_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight56_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight57_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight57_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight58_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight58_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight59_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight59_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight60_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight60_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight61_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight61_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight62_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight62_K01                                    0x00A4

#define      W_IB8_CarRadar_RearRight63_K01                                    0x0068
#define      H_IB8_CarRadar_RearRight63_K01                                    0x00A4


#endif
