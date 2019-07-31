// External custom code to be run from a script

void do_extern_action (unsigned char n) {
	// Add custom code here.
	switch (n){
		case 1: print_border_str(6, 2, 79, "GET THE 3 KEYS", TRUE);break;
		case 2: print_border_str(6, 2, 79, "OH NO LUCKY IS DEAD TOO", TRUE);break;
		case 3: print_border_str(6, 2, 79, "THERE IS A SECRET PATH", TRUE);break;
	}
}
