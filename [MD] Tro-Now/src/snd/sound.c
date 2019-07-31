#include "..\main.h"

#include "smp_null.h"

// canciones XGM
#include "xgm\tracks.h"

//////////////////////////////////////////////////////////////////////////////////////
// XGM XGM XGM
//////////////////////////////////////////////////////////////////////////////////////
static const SND _XgmList [] =
{
	// song 00
	{ "TITLE  ",      (u8*) xgm01,    sizeof(xgm01) },
	{ "MENU   ",      (u8*) xgm02,    sizeof(xgm02) },
	{ "INGAME ",      (u8*) xgm03,    sizeof(xgm03) },
	{ "RECORDS ",     (u8*) xgm04,    sizeof(xgm03) },

               // <-- Añadir líneas como las de arriba.
               //     Primero título, luego el nombre del VGM, y luego
               //     el patrón, que para nosotros siempre será 0


	{ "EOF",                 0,         0 } // no quitar!
};

static const SND _PcmList [] =
{
	// song 00
	{ "SFX1               ",      (u8*) pcm01,    sizeof(pcm01) },
	{ "SFX2               ",      (u8*) pcm02,    sizeof(pcm02) },
	{ "SFX3               ",      (u8*) pcm03,    sizeof(pcm03) },

               // <-- Añadir líneas como las de arriba.
               //     Primero título, luego el nombre del VGM, y luego
               //     el patrón, que para nosotros siempre será 0


	{ "EOF",                 0,         0 } // no quitar!
};

//////////////////////////////////////////////////////////////////////////////////////
//
// control del z80
//
//////////////////////////////////////////////////////////////////////////////////////
void Z80_startReset(){
    vu16 *pw;

    pw = (u16 *) Z80_RESET_PORT;
    *pw = 0x0000;
}

void Z80_endReset(){
    vu16 *pw;

    pw = (u16 *) Z80_RESET_PORT;
    *pw = 0x0100;
}

u16 Z80_isBusTaken(){
    vu16 *pw;

    pw = (u16 *) Z80_HALT_PORT;
    if (*pw & 0x0100) return 0;
    else return 1;
}

void Z80_upload(const u16 to, const u8 *from, const u16 size, const u16 resetz80){

    z80_busrequest(Z80_BUS_REQUEST);

    // copy data to Z80 RAM (need to use byte copy here)
    u8* src = (u8*) from;
    u8* dst = (u8*) (Z80_RAM + to);
    u16 len = size;

    while(len--) *dst++ = *src++;

    if (resetz80) Z80_startReset();
    z80_busrequest(Z80_BUS_RELEASE);
    // wait bus released
    while(Z80_isBusTaken());
    if (resetz80) Z80_endReset();
}


//////////////////////////////////////////////////////////////////////////////////////
//
// control del YM2612
//
//////////////////////////////////////////////////////////////////////////////////////
void YM2612_write(const u16 port, const u8 data){

    vs8 *pb;

    pb = (u8*) YM2612_BASEPORT;

    // wait while YM2612 busy
    while (*pb < 0);
    // write data
    pb[port & 3] = data;
}

void YM2612_reset(){

    u16 i;
    u16 bus_taken;

    bus_taken = Z80_isBusTaken();
    if (!bus_taken)
        z80_busrequest(Z80_BUS_REQUEST);

    // enable left and right output for all channel
    for(i = 0; i < 3; i++)
    {
        YM2612_write(0, 0xB4 | i);
        YM2612_write(1, 0xC0);
        YM2612_write(2, 0xB4 | i);
        YM2612_write(3, 0xC0);
    }

    // disable LFO
    YM2612_write(0, 0x22);
    YM2612_write(1, 0x00);

    // disable timer & set channel 6 to normal mode
    YM2612_write(0, 0x27);
    YM2612_write(1, 0x00);

    // ALL KEY OFF
    YM2612_write(0, 0x28);
    for (i = 0; i < 3; i++)
    {
        YM2612_write(1, 0x00 | i);
        YM2612_write(1, 0x04 | i);
    }

    // disable DAC
    YM2612_write(0, 0x2B);
    YM2612_write(1, 0x00);

    if (!bus_taken)
        z80_busrequest(Z80_BUS_RELEASE);
}

