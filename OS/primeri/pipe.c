/* READ_SIDE ima vrednost (0), a WRITE_SIDE (1) */
int pipeFd[2];	
ast(-1 != pipe(pipeFd), "Error creating PIPE");

pid_t pid = fork();
ast(-1 != pid, "Creating child error");

if (pid  > 0) {
	// Parent process
	close(pipeFd[WRITE_SIDE]); // Zatvaramo pipe za pisanje

	/* Cekamo na dete proces da zavrsi izvrsavanje */
	ast(-1 != wait(NULL), "Wait error"); 

	/* dup2() u datom pozivu duplira fd read pipe-a i dodeljuje mu  
	 vrednost (STDIN_FILENO). */
	ast(-1 != dup2(pipeFd[READ_SIDE], STDIN_FILENO), "Error assigning FD");
	close(pipeFd[READ_SIDE]); // zatvaramo fd jer je prosledjen na stdin 

	char * args[] = {"wc", "-l",  NULL};
	ast(-1 != execvp("wc", args), "Error executing wc");
} else {
	// Child process
	close(pipeFd[READ_SIDE]); // Zatvaramo pipe za citanje
	
	/* dup2() u datom pozivu duplira write fd pipe-a i dodeljuje mu  *
	 * vrednost 2 tj. (STDOUT_FILENO). */
	ast(-1 != dup2(pipeFd[WRITE_SIDE], STDOUT_FILENO), \
			"Error assigning FD");
	close(pipeFd[1]); // zatvaramo fd jer je prosledjen na stdout

	char * args[] = {"ls", NULL};
	ast(-1 != execvp("ls", args), "Error executing ls");
}

/* Ako se program izvrsi ocekivano, ovaj deo koda nikada nece biti dosegnut */
