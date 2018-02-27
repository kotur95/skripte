---
title: KK - Nedejla 02
author: <a href='https://github.com/kotur95/skripte'>github.com/kotur95/skripte</a>
date: 27.02.2018.
---

# Objektno-Orijentisani C++

Sta je to tacno javni interfejs?

Konstruktorska sintaksa

```c++
/* Lista inicijalizacije promenljivih */

/* Ovakav konstruktor alocira prostor za promenljivu,
bez kreiranja dodatne promenljive koja se potom dodeljuje klasnom tipu. */

Razlomak(int a = 0, int b = 1) :_brojilac(a), _imenilac(b) {
	/* Telo konstruktora */
}
```

Sada zelimo da napravimo da se razlomak ispisuje na sledeci nacin:

```c++
Razlomak r;
cout << r << endl;
```
Da bi ovakva sintaksa bila moguca, potrebno je da definisemo operator `<<` na razlomcima.
Ovaj konstruktor ce morati da se definise van klase razlomak zasto?

Interno to se prevodi 'metod' `cout.operator<<(r)` koja se izvrsava nad cout sa argumentom `r`. Dakle operator `<<` treba da bude definisan u klasi `ostream`.  

Pisemo van svih klasa operator <<.
- Povratna vrednost ovog operatora treba da bude ostream.

```c++
/* Prosledjujemo razlomak i ostream po referenci Razlomak 
radi ustede memorije, a ostream mozemo menjati. */

ostream operator << (ostream &izlaz, const Razlomak & r) {
	izlaz << r.vratiBrojilac() << "/" << r.vratiIMenilac();
	return izlaz;
}
```

Kako cemo uraditi slicnu stvar za `cin`?

```c++
Razlomak r;
cout << "Unesi razlomak r: ";

/* Mozemo vise da probamo */
try {
	cin >> r;
} catch (char * s) {
	cerr << s << endl;
	exit(EXIT_FAILURE);
} catch (int s) {

} catch (const char * s) {
	cout << "Neispravan unos razlomka" << endl;
}

/* Ili da hvatamo sve tipove izuzetaka */
catch (...) {
	cout << "Nepoznat tip izuzetka" << endl;
}
```

```
Neispravan format razlomka
initiate called after throwing instance const char*
```

Na isti nacin i iz istih razloga pisemo operator `>>` van klase Razlomak:

```c++
istream& operator >> (istream& ulaz, Razlomak& r) {
	/* Ovde ima problem jer su imenilac i brojilac 
	privatni clanovi i ne mozemo im pristupiti */
	ulaz >> r._brojilac << "/" r.vratiImenilac();
}
```

Resavanje ovog problema moze da se uradi tzv `prijateljskom funkcijom` - To su funkcije ili klase koje mogu da pristupaju privatnim podacima neke druge klase.

Zelimo nekoj konkretnoj klasi ili nekoj konkretnoj funkciji da dozvolimo da pristupaju nekoj `funkciji/klasi`. To ne moze funkcija ili klasa sama sebi da kaze vec se to radi u klasi cijim se podacima pristupa.
Dodajemo:

```c++
	friend
```

Umesto toka cu da napravim javni interfejs iz klase razlomak koji ce da obezbedi
pisanje razlomka (pravimo metod u klasi razlomak)

```c++
void pisi(ostream& izlaz) const {
	izlaz << _brojilac << "/" << _imenilac;
}
```

Onda radimo za ispisivanje na izlaz:

```c++
ostream& operator << (ostream& izlaz, const RAzlomak & r) {
	r.pisi(izlaz);
	return izlaz;
}

```

Slicno bi bilo za upis:

```c++
ostream& operator << (ostream& ulaz,  RAzlomak & r) {
	r.citaj(ulaz);
	return izlaz;
}

```

```c++
 char c;
 cin << r.brojilac << c << r.imenilac;

 if (c != '/') {
	/* Ne mora klasa exception kao u javi,
	moze objekat bilo kog tipa. */

	throw "Neispravan format razlomka" 
}
	
 return ulaz;
```

U klasu `Razlomak` dodajemo metode `citaj()` i `pisi()` i tu hvatamo exception-e
```c++
 /* Ovde ide kod za citaj() i pisi() */
```

Dalje zelimo da implementiramo nesto nad razlomcima (sabiranje/oduzimanje npr)

```c++
cout << "Unesi razlomak r" << endl;
cin >> r;

cout << "Unesi razlomak r" << endl;
cin >> p;

cout << "r + p = " << r + p;

/* Ovo nece raditi dok ne implementiramo op. + */
```

