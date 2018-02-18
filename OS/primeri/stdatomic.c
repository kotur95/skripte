#include <stdio.h>
#include <stdatomic.h> 
/* Standard >= C11 - potrebno */

/* Atomicna promenljiva */
atomic_int gaInt;

int main()
{

	/* Koriscenjem atomicnih funkcija za 
	 * incijalizaciju i dodelu promenljive */
	 
	atomic_init(&gaInt, 0);	
	atomic_fetch_add(&gaInt, 256);

	int vr = atomic_load(&gaInt);

	printf("%d\n", vr);

	return 0;
}
