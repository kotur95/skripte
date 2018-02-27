---
title: KK - Nedejla 01
author: github.com/kotur95/skripte
date: 20.02.2018.
---

# Uvod u programski jezik C++ 

Prevodjenje programa: `g++ program.cpp -o program -Wall`

## Proceduralni deo C++ (C i jos toga)

Prevodilac za programski jezik C++ moze da kompajluje izvorni C kod. U ovoj sekciji bice predstavljene nadogradnje jezika C++ u odnosu na C a koje se odnose na proceduralni deo jezika.  

```c++
#include <iostream>

using namespace std;

int main() {

	cout << "Hello world" << endl;

	return 0;
}
```

**`iostream`** - Zaglavlje standardne cpp biblioteke koje sadrzi funkcije za input/output

**`namespace`** - Ekvivalent paketa u programskom jeziku `Java`

**`cout`** - Objekat klase stream (vezan za standardni izlaz) ostream

**`<<`** - operator izlaza (prosledjuje izlaz na stream) levo asocijativan operator, vraca levi operand.

**`>>`** - operator ulaza (iz cin-a unesi u promenljivu)

**`endl`** - Kraj reda

 Pored prenosa argumenata po vrednosti (kao u C-u) i tzv. prenosa vrednosti pokazivaca imamo i prenos po referenci.

 ```c++
void swap(int &a, int &b) {
	int pom = a;
	a = b;
	b = pom;
 }

 int main() {
	int a = 3, b = 2;
	cout << "a=" << a << " " << "b=" << b << endl;
	swap(a, b);
	cout << "a=" << a << " " << "b=" << b << endl;
 }
 ```
**`Standardni izlaz:`**

 ```
a=3 b=2
a=2 b=3
 ```
Primetimo da u main-u nismo na kraju stavili `return 0`. Default ponasanje u C++ je da kada nenavedemo povratnu vrednost u funkciji `main` ona po default-u bude `0`.

Postoje 2 osonovna razloga za prenos vrednosti po referenci:

1) Zbog izmene prosledjenih argumenata
2) Zbog memorijske efikasnosti (sa specifikatorom `const`)

Konstruktor se moze koristiti za inicijalizaciju promenljivih.

```c++
/* Sledece dve naredbe rade identicnu stvar */
int n = 5; // n se inicijalizuje na celobrojnu vrednost 5
int m(5); // m se inicijalizuje na celobrojnu vrednost 5
```

Ukljucivanje C-ovskih biblioteka vrsi se tako sto se na ima biblioteke doda prefix 'c'. Na primer za ukljucivanje stdio.h C biblioteke navelibi direktivu `#include <cstdio>`.

## Preopterecivanje funkcija 

U C-u svaka funkcija bez obzira na tipove njenih argumenata nije mogla da nosi isti naziv. C++ Obezbedjuje da naziv funkcije bude isti za funkcije koje imaju razlicit potpis.

```c++
int kvadrat_broja(int n) {
	return n*n;
}

float kvadrat_broja(float n) {
	return n*n;
}

int main() {
	cout << kvadrat_broja(5) << endl;
	cout << kvadrat_broja(5.5) << endl;

	return 0;
}
```
**`Standardni izlaz:`**

 ```
25
25.25 
 ```


**`inline funkcije`** - Kljucna rec inline sugerise prevodiocu da poziv te funkcije zameni telom te funkcije. (Kao makro) `inline int kvadrat_broja(int n);`

Dinamicka alokacija u Cpp-u vrsi se pomocu kljucnih reci: `new` i `delete`


## Objektno-Orijentisani CPP
* Klasa razlomak

Konstruktor predstavlja posebnu vrstu metoda kojom se kreira instanca tj. objekat odedjene klase. U C++ ukoliko ne definisemo ni jedan konstruktor postoji 'default' konstruktor koji kreira objekat odredjene klase ali polja te klase mogu imaju neke neodredjene vrednosti (junk) tj. imace vrednosti koje su predodredjene default konstruktorom za taj tip podataka.

Najcesce cemo deklaracije metoda klase razdvajati od njihove implementacije (nalik u c-u). U header fajlu deklarisemo klasu, njene promenljive i metode a u izvornom `c++` fajlu implementiramo klasne metode. Na sledecem primeru data je klasa `razlomak` ciji metod `postaviRazlimak()` deklarisemo u `header`fajlu (`.hpp`) a implementiramo u `izvornom` fajlu (`.cpp`).

`Razlomak.hpp`
```c++
#ifndef __RAZLOMAK_H__
#define __RAZLOMAK_H__

class Razlomak {
private:
	int _imenilac;
	int _brojilac;
public:
	void postaviRazlomak(int b, int i);
};

#endif // __RAZLOMAK_H__
```

`Razlomak.cpp`
```c++
#include "Razlomak.hpp"

void Razlomak::postaviRazlomak(int b, int i) { 
	_brojilac = b;
  	_imenilac = i;
}
```

* Postoji podrazumevani konstruktor koji prima 0 argumenata. Taj konstruktor se brise kada korisnik napravi bar jedan konstruktor.
* Podrazumevana vidljivost u klasi (ukoliko ne navedemo eksplicitno) je private.
* Nepisana konvencija moze biti da private clanovi klasa pocinju sa _ radi lepse organizacije koda.
* Podrazumevani pozove konstruktor bez argumenata
* Podrazumevani konstruktor poziva konstruktor bez argumenata za klasu razlomak.
* Podrazumevane vrednosti a=0, b=1
* Kada napisemo jedan, podrazumevani konstruktor vise ne postoji.

Ako dodamo const ispred this prosledjujemo metodu, kome se pristupa unutar klase. Da bi mogli da vracamo (get) tj. da getujemo .. Dodacemo metod() const{} - Ne menja objekat na kome se poziva, deklarisemo tu metodu kao konstantnu. I dalje ce se taj motod koristiti i na nekonstantne objekte lagano.
