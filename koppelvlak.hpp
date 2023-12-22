#ifndef KOPPELVLAK_H
#define KOPPELVLAK_H

#define ONGESIGNEERD_LANG_GETAL 429496729
#define BEGINGROOTTE 8
#define BUITEN_REEKS 1
#define NIET_GEVONDEN 2

#include <cstdlib>

void mapFoutmelder(int foutmelding)
{
	if(foutmelding == BUITEN_REEKS)
	{
		std::cerr << "\n\033[1;31mFoutmelding\033[0m: jij hebt geprobeerd om buiten de reeks iets te veranderen!" << std::endl;
		exit(1);
	}
	else if(foutmelding == NIET_GEVONDEN)
	{
		std::cerr << "\n\033[1;31mFoutmelding\033[0m: het element dat jij hebt opgevraagd kon niet worden gevonden!" << std::endl;
		exit(1);
	}
}

template <class A, class B> class Koppelvlak
{
	public:
		Koppelvlak(unsigned long int i = BEGINGROOTTE)
		{
			if(i <= 0) { i = BEGINGROOTTE; }
			else { bufferA = new A[i]; bufferB = new B[i]; wareGrootte = i; }
			sw = 0;
		}
		Koppelvlak(A a, B b)
		{
			sw = 1;
			wareGrootte = BEGINGROOTTE;
			bufferA = new A[wareGrootte];
			bufferB = new B[wareGrootte];
			bufferA[0] = a;
			bufferB[0] = b;
		}
		
		Koppelvlak(const Koppelvlak &c)  // kopieerconstructie 
		{
			//std::cout << "Dit gaat via de geautomatiseerde kopieermachine!" << std::endl;
			sw = c.sw;
			wareGrootte = c.wareGrootte;
			bufferA = new A[wareGrootte];
			bufferB = new B[wareGrootte];
			for(unsigned long int i = 0; i < sw; i++)
				{ bufferA[i] = c.bufferA[i]; bufferB[i] = c.bufferB[i]; }
		}
		~Koppelvlak() { delete[] bufferA; bufferA = NULL; delete[] bufferB; bufferB = NULL; }
		//~Koppelvlak()
		//{
		//	std::cout << "AFSLUITER AANGEROEPEN!!!" <<std::endl;
		//	show_all();
		//}
				
		B &operator[](A c)
		{
			for(unsigned long int i = 0; i < sw; i++)
			{ if(c == bufferA[i]) { return bufferB[i]; } }
			mapFoutmelder(NIET_GEVONDEN);
			//throw std::invalid_argument("Element niet gevonden!");
			return bufferB[0];
		}
		B const &operator[](A c) const
		{
			for(unsigned long int i = 0; i < sw; i++)
			{ if(c == bufferA[i]) { return bufferB[i]; } }
			mapFoutmelder(NIET_GEVONDEN);
			//throw std::invalid_argument("Element niet gevonden!");
			return bufferB[0];
		}
		
		bool operator==(Koppelvlak &v)
		{
			if(this->sw != v.sw) { return false; }
			for(unsigned long int i = 0; i < sw; i++)
			{
				if(this->bufferA[i] != v.bufferA[i]) { return false; }
				if(this->bufferB[i] != v.bufferB[i]) { return false; }
			}
			return true;
		}
		
		Koppelvlak operator=(const Koppelvlak &v)
		{
			sw = v.sw;
			wareGrootte = v.wareGrootte;
			bufferA = new A[wareGrootte];
			bufferB = new B[wareGrootte];
			for(unsigned long int i = 0; i < sw; i++)
			{
				bufferA[i] = v.bufferA[i];
				bufferB[i] = v.bufferB[i];
			}
			return *this;  // geeft een verwijzing naar mijzelf
		}

		bool isGelijkAan(Koppelvlak &v) { return v == *this; }
		void KopieVan(Koppelvlak &v) { *this = v; }
		
		bool uniekDrukkenA(A a, B b)
		{
			for(unsigned long int i = 0; i < sw; i++) { if(a == bufferA[i]) { return false; } }
			druk(a, b); return true;
		}
		
		bool uniekDrukkenB(A a, B b)
		{
			for(unsigned long int i = 0; i < sw; i++) { if(b == bufferB[i]) { return false; } }
			druk(a, b); return true;
		}
		
		bool uniekInvoegenA(A a, B b)
		{
			for(unsigned long int i = 0; i < sw; i++) { if(a == bufferA[i]) { return false; } }
			invoegen(a, b); return true;
		}
		
		bool uniekInvoegenB(A a, B b)
		{
			for(unsigned long int i = 0; i < sw; i++) { if(b == bufferB[i]) { return false; } }
			invoegen(a, b); return true;
		}
		
		void invoegen(A a, B b, unsigned long int p = 0)
		{
			if(sw >= wareGrootte) { verdubbelGrootte(); };
			if(p > sw) { return; }
			for(unsigned long int i = sw; i > p; i--)
			{
				bufferA[i] = bufferA[i-1];
				bufferB[i] = bufferB[i-1];
			}
			bufferA[p] = a;
			bufferB[p] = b;
			sw++;
		}
		
		void druk(A a, B b)
		{
			if(sw >= wareGrootte) { verdubbelGrootte(); }
			bufferA[sw] = a;
			bufferB[sw] = b;
			sw++;
		}
		
		bool ZitHetErinA(A c)
		{ for(unsigned long int i = 0; i < sw; i++) { if(c == bufferA[i]) { return true; } } return false; }
		
		bool ZitHetErinB(B c)
		{ for(unsigned long int i = 0; i < sw; i++) { if(c == bufferB[i]) { return true; } } return false; }
		
		unsigned long int tellenA(A c)
		{
			unsigned long int teller = 0;
			for(unsigned long int i = 0; i < sw; i++) { if(c == bufferA[i]) { teller++; } }
			return teller;
		}
		
		unsigned long int tellenB(B c)
		{
			unsigned long int teller = 0;
			for(unsigned long int i = 0; i < sw; i++) { if(c == bufferB[i]) { teller++; } }
			return teller;
		}
		
		unsigned long int zoekenA(A c)
		{
			for(unsigned long int i = 0; i < sw; i++)
			{ if(c == bufferA[i]) { return i; } }
			return sw;
		}
		
		unsigned long int zoekenB(B c)
		{
			for(unsigned long int i = 0; i < sw; i++)
			{ if(c == bufferB[i]) { return i; } }
			return sw;
		}
		
		A omgekeerdVerkrijgen(B c)
		{
			for(unsigned long int i = 0; i < sw; i++)
			{ if(c == bufferB[i]) { return bufferA[i]; } }
			mapFoutmelder(NIET_GEVONDEN);
			return bufferB[0];
		}
		
		B verkrijg(A c)
		{
			for(unsigned long int i = 0; i < sw; i++)
			{ if(c == bufferA[i]) { return bufferB[i]; } }
			mapFoutmelder(NIET_GEVONDEN);
			return bufferB[0];
		}
		
		void verwijder(unsigned long int i)
		{
			if( i >= sw ) { return; }
			for(unsigned long int j = i+1; j < sw; j++)
			{
				bufferA[j-1] = bufferA[j];
				bufferB[j-1] = bufferB[j];
			}
			sw--;
		}
		
		void verwijderReeks(unsigned long int a, unsigned long int b)
		{
			if(a > b || b >= sw) { return; }
			int reeks = b - a + 1;
			for(unsigned long int j = a+reeks; j < sw; j++)
			{
				bufferA[j-reeks] = bufferA[j];
				bufferB[j-reeks] = bufferB[j];
			}
			sw -= reeks;
		}
		
		unsigned long int vervangAlleA(A a, A b)
		{
			unsigned long int vervangen = 0;
			for(unsigned long int i = sw - 1; i < ONGESIGNEERD_LANG_GETAL; i--)
			{ if(a == bufferA[i]) { bufferA[i] = b; vervangen++; } }
			return vervangen;
		}
		
		unsigned long int vervangAlleB(B a, B b)
		{
			unsigned long int vervangen = 0;
			for(unsigned long int i = sw - 1; i < ONGESIGNEERD_LANG_GETAL; i--)
			{ if(a == bufferB[i]) { bufferB[i] = b; vervangen++; } }
			return vervangen;
		}
		
		unsigned long int verwijderAlleA(A c)
		{
			unsigned long int verwijderd = 0;
			for(unsigned long int i = sw - 1; i < ONGESIGNEERD_LANG_GETAL; i--)
			{ if(c == bufferA[i]) { verwijder(i); verwijderd++; } }
			return verwijderd;
		}
		
		unsigned long int verwijderAlleB(B c)
		{
			unsigned long int verwijderd = 0;
			for(unsigned long int i = sw - 1; i < ONGESIGNEERD_LANG_GETAL; i--)
			{ if(c == bufferB[i]) { verwijder(i); verwijderd++; } }
			return verwijderd;
		}
		
		void wissel(unsigned long int a, unsigned long int b)
		{
			A w = verkrijgA(a); A x = verkrijgA(b);  wijzigA(b, w); wijzigA(a, x);
			B y = verkrijgB(a); B z = verkrijgB(b);  wijzigB(b, y); wijzigB(a, z);
		}
		
		void wisselA(unsigned long int a, unsigned long int b)
		{ A x = verkrijgA(a); A y = verkrijgA(b);  wijzigA(b, x); wijzigA(a, y); }
		
		void wisselB(unsigned long int a, unsigned long int b)
		{ B x = verkrijgB(a); B y = verkrijgB(b);  wijzigB(b, x); wijzigB(a, y); }
		
		bool leeg() { return sw == 0; }
		bool isLeeg() { return sw == 0; }
		void opschonen() { sw = 0; }
		void grondigOpschonen() { sw = 0; bijstellen(); }
		unsigned long int grootte() { return sw; }
		unsigned long int werkelijkeGrootte() { return wareGrootte; }
		
		bool wijzigA(unsigned long int i, A c) { if(i < sw) { bufferA[i] = c; return true;} return false; }
		
		bool wijzigB(unsigned long int i, B c) { if(i < sw) { bufferB[i] = c; return true;} return false; }
		
		bool bijstellen(unsigned long int i = 0)
		{
			if(i == 0) {  }
			if(i >= sw && i != wareGrootte) { afstellen(i); return true; } else { return false; }
		}
		
		A verkrijgA(unsigned long int i)
		{ if(i < sw) { return bufferA[i]; } mapFoutmelder(BUITEN_REEKS); return bufferA[0]; }
		B verkrijgB(unsigned long int i)
		{ if(i < sw) { return bufferB[i]; } mapFoutmelder(BUITEN_REEKS); return bufferB[0]; }
		
		A knalA() { if(sw >= 1) { sw--; return bufferA[sw]; } else { return bufferA[0]; } }
		A topA() { if(sw >= 1) { return bufferA[sw-1]; } else { return bufferA[0]; } }
		
		B knalB() { if(sw >= 1) { sw--; return bufferB[sw]; } else { return bufferB[0]; } }
		B topB() { if(sw >= 1) { return bufferB[sw-1]; } else { return bufferB[0]; } }
		
		void statistieken()
		{
			std::cout << "Stapelwijzer: " << sw << std::endl;
			std::cout << "Ware grootte: " << wareGrootte << std::endl << std::endl;
		}
		
		void toonGegevens()
		{
			for(unsigned long int i = 0; i < sw; i++)
			{
				std::cout << i+1 << ") " << bufferA[i] << std::endl;
				std::cout << i+1 << ") " << bufferB[i] << std::endl;
			}
		}
		
	private:
	
		/*ostream& operator<<(ostream &os, const vector<kint> x)
		{

			for(kint i = 0; i < x.size(); i++)
			{
				if(i % 3 == 0 && i != 0) { os << " "; }
				os << to_string(x[i]);
			}
			return os;
		}*/
	
		void afstellen(unsigned long int nieuweGrootte)
		{
			if(nieuweGrootte == 0) { nieuweGrootte = 1; }
			A *tijdelijk = new A[wareGrootte];
			for(unsigned long int i = 0; i < wareGrootte; i++) { tijdelijk[i] = bufferA[i];}
			delete[] bufferA; bufferA = NULL; bufferA = new A[nieuweGrootte];
			int t = (wareGrootte > nieuweGrootte ? nieuweGrootte : wareGrootte);
			if(t > sw) { t = sw; }
			for(unsigned long int i = 0; i < t; i++)
			{ bufferA[i] = tijdelijk[i]; }
			delete[] tijdelijk; tijdelijk = NULL;
			
			B *tijdelijk2 = new B[wareGrootte];
			for(unsigned long int i = 0; i < wareGrootte; i++) { tijdelijk2[i] = bufferB[i];}
			delete[] bufferB; bufferB = NULL; bufferB = new B[nieuweGrootte];
			for(unsigned long int i = 0; i < t; i++)
			{ bufferB[i] = tijdelijk2[i]; }
			delete[] tijdelijk2; tijdelijk2 = NULL; wareGrootte = nieuweGrootte;
		}
	
		void verdubbelGrootte()
		{
			A *tijdelijk = new A[wareGrootte];
			for(unsigned long int i = 0; i < wareGrootte; i++) { tijdelijk[i] = bufferA[i];}
			delete[] bufferA; bufferA = NULL; wareGrootte *= 2;
			bufferA = new A[wareGrootte];
			for(unsigned long int i = 0; i < wareGrootte / 2; i++) { bufferA[i] = tijdelijk[i]; }
			//std::cout << "Komt het programma hier wel?" << std::endl; 
			delete[] tijdelijk; tijdelijk = NULL;
			
			wareGrootte /= 2;
			B *tijdelijk2 = new B[wareGrootte];
			for(unsigned long int i = 0; i < wareGrootte; i++) { tijdelijk2[i] = bufferB[i];}
			delete[] bufferB; bufferB = NULL; wareGrootte *= 2;
			bufferB = new B[wareGrootte];
			for(unsigned long int i = 0; i < wareGrootte / 2; i++) { bufferB[i] = tijdelijk2[i]; }
			//std::cout << "Komt het programma hier wel?" << std::endl; 
			delete[] tijdelijk2; tijdelijk2 = NULL;
		}
	
		unsigned long int wareGrootte;
		unsigned long int sw;
		A *bufferA;
		B *bufferB;
		//T *TArray;
		//T *TArray = new T[BEGINGROOTTE];
};

template<class A, class B>
std::ostream &operator<<(std::ostream &uitvoer, Koppelvlak<A, B> &c)
{
	for(unsigned long long int i = 0; i < c.grootte(); i++)
	{ uitvoer << i+1 << ") " << c.verkrijgA(i) << "\t" << c.verkrijgB(i) << std::endl; }
	return uitvoer;
}

#endif // KOPPELVLAK_H
