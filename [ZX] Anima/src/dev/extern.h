// External custom code to be run from a script

void do_extern_action (unsigned char n) {
	// Add custom code here.	
	switch (n){
		case 1: print_border_str(2, 2, 79, "HAS ENCONTRADO SANTO GRIAL", TRUE); break;
		
		case 2: print_border_str(2, 2, 79, "NO TIENES PODER SOBRE MI", TRUE); break;
	
	}
}