/* Dit is het hoofdbestand drager.hpp */

#define BEGINGROOTTE 8
#define BUITEN_REEKS 1
#define wilget(min,max) rand()%(max-min+1)+min
#include <cstdlib>

void dragerFoutmelder(int foutmelding)
{
	if(foutmelding == BUITEN_REEKS)
	{
		std::cerr << "\n\033[1;31mFoutmelding\033[0m: jij hebt geprobeerd om buiten de reeks iets te veranderen!" << std::endl;
		exit(1);
	}
}

template <class T> class Drager
{
	public:
		Drager(unsigned long long int i = BEGINGROOTTE)
		{
			sw = 0;
			if(i > 0) { buffer = new T[i]; wareGrootte = i; }
			else { buffer = new T[BEGINGROOTTE]; wareGrootte = BEGINGROOTTE; }
		}
		Drager(T c)
		{
			sw = 1;
			wareGrootte = BEGINGROOTTE;
			buffer = new T[wareGrootte];
			buffer[0] = c;
		}
		
		Drager(const Drager &c)  // kopieerconstructie 
		{
			//std::cout << "Dit gaat via de geautomatiseerde kopieermachine!" << std::endl;
			sw = c.sw;
			wareGrootte = c.wareGrootte;
			buffer = new T[wareGrootte];
			for(unsigned long long int i = 0; i < sw; i++) { buffer[i] = c.buffer[i]; }
		}
		~Drager() { delete[] buffer; buffer = NULL; }
		
		T &operator[](unsigned long long int c)
			{ if(! (c >= 0 && c < sw)) { dragerFoutmelder(BUITEN_REEKS); } return buffer[c]; }
		T const &operator[](unsigned long long int c) const
			{ if(! (c >= 0 && c < sw)) { dragerFoutmelder(BUITEN_REEKS); } return buffer[c]; }
			
		Drager operator+=(T c) { druk(c); return *this; }
		Drager operator+=(Drager c) { for(unsigned long long int i = 0; i < c.sw; i++) { druk(c[i]); } return *this; }
		
		Drager operator-=(T c){ allesVerwijderen(c); return *this; }
		Drager operator-=(Drager c)
			{ for(unsigned long long int i = 0; i < c.sw; i++) { allesVerwijderen(c[i]); return *this; } }
			
		Drager operator*=(Drager c)
		{
			for(unsigned long long int i = 0; i < c.sw; i++) { if(! ZitHetErin(c[i])) { allesVerwijderen(c[i]); } }
			return *this;
		}
		
		Drager operator/=(Drager c)
		{
			for(unsigned long long int i = 0; i < c.sw; i++)
			{
				if(ZitHetErin(c[i])) { allesVerwijderen(c[i]); }
				else { uniekDrukken(c[i]); }
			}
			return *this;
		}
			
		Drager operator=(const Drager c)
		{
			sw = c.sw;
			wareGrootte = c.wareGrootte;
			buffer = new T[wareGrootte];
			for(unsigned long long int i = 0; i < sw; i++) { buffer[i] = c[i]; }
			return *this;  // geeft een verwijzing naar mijzelf
		}
		
		Drager operator=(T c)
		{
			sw = 1;
			wareGrootte = BEGINGROOTTE;
			buffer = new T[wareGrootte];
			buffer[0] = c;
			return *this;  // geeft een verwijzing naar mijzelf
		}
		
		bool operator==(Drager c)
		{
			if(sw != c.sw) { return false; }
			for(unsigned long long int i = 0; i < sw; i++) { if(buffer[i] != c[i]) { return false; } }
			return true;
		}
		
		bool operator!=(Drager c)
		{
			if(sw != c.sw) { return true; }
			for(unsigned long long int i = 0; i < sw; i++) { if(buffer[i] != c[i]) { return true; } }
			return false;
		}
		
		Drager operator+(T c) { Drager v = *this; v.druk(c); return v; }
		Drager operator+(Drager &c)
		{
			Drager v;
			for(unsigned long long int i = 0; i < sw; i++) { v.druk(buffer[i]); }
			for(unsigned long long int i = 0; i < c.sw; i++) { v.druk(c[i]); }
			return v;
		}
		
		Drager operator-(T c) { Drager v = *this; v.allesVerwijderen(c); return v; }
		Drager operator-(Drager &c)
		{
			Drager v = *this;
			for(unsigned long long int i = 0; i < c.sw; i++) { v.allesVerwijderen(c[i]); }
			return v;
		}
		
		Drager operator*(Drager &c)
		{
			Drager v;
			for(unsigned long long int i = 0; i < sw; i++)
			{ if(c.ZitHetErin(buffer[i])) { v.uniekDrukken(buffer[i]); } }
			return v;
		}
		
		Drager operator/(Drager &c)
		{
			Drager v;
			for(unsigned long long int i = 0; i < sw; i++)
			{ if(! c.ZitHetErin(buffer[i])) { v.uniekDrukken(buffer[i]); } }
			
			for(unsigned long long int i = 0; i < c.sw; i++)
			{ if(! ZitHetErin(c[i])) { v.uniekDrukken(c[i]); } }
			return v;
		}
		
		bool isGelijkAan(Drager &v) { return v == *this; }
		void KopieVan(Drager &v) { *this = v; }
		
		bool uniekDrukken(T x)
		{
			for(unsigned long long int i = 0; i < sw; i++) { if(x == buffer[i]) { return false; } }
			druk(x); return true;
		}
		
		bool uniekInvoegen(T x)
		{
			for(unsigned long long int i = 0; i < sw; i++) { if(x == buffer[i]) { return false; } }
			invoegen(x); return true;
		}
		
		void invoegen(T x)
		{
			if(sw >= wareGrootte) { verdubbelGrootte(); };
			for(unsigned long long int i = sw; i > 0; i--) { buffer[i] = buffer[i-1]; }
			buffer[0] = x;
			sw++;
		}
		
		void invoegen(unsigned long long int x, T y)
		{
			if(sw >= wareGrootte) { verdubbelGrootte(); };
			for(unsigned long long int i = sw; i > x; i--) { buffer[i] = buffer[i-1]; }
			buffer[x] = y;
			sw++;
		}
		
		void druk(T x) { if(sw >= wareGrootte) { verdubbelGrootte(); } buffer[sw] = x; sw++; }
		
		bool ZitHetErin(T x)
		{ for(unsigned long long int i = 0; i < sw; i++) { if(x == buffer[i]) { return true; } } return false; }
		
		unsigned long long int tellen(T x)
		{
			unsigned long long int teller = 0;
			for(unsigned long long int i = 0; i < sw; i++) { if(x == buffer[i]) { teller++; } }
			return teller;
		}
		
		unsigned long long int zoeken(T x)
		{
			for(unsigned long long int i = 0; i < sw; i++)
			{ if(x == buffer[i]) { return i; } }
			return sw;
		}
		
		void verwijderen(unsigned long long int i)
		{
			if( i >= sw ) { return; }
			for(unsigned long long int j = i+1; j < sw; j++) { buffer[j-1] = buffer[j]; }
			sw--;
		}
		
		void verwijderReeks(unsigned long long int a, unsigned long long int b)
		{
			if(a > b || b >= sw) { return; }
			int reeks = b - a + 1;
			for(unsigned long long int j = a+reeks; j < sw; j++) { buffer[j-reeks] = buffer[j]; }
			sw -= reeks;
		}
		
		unsigned long long int allesVervangen(T x, T y)
		{
			unsigned long long int vervangen = 0;
			for(int64_t i = sw - 1; i >= 0; i--)
			{ if(x == buffer[i]) { wijzig(i,y); vervangen++; } }
			return vervangen;
		}
		
		unsigned long long int allesVerwijderen(T x)
		{
			unsigned long long int verwijderd = 0;
			for(int64_t i = sw - 1; i >= 0; i--)
			{ if(x == buffer[i]) { verwijderen(i); verwijderd++; } }
			return verwijderd;
		}
		
		void grabbelen()
		{
			Drager v = *this;
			Drager<uint32_t> reeks(sw+1);
			uint32_t wg = 0;
			
			for(unsigned long long int i = 0; i < v.sw; i++)
			{
				wg = wilget(0, v.sw-1);
				//std::cout << "wg: " << wg << std::endl;
				if(! reeks.ZitHetErin(wg)) { reeks += wg; }
				else
				{
					for(unsigned long long int j = 0; j < v.sw; j++)
					{
						wg++; if(wg >= v.sw) { wg = 0; }
						if(! reeks.ZitHetErin(wg)) { reeks += wg; break; }
					}
				}
				buffer[i] = v[wg];
			}
		}
		
		void wissel(unsigned long long int a, unsigned long long int b)
		{
			if(! (a >= 0 && a < sw)) { dragerFoutmelder(BUITEN_REEKS); }
			if(! (b >= 0 && b < sw)) { dragerFoutmelder(BUITEN_REEKS); }
			T x = buffer[a]; buffer[a] = buffer[b];  buffer[b] = x;
		}
		
		bool leeg() { return sw == 0; }
		bool isLeeg() { return sw == 0; }
		void opschonen() { sw = 0; }
		void grondigOpschonen() { sw = 0; bijstellen(); }
		unsigned long long int grootte() const { return sw; }
		unsigned long long int werkelijkeGrootte() { return wareGrootte; }
		
		bool wijzig(unsigned long long int i, T x) { if(i < sw) { buffer[i] = x; return true;} return false; }
		
		void reserveer(unsigned long long int i = 1)
		{
			sw += i;
			//if(sw >= wareGrootte) { afstellen(i*2); }
			if(sw >= wareGrootte) { verdubbelGrootte(); }
		}
		
		bool bijstellen()
		{
			if(sw != wareGrootte && !(sw == 0 && wareGrootte == 1)) { afstellen(sw); return true; } else { return false; }
		}
		
		T *adres(unsigned long long int i) { if(i < sw) { return &buffer[i]; } return &buffer[0]; }
		void kunstmatigStapelwijzerBijstellen(unsigned long long int i) { sw = i; }
		
		T verkrijg(unsigned long long int i) { if(i < sw) { return buffer[i]; }  return buffer[0]; }
		T knal() { if(sw >= 1) { sw--; return buffer[sw]; } else { return buffer[0]; } }
		T top() { if(sw >= 1) { return buffer[sw-1]; } else { return buffer[0]; } }
		
		void statistieken()
		{
			std::cout << "Stapelwijzer: " << sw << std::endl;
			std::cout << "Ware grootte: " << wareGrootte << std::endl << std::endl;
		}
		
		void toonGegevens()
		{
			for(unsigned long long int i = 0; i < sw; i++)
			{
				std::cout << i+1 << ") " << buffer[i] << std::endl;
			}
		}
		
		void afstellen(unsigned long long int nieuweGrootte)
		{
			if(nieuweGrootte == 0) { nieuweGrootte = 1; }
			T *tijdelijk = new T[wareGrootte];
			for(unsigned long long int i = 0; i < wareGrootte; i++) { tijdelijk[i] = buffer[i];}
			delete[] buffer; buffer = NULL; buffer = new T[nieuweGrootte];
			unsigned long long int t = (wareGrootte > nieuweGrootte ? nieuweGrootte : wareGrootte);
			if(t > sw) { t = sw; }
			for(unsigned long long int i = 0; i < t; i++) { buffer[i] = tijdelijk[i]; }
			delete[] tijdelijk; tijdelijk = NULL; wareGrootte = nieuweGrootte;
		}
		
	private:
	
		/*ostream& operator<<(ostream &os, const vector<kint> x)
		{

			for(kint i = 0; i < x.grootte(); i++)
			{
				if(i % 3 == 0 && i != 0) { os << " "; }
				os << to_string(x[i]);
			}
			return os;
		}*/
	
		void verdubbelGrootte()
		{
			T *tijdelijk = new T[wareGrootte];
			for(unsigned long long int i = 0; i < wareGrootte; i++) { tijdelijk[i] = buffer[i];}
			delete[] buffer; buffer = NULL; wareGrootte *= 2;
			buffer = new T[wareGrootte];
			for(unsigned long long int i = 0; i < wareGrootte / 2; i++) { buffer[i] = tijdelijk[i]; }
			delete[] tijdelijk; tijdelijk = NULL;
		}
		
		unsigned long long int wareGrootte;
		unsigned long long int sw;
		T *buffer;
};

template<class T>
std::ostream &operator<<(std::ostream &uitvoer, Drager<T> &c)
{
	for(unsigned long long int i = 0; i < c.grootte(); i++) { uitvoer << i+1 << ") " << c[i] << std::endl; }
	return uitvoer;
}
