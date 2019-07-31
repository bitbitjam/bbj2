// External custom code to be run from a script

void do_extern_action (unsigned char n) {
	switch(n){
	    case 1:
	        //               X  Y  COLOR            BORDE
	        print_border_str(11, 9 , 79, "QUE NO ESCAPE", FALSE);
	        break; //IMPORTANTE
	    case 2:
	        print_border_str(9, 2, 79, "ESCAPA Y VE AL PUEBLO", FALSE);
	        break;
	}
	
}
