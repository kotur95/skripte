#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

/* Error handling */
void ast(int statement, const char * usr_msg);  
void err_chk(int statement, const char * usr_msg);
void pth_check(int pth_err, const char * usr_msg);

void ast(int statement, const char * usr_msg)
{
	if (!statement) {
		perror(usr_msg);
		exit(EXIT_FAILURE);
	}
}

void err_check(int statement, const char * usr_msg)
{
	if (!statement) {
		fprintf(stderr, "%s\n", usr_msg);
		exit(EXIT_FAILURE);
	}
}

void pth_check(int pth_err, const char * usr_msg)
{
	if (pth_err > 0) {
		errno = pth_err;
		ast(false, usr_msg);
	}
}

