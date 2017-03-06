#include<cstdio>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<iomanip>
using namespace std;

struct TOWAR{
	string nazwaTowaru;
	int ilosc;
	string jednostka;
	double cenaJednostkowa;
	
	TOWAR * next;
	TOWAR * prev;		
};

struct KATEGORIA{
	string nazwaKategorii;
	TOWAR * pierwszyTowar;
	TOWAR * ostatniTowar;
	int liczba;
		
	KATEGORIA * next;
	KATEGORIA * prev;
};

KATEGORIA * first;
KATEGORIA * last;
FILE * file;
bool CzyKoniec = false;
int DlugoscKategorii = 0;
int DlugoscTowaru = 0;
int DlugoscIlosci = 0;
bool Inicjalizacja = true;

KATEGORIA * StworzKategorie(string nazwa){
	KATEGORIA * newEl = new KATEGORIA;
	last -> next = newEl;
	newEl -> next = NULL;
	newEl -> prev = last;
	last = newEl;
	
	last -> nazwaKategorii = nazwa;	
	last -> pierwszyTowar = NULL;
	last -> ostatniTowar = NULL;
	
	last -> liczba = 0;
	
	return last;
}

KATEGORIA * CzyKategoriaIstnieje(string nazwa){
	KATEGORIA * kat;
	kat = first;
	bool jest = false;
	
	while(kat != NULL){
		if(kat -> nazwaKategorii == nazwa){
			jest = true;
			break;
		}
		else{
			kat = kat -> next;
		}
	}

	if(jest){
		return kat;
	}
	else{
		return NULL;
	}
}

void PobierzCene(TOWAR * towar){
	char znak;
	string napis;
	
	while(true){
		znak = (char)fgetc(file);
		
		if(znak != '\n' && znak != EOF){
			napis = napis + znak;
		}
		else if(znak == EOF){
			CzyKoniec = true;
			break;
		}
		else{
			break;
		}
	}
	
	towar -> cenaJednostkowa = atof(napis.c_str());
}

void PobierzIlosc(TOWAR * towar){
	char znak;
	string napis;
	bool wyjdz = false;
	int dlugosc;
	
	while(!wyjdz){
		znak = (char)fgetc(file);
		
		if(znak != ':' && znak != ' '){
			napis = napis + znak;
			dlugosc++;
		}
		else if(znak == ' '){
			towar -> ilosc = atof(napis.c_str());
			napis = "";
			
			while(true){
				znak = (char)fgetc(file);
				
				if(znak != ':'){
					napis = napis + znak;
					dlugosc++;
				}
				else{
					wyjdz = true;
					break;
				}
			}
			break;
		}
		else if(znak == ':'){
			towar -> ilosc = atof(napis.c_str());
			napis = "";
			break;
		}
		else{
			break;
		}
	}
	
	if(napis != ""){
		towar -> jednostka = napis;
	}
	else{
		towar -> jednostka = "sztuk";
		dlugosc = dlugosc + 6;
	}
	
	if(dlugosc > DlugoscIlosci){
		DlugoscIlosci = dlugosc;
	}
}

void StworzTowar(KATEGORIA * kategoria, string nazwa, bool dopisz = false, TOWAR ** tow = NULL){
	TOWAR * towar = new TOWAR;
	TOWAR * ostatni;
	
	if(kategoria -> pierwszyTowar == NULL){
		kategoria -> pierwszyTowar = towar;
		kategoria -> ostatniTowar = towar;
		towar -> next = NULL;
		towar -> prev = NULL;
	}
	else{
		ostatni = kategoria -> ostatniTowar;
		kategoria -> ostatniTowar = towar;
		
		ostatni -> next = towar;
		towar -> prev = ostatni;
		towar -> next = NULL;
	}
	
	kategoria -> liczba = kategoria -> liczba + 1;
	towar -> nazwaTowaru = nazwa;
	if(dopisz == false){
		PobierzIlosc(towar);
		PobierzCene(towar);
	}
	else{
		*tow = towar;
	}
	
}

