char * fifo_path = argv[1];
int fifoFd;
ast(-1 != (fifoFd = open(fifo_path, O_RDONLY)), "Error opening fifo");

char buf[BUF_MAX];
int bytesRead;

while (1) {
	ast(-1 != (bytesRead = read(fifoFd, buf, BUF_MAX)), "Reading error");
	ast(-1 != write(STDOUT_FILENO, buf, strlen(buf)), "Writing error");
	ast(-1 != write(STDIN_FILENO, "\n", 1), "Writing error");

	if (strcasecmp(buf, "kraj") == 0)
		break;
}
