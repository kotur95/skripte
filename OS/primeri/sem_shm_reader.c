int main(int argc, char * argv[])
{
	/* Program prima putanju do deljenje pemorije */
	err_check(argc == 2, "Invalid number of arguments");

	char * name = argv[1];
	unsigned data_size;
	_shared_array * shm_array = get_mem_block(name, &data_size);
	ast(-1 != sem_wait(&shm_array->safe_to_read), "Sem wait err");
	for (int i=0; i < shm_array->length; i++) 
		printf("%.2lf ", shm_array->array[i]);

	printf("\n");
	
	munmap(shm_array, data_size);
	shm_unlink(name);

	return 0;
}
