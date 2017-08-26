// Provera postojanja file-a
ast(-1 != access(argv[1], F_OK), "Unable to access file");
int fileFd;
// Otvaranje preko fd-a putem open f-je
ast( -1 != (fileFd = open(argv[1], O_RDWR | O_NONBLOCK)), \
		"Error opening file for reading");

struct stat sbuf;
// Provera da li je fajl regularan
ast(-1 != fstat(fileFd, &sbuf), "Error stat");
err_check((sbuf.st_mode & S_IFMT) != S_IFREG, "Not regular file");
