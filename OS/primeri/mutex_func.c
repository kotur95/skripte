/* Kreira nit i zapocinje izvrsavanje od prosledjene
 * funkcije (arg start routine) */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
		  void *(*start_routine) (void *), void *arg); 

/* Ceka da nit zavrsi za izvrsavanjem i koristi 
 * se za uzimanje povratne vrednosti niti */
int pthread_join(pthread_t thread, void **retval);

/* Inicijalizuje vrednost mutex-a primer 1 je dinamicki 
 * metod, a primer 2 staticki. */
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
           const pthread_mutexattr_t *restrict attr);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Unistava mutex objekat (moze biti ponovo
 * inicijalizovan) pomocu mutex_init(). */
int pthread_mutex_destroy(pthread_mutex_t *mutex);

/* Funkcije za otkljucavanje/zakljucavanje mutex-a */
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);


