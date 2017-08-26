void * threadfunc2(void * arg)
{
	printf("Druga nit, string je: %s \n", (char *) arg);
	pthread_exit(0);
}

void * threadfunc3(void * arg) 
{
	printf("Treca nit, string je: %s \n", (char *) arg);
	static double retval = 3.14;
	retval = 2.72;
	pthread_exit(&retval);
}
                  /* static modifikator nam omogucava        */ 
int main()        /* kreiranje promenljivih koje postoje ceo */       
{                 /* zivotni vek programa (sl. kao glob.)    */             	   
	static char str1[MAX_LEN]; 
	static char str2[MAX_LEN]; 

	printf("Unesite 2 stringa (argumente niti): \n");
	scanf("%s %s", str1, str2);

	/* 1 nit vec postoji ('glavna'). Kreiranje 2. niti */
	pthread_t thr2;	
	int status = pthread_create(&thr2, NULL, threadfunc2, str1);
	pth_check(status, "Error creating thread");

	/* Kreiranje 3. niti */
	pthread_t thr3;
	status = pthread_create(&thr3, NULL, threadfunc3, str2);
	pth_check(status, "Error creating thread");

	void * retval_thr3; // Pokazivac za povratnu vrednost iz niti 3
	/* Cekamo niti da zavrse */
	pth_check(pthread_join(thr2, NULL), "pth join error");
	pth_check(pthread_join(thr3, &retval_thr3), "pth join error");

	printf("Povratna vrednost thr3: %.2lf\n", *((double *) retval_thr3));

	return 0;
}
