pid_t pid = fork();
ast(-1 != pid, "Unable to create child");

if (pid  > 0) {
	// Parent branch
	printf("Hello from parent, pid: %d\n", pid);
} else {
	// Child branch
	printf("Hello from child, pid: %d\n", pid);	
	exit(EXIT_SUCCESS); 
}

/* Child call's exit() before reaching this part of code,  *
 * Parent conitnue's executing after leaving parent branch */

