---
title: KK vežbe - Nedejla 03
author: "[github.com/kotur95/skripte](http://github.com/kotur95/skripte)"
date: 05.03.2018.
---

# Programski jezik C++ (nastavak)

## Zamenjivanje default copy konstruktora

`Niz` sa proslog casa:
```c++
#include <iostream>
#include "niz.hpp"

int main() {
	for (int i=0; i< 100000; i++) {
		cout << "Usao u petlju" << endl;
		Niz n(1000);
		// ...
		// Copy konstruktor
		Niz n1(n); //  (<- Na ovoj liniji je problem)
		// ...
		cout << "stigao" << endl;
	} 
	return 0;
}
```
`Izlaz:`
```
Usao u petlju
Stigao
Dobijamo gresku: 2x brises jednu istu adresu
```
Greska je u `copy` konstruktoru, postoji podrazumevani a nas `custom` nismo implementirali.
Naime, videli smo da podrazumevani destruktor za svaki clan podatak poziva njegov default destruktor. Na taj nacin i podrazumevani copy konstruktor poziva copy konstruktor za svaki clan (podatak) klase.

```c++
class Niz {
	int _velicina; // -> kopira se iz n->n1
	int _kapacitet; // -> kopira se iz n->n1
	int *_elementi; // -> kopira se iz n->n1
};

n = Niz();
n1 = Niz(n);
```

