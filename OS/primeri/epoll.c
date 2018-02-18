/* Citanje kroz pipe-ove i ispisivanje na stdout */

int epfd = epoll_create(1);

struct epoll_event event;
union epoll_data data;
data.fd = STDIN_FILENO;

event.events = 0;
event.events |= EPOLLIN;
event.data = data;
int epret;

epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
int bytesRead;
char buf[1024];

while (1) 
{
	epret = epoll_wait(epfd, &event, 1, 5000);

	if (epret == 0)
		break;
	else {
		for (int i=0; ; i++) {
			
			if (event.events & EPOLLIN) {
				bytesRead = read(data.fd, buf, 1024);
				buf[bytesRead] = '\0';

				fputs(buf, stdout);
				break;
			}
		}

	}
}
