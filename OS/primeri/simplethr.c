void * threadfunc2() {
	printf("Stampam iz prve niti.\n");
	pthread_exit(0);
}

void * threadfunc3() {
	printf("Stanmpam iz druge niti.\n");
	pthread_exit(0);
}

int main() 
{      
	/* Jedna nit programa je tekuca nit */
	/* Kreiranje druge niti programa */
	pthread_t thr2;	 
	int retval = pthread_create(&thr2, NULL, threadfunc2, NULL);

	/* Obrada greske */
	pth_check(retval, "Error creating thread"); 

	/* Kreiranje trece niti programa */
	pthread_t thr3; 
	retval = pthread_create(&thr3, NULL, threadfunc3, NULL);
	pth_check(retval, "Error creating thread"); 

	/* Cekanamo da niti zavrse */
	retval = pthread_join(thr2, NULL); 
	pth_check(retval, "Error waiting thread"); 

	retval = pthread_join(thr3, NULL);
	pth_check(retval, "Error waiting thread"); 

	return 0;
}
