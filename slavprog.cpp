#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>
#include <ctime>
#include "drager.hpp"
#include "koppelvlak.hpp"
#include "splitter.hpp"
#include "markwiering.h"
//#include "splitter.h"
#include "stringoperaties.h"
using namespace std;

typedef unsigned short int kint;
typedef long long lint;
bool ontbeest = 1;
#define log(x) if(ontbeest) { cout << x << endl; }

struct Paar
{
	Paar(string lijn, char scheidingsteken)
	{
		int scheidingsplek = 0;
		loop(lijn)
		{
			if(lijn[i] != scheidingsteken) { a += lijn[i]; }
			else { scheidingsplek = i+1; break; }
		}
		for(int i = scheidingsplek; i < lijn.size(); i++) { b += lijn[i]; }
	}
	
	string a, b;
};

Koppelvlak<string, string> chertogi;
void gegevensVoobereiden()
{
	string lijn = "";
	ifstream bestand("Chertogi.txt");
	if(bestand.is_open())
	{
		while(getline(bestand, lijn))
		{
			if(LegeLijn(lijn)) { continue; }
			Paar paar(lijn, '#');
			chertogi.druk(paar.a, paar.b);
		}
		bestand.close();
	}
	else { cerr << "Kon Chertogi.txt niet vinden!" << endl; }
}

int aantalMaanddagen(int maand, int jaar = 2001)
{
	int dagen = 31;
	if(maand == 4 || maand == 6 || maand == 9 || maand == 11) { dagen = 30; }
	else if(maand == 2)
	{
		dagen = 28;
		if((jaar % 4 == 0 && jaar % 100 != 0) || jaar % 400 == 0) { dagen++; }
	}
	return dagen;
}

string DagNaam(int bladwijzer)
{
	string maanden[7] = {"Понедѣльникъ", "Вторникъ", "Среда", "Четвергъ", "Пятница", "Суббота", "Воскресенье"}; //Ѣ ѣ
	if(bladwijzer < 1 || bladwijzer > 7) { return "неизвестен"; }
	return maanden[bladwijzer-1];
}

