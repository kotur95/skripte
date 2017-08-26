struct flock 
{
	short l_type;    /* Tip katanca: F_RDLCK, F_WRLCK, F_UNLCK */
	short l_whence;  /* Odakle je start: SEEK_SET, SEEK_CUR, SEEK_END */
	off_t l_start;   /* Pocetni ofset od vrednosti whence */
	off_t l_len;     /* Broj bajtova koji zelimo da zakljucamo */
	pid_t l_pid;     /* PID procesa koji nam blokira zakljucavanje 
  			      * fajla Postavlja se automatski ako fcntl
			      * prosledimo  F_GETLK ili F_OFD_GETLK) */
};
