#include "main.h"
#include "snd/psg/sfxbank.h"

////////////////////////////////////////////////////////////////////////////////
//
// int main()
//
////////////////////////////////////////////////////////////////////////////////
int main(){

    u8 exit = 0;
    u8 num_players = 2, num_human_players = 1;

    speed = score = 0;

    // Pone las copias "shadow" de los registros a sus
    // valores por defecto, establecidos en hw_md.s
    VDP_init();

    // carga el driver XGM en el z80
    SND_loadDriver_XGM();

    // inicia la SRAM
    initSRAM();

    // inicio el banco de sonidos psg
    psgFxInit(sfxbank_data);

    // pantalla de arranque (falsa) de la bios
    startupScreen();

    // pantalla de creditos
    creditsScreen();

    ////////////////////////////////////////
    //  bucle infinito
    ////////////////////////////////////////
	while(1){

	    // attract mode
	    while( !exit ){

            draw1985Logo();     // pantalla del logo

            introScreen();    // intro

            // pantalla de titulo
            exit = titleScreen();

            // atract
            if(!exit)
                recordsScreen(0);
	    }

	    menuScreen( &num_players, &num_human_players );

        // doy valor inicial a las vars del juego,
        // carga los tiles e inicia sus sprites
        initGame( num_players );

        gameLoop( num_players, num_human_players );
        recordsScreen(1);

        exit = 0;
	}

    return 0;
}
