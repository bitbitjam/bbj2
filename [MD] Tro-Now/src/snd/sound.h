#ifndef _SOUNDS_H_
#define _SOUNDS_H_

#define sfx01_size      28
#define sfx02_size      28

extern const u8 sfx01_data[sfx01_size];
extern const u8 sfx02_data[sfx02_size];

//////////////////////////////////////////////////////////////////////////////////////
// PSG PSG PSG
//////////////////////////////////////////////////////////////////////////////////////
#define PSG_DATA		0xc00011
#define PSG_VCH_MAX	4

struct {
	const u8 *data;
	i16 total;
	struct {
		struct {
			i16 ptr;
			i16 wait;
			i16 time;
			u16 div;
			u8 vol;
		} slot[PSG_VCH_MAX];
	} chn[4];
} PSGFX;

u16 psgFxInit( const u8 *data );
void   psgFxPlay      ( u16 psg );
void   psgFxFrame     ( void );
u16    psgfx_nb_tracks ( );


//////////////////////////////////////////////////////////////////////////////////////
// XGM XGM
//////////////////////////////////////////////////////////////////////////////////////
#define Z80_RAM_START                   0xA00000
#define Z80_RAM_END                     0xA01FFF
#define Z80_RAM                         Z80_RAM_START
#define Z80_RAM_LEN                     ((Z80_RAM_END - Z80_RAM_START) + 1)

#define Z80_HALT_PORT                   0xA11100
#define Z80_RESET_PORT                  0xA11200
#define Z80_DRV_PARAMS                  0xA00104
#define Z80_DRV_COMMAND                 0xA00100
#define Z80_DRV_STATUS                  0xA00102
#define Z80_DRV_STAT_PLAYING_SFT        0
#define Z80_DRV_COM_PLAY_SFT            0
#define Z80_DRV_COM_PLAY                (1 << Z80_DRV_COM_PLAY_SFT)

#define Z80_DRV_CH0_SFT                 0
#define Z80_DRV_CH1_SFT                 1
#define Z80_DRV_CH2_SFT                 2
#define Z80_DRV_CH3_SFT                 3

#define Z80_DRV_CH0                     (1 << Z80_DRV_CH0_SFT)
#define Z80_DRV_CH1                     (1 << Z80_DRV_CH1_SFT)
#define Z80_DRV_CH2                     (1 << Z80_DRV_CH2_SFT)
#define Z80_DRV_CH3                     (1 << Z80_DRV_CH3_SFT)

#define Z80_BUS_REQUEST     0x0100
#define Z80_BUS_RELEASE     0x0000

#define SOUND_PCM_CH1       Z80_DRV_CH0_SFT
#define SOUND_PCM_CH2       Z80_DRV_CH1_SFT
#define SOUND_PCM_CH3       Z80_DRV_CH2_SFT
#define SOUND_PCM_CH4       Z80_DRV_CH3_SFT

#define SOUND_PCM_CH1_MSK   Z80_DRV_CH0
#define SOUND_PCM_CH2_MSK   Z80_DRV_CH1
#define SOUND_PCM_CH3_MSK   Z80_DRV_CH2
#define SOUND_PCM_CH4_MSK   Z80_DRV_CH3


#define YM2612_BASEPORT     0xA04000

#define PSG_PORT            0xC00011

typedef struct
{
	u8  title[19];
	u8 *data;
	long  size;
}
SND;

extern const u8 z80_xgm[0x1700];

void SND_loadDriver_XGM(void);

long SND_loadSong_XGM(u8 song);
char *SND_nameSong_XGM(u8 song);
void SND_stopPlay_XGM();

long SND_loadPcm_XGM(u8 sfx);
char *SND_namePcm_XGM(u8 sfx);

#endif