`Copy konstruktor` kopira pokazivace na elemente niza, a ne same vrednosti na tim adresama. Kada dodje do poziva destruktora pozivaju se destruktori za elemente na adresama &n1 i &n koji pokazuju na isti niz. (destruktor iz klase niz glasi otprilike `delete [] _elementi`. Dakle problem je sto se 2x izvrsava dealokacija memorije za isti niz. Ali obratimo paznju i zakljucimo da problem nije u destruktoru (koji radi ocekivano) vec u copy konstruktoru.

**Resenje:**
Pravimo `custom copy` konstruktor:
```c++
Niz::Niz(const Niz &n) {
	_velicina = n._velicina;
	_kapacitet = n._kapacitet;
	_elementi = new int[_kapacitet];

	/* Prepisujemo elemente niza */
	for (int i=0; i<_velicina; i++) {
		elementi[i] = n._elementi[i];
	}
}
```
Kada smo napravili nas `custom copy` konstruktor `default` `copy` konstruktor se brise.

- Ako ne napravimo destruktor a napravimo copy konstruktor u 99.9% slucajeva nam treba i jedan i drugi.
- Obicno ako nam treba `copy` konstruktor, operatora dodele ili destruktor trebace nam dalje negde u kodu sva tri.

Da li je operator `=` konstruktor kopije?? Odgovor je `Da` i `Ne`, naime pogledajmo sledeci primer:
 
```c++
/* Konstruktor koji prima velicinu niza */
Niz n(1000);

/* Ovo jeste konstruktor kopije */
Niz n1(n);

/* Ovo jeste konstruktor kopije */
Niz n2 = n // <=> Niz n2(n)

/* Ovo nije kontruktor kopije, ovo je nesto apsolutno drugo */
Niz n1 = n; // Ovo je operator dodele
```

default operator dodele ce za `n1 = n` u pozadini uraditi sledece:
`_elementi = rop._elementi`

Za prost tip operator dodele dodeli vrednost, a za pokazivacki tip ne vrsi alokaciju vec dodeli vrednost pokazivaca na _elementi i oni pokazuju na iste resurse (isti je pokazivac).

Ovaj problem razresavamo isto kao kod konstruktora kopije. *Duboko* kopiranje je potrebno i ovde uraditi za dinamicki alocirane podatke.

```c++
_elementi = new int[_kapacitet];

	/* Prepisujemo elemente niza */
	for (int i=0; i<_velicina; i++) {
		elementi[i] = n._elementi[i];
	}
```

Pogledajmo sada sledeci primer: `x = y = z`. Ovo je moguce u jezicima `c/c++`, operator dodele je desno asocijativan operator.

Potpis za `operator dodele` bilo kojeg tipa koji kreiramo je gotovo identican, npr. za tip `Niz`:

`Niz operator=(const Niz &n);`

```c++
Niz Niz::operator=(const Niz &n) {
	_velicina = n._velicina;
	_kapacitet = n._kapacitet;

	_elementi = new int[_kapacitet];

	for (int i=0; i<_velicina; i++) {
		elementi[i] = n._elementi[i];
	}
}
```

Primetimo da gotovo identican kod koristimo u operatoru dodele i u prethodno navedenom copy konstruktoru. 

"`Kada isti kod napisemo 2 puta, program pocinje da smrducka.`" (*Filip Maric 1. mart 2018.*)

Da nam kod ne bi smrduckao, napravicemo pomovni metod koji se zove `init()` za inicijalizaciju objekta, i `deinit()` za deinicijalizaciju objekta.

```c++
/* Implementacije inicijalizatora */
Niz Niz::init=(const Niz &n) {
	_velicina = n._velicina;
	_kapacitet = n._kapacitet;

	_elementi = new int[_kapacitet];

	for (int i=0; i<_velicina; i++) {
		elementi[i] = n._elementi[i];
	}
}

/* Implementacije deinicijalizatora */
void Niz::deinit() {
	delete [] _elementi;
}
```

Onda mozemo implementirati operator dodele na sledeci nacin:
```c++
Niz & Niz::operator=(const Niz &n) {
	// #1 Ovde fali destruktor ... dodajemo, ili:
	deinit(); // Neophodno zbog curenja memorije
	init(n);
	return *this;
}
```
Problem moze nastati ako nemamo metod `deinit()`. Ako smo alocirali svaki put po `1000` elemenata iscurelo bi nam po `sizeof(element)*1000` memorije.
```

Kod iznad (operator dodele) nece raditi za `n = n`. Dati kod je sulud, pa nikada necemo uraditi `n = n` u programu. Ali npr. moze se desiti nesto nalik `*p = *q` a prethodno pokazivaci p i q pokazuju na n: `p->n` i `q->n`.

Da bi razresili ovaj problem, sve sto treba da uradimo je da proverimo da li `p` i `q` pokazuju na istu adresi ako pokazuju na istu adresu ne radimo nista. U suprotnom pozivamo `init()` metod.

```c++
Niz & Niz::operator=(const Niz &n) {
	/* Ukoliko this i n ne pokazuju na istu adresu */
	if (this != &n) //   <- Ovo je dodato u odnosu na prethodni pr. 
		deinit(); // <-
	init(n);
	return *this;
}
```

## Sablonizovane funkcije

Pogledajmo sledeci primere koda:

```c++
#include <iostream>

using namespace std;

int manji(int a, int b) {
	return a < b ? a : b;
}

/* Preopterecujemo funkciju manji */
double manji(double a, double b) {
	return a < b ? a : b;
}

/* Opet preopterecujemo funkciju manji za char-ove */
char manji(char a, char b) {
	return a < b ? a : b;
}

int main()
{
	int x = 1, y = 4;
	double x_1 = 1, y_1 = 4;
	double x_2 = 1.9, y_2 = 4.2;
	char c1 = 'a', c2 = 'b';

	/* Zelim da ispisem minimum x,y: */
	cout << manji(x, y) << endl;
	cout << manji(x_1, y_1) << endl;
	cout << manji(x_2, y_2) << endl;
	cout << manji(c1, c2) << endl;

	return 0;
}
```
**Izlaz:**
```
1
1.0
1.9
a
```

Broj tipova moze biti jako veliki a funkcija da bude identicna, zelimo nekako da napravimo jednu funkciju koja moze da radi sa dosta tipova.

`Sablonska funkcija:`
```c++
/* Jedna funkcija za sve tipove */
template <class T>
T manji(T a, T b) {
	return a < b ? a : b;
}
```
A sta ako funkciju pozovemo sa: `manji(2, 2.3);` -> Dolazi do greske, ne moze `double` i `int` da zameni sa `T` jer smo rekli da nasa funkcija ocekuje samo jedan tip u sablonu.

**Napomena:** Ovo mozemo resiti *"glupo"* operatorom `cast-ovanja`. Ali mozemo i pametnije, eksplicitnim navodjenjem koji tip zelimo da koristimo u sablonu:
```c++
/* Od svih funkcija koje obezbedjuje sablonska
funkcija pozovi funkciju sa sablonom T -> int */
            /* Uocimo */
            /*  |  */
            /*  V  */
cout << manji<int>(2.1, 3) << endl;
```
**Izlaz:**
```
2
```

**Primer (Uporedjivanje C-ovskih stringova):**
```c++
char * s1 = "mirko";
char * s2 = "spasic";

/* Uporedjujemo dve adrese a ne npr. duzine stringa, to nije ispravno */
cout << manji(s1, s2) << endl;
```
**Izlaz (nije uvek isti):**
```
mirko (ili) spasic
```

Da bi funkcija radila za pokazivace (c-ovske) stringove, potrebno je napraviti posebnu funkciju:
```c++
/* Vraca kraci string */
char * manji(char * a, char * b) {
	if (strcmp(a,b) < 0)
		return a;
	else
		return b;
}

int main() {
	char * s1 = mirko;
	char * s2 = spasic;
	cout << manji(s1, s2) << endl;	

	return 0;
}
```
**Izlaz:**
```
Mirko
```
## Sablonizovane klase

### Nesablonizovana klasa (Par)

Osim za funkcije, sablone mozemo da radimo i za nase korisnicke klase. Naime potrebno je da eksplicitno navedemo `tip` pri samoj deklaraciji/inicijalizaciji promenljive kada kreiramo objekat koji zahteva sablon za razliku od funkcije gde je kompilator to radio automatski za nas. Pogledajmo primer klase koja nije sablonizovana a zelimo da je sablonizujemo.

`Nesablonizovana klasa Par (uredjen par):`
```c++
class Par {

public:
	Par(int a, int b)
		:_prvi(a), _drugi(b)
	{}

	void pisi(ostream &izlaz) const {
		izlaz << "(" << _prvi << "," << _drugi << ")";
	}

private:
	int _prvi;
	int _drugi;
};

ostream & operator <<(ostream &izlaz, const Par &p) {
	p.pisi(izlaz);
	return izlaz;
}

int main() {
	Par p(1,2);
	cout << p << endl;

	return 0;
}
```
**Izlaz:**
```
(1,2)
```
### Sablonizovana klasa (Par)

`Sablonizovana klasa Par (uredjen par):`
```c++
template <class T>
class Par {
public:
	Par(T a, T b)
		:_prvi(a), _drugi(b)
	{}

	void pisi(ostream &izlaz) const {
		izlaz << "(" << _prvi << "," << _drugi << ")";
	}

private:
	T _prvi;
	T _drugi;
};

/* Obavezno (da bi ga koristili) moramo da definisemo i sablonski operator << */
template <class T>
ostream &operator<<(ostream &izlaz, const T &p) {
	p.pisi(izlaz);
	return izlaz;
}

int main() {
	/* Dakle za razliku od funkcija, kod pravljenja promenljive
	moramo da koristimo punu sintaksu (kazemo koji je tip Par) */
	/*   |   */
	/*   V   */
	Par<int> p(1,2);
	Par<int> p0(1,2);
	Par<double> p1(3,4);

	/* Par parova: */
	/* Ovde treba 'blanko' zbog gramzivosti c++ sintaksnog analizatora! */
	/*          |          */
	/*          v          */
	Par<Par<int> > p3(p0, p)
	/* Inace ce >> da prepozna kao operator */

	cout << p << endl;
	cout << p1 << endl;
	cout << p3 << endl;

	return 0;
}
```
**Izlaz:**
```
(1, 2)
(1.0, 2.0)
((1, 2), (1, 2))
```
Takodje nije moguce napraviti par parova koji su razlicitih tipova npr da par parova bude par tipa `int` i par tipa `double`: `((int, int), (double, double))`

Najcesce necemo toliko da sablonizujemo sami vec cemo koristiti gotove sablone iz `STL`-a (Standardne `c++` biblioteke).

## STL - Standardna biblioteka (sablona)
### Klasa Vector

`Klasa vector (dinamicki niz):`
```c++
#include <iostream>
#include <vector> // Ukljucivanje zaglavlja za vector

using namespace std; // Namespace u kome je vector

int main() {
	vector<int> v1; // (Konstruktor sa 0 argumenata)
	vector<int> v2(10);

	cout << v2.size() << endl;
	cout << v2.capacity() << " " << endl; // Kapacitet

	for (unsigned i=0; v.size(); i++)
		cout << v2[i] = i*i;

	for (unsigned i=0; v.size(); i++)
		cout << v2[i]a << endl;
	
	v2.resize(12); // povecava velicinu
	v2.resize(8); // sece velicinu
	v2.reserve(8); // menja kapacitet

	// Od c++11 standarda moze:
	for (auto a : v)
		cout << a;

	/* Prolazak kroz vektor v2 pomocu iteratora */
	for (vector<int>::iterator i = v2.begin(); i != v2.end(); i++) 
		cout << *i << endl;

	/* v.begin() je "pokazivac" na prvi element, a v.end() na predposlednji */

	/* Od standarda c++11, postoji kljucna rec auto 
	for (auto i = v2.begin(); i != v2.end(); i++) 
		cout << *i << endl;

	/* Iteratori za kretanje unazad */
	for (auto i = v2.rbegin(); i!= v2.rend(); i++)
		cout << *i << endl;

	return 0;
}
```

`auto` - Kljucna rec na osnovu koje kompilator sam zajljuci koji je tip promenljive.
Primerimo da ako smanjimo samo `size` vektora a ne `capacity`, kapacitet ostaje isti memorija se ne oslobadja dok to eksplicitno ne zatrazimo za vreme zivotnog veka promenljive.

### Klasa List 

**Klasa `list`:**
```c++
list<double> l;
l.push_back(1.1);
l.push_front(0.1);

for (lista<double>::iterator i = l.begin(); i !+ l.end(); i++)
	cout << *i << " ";

cout << endl;
cout << l.size() << endl;
```
```
1.1 0.1
2
```
Za dodatne detalje posetiti: [http://www.cplusplus.com/reference/imeklase/imeklase/](http://www.cplusplus.com/reference/imeklase/imeklase/)

### Klasa set (skup)

```c++
/* Na pocetku fajla treba da stoji: #include <set> */
set <int> s;
cout << s.size() << endl;
s.insert(1);
s.insert(2);
s.insert(3);
s.insert(3);

cout << s.size() << endl;

for (auto i = s.begin(); i != s.end(); i++)
	cout << *i << " ";

/* Provera da li element postoji u skupu */
if (s.count(5) > 0) 
	cout << "5 postoji u skupu" << endl;
else 
	cout << "5 ne postoji u skupu" << endl;

/* Brisanje iz skupa */
s.erase(1); // velicina se smanjuje
/* s.erase(iterator od, iterator do); -> Postoji i to */
```
### Klasa string

```c++
#include <string> // Zaglavlje za string

string s1("mirko");
string s2("slavko");
cout << s << endl;

s.size(); // < = > 
s.length(); // Vraca velicinu stringa (duzinu)
s1 += s2; // Konkatanacija (nadovezivanje) stringova

cout << s1 << endl;
cout << s1.find_first_of("sl"); // -> Vraca indeks prvog pojavljivanja 
s1.c_str() // Vraca c-ovski string (char *)
```
**Izlaz:**
```
mirko
mirkoslavko 
5
```

### Klasa map (mapa)

Za razliko od dosadasnjih kontejnerskih tipova, mapa je asocijativni tip.

```c++
#include <map>

using namespace std;

int main() {

	map(<string>, <int>) ocene;
	cout << ocene.size() << " (velicina)" << endl;

	ocene["branko"] = 8;
	ocene["sima"] = 9;
	ocene["andrej"] = 10;

	cout << ocene.size() << " (velicina)" << endl;

	ocene["andrej"] = 9;
	cout << ocene.size() << " (velicina)" << endl;

	for (map<string, int>::iterator i = ocene.begin(); i != ocene.end(); i++)
		cout << i->first << " - " << i->second << endl;

	/* Ako pristupamo kljucu u mapi koji ne postoji automatski se kreira  
	kljuc u mapi sa default vrednosti za taj tip (ovde int) */
	cout << "mirko - " << ocene["mirko"] << endl;

	/* da nadjemo mirka */
	mapa<string, int>::iterator j = ocene.find("mirko");

	/* da li mirko postoji ? */
	if (j != ocene.end())
		cout << "dejan - " << ocene["dejan"]->second << endl;
	else
		cout << "Ne postoji student sa tim imenom" << endl;

	return 0;
}
```
**Izlaz:**
```
0 (velicina)
3 (velicina)
3 (velicina)
branko - 8
sima - 9
andrej - 9
mirko - 0
Ne postoji student sa tim imenom
```