void PSG_init(){

    vu8 *pb;
    u16 i;

    pb = (u8*) PSG_PORT;

    for (i = 0; i < 4; i++)
    {
        // set tone to 0
        *pb = 0x80 | (i << 5) | 0x00;
        *pb = 0x00;

        // set envelope to silent
        *pb = 0x90 | (i << 5) | 0x0F;
    }
}


//////////////////////////////////////////////////////////////////////////////////////
//
// funciones del driver XGM
//
//////////////////////////////////////////////////////////////////////////////////////
void SND_startPlay_XGM(const u8 *song){

    u8 ids[0x100-4];
    u32 addr;
    u16 i;
    vu8 *pb;

    // load the appropried driver if not already done
    //Z80_loadDriver(Z80_DRIVER_XGM, TRUE);

    z80_busrequest(Z80_BUS_REQUEST);

    // prepare sample id table
    for(i = 0; i < 0x3F; i++)
    {
        // sample address in sample bank data
        addr = song[(i * 4) + 0] << 8;
        addr |= song[(i * 4) + 1] << 16;

        // silent sample ? use null sample address
        if (addr == 0xFFFF00) addr = (u32) smp_null;
        // adjust sample address (make it absolute)
        else addr += ((u32) song) + 0x100;

        // write adjusted addr
        ids[(i * 4) + 0] = addr >> 8;
        ids[(i * 4) + 1] = addr >> 16;
        // and recopy len
        ids[(i * 4) + 2] = song[(i * 4) + 2];
        ids[(i * 4) + 3] = song[(i * 4) + 3];
    }

    // upload sample id table (first entry is silent sample, we don't transfer it)
    Z80_upload(0x1C00 + 4, ids, 0x100 - 4, FALSE);

    // Z80 upload release bus after operation
    z80_busrequest(Z80_BUS_REQUEST);

    // point to Z80 XGM address parameter
    pb = (u8 *) (Z80_DRV_PARAMS + 0x00);

    // get song address and bypass sample id table
    addr = ((u32) song) + 0x100;

    // bypass sample data (use the sample data size)
    addr += song[0xFC] << 8;
    addr += song[0xFD] << 16;

    // and bypass the music data size field
    addr += 4;

    // set XGM music data address
    pb[0x00] = addr >> 0;
    pb[0x01] = addr >> 8;
    pb[0x02] = addr >> 16;
    pb[0x03] = addr >> 24;

    // point to Z80 command
    pb = (u8 *) Z80_DRV_COMMAND;
    // set play XGM command
    *pb |= (1 << 6);

    z80_busrequest(Z80_BUS_RELEASE);
}

u8 SND_isPlaying_XGM(){

    vu8 *pb;
    u8 ret;

    // load the appropried driver if not already done
    //Z80_loadDriver(Z80_DRIVER_XGM, TRUE);

    z80_busrequest(Z80_BUS_REQUEST);

    // point to Z80 status
    pb = (u8 *) Z80_DRV_STATUS;
    // play status
    ret = *pb & (1 << 6);

    z80_busrequest(Z80_BUS_RELEASE);

    return ret;
}

void SND_stopPlay_XGM(){
    vu8 *pb;

    // load the appropried driver if not already done
    //Z80_loadDriver(Z80_DRIVER_XGM, TRUE);

    z80_busrequest(Z80_BUS_REQUEST);

    // point to Z80 command
    pb = (u8 *) Z80_DRV_COMMAND;

    // set stop XGM command
    *pb |= (1 << 4);

    z80_busrequest(Z80_BUS_RELEASE);
}

