///=========================
#ifndef __APP_DSP_MIX_H
#define __APP_DSP_MIX_H





#define DSP_MIXER_MAX          186
#define DSP_MIXER_DEFAULT      144
#define DSP_MIXER_MIN            0


extern void Dsp_Mix_Init(void);
extern SCH_BOOL Dsp_Mix_Mixer(SCH_U8 Channel,SCH_U8 *data);
extern SCH_BOOL Dsp_Mix_Input(SCH_U8 data);

#endif