```c++
/* C-ovska sintaksa */
Razlomak saberi(const Razlomak & a, const Razlomak &b) {
	Razlomak tmp;
	
	tmp.postaviImenilac(a.vratiImenilac() * b.vratiImenilac());
	tmp.postaviBrojilac(a.vratiBrojilac*b.vratiImenilac + a.vratiImenilac() * b.vratiBrojilac())

	return tmp;
}
```

```c++
/* C++ Sintaksa */
/* Umesto toga bar metod u klasi da napravimo */

/* Prvi const kod a odozgo je sad na kraju metoda */
Razlomak Razlomak::saberiSa(const Razlomak &b) const {
	return Razlomak(Konstruktor);
}
```

Ovo nam se ne svijda jer bi uvek morali da pisemo `razlomak.saberiSa(razlomak2)` ...

Umesto toga zelmo da implementiramo bas operator `+`
Jedino sto treba da uradimo je da metod `saberiSa()` preimenujemo u `operator+()`

Oduzimanje, mnozenje, deljenje ... se radi na slican nacin.

Do sada smo radili binarne operator.. Ne mozemo da definisemo nove operatore (skup operatora je fiksan) samo mozemo da ih predefinisemo. Ukoliko zelimo da vidimo koji su to operatoru mozemo uneti u komandnoj liniji:`` (ne mozemo im promeniti asocijativnost i prioritet). Mozemo samo da kazemo kako se izvrsava ta operacija.

Hajde da napravimo operator ~ (npr) da vrati recriprocan razloma:

```c++
/* ~R treba da vrati 1/R */

Razlomak operator ~() const {
	return Razlomak(imenilac, brojilac);
}
```
Slicno mozemo da predefinisemo operator `==` koji vraca `bool` primao bi jedan argument koji poredi sa `this`-om. Razlicito `!=` bi mogli da vratimo negaciju od `==`.


`Operator ++ prefiksni i postfiksni`
```c++
/* r je 1/2 */

cout << r << endl;
cout << ++r << endl;
cout << r << endl;

cout << r << endl;
cout << r++ << endl;
cout << r << endl;
```
```c++
/* Prefiksni nema argumente */
Razlomak Razlomak::operator++() {
	_brojilac += _imenilac;
	return *this;
}

/* Kako prevodilac zna da je prefiksni ili postfiksni */

/* Postfiksni ima argument (fiktivni) int*/
Razlomak Razlomak::operator++(int) {
	Razlomak tmp(_brojilac, _imenilac);
	_brojilac += _imenilac;

*/ Umesto linije iznad moglo je i: */
	++(*this)
/* ali je ovo malo neefikasno */

	return tmp;
}
```

`cast` operator zelimo da implementiramo npt `5/2` da kastujemo u double `2.5`

```c++
cint >> r;
cout << "double(r) = " << double(r) << endl;
```

U klasi Razlomak definisemo cast operator:
Kod `cast` operatora za razliku od ostalih operatora nemamo povratnu vrednost 
vec se pretpostavlja da je povratna vrednost bas ono na sta se kastuje.
```c++
operator Razlomak::double() const {
	return (double)_brojilac/_imenilac;
}
```
```
Ulaz: 1/2
Izlaz: 0.5
```

Da li je moguce uraditi nesto ovako? 
```c++
/* Ne ocekujemo da ovo radi sa konstantama */
cout << "r + 1 = " << r + 1 << endl;

/* Ali ce operaor + da radi ali nece raditi ocekivano */

/* Znace da cast-uje int u razlomak jer ce konstruktor da se 
koristi kao cast operator (ako ne postoji eksplicitni cast operator) */

/* Implicitno ce cast-ovati 1 -> 1/1 kao ispod: */
cout << "r + 1 = " << r + Razlomak(1) << endl;

/* Mogu da sprecim kompilator da radi implicitno cast-ovanje
reci cu mu da radi samo eksplicitno - To se vrsi tako sto ispred
konstruktora stavimo kljucnu rec 'explicit' */

explicit Ralomak(const int &b);

/* Ukoliko cast operator u double postoji */
cout << "r + 1 = " << r + 1 << endl;
cout << "r + 1 = " << (double) r << endl;
/* Ovo vraca crednost 2.5 TODO: Proveri */
```

Zna da treba da se kastuje `int` u `Razlomak` jer je jedino moguce kastovati u tom smeru. Morali bi da napravimo slican `cast` operator za `int`.

Kljucna rec `explicit` moze da stoji uz bilo koji konstruktor.