void SND_resumePlay_XGM(){

    vu8 *pb;

    // load the appropried driver if not already done
    //Z80_loadDriver(Z80_DRIVER_XGM, TRUE);

    z80_busrequest(Z80_BUS_REQUEST);

    // point to Z80 command
    pb = (u8 *) Z80_DRV_COMMAND;
    // set resume XGM command
    *pb |= (1 << 5);

    z80_busrequest(Z80_BUS_RELEASE);
}

u8 SND_isPlayingPCM_XGM(const u16 channel_mask){

    vu8 *pb;
    u8 ret;

    // load the appropried driver if not already done
    //Z80_loadDriver(Z80_DRIVER_XGM, TRUE);

    z80_busrequest(Z80_BUS_REQUEST);

    // point to Z80 status
    pb = (u8 *) Z80_DRV_STATUS;
    // play status
    ret = *pb & (channel_mask << Z80_DRV_STAT_PLAYING_SFT);

    z80_busrequest(Z80_BUS_RELEASE);

    return ret;
}

void SND_setPCMFast_XGM(const u8 id, const u8 *sample, const u32 len){

    vu8 *pb;

    // point to sample id table
    pb = (u8 *) (0xA01C00 + (id * 4));

    // write sample addr
    pb[0x00] = ((u32) sample) >> 8;
    pb[0x01] = ((u32) sample) >> 16;
    pb[0x02] = len >> 8;
    pb[0x03] = len >> 16;
}

void SND_setPCM_XGM(const u8 id, const u8 *sample, const u32 len){
    // load the appropried driver if not already done
    //Z80_loadDriver(Z80_DRIVER_XGM, TRUE);

    z80_busrequest(Z80_BUS_REQUEST);

    SND_setPCMFast_XGM(id, sample, len);

    z80_busrequest(Z80_BUS_RELEASE);
}

void SND_startPlayPCM_XGM(const u8 id, const u8 priority, const u16 channel){

    vu8 *pb;

    // load the appropried driver if not already done
    //Z80_loadDriver(Z80_DRIVER_XGM, TRUE);

    z80_busrequest(Z80_BUS_REQUEST);

    // point to Z80 PCM parameters
    pb = (u8 *) (Z80_DRV_PARAMS + 0x04 + (channel * 2));

    // set PCM priority and id
    pb[0x00] = priority & 0xF;
    pb[0x01] = id;

    // point to Z80 command
    pb = (u8 *) Z80_DRV_COMMAND;
    // set play PCM channel command
    *pb |= (Z80_DRV_COM_PLAY << channel);

    z80_busrequest(Z80_BUS_RELEASE);
}

void SND_stopPlayPCM_XGM(const u16 channel){

    vu8 *pb;

    // load the appropried driver if not already done
    //Z80_loadDriver(Z80_DRIVER_XGM, TRUE);

    z80_busrequest(Z80_BUS_REQUEST);

    // point to Z80 PCM parameters
    pb = (u8 *) (Z80_DRV_PARAMS + 0x04 + (channel * 2));

    // use silent PCM (id = 0) with minimum priority
    pb[0x00] = 0;
    pb[0x01] = 0;

    // point to Z80 command
    pb = (u8 *) Z80_DRV_COMMAND;
    // set play PCM channel command
    *pb |= (Z80_DRV_COM_PLAY << channel);

    z80_busrequest(Z80_BUS_RELEASE);
}


