int numFds=1;
struct pollfd fds[numFds];

for (int i=0; i<numFds; i++) {
	fds[i].fd = STDIN_FILENO;
	fds[i].events = 0;
	fds[i].events |= POLLIN;
}

char buf[1024];
int pret;
int br;

while(1) {

	pret = poll(fds, numFds, 5000);

	if (pret == 0)
		break;
	else {
		br = read(fds[0].fd, buf, 1024);
		buf[br] = '\0';
		fputs(buf, stdout);
	}
}