Ako kazemo da je i cast operator implicitan a i konstruktor kompilator nece znati koji od dva kandidata da koristi i izbacice gresku (da li konstruktor ili (double) cast operator.

TODO Videti sta se tu desava kad su oba explicit, oba implicit.

## Desktruktori

U zasebne fajlove pisemo deklaracije/definicije.

`niz.hpp:`
```c++
#ifndef __NIZ_HPP__
#define __NIZ_HPP__

class niz {
private:
	int _velicina;
	int _kapacitet;
	int *_elementi; // Pokazivac na elemente
public:
	Niz(int a) 
		: _velicina(a), _kapacitet(a), _elementi(int[a]) {}

	int velicina() const {
		return _velicina;
	}

	int& operator[](int i) const; // implementiran u niz.cpp fajlu
	int operator[](int i); // Neophodan bas za dodelu (menjanje)
}

#endif
```
Ukoliko je metod jako prost mozemo ga napisati u `.hpp` fajlu.

`niz.cpp:`
```c++
#include "niz.hpp"

 int Niz::operator[](int i) const {
 	if (i >= velicina)
 		throw "Prevelik indeks niza";
 	
 	return _elementi[i];
 }

/* Nova verzija - Siri niz */
int& Niz::operator[](int i) const {
	if (i >= _kapacitet) {
		int *novi = new int[i + 10]

		for (int j=0; j < _velicina; j++)
			novi[j] = _elementi[j];

		_kapacitet = j + 10;
		_velicina = j + 1;
		delete [] _elementi;
		_elementi = novi;

	} else if (i >= velicina) {
		_velicina = j + 1;
	}
}
```

`1) main.cpp:`
```c++
int main () {

	Niz n(10);

	for (int i=0; i < n.velicina(); i++)
		cout << n[i] << endl;

	reuturn 0;
}
```

`2) main.cpp:`
```c++
int main () {

	Niz n(10);

	for (int i=0; i < n.velicina(); i++) {
		/* Ovo nece da radi dok ne definisemo odgovarajuci operator 
		koji smo definisali sa int & operator[] ... jer levi operand nije lvalue */
		n[i] = i*i; 
	}

	for (int i=0; i < n.velicina(); i++)
		cout << n[i] << endl;
	
	niz[100] = 100;

	reuturn 0;
}
```


Ono sto zelimo da resimo je dinamicko povecavanje velicine niza.



`3) main`
```c++
int main() {
	Niz n(1);

	/* Ako alociramo samo po 10 elemenata ponovne 
	realokacije ce oduzeti jako puno vremena. */

	for (int i=0; i <= 1000000; i++) 
		n[i] = i;

	/* Ovo radi dosta dosta brze */
	/* 4MB memorije je potrebno */
	for (int i=1000000; i >= 0; i--) 
		n[i] = i;
		
		/* Pocetak: Ovde imamo vece curenje memorije */
		for (int j=0; j<1000; j++) {

			/* Ovo radi dosta dosta brze */
			/* 4MB memorije je potrebno (kada nije u
			for petlji (ocu) */

			for (int i=1000000; i >= 0; i--) 
				n[i] = i;

		}
		/* Kraj: Ovde imamo curenje memorije */
		
		/* Niz n(1) - Curi nam jer nastavlja da zivi na kraju bloka for-a */

		
		/* Podrazumevani destruktor ne radi ono so nam treba */
	}
	return 0;
}
```

**Faze:**
TODO Dodaj ispravno koje su faze

1) Alokacija
2) Inicijalizacija 
2) Deinicijalizacija
2) Dealokacija

Postoji podrazumevani destruktor (on unistava klasne tipove na kraju bloka)

Podrazumevani destruktor za svaki clan podataka poziva podrazumevani destruktor za clanove. Npr. destruktor za int oslobadja ta 4 bajta koja int zauzima.

Ako bi imali klasni objekat, podrazumevani destrutor bi pozvao destruktor za njega. U zavisnosti od tipa podatka destruktor ce uraditi ono sto treba.

Posto imamo pokazivac na niz, podrazumevani nece osloboditi prostor na ocekivani nacin (nece osloboditi clanove niza).

`Implementacija destruktora za klasnu niz`
Radimo onaj visak koji podrazumevani ne radi,
kada napravimo svoj nakon naseg destruktora se vrsi
i ono sto podrazumevani ne rade, trebaju da se izbrisu clanovi niza
ali ne treba ono sto radi podrazumevanu destruktor.

```c++
Niz::~Niz() {
	delete [] _elementi;
}
```

Za razliku od `Jave` c++ nema `garbage collector` imamo vise posla ali i bolju kontrolu nad memorijom.


## Operator dodele


## Konstruktor kopije