//////////////////////////////////////////////////////////////////////////////////////
//
// void SND_loadDriver_XGM(void){
// carga el driver XGM
//
//////////////////////////////////////////////////////////////////////////////////////
void SND_loadDriver_XGM(void){

    const u8 *drv;
    u16 len;

    vu8 *pb;
    u32 addr;

    drv = z80_xgm;
    len = sizeof(z80_xgm);

    // clear z80 sram
    z80_memclr((void *)0xA00000, 0x2000);

    // upload Z80 driver and reset Z80
    Z80_upload(0, drv, len, 1);

    // reset sound chips
    YM2612_reset();
    PSG_init();

    // misc parameters initialisation
    z80_busrequest(Z80_BUS_REQUEST);
    // point to Z80 sample id table (first entry = silent sample)
    pb = (u8 *) (0xA01C00);

    addr = (u32) smp_null;
    // null sample address (256 bytes aligned)
    pb[0] = addr >> 8;
    pb[1] = addr >> 16;
    // null sample length (256 bytes aligned)
    pb[2] = sizeof(smp_null) >> 8;
    pb[3] = sizeof(smp_null) >> 16;
    z80_busrequest(Z80_BUS_RELEASE);
    // just wait a bit of time
    delay(2);
}


//////////////////////////////////////////////////////////////////////////////////////
//
// long SND_loadSong_XGM(u8 song){
// carga la cancion XGM indicada
//
//////////////////////////////////////////////////////////////////////////////////////
long SND_loadSong_XGM(u8 song){

    SND_startPlay_XGM(_XgmList[song].data);

    return _XgmList[song].size;
}


long SND_loadPcm_XGM(u8 sfx){

    SND_setPCM_XGM(66, _PcmList[sfx].data, _PcmList[sfx].size);

    if (SND_isPlayingPCM_XGM(SOUND_PCM_CH4_MSK))
        SND_stopPlayPCM_XGM(SOUND_PCM_CH4);
    else
        SND_startPlayPCM_XGM(66, 10, SOUND_PCM_CH4);

    return _PcmList[sfx].size;
}


//////////////////////////////////////////////////////////////////////////////////////
// PSG PSG PSG
//////////////////////////////////////////////////////////////////////////////////////
static void _psgfx_addch ( u16 chn,u16 off );

//////////////////////////////////////////////////////////////////////////////////////
//
// static void _psgfx_addch ( u16 chn, u16 off ){
//
//////////////////////////////////////////////////////////////////////////////////////
static void _psgfx_addch ( u16 chn, u16 off ){

	s16 i, j, vchn, ntime, tmax, vcnt;

	if(chn<2)
	{
		tmax=PSG_VCH_MAX;

		for(i=2;i>=0;i--)
		{
			vcnt=0;

			for(j=0;j<PSG_VCH_MAX;j++)
			{
				if(PSGFX.chn[i].slot[j].ptr>=0)
				{
					vcnt++;
				}
			}

			if(vcnt==0)
			{
				chn=i;
				break;
			}
			if(vcnt<tmax)
			{
				tmax=vcnt;
				chn=i;
			}
		}
	}

	vchn=-1;

	for(i=0;i<PSG_VCH_MAX;i++)
	{
		if(PSGFX.chn[chn].slot[i].ptr<0)
		{
			vchn=i;
			break;
		}
	}

	if(vchn<0)
	{
		tmax=-1;

		for(i=0;i<PSG_VCH_MAX;i++)
		{
			ntime=PSGFX.chn[chn].slot[i].time;

			if(ntime>tmax)
			{
				tmax=ntime;
				vchn=i;
			}
		}
	}

	PSGFX.chn[chn].slot[vchn].ptr=off;
	PSGFX.chn[chn].slot[vchn].wait=0;
	PSGFX.chn[chn].slot[vchn].time=0;
}


