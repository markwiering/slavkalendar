#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#define herhaal(x) for(unsigned int i = 0; i < x; i++)
#define herhaalc(x, c) for(unsigned int c = 0; c < x; c++)
#define loop(x) for(unsigned int i = 0; i < x.size(); i++)
#define loopc(x, c) for(unsigned int c = 0; c < x.size(); c++)
#define revloop(x) for(int i = x.size()-1; i >= 0; i--)
#define revloopc(x, c) for(int c = x.size()-1; c >= 0; c--)
#define controleer(x) while(std::cin.fail()){VerkeerdeInvoer(x);}

#define lus(x) for(unsigned int i = 0; i < x.grootte(); i++)
#define lusc(x, c) for(unsigned int c = 0; c < x.grootte(); c++)
#define omglus(x) for(int i = x.grootte()-1; i >= 0; i--)
#define omglusc(x, c) for(int c = x.grootte()-1; c >= 0; c--)

#define RIJ_GROOTTE(x) (sizeof(x)/sizeof(x[0]))
// idee: overload new

#ifndef wilget
int wilget(int min, int max) { return rand()%(max-min+1)+min; }
#endif

/*typedef unsigned long long int uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;

typedef signed long long int int64_t;
typedef signed int int32_t;
typedef signed short int int16_t;
typedef signed char int8_t;*/

template <typename T>
void VerkeerdeInvoer(T &x, std::string tekst = "\nVoer een geldige waarde in: ")
{
	std::cin.clear();
	std::cin.ignore();
	//std::cout << "\nVoer een geldige waarde in: ";
	std::cerr << tekst;
	std::cin >> x;
}

template <typename T>
void wissel(T &a, T &b) { T c = a; a = b; b = c; }

uint64_t TotDeMacht(uint64_t waarde, uint8_t exponent)
{
	uint64_t resultaat = 1;
	for(uint8_t i = 0; i < exponent; i++)
		{ resultaat = resultaat * waarde; }
	return resultaat;
}

uint64_t TotDeMacht64(uint64_t waarde, uint8_t exponent)
{
	uint64_t resultaat = 1;
	for(uint8_t i = 0; i < exponent; i++)
		{ resultaat = resultaat * waarde; }
	return resultaat;
}

uint32_t TotDeMacht32(uint32_t waarde, uint8_t exponent)
{
	uint32_t resultaat = 1;
	for(uint8_t i = 0; i < exponent; i++)
		{ resultaat = resultaat * waarde; }
	return resultaat;
}

uint16_t TotDeMacht16(uint16_t waarde, uint8_t exponent)
{
	uint16_t resultaat = 1;
	for(uint8_t i = 0; i < exponent; i++)
		{ resultaat = resultaat * waarde; }
	return resultaat;
}

uint8_t TotDeMacht8(uint8_t waarde, uint8_t exponent)
{
	uint8_t resultaat = 1;
	for(uint8_t i = 0; i < exponent; i++)
		{ resultaat = resultaat * waarde; }
	return resultaat;
}

int64_t stoi64s(std::string arg)
{
	bool negatief = false, voorteken = false;
	if(arg[0] == '-')
		{ negatief = true; voorteken = true; }
	else if(arg[0] == '+')
		{ voorteken = true; }
	
	int64_t getal = 0;
	for(int i = arg.size()-1, j = 0; i >= voorteken; i--, j++)
		{ getal += int64_t(arg[i]-48) * TotDeMacht(10, j); }
	if(negatief) { getal *= -1; }
	return getal;
}

uint64_t stoi64(std::string arg)
{
	uint64_t getal = 0;
	for(int i = arg.size()-1, j = 0; i >= 0; i--, j++)
		{ getal += uint64_t(arg[i]-48) * TotDeMacht(10, j); }
	return getal;
}

uint32_t stoi32(std::string arg)
{
	uint32_t getal = 0;
	for(int i = arg.size()-1, j = 0; i >= 0; i--, j++)
		{ getal += uint32_t(arg[i]-48) * TotDeMacht(10, j); }
	return getal;
}

uint16_t stoi16(std::string arg)
{
	uint16_t getal = 0;
	for(int i = arg.size()-1, j = 0; i >= 0; i--, j++)
		{ getal += uint16_t(arg[i]-48) * TotDeMacht(10, j); }
	return getal;
}

