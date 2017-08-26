#define BUF_LEN (1024)

typedef struct {
	char buf[BUF_LEN];
	sem_t safe_to_read;
	sem_t safe_to_write;
}_podatak;