//////////////////////////////////////////////////////////////////////////////////////
//
// u16 psgFxInit( const u8 *data )
// inicia el psg con los datos dados
//
//////////////////////////////////////////////////////////////////////////////////////
u16 psgFxInit( const u8 *data ){

	volatile u8 *pb;
	u16 i, j;

	pb = (u8*)PSG_DATA;

	*pb = 0x9f;
	*pb = 0xbf;
	*pb = 0xdf;
	*pb = 0xff;

	PSGFX.data = data;
	PSGFX.total = (data[0]<<8) + data[1];

	for( i = 0; i < 4; i++){
		for( j = 0; j < PSG_VCH_MAX; j++ ){
			PSGFX.chn[i].slot[j].ptr=-1;
			PSGFX.chn[i].slot[j].wait=0;
		}
	}

	return PSGFX.total;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// void psgFxPlay( u16 psg ){
//
//////////////////////////////////////////////////////////////////////////////////////
void psgFxPlay( u16 num ){

    i16 aa,chn,eoff,doff,chcnt;

	if(num>=PSGFX.total) return;
	if(!PSGFX.data) return;

	eoff=2+(num<<1);
	doff=(PSGFX.data[eoff]<<8)+PSGFX.data[eoff+1];
	chcnt=PSGFX.data[doff++];

	for(aa=0;aa<chcnt;aa++)
	{
		eoff=(PSGFX.data[doff++]<<8);
		eoff+=PSGFX.data[doff++];
		chn=PSGFX.data[eoff++];
		_psgfx_addch(chn,eoff);
	}
}


//////////////////////////////////////////////////////////////////////////////////////
//
// void psgFxFrame ( void )
//
//////////////////////////////////////////////////////////////////////////////////////
void psgFxFrame( void ){

	volatile u8 *pb;
	u8 mbyte;
	s16 pchn, vchn, rchn, mvol, nvol;
	u16 div;

	if ( !PSGFX.data )
	{
		return;
	}

	pb=(u8*) PSG_DATA;

	for ( pchn=0; pchn<4; pchn++ )
	{
		for(vchn=0;vchn<PSG_VCH_MAX;vchn++)
		{
			if(PSGFX.chn[pchn].slot[vchn].ptr<0)
			{
				continue;
			}

			PSGFX.chn[pchn].slot[vchn].time++;

			if(PSGFX.chn[pchn].slot[vchn].wait)
			{
				PSGFX.chn[pchn].slot[vchn].wait--;
				continue;
			}

			mbyte=PSGFX.data[PSGFX.chn[pchn].slot[vchn].ptr++];

			switch(mbyte&0xc0)
			{
				case 0x00:/*0=eof 1..31=wait*/
					if(!mbyte) PSGFX.chn[pchn].slot[vchn].ptr=-1; else PSGFX.chn[pchn].slot[vchn].wait=mbyte-1;
					break;
				case 0x40:/*vol only*/
					PSGFX.chn[pchn].slot[vchn].vol=mbyte&0x0f;
					break;
				case 0x80:/*div only*/
					PSGFX.chn[pchn].slot[vchn].div=((u16)mbyte<<8)|PSGFX.data[PSGFX.chn[pchn].slot[vchn].ptr++];
					break;
				case 0xc0:/*vol and div*/
					PSGFX.chn[pchn].slot[vchn].vol=(mbyte>>2)&0x0f;
					PSGFX.chn[pchn].slot[vchn].div=((u16)(mbyte&0x03)<<8)|PSGFX.data[PSGFX.chn[pchn].slot[vchn].ptr++];
					break;
			}
		}

		rchn=-1;
		mvol=16;

		for ( vchn = 0; vchn < PSG_VCH_MAX; vchn++ )
		{
			if ( PSGFX.chn[pchn].slot[vchn].ptr < 0 )
			{
				continue;
			}

			nvol=PSGFX.chn[pchn].slot[vchn].vol;

			if(nvol<mvol)
			{
				mvol=nvol;
				rchn=vchn;
			}
		}

		if(rchn>=0)
		{
			vchn=rchn;
			rchn=pchn<<5;
			*pb=0x80|0x10|rchn|PSGFX.chn[pchn].slot[vchn].vol;
			div=PSGFX.chn[pchn].slot[vchn].div;
			*pb=0x80|rchn|(div&0x0f);
			*pb=div>>4;
		}
	}
}