TOWAR * CzyTowarIstnieje(KATEGORIA * kategoria, string nazwa){
	TOWAR * towar;
	towar = kategoria -> pierwszyTowar;
	bool jest = false;
	
	if(towar == NULL){
		return NULL;
	}
	else{
		while(towar != NULL){
			if(towar -> nazwaTowaru == nazwa){
				jest = true;
				break;
			}
			else{
				towar = towar -> next;
			}	
		}
	}
	if(jest){
		return towar;
	}
	else{
		return NULL;
	}
}

void DodajDoTowaru(TOWAR * towar){
	float ilosc;
	char znak;
	string napis;
	
	while(true){
		znak = (char)fgetc(file);
	
		if(znak != ':' && znak != ' '){
			napis = napis + znak;
		}
		else{
			break;
		}
	}
	
	ilosc = atoi(napis.c_str());
	
	towar -> ilosc = towar -> ilosc + ilosc;
	
	while(true){
		znak = (char)fgetc(file);
	
		if(znak == '\n'){
			break;
		}
		else if(znak == EOF){
			CzyKoniec = true;
			break;
		}
	}
}

void OdczytajTowar(KATEGORIA * kategoria){
	char znak;
	string napis;
	TOWAR * towar;
	int dlugosc = 0;
	
	while(true){
		znak = (char)fgetc(file);
	
		if(znak != ':'){
			napis = napis + znak;
			dlugosc++;
		}
		else{
			break;
		}
	}
	
	if(dlugosc > DlugoscTowaru){
		DlugoscTowaru = dlugosc;
	}
	
	towar = CzyTowarIstnieje(kategoria, napis);
	
	if(towar == NULL){
		StworzTowar(kategoria, napis);
	}
	else{
		DodajDoTowaru(towar);
	}
}

void OdczytajKategorie(){
	char znak;
	string napis;
	KATEGORIA * kat;
	int dlugosc = 0;
	
	while(true){
		znak = (char)fgetc(file);
	
		if(znak != ':'){
			napis = napis + znak;
			dlugosc++;
		}
		else{
			break;
		}
	}
	
	if(dlugosc > DlugoscKategorii){
		DlugoscKategorii = dlugosc;
	}
	
	if(!Inicjalizacja){
		kat = CzyKategoriaIstnieje(napis);
		
		if(kat == NULL){
			kat = StworzKategorie(napis);
		}
	}
	else{
		kat = first;
		kat -> nazwaKategorii = napis;	
		kat -> pierwszyTowar = NULL;
		kat -> ostatniTowar = NULL;
		Inicjalizacja = false;
	}
	
	OdczytajTowar(kat);	
}

////////////////////////WYPISYWANIE////////////////////////////
TOWAR * WypiszTowar(TOWAR * towar){	
	string iloscBuff;
	iloscBuff = to_string(towar -> ilosc) + " " + towar -> jednostka;
	
	cout.width(DlugoscTowaru);
	cout << right << towar -> nazwaTowaru << "|";
	
	cout.width(DlugoscIlosci);
	cout << right << iloscBuff << "|";
//	cout << right << towar -> ilosc << " ";
//	cout << right << towar -> jednostka << "|";
	
	printf("%.2f", towar -> cenaJednostkowa * towar -> ilosc);
	
	return towar -> next;
}

KATEGORIA * WypiszKategorie(KATEGORIA * kategoria){
	TOWAR * towar;
	towar = kategoria -> pierwszyTowar;

	while(towar != NULL){
		cout.width(DlugoscKategorii);
		cout << right << kategoria -> nazwaKategorii << "|";
		towar = WypiszTowar(towar);
		if(towar != NULL){
			cout << endl;
		}
	}
	
	return kategoria -> next;
}

KATEGORIA * WypiszIloscKategorii(KATEGORIA * kategoria, int *suma){
	cout << kategoria -> nazwaKategorii << ": " << kategoria -> liczba;
	*suma = *suma + kategoria -> liczba;
	
	return kategoria -> next;
}

