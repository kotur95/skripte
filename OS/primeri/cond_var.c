typedef struct {
	char buffer[BUF_LEN];
	pthread_mutex_t mutex_lock;
	pthread_cond_t cond_var; 
	int signal_occured; 
	/* promenljiva signal_occured mozda nije potrebna, ali 
	 * sluzi da zabelezi signal ukoliko se desio pre  nego 
	 * sto smo ga cekali sa: pthread_cond_wait();, kada 
	 * ne bi postojala mozda bi cekali zauvek :) */ 
}_sh_data;

_sh_data shd = {
	.mutex_lock = PTHREAD_MUTEX_INITIALIZER,
	.cond_var = PTHREAD_COND_INITIALIZER, 
	.signal_occured = 0
};

void * procitaj_sa_stdin()
{
	pthread_mutex_lock(&shd.mutex_lock);
	printf("Citanje iz 1 threada: ");
	fgets(shd.buffer, BUF_LEN, stdin);

	/* saljemo signal da thr2 - moze da pise na stdout */
	shd.signal_occured = 1; 
	pthread_cond_signal(&shd.cond_var);
	pthread_mutex_unlock(&shd.mutex_lock);

	pthread_exit(NULL);
}

void * ispisi_na_stdout() 
{
	pthread_mutex_lock(&shd.mutex_lock);

	/* Ako je signal vec poslat nastavi, ako nije onda ga cekaj */
	if (!shd.signal_occured)
		pthread_cond_wait(&shd.cond_var, &shd.mutex_lock);

	shd.signal_occured = 0;

	fprintf(stdout, "Ispis iz 2 threada: %s", shd.buffer);

	pthread_mutex_unlock(&shd.mutex_lock);

	pthread_exit(NULL);
}

int main()
{
	/* Kreiramo 2 thread-a, jedan ce da cita sa stdin a drugi 
	* da ispisuje na stdout, ali drugi mora da ceka prvi da 
	* procita sa stdin pa tek onda da ispise na stdout */

	pthread_t thr1, thr2;
	pthread_create(&thr1, NULL, procitaj_sa_stdin , NULL);
	pthread_create(&thr2, NULL, ispisi_na_stdout , NULL);
	pthread_join(thr1, NULL);
	pthread_join(thr2, NULL);

	return 0;
}
