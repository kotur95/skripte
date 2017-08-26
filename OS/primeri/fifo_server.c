char * fifo_path = argv[1];
ast(-1 != mkfifo(fifo_path, 0755), "Error creating fifo");

int fifoFd;
ast(-1 != (fifoFd = open(fifo_path, O_WRONLY)), "Error opening fifo");

char buf[BUF_MAX];	
int bytesRead;

while (1) {
	bytesRead = read(STDIN_FILENO, buf, BUF_MAX);
	ast(bytesRead != -1, "Reading error");
	buf[bytesRead-1] = '\0'; // Umesto '\n' ubacujemo '\0'
	ast(-1 != write(fifoFd, buf, bytesRead), "Writing error");
}
