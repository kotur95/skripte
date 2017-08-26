/* Inicijalizacija  */
/* Dinamicka */
 int pthread_mutex_init(pthread_mutex_t *restrict mutex,
           const pthread_mutexattr_t *restrict attr);

/* Staticka */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

/* Funkcije za zaklj./otklj. */
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

/* Ukoliko nam se mutex nalazi u strukturi, mozemo
 * ga staticki inicijalizovati na sledeci nacin */
 
typedef struct
{
	pthread_mutex_t mutex;
	double cena;
	char ime_proizvoda[MAX_LEN];
}_shared_data;

/* Staticka inicijalizacija* mutex-a * 
 * koji se nalazi u strukturi */
 _shared_data jabuka = 
{
	.cena = 97, 
	.ime_proizvoda = "jabuka",
	.mutex = PTHREAD_MUTEX_INITIALIZER
			
 }; 
