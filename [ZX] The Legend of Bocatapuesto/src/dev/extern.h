// External custom code to be run from a script
void show_bicharracos (void){
	// Render
	unsigned char gpit, x, y;
	for (gpit = 0; gpit < 3; gpit ++) {
		x = malotes [enoffs + gpit].x;
		y = malotes [enoffs + gpit].y;
		sp_MoveSprAbs (sp_moviles [gpit], spritesClip, en_an [gpit].next_frame - en_an [gpit].current_frame, VIEWPORT_Y + (y >> 3), VIEWPORT_X + (x >> 3),x & 7, y & 7);
		en_an [gpit].current_frame = en_an [gpit].next_frame;

	}
	
	// Precalc this, comes handy:
	x = player.x >> 6;
	y = player.y >> 6;

	sp_MoveSprAbs (sp_player, spritesClip, player.next_frame - player.current_frame, VIEWPORT_Y + (y >> 3), VIEWPORT_X + (x >> 3), x & 7, y & 7);

}

unsigned char move (void);

void do_extern_action (unsigned char n) {
	// Add custom code here.	
    //move();
	saca_a_todo_el_mundo_de_aqui ();
    show_bicharracos();
    
	switch (n){
		case 1: // EXTERN 1
			//               X  Y  COLOR            BORDE
			print_border_str(5, 5, 40, "SOLDADO VON EINZBERN:", FALSE);
			print_border_str(5, 6, 40, "ALTO! QUIEN VA?", FALSE);
			break; // IMPORTANTE!!!
		case 2: // EXTERN 2
			print_border_str(5, 14, 40, "BERYL: SOY I+IGO MONTOYA", TRUE);
			break;
		// max 255
		case 3: // EXTERN 3
		print_border_str(5, 6, 40, "SOLDADO VON EINZBERN:", FALSE);
		print_border_str(5, 7, 40, "QUE HACES AQUI?", FALSE);
		print_border_str(5, 8, 40, "ACASO NO SABES", FALSE);
		print_border_str(5, 9, 40, "QUE ESTAMOS EN GUERRA?", FALSE);
			break;
		case 4: // EXTERN 4
		print_border_str(5, 12, 40, "BERYL: (MALDITO INCULTO,", FALSE);
		print_border_str(5, 13, 40, " NO TIENE NI IDEA DE", FALSE);
		print_border_str(5, 14, 40, "  LITERATURA... )", FALSE);
			break;
		case 5: // EXTERN 5
		print_border_str(5, 5, 40, "SOLDADO VON EINZBERN:", FALSE);
		print_border_str(5, 6, 40, "ESPERA...! TU ERES", FALSE);
		print_border_str(5, 7, 40, "UN ESPIA DE LOS VELOUR!", FALSE);
		print_border_str(5, 8, 40, "NO PASARAS!", FALSE); 
			break;
		case 6: // EXTERN 6
		print_border_str(6, 9, 40, "ME'LION: EH,TU!", FALSE);
		print_border_str(6, 10, 40, "ERES DE ESOS MALDITOS", FALSE);
		print_border_str(6, 11, 40, "VON EINZBERN!", FALSE);
			break;
		case 7: // EXTERN 7
		print_border_str(8, 8, 40, "BERYL: YO NO SOY VON", FALSE);
		print_border_str(8, 9, 40, "EINZBERN, YO SOY ELIS", FALSE);
			break;
		case 8: // EXTERN 8
		print_border_str(6, 9, 40, "ME'LION: ME DA IGUAL!", FALSE);
		print_border_str(6, 10, 40, "VON EINZBERN O ELIS!", FALSE);
		print_border_str(6, 11, 40, "TODOS SOIS ENEMIGOS", FALSE);
		print_border_str(6, 12, 40, "DEL IMPERIO VELOUR", FALSE);
			break;
		case 9: // EXTERN 9
		print_border_str(8, 9, 40, "BERYL: NO ES LO MISMO", FALSE);
		print_border_str(8, 10, 40, "DOS TAZAS DE TE", FALSE);
		print_border_str(8, 11, 40, "QUE DOS TETAZAS", FALSE);
			break;
		case 10: // EXTERN 10
		print_border_str(8, 8, 40, "ME'LION: BASTA DE", FALSE);
		print_border_str(8, 9, 40, "TONTERIAS!", FALSE);
		print_border_str(8, 10, 40, "ACABARE CONTIGO!", FALSE);
			break;
		case 11: // EXTERN 11
		print_border_str(5, 7, 40, "GALLAHAD: QUIEN VA...?", FALSE);
		print_border_str(5, 8, 40, "ESPERA!ERES TU", FALSE);
		print_border_str(5, 9, 40, "EL QUE HA CAUSADO", FALSE);
		print_border_str(5, 10, 40, "TANTO ALBOROTO", FALSE);
		print_border_str(5, 11, 40, "ENTRE MIS TROPAS?", FALSE);
		print_border_str(5, 12, 40, "CUAL ES TU OBJETIVO?", FALSE);
			break;
		case 12: // EXTERN 12
		print_border_str(5, 9, 40, "BERYL: VOY A CONSEGUIR", FALSE);
		print_border_str(5, 10, 40, "EL BOCATA PARA FUNDAR", FALSE);
		print_border_str(5, 11, 40, "MI PROPIO REINO!", FALSE);
			break;
		case 13: // EXTERN 13
		print_border_str(5, 8, 40, "GALLAHAD: NUNCA PERMITIRE", FALSE);
		print_border_str(5, 9, 40, "QUE ESO PASE!", FALSE);
		print_border_str(5, 10, 40, "TE LAS VERAS", FALSE);
		print_border_str(5, 11, 40, "CON MI ESPADA!", FALSE);
		break;
		case 14: // EXTERN 14
			print_border_str(5, 8, 40, "PRIIIMO, DAME UNA", FALSE);
			print_border_str(5, 9, 40, "MONEEEEDA", FALSE);
			break;
		case 15: // EXTERN 15
		print_border_str(5, 8, 40, "AYYY, QUE APA+AO", FALSE);
		print_border_str(5, 9, 40, "PASA, PASA", FALSE);
		    break;
	}
	
	sp_UpdateNow();
	sp_WaitForNoKey ();
	espera_activa(2000);
    draw_scr_background ();
    
}
