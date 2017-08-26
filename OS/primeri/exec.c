/* Primer poziva execvp gde je drugi argument 
 * vektor, tj. niz argumenata koji se prosledjuje
 * izvrsnom fajlu, a funkcija takodje traga za 
 * izvrsnim fajlom u PATH promenljivoj */
char * args[] = {"ls", "-l",  NULL};
int retval = execvp("ls", args);

/* Umesto prvog argumenta funkcije ls mogli smo 
 * koristiti apsolutnu putanju do ls programa,tj. 
 * poziv je mogao  izgledati na sledeci nacin: */
retval = execvp("/bin/ls", args);

/* Naravno potrebno je i obraditi gresku
 * u slucaju neuspesnog poziva */
ast(-1 != retval, "Error overriding process image");