uint8_t stoi8(std::string arg)
{
	uint8_t getal = 0;
	for(int i = arg.size()-1, j = 0; i >= 0; i--, j++)
		{ getal += uint8_t(arg[i]-48) * TotDeMacht(10, j); }
	return getal;
}

std::string NaarString(int64_t getal)
{
	std::string woord = ""; bool negatief = false;
	if(getal == 0) { woord += '0'; }
	else if(getal < 0) { getal *= -1; negatief = true; }
	while(getal != 0)
	{
		woord = char(48 + getal % 10) + woord;
		getal = getal / 10;
	}
	if(negatief) { woord = '-' + woord; }
	return woord;
}

std::string naarString(int64_t getal)
{
	std::string woord = ""; bool negatief = false;
	if(getal == 0) { woord += '0'; }
	else if(getal < 0) { getal *= -1; negatief = true; }
	while(getal != 0)
	{
		woord = char(48 + getal % 10) + woord;
		getal = getal / 10;
	}
	if(negatief) { woord = '-' + woord; }
	return woord;
}

std::string ONaarString(uint64_t getal)
{
	std::string woord = "";
	if(getal == 0) { woord += '0'; }
	while(getal != 0)
	{
		woord = char(48 + getal % 10) + woord;
		getal = getal / 10;
	}
	return woord;
}

std::string kl(std::string code)
{
	std::string nw = "";
	for(unsigned long int i = 0; i < code.size(); i += 2)
	{
		if(code[i] == -48 && (code[i+1] >= -128 && code[i+1] <= -113))
			{ nw += char(-47); nw += char(code[i+1]+16); }
		else if(code[i] == -48 && (code[i+1] >= -112 && code[i+1] <= -97))
			{ nw += char(-48); nw += char(code[i+1]+32); }
		else if(code[i] == -48 && (code[i+1] >= -96 && code[i+1] <= -81))
			{ nw += char(-47); nw += char(code[i+1]-32); }
		else if(code[i] == -47 && (code[i+1] >= -96 && code[i+1] <= -66) && code[i+1] % 2 == 0)
			{ nw += char(-47); nw += char(code[i+1]+1); }
		else if(code[i] >= 'A' && code[i] <= 'Z') { nw += char(code[i] + 32); i--; }
		else if(code[i] >= 32 && code[i] <= 126) { nw += code[i]; i--; }
		else { nw += code[i]; nw += code[i+1]; }
	}
	return nw;
}

std::string gl(std::string code)
{
	std::string nw = "";
	for(unsigned long int i = 0; i < code.size(); i += 2)
	{
		if(code[i] == -47 && (code[i+1] >= -112 && code[i+1] <= -97))
			{ nw += char(-48); nw += char(code[i+1]-16); }
		else if(code[i] == -48 && (code[i+1] >= -80 && code[i+1] <= -65))
			{ nw += char(-48); nw += char(code[i+1]-32); }
		else if(code[i] == -47 && (code[i+1] >= -128 && code[i+1] <= -113))
			{ nw += char(-48); nw += char(code[i+1]+32); }
		else if(code[i] == -47 && (code[i+1] >= -95 && code[i+1] <= -65) && code[i+1] % 2 != 0)
			{ nw += char(-47); nw += char(code[i+1]-1); }
		else if(code[i] >= 'a' && code[i] <= 'z') { nw += char(code[i] - 32); i--; }
		else if(code[i] >= 32 && code[i] <= 126) { nw += code[i]; i--; }
		else { nw += code[i]; nw += code[i+1]; }
	}
	return nw;
}

std::string purificeer(std::string code)
{
	std::string nw = "";
	for(unsigned long int i = 0; i < code.size(); i += 2)
	{
		if((code[i] == -48 && ((code[i+1] >= -112 && code[i+1] <= -81) || (code[i+1] >= -80 && code[i+1] <= -65) || (code[i+1] >= -128 && code[i+1] <= -65))) || (code[i] == -47 && (code[i+1] >= -128 && code[i+1] <= -65))) { nw += code[i]; nw += code[i+1]; }
		else if((code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z')) { nw += code[i]; i--; }
		else if(code[i] >= 32 && code[i] <= 126) { i--; }
	}
	return nw;
}
