int from, len, lock_time, fd;
char fpath[MAX_LEN];
printf("Enter: fpath, from, len, lock_time");
scanf("%s, %d, %d, %d", fpath, &from, &len, &lock_time); 
ast(-1 != (fd = open(fpath, O_RDWR)), "Opening file error");

/* Pretpostavicemo da je fajl veci od from + len bajtova */
struct flock lock;
lock.l_type = F_WRLCK; // write lock
lock.l_whence = SEEK_SET;
lock.l_start = from;
lock.l_len = len;

/* pokusavamo da zakljucamo fajl na zeljeni nacin:
* F_SETLK: ako ne moze da zakljuca fajl, odmah puca i vraca se nazad
* F_SETLKW: ako ne moze da zakljuca, sacekace dok se resurs ne oslobodi
* F_GETLK: sluzi za ispitivanje da li moze da se postavi zeljeni katanac
* u ovom slucaju nije lose biti pohlepan i uvek ispitivati maksimalne 
* mogucnosti, tj. treba postaviti tip katanca na F_WRLCK. Ako fajl moze
* da se zakljuca u  w modu, sigurno moze i u r modu. Vodite se parolom 
* "ko moze vise moze i manje". Kada se vrsi zakljucavanje uvek treba 
* zakljucavati minimum potrebnih bajtova da bi se omogucio sto  
* veci paralelizam. */

ast(-1 != fcntl(fd, F_SETLK, &lock), "Locking file failed");

printf("Lock acquired, holding %d seconds.\n", lock_time);	  
sleep(lock_time); // Uspavljujemo proces (simulramo drzanje katanca)

lock.l_type = F_UNLCK; // Potrebno je otkljucati fajl
ast(-1 != fcntl(fd, F_SETLK, &lock), "Unlocking file failed");
printf("Lock released ...\n");

/* zatvaranjem deskriptora sa automatski oslobadaju svi katanci */
close(fd);
