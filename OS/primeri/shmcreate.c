typedef struct 
{
	double array[MAX_LEN];
	int length;
}_shared_array;

void * create_mem_block(const char * shmName, unsigned size) 
{
	int memFd = shm_open(shmName, O_RDWR | O_CREAT, 0755);
	ast(-1 != memFd, "Error creating shared memory");

	/* Obavezno postavite fajl na odg. velicinu */
	ast(-1 != ftruncate(memFd, size), "Error truncating file");

	void * addr;
	addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED , \
		       	memFd, 0);
	ast(addr != MAP_FAILED, "Error mapping shm");
	close(memFd);

	return addr;
}

int main(int argc, char * argv[])
{
	/* Program prima putanju do deljenje pemorije */
	err_check(argc == 2, "Invalid number of arguments");
	char * name = argv[1];

	unsigned mem_size = sizeof(_shared_array);
	_shared_array * shm_array = create_mem_block(name, mem_size);

	scanf("%d", &shm_array->length);
	for (int i=0; i < shm_array->length; i++) 
		scanf("%lf", &shm_array->array[i]);
	
	munmap(shm_array, mem_size);

	return 0;
}
