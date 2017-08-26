void sig_handler(int signal) 
{
	/* U signal handler-im treba koristiti samo funkcije koje
	 * su reentrant safe, tj. one koje garantuju korektan 
	 * natavak izvrsavanja nakon prekida. printf() nije 
	 * reentrant safe, ali je data radi ilustracije. */
	switch (signal)  
	{
		case SIGKILL : printf("Kill Ocurred");break;
		case SIGINT  : printf("Interupt Ocurred");break;
		case SIGSTOP : printf("Stop Ocurred");break;
		
		/* strsignal() vraca stringovni opis signala koji je
		 *  nastupio, a kao argument prima redni broj signala. */
		default: printf("Signal occured: %s\n", strsignal(signal));
	}
}

int main()
{
	int retval = signal(SIGINT, sig_handler)
	ast(retval != SIG_ERR, "Error setting signal");

	/* Cekanje signala da nastupi */		
	for(;;) 
		sleep(1);
	
	return 0;
}