string MaandNaam(int bladwijzer)
{
	string maanden[12] = {"Январь", "Февраль", "Мартъ", "Апрѣль", "Май", "Инюнь", "Июль", "Августъ", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"}; //Ѣ ѣ
	if(bladwijzer < 1 || bladwijzer > 12) { return "неизвестен"; }
	return maanden[bladwijzer-1];
}

struct Datum
{
	Datum(int adag = 1, int amaand = 1, int ajaar = 2000, int auur = 0, int aminuten = 0)
	{
		dag = adag;
		maand = amaand;
		jaar = ajaar;
		uur = auur;
		minuten = aminuten;
	}
	Datum(string datum) // D-M-J-UUR-MINUTEN
	{
		Drager<string> lijst = split(datum, '-', ' ');
		if(lijst.grootte() < 5) { cerr << "FOUT: jij hebt de datum in het verkeerde formaat opgegeven! Correct: 07-08-1997 14-23 of 7 8 1997 14 23\n\n"; return; }
		
		dag = stoi64(lijst[0]), maand = stoi(lijst[1]), jaar = stoi(lijst[2]), uur = stoi(lijst[3]), minuten = stoi(lijst[4]);
		if(!v()) { cerr << "FOUT: ongeldigde datum opgegeven!\nDag: " << dag << "\nMaand: " << maand << "\nJaar: " << jaar << endl; }
	}
	
	bool v() // validator
	{
		if(maand < 1 || maand > 12) { return false; }
		if(dag > aantalMaanddagen(maand, jaar)) { return false; }
		
		weekdag = 4;
		int dagenToevoegen = 0;
		Datum houvast(21, 9, 2023, uur, minuten);
		if(*this > houvast) { while(*this > houvast) { ++houvast; dagenToevoegen++; } }
		else if(*this < houvast) { while(*this < houvast) { --houvast; dagenToevoegen--; } }
		
		weekdag += (dagenToevoegen % 7);
		if(weekdag > 7) { weekdag -= 7; }
		else if(weekdag < 1) { weekdag += 7; }
		
		return true;
	}
	int getalwaarde()
	{
		int resultaat = 0;
		string DATUM = naarString(jaar) + naarString(maand) + naarString(dag);
		loop(DATUM) { resultaat += int(DATUM[i] - 48); }
		while(resultaat > 9)
		{
			string RES = naarString(resultaat);
			resultaat = 0;
			loop(RES) { resultaat += int(RES[i] - 48); }
		}
		return resultaat;
	}
	
	int operator-(Datum c) // v - c, huidig - geboorte
	{
		if(c > *this) { return 0; }
		int aantalDagen = 0;
		while(c < *this) { ++c; aantalDagen++; }
		return aantalDagen;
	}
	
	Datum operator=(string datum)
	{
		Drager<string> lijst = split(datum, '-', ' ');
		if(lijst.grootte() < 5) { cerr << "FOUT: jij hebt de datum in het verkeerde formaat opgegeven! Correct: 07-08-1997 14 38 of 7 8 1997 14 38\n\n"; return *this; }
		
		dag = stoi64(lijst[0]), maand = stoi(lijst[1]), jaar = stoi(lijst[2]), uur = stoi(lijst[3]), minuten = stoi(lijst[4]);
		if(!v()) { cerr << "FOUT: ongeldigde datum opgegeven!\nDag: " << dag << "\nMaand: " << maand << "\nJaar: " << jaar << endl; }
		
		return *this;
	}
	
	Datum operator++()
	{
		dag++;
		if(dag > aantalMaanddagen(maand, jaar)) { dag = 1; maand++; }
		if(maand > 12) { maand = 1; jaar++; }
		return *this;
	}
	Datum operator--()
	{
		dag--;
		if(dag < 1 && maand > 1) { maand--; dag = aantalMaanddagen(maand, jaar); }
		else if(dag < 1 && maand == 1) { maand = 12; jaar--; dag = 31; }
		return *this;
	}
	
	bool operator==(Datum c) { return jaar == c.jaar && maand == c.maand && dag == c.dag; }
	bool operator!=(Datum c) { return !(jaar == c.jaar && maand == c.maand && dag == c.dag); }
	bool operator>(Datum c) { return jaar > c.jaar || (jaar == c.jaar && maand > c.maand) || (jaar == c.jaar && maand == c.maand && dag > c.dag); }
	bool operator<(Datum c) { return jaar < c.jaar || (jaar == c.jaar && maand < c.maand) || (jaar == c.jaar && maand == c.maand && dag < c.dag); }
	bool operator>=(Datum c) { return jaar > c.jaar || (jaar == c.jaar && maand > c.maand) || (jaar == c.jaar && maand == c.maand && dag >= c.dag); }
	bool operator<=(Datum c) { return jaar < c.jaar || (jaar == c.jaar && maand < c.maand) || (jaar == c.jaar && maand == c.maand && dag <= c.dag); }
	
	int jaar = 2000, maand = 1, dag = 1, weekdag = 4, uur = 0, minuten = 0;
	bool na1800 = false;
};

std::ostream &operator<<(std::ostream &uitvoer, Datum &c)
{
	//uitvoer << c.dag << "-" << c.maand << "-" << c.jaar << " # weekdag " << c.weekdag;
	uitvoer << c.dag << " " << c.maand << " " << c.jaar << " " << c.uur << " " << c.minuten;
	return uitvoer;
}

int aantalSMaanddagen(int maand, int jaar)
{
	int dagen = 41;
	if(jaar % 16 != 0 && maand % 2 == 0) { dagen = 40; }
	return dagen;
}

string SDagNaam(int bladwijzer)
{
	string maanden[9] = {"Понедѣльникъ", "Вторникъ", "Тритейникъ", "Четвергъ", "Пятница", "Шестница", "Седмица", "Осьмица", "Недѣля"}; //Ѣ ѣ
	if(bladwijzer < 1 || bladwijzer > 9) { return "неизвестен"; }
	return maanden[bladwijzer-1];
}

string SMaandNaamOmschrijving(int bladwijzer)
{
	string maanden[9] = {"Божественного начала", "Новых Даров", "Белого Сияния и Покоя Мира", "Вьюг и Стужи", "Пробуждения Природы", "Посева и Нарачения", "Ветров", "Получение Даров Природы", "Завершения"}; //Ѣ ѣ
	if(bladwijzer < 1 || bladwijzer > 9) { return "неизвестен"; }
	return "месяц " + maanden[bladwijzer-1];
}

string SMaandNaam(int bladwijzer)
{
	string maanden[9] = {"Рамхатъ", "Айлѣтъ", "Бейлѣтъ", "Гэйлѣтъ", "Дайлѣтъ", "Элѣтъ", "Вэйлѣтъ", "Хэйлѣтъ", "Тайлѣтъ"}; //Ѣ ѣ
	if(bladwijzer < 1 || bladwijzer > 9) { return "неизвестен"; }
	return maanden[bladwijzer-1];
}

// Лѣто 7532 от сотворения мира в звёздном храме
// Лѣто 13032 от Великого Похолодания
// Разница: 5500

struct SlavTijd
{
	SlavTijd(int aseconden = 0)
	{
		if(aseconden >= 64800) { halveseconden = 2*(aseconden - 64800); }
		else { halveseconden = 2*(aseconden + 21600); }
		v();
	}
	SlavTijd(int aseconden, int aminuten, int auren)
	{
		int t = auren*3600 + aminuten*60 + aseconden;
		if(t >= 64800) { halveseconden = 2*(t - 64800); }
		else { halveseconden = 2*(t + 21600); }
		v();
	}
	
	void w(int aseconden, int aminuten, int auren)
	{
		int t = auren*3600 + aminuten*60 + aseconden;
		if(t >= 64800) { halveseconden = 2*(t - 64800); }
		else { halveseconden = 2*(t + 21600); }
		v();
	}
	
	void w(int aseconden)
	{
		if(aseconden >= 64800) { halveseconden = 2*(aseconden - 64800); }
		else { halveseconden = 2*(aseconden + 21600); }
		v();
	}
	
	Drager<int> casti(int aminuten, int auren)
	{
		Drager<int> mogelijkheden;
		int t = 0;
		for(int i = 0; i < 3; i++)
		{
			t = auren*3600 + aminuten*60 + i*20;
			auren >= 18 ? halveseconden = 2*(t - 64800) : halveseconden = 2*(t + 21600); v();
			mogelijkheden.uniekDrukken(cast);	
		}
		
		t += 19;
		auren >= 18 ? halveseconden = 2*(t - 64800) : halveseconden = 2*(t + 21600); v();
		mogelijkheden.uniekDrukken(cast);
		return mogelijkheden;
	}
	
	void v()
	{
		godina = halveseconden / 10800;
		halveseconden -= godina * 10800;
		cast = halveseconden / 75;
	}
	
	int halveseconden = 0, cast = 0, godina = 0;
};

std::ostream &operator<<(std::ostream &uitvoer, SlavTijd &c)
{
	uitvoer << c.godina << ":" << c.cast;
	return uitvoer;
}

struct SDatum
{
	SDatum(int adag = 1, int amaand = 1, int ajaar = 7532, int aweekdag = 2, int akl144 = 12) // van de 9
	{
		dag = adag;
		maand = amaand;
		jaar = ajaar;
		weekdag = aweekdag;
		kl144 = akl144;
	}
	SDatum(string datum) // D-M-J
	{
		Drager<string> lijst = split(datum, '-', ' ');
		if(lijst.grootte() < 3) { cerr << "FOUT: jij hebt de datum in het verkeerde formaat opgegeven! Correct: 07-08-1997 of 7 8 1997\n\n"; return; }
		
		dag = stoi64(lijst[0]), maand = stoi(lijst[1]), jaar = stoi(lijst[2]);
		if(!v()) { cerr << "FOUT: ongeldigde datum opgegeven!\nDag: " << dag << "\nMaand: " << maand << "\nJaar: " << jaar << endl; }
	}
	bool v() // validator
	{
		if(maand < 1 || maand > 9) { return false; }
		if(dag > aantalSMaanddagen(maand, jaar)) { return false; }
		
		weekdag = 2;
		int dagenToevoegen = 0;
		SDatum opor(1,1,7532, 2, 12); //donderdag 21 september 2023
		if(*this > opor) { while(*this > opor) { ++opor; dagenToevoegen++; } }
		else if(*this < opor) { while(*this < opor) { --opor; dagenToevoegen--; } }
		
		weekdag += (dagenToevoegen % 9);
		if(weekdag > 9) { weekdag -= 9; }
		else if(weekdag < 1) { weekdag += 9; }
		
		return true;
	}
	
	int getalwaarde()
	{
		int resultaat = 0;
		string DATUM = naarString(jaar) + naarString(maand) + naarString(dag);
		loop(DATUM) { resultaat += int(DATUM[i] - 48); }
		while(resultaat > 9)
		{
			string RES = naarString(resultaat);
			resultaat = 0;
			loop(RES) { resultaat += int(RES[i] - 48); }
		}
		return resultaat;
	}
	
	int operator-(SDatum c) // v - c, huidig - geboorte
	{
		if(c > *this) { return 0; }
		int aantalDagen = 0;
		while(c < *this) { ++c; aantalDagen++; }
		return aantalDagen;
	}
	
	SDatum operator=(SDatum c) { dag = c.dag; maand = c.maand; jaar = c.jaar; return *this; }
	SDatum operator=(string datum)
	{
		Drager<string> lijst = split(datum, '-', ' ');
		if(lijst.grootte() < 3) { cerr << "FOUT: jij hebt de datum in het verkeerde formaat opgegeven! Correct: 07-08-1997 of 7 8 1997\n\n"; return *this; }
		
		dag = stoi64(lijst[0]), maand = stoi(lijst[1]), jaar = stoi(lijst[2]);
		if(!v()) { cerr << "FOUT: ongeldigde datum opgegeven!\nDag: " << dag << "\nMaand: " << maand << "\nJaar: " << jaar << endl; }
		
		return *this;
	}
	
	SDatum operator++()
	{
		dag++; weekdag++;
		if(dag > aantalSMaanddagen(maand, jaar)) { dag = 1; maand++; }
		if(maand > 9) { maand = 1; jaar++; kl144++; }
		if(weekdag > 9){ weekdag = 1; }
		if(kl144 > 144){ kl144 = 1; }
		return *this;
	}
	SDatum operator--()
	{
		dag--; weekdag--;
		if(dag < 1 && maand > 1) { maand--; dag = aantalSMaanddagen(maand, jaar); }
		else if(dag < 1 && maand == 1) { maand = 9; jaar--; kl144--; dag = 41; }
		if(weekdag < 1){ weekdag = 9; }
		if(kl144 < 1){ kl144 = 144; }
		return *this;
	}
	
	Datum geefGregoriaanseDatum()
	{
		weekdag = 2;
		int dagenToevoegen = 0;
		SDatum opor(1,1,7532, 2); //donderdag 21 september 2023, 12/144 по круголету
		Datum houvast(21, 9, 2023);
		if(*this > opor) { while(*this > opor) { ++opor; dagenToevoegen++; } }
		else if(*this < opor) { while(*this < opor) { --opor; dagenToevoegen--; } }
		else if(*this == opor) { return houvast; }
		
		weekdag += (dagenToevoegen % 9);
		if(weekdag > 9) { weekdag -= 9; }
		else if(weekdag < 1) { weekdag += 9; }
		
		if(dagenToevoegen > 0) { herhaal(dagenToevoegen) { ++houvast; } }
		else if(dagenToevoegen < 0) { herhaal(-1*dagenToevoegen) { --houvast; } }
		return houvast;
	}
	
	bool operator==(SDatum c) { return jaar == c.jaar && maand == c.maand && dag == c.dag; }
	bool operator!=(SDatum c) { return !(jaar == c.jaar && maand == c.maand && dag == c.dag); }
	bool operator>(SDatum c) { return jaar > c.jaar || (jaar == c.jaar && maand > c.maand) || (jaar == c.jaar && maand == c.maand && dag > c.dag); }
	bool operator<(SDatum c) { return jaar < c.jaar || (jaar == c.jaar && maand < c.maand) || (jaar == c.jaar && maand == c.maand && dag < c.dag); }
	bool operator>=(SDatum c) { return jaar > c.jaar || (jaar == c.jaar && maand > c.maand) || (jaar == c.jaar && maand == c.maand && dag >= c.dag); }
	bool operator<=(SDatum c) { return jaar < c.jaar || (jaar == c.jaar && maand < c.maand) || (jaar == c.jaar && maand == c.maand && dag <= c.dag); }
	
	Drager<int> casti;
	SlavTijd stijd; // sec, min, uur
	int jaar = 7532, maand = 1, dag = 1, weekdag = 2, kl144 = 12, godina = 0; //van de 9
};

std::ostream &operator<<(std::ostream &uitvoer, SDatum &c)
{
	//uitvoer << c.dag << "-" << c.maand << "-" << c.jaar << " # weekdag " << c.weekdag;
	uitvoer << SDagNaam(c.weekdag) << " " << c.dag << " " << SMaandNaam(c.maand) << " (" << SMaandNaamOmschrijving(c.maand) << ") " << c.jaar << " от сотворения мира в звёздном храме.\nКруголета: " << c.kl144;
	return uitvoer;
}

SDatum geefSDatum(Datum c)
{
	c.weekdag = 4;
	int dagenToevoegen = 0;
	SDatum opor(1,1,7532, 2); //donderdag 21 september 2023
	Datum houvast(21, 9, 2023);
	if(c > houvast) { while(c > houvast) { ++houvast; dagenToevoegen++; } }
	else if(c < houvast) { while(c < houvast) { --houvast; dagenToevoegen--; } }
	else if(c == houvast && !c.na1800) { return opor; }
	
	if(c.na1800 || c.uur >= 18) { dagenToevoegen++; }
	
	c.weekdag += (dagenToevoegen % 7);
	if(c.weekdag > 7) { c.weekdag -= 7; }
	else if(c.weekdag < 1) { c.weekdag += 7; }
	
	if(dagenToevoegen > 0) { herhaal(dagenToevoegen) { ++opor; } }
	else if(dagenToevoegen < 0) { herhaal(-1*dagenToevoegen) { --opor; } }

	opor.stijd.w(0, c.minuten, c.uur);
	opor.godina = opor.stijd.godina;
	opor.casti = opor.stijd.casti(c.minuten, c.uur);
	
	return opor;
}

int karakterChertogi(int maand){ while(maand > 0) { maand -= 10; } maand += 10; return maand; }
Drager<string> kakoiChertog(SDatum d)
{
	Drager<string> lijst;
	int j = d.jaar;
	
	if(d == SDatum(1, 1, j)) { lijst += "Девы и Вепра"; }
	else if(d == SDatum(4, 2, j)) { lijst += "Щуки и Лебедя"; }
	else if(d == SDatum(7, 3, j)) { lijst += "Змея и Ворона"; }
	else if(d == SDatum(12, 4, j)) { lijst += "Медведя и Бусла"; }
	else if(d == SDatum(22, 5, j)) { lijst += "Волка и Лисы"; }
	else if(d == SDatum(26, 6, j)) { lijst += "Тура и Лося"; }
	else if(d == SDatum(31, 7, j)) { lijst += "Финиста и Коня"; }
	else if(d == SDatum(35, 8, j)) { lijst += "Орла и Раса"; }
	
	if(d >= SDatum(18, 9, j) && d <= SDatum(1, 1, j)) { lijst += "Девы"; }
	if(d >= SDatum(1, 1, j) && d <= SDatum(22, 1, j)) { lijst += "Вепра"; }
	if(d >= SDatum(22, 1, j) && d <= SDatum(4, 2, j)) { lijst += "Щуки"; }
	if(d >= SDatum(4, 2, j) && d <= SDatum(25, 2, j)) { lijst += "Лебедя"; }
	if(d >= SDatum(25, 2, j) && d <= SDatum(7, 3, j)) { lijst += "Змея"; }
	if(d >= SDatum(7, 3, j) && d <= SDatum(29, 3, j)) { lijst += "Ворона"; }
	if(d >= SDatum(29, 3, j) && d <= SDatum(12, 4, j)) { lijst += "Медведя"; }
	if(d >= SDatum(12, 4, j) && d <= SDatum(37, 4, j)) { lijst += "Бусла"; }
	if(d >= SDatum(37, 4, j) && d <= SDatum(22, 5, j)) { lijst += "Волка"; }
	if(d >= SDatum(22, 5, j) && d <= SDatum(4, 6, j)) { lijst += "Лисы"; }
	if(d >= SDatum(4, 6, j) && d <= SDatum(26, 6, j)) { lijst += "Тура"; }
	if(d >= SDatum(26, 6, j) && d <= SDatum(9, 7, j)) { lijst += "Лося"; }
	if(d >= SDatum(9, 7, j) && d <= SDatum(31, 7, j)) { lijst += "Финиста"; }
	if(d >= SDatum(31, 7, j) && d <= SDatum(13, 8, j)) { lijst += "Коня"; }
	if(d >= SDatum(13, 8, j) && d <= SDatum(35, 8, j)) { lijst += "Орла"; }
	if(d >= SDatum(35, 8, j) && d <= SDatum(18, 9, j)) { lijst += "Раса"; }
	return lijst;
}
string welkVierdeel(int dag)
{
	if(dag >= 1 && dag <= 10) { return "_1_10"; }
	if(dag >= 11 && dag <= 20) { return "_11_20"; }
	if(dag >= 21 && dag <= 30) { return "_21_30"; }
	if(dag >= 31 && dag <= 40) { return "_31_40"; }
	if(dag == 41) { return "_41"; }
	return "Onbekend";
}

string opschonen(string tekst)
{
	string nw = "";
	loop(tekst) { if(!(tekst[i] >= 32 && tekst[i] <= 127)) { nw += tekst[i]; } }
	return nw;
}

int naamWaarde(string tekst)
{
	int r = 0;
	string t = "";
	for(unsigned int i = 0; i < tekst.size(); i += 2)
	{
		t += tekst[i];
		t += tekst[i+1];
		
		if(t == "а" || t == "и" || t == "й" || t == "і" || t == "ї" || t == "ы" || t == "ѵ") { r += 1; }
		else if(t == "б" || t == "ж" || t == "к" || t == "р" || t == "л" || t == "ш" || t == "я" || t == "ѧ" || t == "ѩ") { r += 2; }
		else if(t == "г" || t == "с" || t == "ц") { r += 3; }
		else if(t == "д" || t == "м" || t == "т") { r += 4; }
		else if(t == "є" || t == "е" || t == "н" || t == "х" || t == "ѯ") { r += 5; }
		else if(t == "в" || t == "у" || t == "э" || t == "ѣ") { r += 6; }
		else if(t == "з" || t == "ѕ" || t == "о" || t == "ѡ" || t == "ч" || t == "ю") { r += 7; }
		else if(t == "п" || t == "ф" || t == "ѳ" || t == "ё") { r += 8; }
		else if(t == "щ" || t == "ъ" || t == "ь") { r += 9; }
		
		t = "";
	}
	
	while(r > 9)
	{
		string RES = naarString(r);
		r = 0;
		loop(RES) { r += int(RES[i] - 48); }
	}
	return r;
}

struct Persoon
{
	Persoon(string anaam, Datum datum) { naam = anaam; geboortedatum = datum; v(); }
	Persoon(string anaam, string datum) { naam = anaam; geboortedatum = datum; v(); }
	Persoon(string regel)
	{
		Drager<string> lijst = split(regel, ';', ';');
		if(lijst.grootte() < 2) { cerr << "Er gaat iets mis!" << endl; }
		naam = lijst[0]; geboortedatum = lijst[1]; v();
	}
	Persoon() { naam = ""; geboortedatum = "1 1 2000 0 0"; v(); }
	string toon() { return naam + ", " + naarString(geboortedatum.dag) + "-" + naarString(geboortedatum.maand) + "-" + naarString(geboortedatum.jaar); }
	void v()
	{
		string nw = kl(opschonen(naam));
		naamwaarde = naamWaarde(nw);
	}
	
	string naam = "";
	Datum geboortedatum;
	int naamwaarde = 0;
};

std::ostream &operator<<(std::ostream &uitvoer, Persoon &c)
{
	uitvoer << c.naam << ";" << c.geboortedatum;
	return uitvoer;
}

int dagwaarde(int a, int b, int c)
{
	int r = a + b + c;
	while(r > 9)
	{
		string RES = naarString(r);
		r = 0;
		loop(RES) { r += int(RES[i] - 48); }
	}
	return r;
}

struct Wigri
{
	Wigri(Datum d)
	{
		int resultaat = 0;
		string tijd = naarString(d.uur) + naarString(d.minuten);
		loop(tijd) { wigri[int(tijd[i] - 48)-1]++; resultaat += int(tijd[i] - 48); }
		while(resultaat > 9)
		{
			string RES = naarString(resultaat);
			resultaat = 0;
			loop(RES) { resultaat += int(RES[i] - 48); }
		}
		wigri[resultaat-1]++;
		resultaat = 0;
		
		string DATUM = naarString(d.jaar) + naarString(d.maand) + naarString(d.dag);
		loop(DATUM) { wigri[int(DATUM[i] - 48)-1]++; resultaat += int(DATUM[i] - 48); }
		while(resultaat > 9)
		{
			string RES = naarString(resultaat);
			resultaat = 0;
			loop(RES) { wigri[int(RES[i] - 48)-1]++; resultaat += int(RES[i] - 48); }
		}
		wigri[resultaat-1]++;
	}
	int wigri[9] = {0,0,0,0,0,0,0,0,0};
};

std::ostream &operator<<(std::ostream &uitvoer, Wigri &c)
{
	//herhaal(9) { uitvoer << i+1 << ": " << c.wigri[i] << "\n"; }
	uitvoer << "4) " << c.wigri[3] << "\t 9) " << c.wigri[8] << "\t 2) " << c.wigri[1] << "\n"
			<< "3) " << c.wigri[2] << "\t 5) " << c.wigri[4] << "\t 7) " << c.wigri[6] << "\n"
			<< "8) " << c.wigri[7] << "\t 1) " << c.wigri[0] << "\t 6) " << c.wigri[5] << "\n";
	return uitvoer;
}

Datum verkrijgGeboortedatum()
{
	Datum datum;
	int geldigeDagen = 0, voorofna1800 = 0;
	
	cout << "Получение даты по григорианскому календарью\n\nВведите год рождения (1900-2023): ";
	cin >> datum.jaar;
	while(cin.fail()) { VerkeerdeInvoer(datum.jaar, "\nВведите цифру, а не текст: "); }
	cout << "\n1) Январь\t7) Июль\n2) Февраль\t8) Август\n3) Март\t\t9) Сентябрь\n4) Апрель\t10) Октябрь\n5) Май\t\t11) Ноябрь\n6) Инюнь\t12) Декабрь\n\nВведите месяц рождения (1-12): ";
	cin >> datum.maand;
	while(cin.fail() || datum.maand < 1 || datum.maand > 12)
		{ VerkeerdeInvoer(datum.maand, "\nВведите правильный месяц (1-12): "); }
	geldigeDagen = aantalMaanddagen(datum.maand, datum.jaar);
	cout << "Введите день месяца рождения(1-" << geldigeDagen << "): ";
	cin >> datum.dag;
	while(cin.fail() || datum.dag < 1 || datum.dag > geldigeDagen)
		{ VerkeerdeInvoer(datum.dag, "\nВведите правильный день месяца рождения (1-" + naarString(geldigeDagen) + "): "); }
		
	cout << "Введите час рождения (0-23): ";
	cin >> datum.uur;
	while(cin.fail() || datum.uur < 0 || datum.uur > 23)
		{ VerkeerdeInvoer(datum.uur, "\nВведите правильный час рождения (0-23): "); }
		
	cout << "Введите минуту рождения (0-59): ";
	cin >> datum.minuten;
	while(cin.fail() || datum.minuten < 0 || datum.minuten > 59)
		{ VerkeerdeInvoer(datum.minuten, "\nВведите правильную минуту рождения (0-59): "); }
		
	/*cout << "Вы родились после 18:00?\n\n1) Да, после 18:00, от 18:00 до 23:59!\n2) Нет, до 18:00, от 0:00 до 17:59!\n\nВведите выбор (1-2): ";
	cin >> voorofna1800;
	while(cin.fail() || voorofna1800 < 1 || voorofna1800 > 2)
		{ VerkeerdeInvoer(voorofna1800, "\nВведите понятный для программы выбор (1 или 2): "); }*/
	if(voorofna1800 == 1 || datum.uur >= 18) { datum.na1800 = true; }
	return datum;
}

SDatum verkrijgSGeboortedatum()
{
	SDatum datum;
	int geldigeDagen = 0;
	cout << "Получение даты по Коляды Даръ\n\nВведите год рождения (7409-7532): ";
	cin >> datum.jaar;
	while(cin.fail()) { VerkeerdeInvoer(datum.jaar, "\nВведите цифру, а не текст: "); }
	cout << "\n1) Рамхатъ\t4) Гэйлѣтъ\t7) Вэйлѣтъ\n2) Айлѣтъ\t5) Дайлѣтъ\t8) Хэйлѣтъ\n3) Бейлѣтъ\t6) Элѣтъ\t9) Тайлѣтъ\n\nВведите месяц рождения (1-9): ";
	cin >> datum.maand;
	while(cin.fail() || datum.maand < 1 || datum.maand > 9)
		{ VerkeerdeInvoer(datum.maand, "\nВведите правильный месяц (1-9): "); }
	geldigeDagen = aantalSMaanddagen(datum.maand, datum.jaar);
	cout << "Введите день месяца рождения (1-" << geldigeDagen << "): ";
	cin >> datum.dag;
	while(cin.fail() || datum.dag < 1 || datum.dag > geldigeDagen)
		{ VerkeerdeInvoer(datum.dag, "\nВведите правильный день месяца рождения (1-" + naarString(geldigeDagen) + "): "); }
	return datum;
}

Drager<string> chertog(SDatum d)
{
	Drager<string> lijst;
	return lijst;
}

bool geldigGetal(string getal)
{
	for(unsigned int i = 0; i < getal.size(); i++)
		{ if(!(getal[i] >= '0' && getal[i] <= '9')) { return false; } }
	return true;
}

Koppelvlak<string, string> resultaatBioritmes(int aantalDagen)
{
	Koppelvlak<string, string> resultaat;
	int fase[5] = {aantalDagen % 23, aantalDagen % 28, aantalDagen % 33, aantalDagen % 54, aantalDagen % 84};
	int cycli[5] = {23, 28, 33, 54, 84};
	
	herhaal(5)
	{
		if(fase[i] == 0) { resultaat.druk("0", "идёт к позитиву"); }
		else if(fase[i] == cycli[i]/2) { resultaat.druk("0", "идёт вниз"); }
		else if(fase[i] > 0 && fase[i] < cycli[i]/4) { resultaat.druk("+", "поднимается"); }
		else if(fase[i] == cycli[i]/4) { resultaat.druk("+", "пик"); }
		else if(fase[i] > cycli[i]/4 && fase[i] < cycli[i]/2) { resultaat.druk("+", "снижается"); }//
		else if(fase[i] > cycli[i]/2 && fase[i] < 3*cycli[i]/4) { resultaat.druk("-", "снижается"); }
		else if(fase[i] == 3*cycli[i]/4) { resultaat.druk("-", "дно"); }
		else if(fase[i] > 3*cycli[i]/4) { resultaat.druk("-", "поднимается"); }
	}
	
	herhaal(5) { cout << "Цикл " << i+1 << ") [" << cycli[i] << "], день " << fase[i] << "\n"; }
	return resultaat;
}

void SlavischeDatumInvoeren()
{
	SDatum geboortedatum = verkrijgSGeboortedatum();
	Datum datum = geboortedatum.geefGregoriaanseDatum();
	datum.v();
	Datum datumV = datum;
	--datumV;
	datumV.v();
	cout << "\n" << datumV << " от 18:00 до 23:59\n";
	cout << datum << " от 0:00 до 17:59\n";
}

void GregoriaanseDatumInvoeren(Datum huidigedatum)
{
	Datum geboortedatum = verkrijgGeboortedatum();
	SDatum sdatum = geefSDatum(geboortedatum);
	int CZ = geboortedatum.getalwaarde(), OCZ = sdatum.getalwaarde();
	cout << "\n" << sdatum << endl;
	
	Drager<string> lijst = kakoiChertog(sdatum);
	cout << "\n\t< Чертог"; if(lijst.grootte() > 1) { cout << "и"; } cout << " >\n";
	lus(lijst) { cout << lijst[i] << ": " << chertogi[lijst[i]] << "\n"; }
	cout << "\n\t< Подробно >\n" << chertogi["С" + NaarString(sdatum.maand) + welkVierdeel(sdatum.dag)] << endl;
	cout << "\n\t< Круголета >\n" << chertogi["K" + NaarString(sdatum.kl144)] << endl;
	cout << "\n\t< Цифра Жизни >\n" << chertogi["CZ" + NaarString(CZ)] << endl;
	cout << "\n\t< Обереговая Цифра Жизни >\n" << chertogi["OCZ" + NaarString(OCZ)] << endl;
	
	cout << "\n\t< Година рождения: " << sdatum.godina << " >\n" << chertogi["Godina" + NaarString(sdatum.godina)] << endl;
	cout << "\n\t< Часть рождения >\n";
	lus(sdatum.casti) { cout << sdatum.casti[i] << ": " << chertogi["Cast" + NaarString(sdatum.casti[i])] << "\n"; }
	
	Wigri wig(geboortedatum);
	cout << "\nЖреческий квадрат (вихри)\n" << wig << endl;
	
	int aantalDagen = huidigedatum - geboortedatum;
	
	//Здесь начинаю биологические ритмы вычислять.
	Koppelvlak<string, string> resultaat = resultaatBioritmes(aantalDagen);
	cout << "\nВы живёте уже " << aantalDagen << " дней!\n";
	cout << "\nФизический цикл: " << resultaat.verkrijgA(0) << ", " << resultaat.verkrijgB(0);
	cout << "\nЭмоциональный цикл: " << resultaat.verkrijgA(1) << ", " << resultaat.verkrijgB(1);
	cout << "\nИнтеллектуальный цикл: " << resultaat.verkrijgA(2) << ", " << resultaat.verkrijgB(2);
	cout << "\nПсихокинетический цикл: " << resultaat.verkrijgA(3) << ", " << resultaat.verkrijgB(3);
	cout << "\nДуховно-целительный цикл: " << resultaat.verkrijgA(4) << ", " << resultaat.verkrijgB(4);
	cout << "\n\n";
}

void AllesUitrekenen(Persoon u)
{
	time_t tqw = std::time(0);   // get time now
	tm* nu = std::localtime(&tqw);
	Datum huidigedatum(nu->tm_mday, nu->tm_mon + 1, nu->tm_year + 1900);
	
	Datum geboortedatum = u.geboortedatum;
	SDatum sdatum = geefSDatum(geboortedatum);
	int CZ = geboortedatum.getalwaarde(), OCZ = sdatum.getalwaarde();
	cout << "\n < Дата рождения >\n" << sdatum << endl;
	
	int dw = dagwaarde(geboortedatum.getalwaarde(), u.naamwaarde, huidigedatum.getalwaarde());
	Drager<string> lijst = kakoiChertog(sdatum);
	cout << "\n\t< Чертог"; if(lijst.grootte() > 1) { cout << "и"; } cout << " >\n";
	lus(lijst) { cout << lijst[i] << ": " << chertogi[lijst[i]] << "\n"; }
	cout << "\n\t< Подробно >\n" << chertogi["С" + NaarString(sdatum.maand) + welkVierdeel(sdatum.dag)] << endl;
	cout << "\n\t< День четверти >\n" << chertogi["D" + NaarString(sdatum.dag % 10)] << endl;
	cout << "\n\t< Круголета >\n" << chertogi["K" + NaarString(sdatum.kl144)] << endl;
	cout << "\n\t< Цифра Жизни >\n" << chertogi["CZ" + NaarString(CZ)] << endl;
	cout << "\n\t< Обереговая Цифра Жизни >\n" << chertogi["OCZ" + NaarString(OCZ)] << endl;
	cout << "\nЧисло Ваша имя: " << chertogi["CI" + NaarString(u.naamwaarde)] << endl;
	cout << "Качество дня: " << chertogi["CD" + NaarString(dw)] << endl;
	
	cout << "\n\t< Година рождения: " << sdatum.godina << " >\n" << chertogi["Godina" + NaarString(sdatum.godina)] << endl;
	cout << "\n\t< Часть рождения >\n";
	lus(sdatum.casti) { cout << sdatum.casti[i] << ": " << chertogi["Cast" + NaarString(sdatum.casti[i])] << "\n"; }
	
	Wigri wig(geboortedatum);
	cout << "\nЖреческий квадрат (вихри)\n" << wig << endl;
	
	int aantalDagen = huidigedatum - geboortedatum;
	
	//Здесь начинаю биологические ритмы вычислять.
	Koppelvlak<string, string> resultaat = resultaatBioritmes(aantalDagen);
	cout << "\nВы живёте уже " << aantalDagen << " дней!\n";
	cout << "\nФизический цикл: " << resultaat.verkrijgA(0) << ", " << resultaat.verkrijgB(0);
	cout << "\nЭмоциональный цикл: " << resultaat.verkrijgA(1) << ", " << resultaat.verkrijgB(1);
	cout << "\nИнтеллектуальный цикл: " << resultaat.verkrijgA(2) << ", " << resultaat.verkrijgB(2);
	cout << "\nПсихокинетический цикл: " << resultaat.verkrijgA(3) << ", " << resultaat.verkrijgB(3);
	cout << "\nДуховно-целительный цикл: " << resultaat.verkrijgA(4) << ", " << resultaat.verkrijgB(4);
	cout << "\n\n";
	cin.ignore();
	cin.get();
}

Persoon PersoonToevoegen()
{
	string lijn = "", naam = "";
	cout << "Введите Вашу имю (ОБЯЗАТЕЛЬНО КИРИЛЛИЦЕЙ!!!): ";
	getline(cin, naam);
	//naam = geefTekst(naam);
	Datum geboortedatum = verkrijgGeboortedatum();
	return Persoon(naam, geboortedatum);
}

void Hoofdmenu()
{
	HOOFDMENU:
	time_t tqw = std::time(0);   // get time now
	tm* nu = std::localtime(&tqw);
	Datum huidigedatum(nu->tm_mday, nu->tm_mon + 1, nu->tm_year + 1900);
	if(nu->tm_hour >= 18) { huidigedatum.na1800 = true; }
	SDatum slavdat = geefSDatum(huidigedatum);
	SlavTijd t(nu->tm_sec, nu->tm_min, nu->tm_hour);
	cout << "\n\tЧасы " << t << "\n\n" << slavdat << " из 144\n";
	cout << "\n\t< Круголета >\n" << chertogi["K" + NaarString(slavdat.kl144)] << "\n\n";
	
	Drager<Persoon> personen;
	string lijn = "";
	ifstream bestand("SDuhi.txt");
	if(bestand.is_open())
	{
		while(getline(bestand, lijn))
			{ if(!LegeLijn(lijn)) { personen += Persoon(lijn); } }
		bestand.close();
	}
	else { cerr << "Kon SDuhi.txt niet vinden!" << endl; }
	
	cout << "\nВычислитель чертоги, жреческий квадрат и биоритмы.\n\n";
	lus(personen) { cout << i+1 << ") " << personen[i].toon() << endl; }
	cout << "\nФ или A) Добавить нового человека! :-D";
	cout << "\nЫ или S) Удалить кого-то... :-(";
	cout << "\nВ или D) Выйти из программы";
	cout << "\n\nВведите свой выбор: ";
	
	string keuze = "";
	getline(cin, keuze);
	keuze = kl(keuze);
	
	if(ZitHetErin(keuze, "ф") || keuze == "a")
		{ personen += PersoonToevoegen(); ofstream sb("SDuhi.txt"); lus(personen) { sb << personen[i] << endl; }
		sb.flush();
		sb.close(); goto HOOFDMENU; }
	else if(ZitHetErin(keuze, "ы") || keuze == "s")
	{
		int verwijderen = 0;
		cout << "Введите номер, чтоб удалить: ";
		cin >> verwijderen;
		if(cin.fail() || verwijderen < 1 || verwijderen > personen.grootte())
			{ cerr << "\nЯ не понял кого нужно удалить. Отменяю действия.\n"; goto HOOFDMENU; }
		
		personen.verwijderen(verwijderen-1);
		ofstream sb("SDuhi.txt");	
		lus(personen) { sb << personen[i] << endl; }
		sb.flush();
		sb.close();
		goto HOOFDMENU;
	cout << "\n\n";
	}
	else if(ZitHetErin(keuze, "в") || keuze == "d") { return; }
	else if(!geldigGetal(keuze)) { goto HOOFDMENU; }
	
	int nkeuze = stoi64(keuze);
	if(nkeuze >= 1 && nkeuze <= personen.grootte()) { AllesUitrekenen(personen[nkeuze-1]); }
	
	goto HOOFDMENU;
}

int main(int argc, char* argv[])
{
	gegevensVoobereiden();
	Hoofdmenu();
	return 0;
}

/*
 * 
	cout << "\n1) Январь\n2) Февраль\n3) Март\n4) Апрель\n5) Май\n6) Инюнь\n7) Июль\n8) Август\n9) Сентябрь\n10) Октябрь\n11) Ноябрь\n12) Декабрь\n\nВведите месяц рождения (1-12): ";
	
	cout << "\n1) Январь\t7) Июль\n2) Февраль\t8) Август\n3) Март\t\t9) Сентябрь\n4) Апрель\t10) Октябрь\n5) Май\t\t11) Ноябрь\n6) Инюнь\t12) Декабрь\n\nВведите месяц рождения (1-12): ";
	
	cout << "\n1) Январь\t5) Май\t\t9) Сентябрь\n2) Февраль\t6) Инюнь\t10) Октябрь\n3) Март\t\t7) Июль\t\t11) Ноябрь\n4) Апрель\t8) Август\t12) Декабрь\n\nВведите месяц рождения (1-12): ";
 * 
 * 
 * Надо вычислить:
 * 1. Число рождения (получилось)
 * 2. Число имени
 * 3. Число нынешнего дня (легко)
 * 4. 
 */
