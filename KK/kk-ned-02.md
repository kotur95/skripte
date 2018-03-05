---
title: KK vežbe - Nedejla 02
author: "[github.com/kotur95/skripte](http://github.com/kotur95/skripte)"
date: 27.02.2018.
---

# Objektno-Orijentisani koncepti (C++)

## Konstruktori

Konstruktorska sintaksa

```c++
/* Lista inicijalizacije promenljivih */

/* Ovakav konstruktor alocira prostor za promenljivu,
bez kreiranja dodatne promenljive koja se potom dodeljuje klasnom tipu. */

Razlomak(int a = 0, int b = 1) :_brojilac(a), _imenilac(b) {
	/* Telo konstruktora */
}
```

## Operatori

Zelimo da omogucimo sledecu sintakstu.

`Razlomak, ostream i operator <<:`
```c++
Razlomak r;
cout << r << endl;
```
Da bi ovakva sintaksa bila moguca, potrebno je da definisemo operator `<<` nad ostream-om i Razlomkom.
Ovaj operator ce morati da se definise van klase razlomak - zasto?

Interno to se prevodi u 'metod' `cout.operator<<(r)` koja se izvrsava nad cout sa argumentom `r`. Dakle operator `<<` treba da bude u okviru klase `ostream`.

Pisemo van svih klasa operator `<<`, povratna vrednost ovog operatora treba da bude objekat tipa `ostream`.

`Implementacija peratora <<:`
```c++
/* Prosledjujemo razlomak i ostream po referenci Razlomak 
radi ustede memorije, a ostream da bi ga menjali. */
ostream operator << (ostream &izlaz, const Razlomak & r) {
	izlaz << r.vratiBrojilac() << "/" << r.vratiIMenilac();
	return izlaz;
}
```

Kako cemo uraditi istu stvar za ulaz, kao u primeru ispod?

`Operator >>:`
```c++
Razlomak r;
cout << "Unesi razlomak r: ";

/* Hvatamo izuzetke pojedinacno */
try {
	cin >> r;
} catch (char * s) {
	cerr << s << endl;
	exit(EXIT_FAILURE);
} catch (int s) {

} catch (const char * s) {
	cout << "Neispravan unos razlomka" << endl;
}

/* Hvatamo sve tipove izuzetaka odjednom */
catch (...) {
	cout << "Nepoznat tip izuzetka" << endl;
}
```
**Izlaz:**
```
Neispravan format razlomka
initiate called after throwing instance const char*
```

Operator `>>` implementiramo slicno kao operator `<<`.

```c++
istream& operator >> (istream& ulaz, Razlomak& r) {
	/* Ovde ima problem jer su imenilac i brojilac 
	privatni clanovi i ne mozemo im pristupiti */
	ulaz >> r._brojilac << "/" r.vratiImenilac();
}
```

Problem pristupanja privatnim clanovima klase Razlomak mozemo razresiti tzv. `prijateljskom funkcijom`, to su funkcije ili klase koje mogu da pristupaju privatnim podacima neke druge klase.

```c++
	/* Todo za friend klasu */
	friend
```

Bolji nacin razresavanja pristupa privatnim clanovima klase `Razlomak` jeste pravljenje javnog interfejsa (metoda) u okviru klase. Prilikom pozivanja ovog metoda neophodno je proslediti mu `ostream` na koji treba da ispise razlomak.

```c++
void Razlomak::pisi(ostream& izlaz) const {
	izlaz << _brojilac << "/" << _imenilac;
}
```

Onda implementiramo operator `<<` na sledeci nacin:
```c++
ostream& operator << (ostream& izlaz, const Razlomak & r) {
	r.pisi(izlaz);
	return izlaz;
}

```

Slicno bi bilo za citanje sa `cin`:

```c++
ostream& operator >> (ostream& ulaz,  Razlomak & r) {
	r.citaj(ulaz);
	return izlaz;
}
```

`Citanje razlomka sa standardnog ulaza:`
```c++
 char c;
 /*             Razlomacka crta      */
 /*                   |              */
 /*                   V              */
 cin << r.brojilac << c << r.imenilac;

/* Ako je c razlicito od razlomacke crte */
 if (c != '/') {
	/* Izuzetak ne mora biti pripadnik klase exception 
	kao u Javi, moze objekat bilo kog tipa. */
	throw "Neispravan format razlomka";
}
 return ulaz;
```
U klasu `Razlomak` treba dodati metode `citaj()` i `pisi()` i tu uhvatiti izuzetke.

Dalje zelimo da implementiramo nesto nad razlomcima (sabiranje/oduzimanje npr)

```c++
cout << "Unesi razlomak r" << endl;
cin >> r;

cout << "Unesi razlomak r" << endl;
cin >> p;

cout << "r + p = " << r + p;

/* Ovo nece raditi dok ne implementiramo operator + */
```

`C-ovski nacin:`
```c++
/* C-ovska sintaksa */
Razlomak saberi(const Razlomak & a, const Razlomak &b) {
	Razlomak tmp;
	
	tmp.postaviImenilac(a.vratiImenilac() * b.vratiImenilac());
	tmp.postaviBrojilac(a.vratiBrojilac*b.vratiImenilac + a.vratiImenilac() * b.vratiBrojilac())

	return tmp;
}
```

`C++ nacin:`
```c++
/* C++ Sintaksa */
/* Umesto toga bar metod u klasi da napravimo */

/* Prvi const kod a odozgo je sad na kraju metoda */
Razlomak Razlomak::saberiSa(const Razlomak &b) const {
	return Razlomak(Konstruktor);
}
```

