typedef struct {
	double array[MAX_LEN];
	int length;
	sem_t safe_to_read;
	sem_t safe_to_write;
}_shared_array;

int main(int argc, char * argv[])
{
	/* Program prima putanju do deljenje pemorije */
	err_check(argc == 2, "Invalid number of arguments");
	char * name = argv[1];

	unsigned mem_size = sizeof(_shared_array);
	_shared_array * shm_array = create_mem_block(name, mem_size);
	
	/* Incijalizacija semafora se vrsi samo u jednom programu */
	ast(-1 != sem_init(&shm_array->safe_to_read, 1, 0), "Sem init err");
	ast(-1 != sem_init(&shm_array->safe_to_write, 1, 1), "Sem init err");

	ast(-1 != sem_wait(&shm_array->safe_to_write), "Sem wait err");
	scanf("%d", &shm_array->length);
	for (int i=0; i < shm_array->length; i++) 
		scanf("%lf", &shm_array->array[i]);
	
	sem_post(&shm_array->safe_to_read);
	munmap(shm_array, mem_size);

	return 0;
}
