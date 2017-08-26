/* Provera za niti */

pthread_t thread;
int status = pthread_create(&thread, NULL, threadfunc2, NULL);
pth_check(status, "Error creating thread");