void Welcome(){
	cout << "Co chcesz zrobic?" << endl;
	cout << "(w) Wystwietlic liste," << endl;
	cout << "(p) Wystwietlic podsumowanie," << endl;
	cout << "(d) Dodac cos do listy" << endl;
	cout << "(q) Wyjsc" << endl;
}

void DopiszDoPliku(KATEGORIA * kategoria, TOWAR * towar){
	fputc('\n', file);
	fputs(kategoria -> nazwaKategorii.c_str(), file);
	
	fputc(':', file);
	fputs(towar -> nazwaTowaru.c_str(), file);
	
	fputc(':', file);
	fprintf(file, "%i", towar -> ilosc);
	fputc(' ', file);
	fputs(towar -> jednostka.c_str(), file);
	
	fputc(':', file);
	fprintf(file, "%.2f", towar -> cenaJednostkowa);
}

void Dopisz(){
	string nazwaKat;
	string nazwaTow;
	int ile;
	float cena;
	char opcja;
	string jednostka;
	KATEGORIA * kat;
	TOWAR * tow;
	
	cout << "Podaj kategorie: ";
	cin >> nazwaKat;
	
	if(nazwaKat.size() > DlugoscKategorii){
		DlugoscKategorii = nazwaKat.size();
	}
	
	kat = CzyKategoriaIstnieje(nazwaKat);
		
	if(kat == NULL){
		kat = StworzKategorie(nazwaKat);
	}
	
	cout << "Podaj towar: ";
	cin >> nazwaTow;
	
	if(nazwaTow.size() > DlugoscKategorii){
		DlugoscKategorii = nazwaTow.size();
	}
	
	tow = CzyTowarIstnieje(kat, nazwaTow);
	
	cout << "Podaj ile sztuk: ";
	cin >> ile;
	
	if(tow == NULL){
		StworzTowar(kat, nazwaTow, true, &tow);

		cout << "Jednostka standartowa 'sztuk' (t), wlasna (n): ";
		cin >> opcja;
		
		if(opcja == 't'){
			jednostka = "sztuk";
		}
		else{
			cin >> jednostka;
		}
		
		tow -> jednostka = jednostka;
		
		cout << "Jaka cena:";
		cin >> cena;
		
		cout << "Cena jednostkowa (j) czy calkowita (c): ";
		cin >> opcja;
		
		if(opcja == 'j'){
			tow -> cenaJednostkowa = cena;
		}else{
			tow -> cenaJednostkowa = cena / (float)ile;
		}
		tow -> ilosc = ile;
	}
	else{
		tow -> ilosc = tow -> ilosc + ile;
	}

	cout << kat -> nazwaKategorii << "|" << tow -> nazwaTowaru << "|" << tow -> ilosc << " " << tow -> jednostka << "|" << tow -> ilosc * tow -> cenaJednostkowa << endl << endl;
	
	DopiszDoPliku(kat, tow);
}

int main(int argc, char ** argv){
	
	file = fopen(argv[1], "r+");
	
  	if (file == NULL){
		perror("Nie moge otworzyc pliku z baza");
		return -2;
	}
	
	KATEGORIA * kategoria = new KATEGORIA;
	kategoria -> next = NULL;
	kategoria -> prev = NULL;
	kategoria -> pierwszyTowar = NULL; 
	kategoria -> liczba = 0;
	
	first = kategoria;
	last = kategoria;
	int suma = 0;
	char opcja;
		
	while(!CzyKoniec){
		OdczytajKategorie();
	}
	
	Welcome();
	cin >> opcja;
	
	while(opcja != 'q'){
		if(opcja == 'w'){
			kategoria = first;
	
			while(kategoria != NULL){
				kategoria = WypiszKategorie(kategoria);
				cout << endl;
			}
		}
		else if(opcja == 'p'){
			kategoria = first;
	
			while(kategoria != NULL){
				kategoria = WypiszIloscKategorii(kategoria, &suma);
				cout << endl;
			}
	
			cout << endl << "Lacza suma kategorii to: " << suma << endl;
		}
		else if(opcja == 'd'){
			Dopisz();
		}
		
		cin >> opcja;
	}
	
	
	fclose(file);
	return 0;
}
