typedef struct {
	double array[MAX_LEN];
	int length;
}_shared_array;

void * get_mem_block(const char *fpath, unsigned *size)
{
	int memFd = shm_open(fpath, O_RDWR, 0);
	ast(memFd != -1, "Error opening shared memory");

	struct stat finfo;
	ast(-1 != fstat(memFd, &finfo), "Unable to get file stats");
	*size = finfo.st_size;

	void * addr = NULL;
	addr = mmap(0, *size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);
	ast(addr != NULL, "Error maping memory");

	close(memFd);
	return addr;
}

int main(int argc, char * argv[])
{
	/* Program prima putanju do deljenje pemorije */
	err_check(argc == 2, "Invalid number of arguments");

	char * name = argv[1];
	unsigned data_size;
	_shared_array * shm_array = get_mem_block(name, &data_size);
	for (int i=0; i < shm_array->length; i++) 
		printf("%.2lf ", shm_array->array[i]);

	printf("\n");
	
	munmap(shm_array, data_size);
	shm_unlink(name);

	return 0;
}