Ovo nam se ne svijda jer ne moramo uvek da pisemo `razlomak.saberiSa(razlomak2)`, umesto toga  implementiramo bas operator `+`, koji nam pruza visi stepen udobnosti. Jedino sto treba da uradimo je da metod `saberiSa()` preimenujemo u `operator+()`
```c++
Razlomak Razlomak::operator+(const Razlomak &b) const {
```

Ostali opeartori se mogu uraditi na slican nacin.

Skup operatora je fiksan i ne mozemo definisati nove operatore, samo mozemo da ih predefinisemo, takodje ne mozemo im promeniti asocijativnost i prioritet.

Do sada smo radili binarne operator, hajde da napravimo unarni operator ~ koji vraca recriprocan razlomak:

`Operator reciprocan razlomak (~):`
```c++
/* ~R treba da vrati 1/R */
Razlomak operator ~() const {
	return Razlomak(imenilac, brojilac);
}
```
Slicno mozemo da predefinisemo operator `==` koji vraca `bool`, primao bi jedan argument koji poredi sa `this`-om. Razlicito `!=` bi mogli da vratimo negaciju od `==`.

`Main program:`
```c++
/* r je 1/2 */
cout << r << endl;
cout << ++r << endl;
cout << r << endl;

cout << r << endl;
cout << r++ << endl;
cout << r << endl;
```
`Operator ++ (prefiksni):`
```c++
/* Prefiksni nema argumente */
Razlomak Razlomak::operator++() {
	_brojilac += _imenilac;
	return *this;
}
```

```
/* Kako prevodilac zna da je prefiksni ili postfiksni */
/* Postfiksni ima argument (fiktivni) int */
Razlomak Razlomak::operator++(int) {
	Razlomak tmp(_brojilac, _imenilac);
	_brojilac += _imenilac;
	return tmp;
}
```

`cast` operator zelimo da implementiramo npt `5/2` da kastujemo u double `2.5`

```c++
cint >> r;
cout << "double(r) = " << double(r) << endl;
```

U klasi Razlomak definisemo `cast` operator:
Kod `cast` operatora za razliku od ostalih operatora nemamo povratnu vrednost 
vec se implicitno pretpostavlja da je povratna vrednost bas ono na sta se kastuje.

`cast operator:`
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
/* Ali ce operaor + da radi ali ne na ocekivani nacin */
/* Znace da cast-uje int u razlomak jer ce konstruktor da se 
koristi kao cast operator (jer ne postoji eksplicitni cast operator) */

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

**Napomena:** Kompilator zna da treba da kastuje `int` u razlomak jer je jedino moguce kastovati u tom smeru.

Kljucna rec `explicit` moze da stoji uz bilo koji konstruktor, ne samo uz `cast` operator.

Ako su cast operator i konstruktor za dati tip `implicitni`, tj. nigde nismo naveli `explicit` kljucnu rec, kompilator ce se buniti jer ne zna koji od dva kandidata da koristi (konstruktor ili `cast` operator).

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

/* Nova verzija programa koji povecava duzinu niza */
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

`No.1) main.cpp:`
```c++
int main () {

	Niz n(10);

	for (int i=0; i < n.velicina(); i++)
		cout << n[i] << endl;

	reuturn 0;
}
```

`No.2) main.cpp:`
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

`No.3) main.cpp`
```c++
int main() {
	Niz n(1);

	/* Ako alociramo samo po 10 elemenata ponovne 
	realokacije ce oduzeti jako puno vremena. */

	for (int i=0; i <= 1000000; i++) 
		n[i] = i;

	/* Ovo radi dosta dosta brze jer odjednom 
	alocira potreban prostor */
	for (int i=1000000; i >= 0; i--) 
		n[i] = i;
		
		/* Pocetak: Ovde imamo vece curenje memorije */
		for (int j=0; j<1000; j++) {

			/* Ovo radi dosta dosta brze */
			/* Naizgled treba nam samo 4MB memorije */

			for (int i=1000000; i >= 0; i--) 
				n[i] = i;

		}
	/* Imamo problem, ne treba nam 4MB nego 1000*4MB.
	/* Niz n(1) - Curi nam jer nastavlja da zivi na kraju bloka for-a */
	/* Podrazumevani destruktor ne radi ono so nam treba */
	}
	return 0;
}
```

**Faze:**
TODO Proveriti ove faze (da li je to sve):

1) Alokacija
2) Inicijalizacija 
2) Deinicijalizacija
2) Dealokacija

Postoji podrazumevani destruktor (on unistava klasne tipove koji nisu dinamicki alocirani na kraju bloka)

Podrazumevani destruktor za svaki clan klase (podatak) poziva podrazumevani destruktor za taj tip (clana). Npr. destruktor za int oslobadja ta 4 bajta koja int zauzima.

Ako bi imali klasni objekat, podrazumevani destrutor bi pozvao destruktor za njega. U zavisnosti od tipa podatka destruktor ce uraditi ono sto treba.

Posto imamo pokazivac na niz, podrazumevani nece osloboditi prostor na ocekivani nacin (nece osloboditi clanove niza koji su dinamicki alocirani vec ce samo osloboditi prostor za cuvanje pokazivaca).

Kada implementiramo nas destruktor, potrebno je da dealociramo samo onaj visak koji podrazumevani ne moze,
kada pozovemo nas destruktor, nakon njega se izvrsava rutina koju bi inace
podrazumevani destruktor izvrsio. Dakle kada pravimo nas `custom` destruktor potrebo je izbrisati
ono sto ne moze podrazumevani destruktor, u nasem primeru treba da dealociramo clanove niza.

`Implementacija destruktora za dinamicki alocirani niz:`
```c++
Niz::~Niz() {
	delete [] _elementi;
}
```

Za razliku od `Jave` c++ nema `garbage collector` imamo vise posla ali i bolju kontrolu nad memorijom.
