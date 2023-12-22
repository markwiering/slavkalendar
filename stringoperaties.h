/*
 * bool 	ZitHetErin		(const string &code, const char teken)
 * bool 	ZitHetErin		(string lijn, string argument)
 * bool 	BegintMet		(string lijn, string argument)
 * bool 	EindigtMet		(string lijn, string argument)
 * string 	VerwijderOverbodigeSpaties(string lijn)
 * string 	VerwijderOverbodigeTabsEnSpaties(string lijn)
 * string 	Omdraaien		(string lijn)
 * string 	VerwijderEnter	(string lijn)
 * kint 	VindBeginpunt	(string lijn, string argument)
 * kint 	VindEindpunt	(string lijn, string argument)
 * uint64_t WoordenTellen	(string lijn, string argument)
 * lint 	TekenTeller		(string lijn, char argument)
 * string 	GeefDeelString	(string lijn, kint beginpunt, kint eindpunt)
 * string 	Vervang			(string lijn, kint a, kint b, string vervanging)
 * string 	Verwijder		(string lijn, kint a, kint b)
 * string 	VindEnVervang	(string lijn, string trefwoord, string vervang_door) 
 * string 	VindEnVerwijder	(string lijn, string trefwoord)
 * string 	VoegIn			(const string &code, const kint &beginpositie, const string &invoeging)
 * bool 	LegeLijn		(string lijn)
*/

bool ZitHetErin(const std::string &code, const char teken)
{
	for(unsigned long int i = 0; i < code.size(); i++)
	{
		if(code[i] == teken) { return true; }
	}
	return false;
}

bool ZitHetErin(std::string lijn, std::string argument)
{
	for(unsigned long int i = 0, j = 0; i < lijn.size(); i++)
	{
		if(lijn[i] == argument[j]) { j++; if(j == argument.size()){ return true; } }
		else if(lijn[i] == argument[0]) { j = 1; }
		else { j = 0; }
	}
	return false;
}

std::string Omdraaien(std::string lijn)
{
	std::string nw = "";
	for(long int i = lijn.size() - 1; i >= 0; i--) { nw += lijn[i]; }
	return nw;
}

uint64_t WoordenTellen(std::string lijn, std::string argument)
{
	uint64_t teller = 0;
	for(unsigned long int i = 0, j = 0; i < lijn.size(); i++)
	{
		if(lijn[i] == argument[j]) { j++; if(j == argument.size()){ teller++; j = 0; } }
		else { j = 0; }
	}
	return teller;
}

unsigned long long TekenTeller(std::string lijn, char argument)
{
	unsigned long long teller = 0;
	for(unsigned long long i = 0; i < lijn.size(); i++)
	{
		if(lijn[i] == argument) { teller++; }
	}
	return teller;
}

bool BegintMet(std::string lijn, std::string argument)
{
	if(argument.size() > lijn.size()) { return false; }
	for(unsigned long int i = 0; i < argument.size(); i++)
	{
		if(argument[i] != lijn[i]) { return false; }
	}
	return true;
}

bool EindigtMet(std::string lijn, std::string argument)
{
	if(argument.size() > lijn.size()) { return false; }
	argument = Omdraaien(argument);
	for(long int i = lijn.size()-1, j = 0; i > lijn.size() -1 -argument.size(); i--, j++)
		{ if(argument[j] != lijn[i]) { return false; } }
	return true;
}

std::string GeefDeelString(std::string lijn, unsigned long int beginpunt, unsigned long int eindpunt)
{
	if(beginpunt > eindpunt) { unsigned long int x = beginpunt; beginpunt = eindpunt; eindpunt = x; }
	std::string nieuwe_lijn = "";
	for(unsigned long int i = beginpunt; i <= eindpunt; i++)
	{
		nieuwe_lijn += lijn[i];
	}
	return nieuwe_lijn;
}

std::string VerwijderOverbodigeSpaties(std::string lijn)
{
	std::string nw = "";
	bool teken_gezien = false;
	
	for(unsigned long int i = 0; i < lijn.size(); i++)
	{
		if(lijn[i] != ' ') { teken_gezien = true; nw += lijn[i]; continue; }
		if(teken_gezien && lijn[i] == ' ' && lijn[i-1] != ' ') { nw += lijn[i]; }
	}
	if(nw[nw.size()-1] == ' ') { return GeefDeelString(nw, 0, nw.size()-2); }
	return nw;
}

std::string VerwijderOverbodigeTabsEnSpaties(std::string lijn)
{
	std::string nw = "";
	bool teken_gezien = false;
	
	for(unsigned long int i = 0; i < lijn.size(); i++)
	{
		if(lijn[i] != ' ' && lijn[i] != '\t') { teken_gezien = true; nw += lijn[i]; continue; }
		if(teken_gezien && (lijn[i] == ' ' || lijn[i] == '\t') && (lijn[i-1] != ' ' && lijn[i-1] != '\t')) { nw += lijn[i]; }
	}
	if(nw[nw.size()-1] == ' ' || nw[nw.size()-1] == '\t')
		{ return GeefDeelString(nw, 0, nw.size()-2); }
	return nw;
}

std::string VerwijderEnter(std::string lijn)
{
	std::string nw = "";
	for(unsigned long int i = 0; i < lijn.size(); i++)
	{
		if(lijn[i] != '\n') { nw += lijn[i]; }
	}
	return nw;
}

unsigned long int VindBeginpunt(std::string lijn, std::string argument)
{
	for(unsigned long int i = 0, j = 0; i < lijn.size(); i++)
	{
		if(lijn[i] == argument[j]) { j++; if(j == argument.size()){ return i+1 - argument.size(); } }
		else { j = 0; }
	}
	return lijn.size();
}

unsigned long int VindEindpunt(std::string lijn, std::string argument)
{
	for(unsigned long int i = 0, j = 0; i < lijn.size(); i++)
	{
		if(lijn[i] == argument[j]) { j++; if(j == argument.size()){ return i; } }
		else { j = 0; }
	}
	return lijn.size();
}

std::string Vervang(std::string lijn, unsigned long int a, unsigned long int b, std::string vervanging)
{
	if(a > b) { int x = a; a = b; b = x; }
	std::string nieuwe_lijn = "";
	for(unsigned long int i = 0; i < lijn.size(); i++)
	{
		if(i < a || i > b) { nieuwe_lijn += lijn[i]; }
		else { nieuwe_lijn += vervanging; i += (b-a); }
	}
	
	return nieuwe_lijn;
}

std::string Verwijder(std::string lijn, unsigned long int a, unsigned long int b)
{
	if(a > b) { int x = a; a = b; b = x; }
	std::string nieuwe_lijn = "";
	for(unsigned long int i = 0; i < lijn.size(); i++)
	{
		if(i < a || i > b) { nieuwe_lijn += lijn[i]; }
		else { i += (b-a); }
	}
	
	return nieuwe_lijn;
}


std::string VindEnVervang(std::string lijn, std::string trefwoord, std::string vervang_door)
{
	if(lijn.size() < trefwoord.size()) { return lijn; }
	
	std::string nieuwe_lijn = "";
	for(unsigned long int i = 0; i < lijn.size(); i++)
	{
		if(lijn[i] == trefwoord[0] && lijn[i+trefwoord.size()-1] == trefwoord[trefwoord.size()-1])
		{
			std::string s = GeefDeelString(lijn, i, i+trefwoord.size()-1);
			if(s == trefwoord) { nieuwe_lijn += vervang_door; i += trefwoord.size()-1; }
			else { nieuwe_lijn += lijn[i]; }
		}
		else { nieuwe_lijn += lijn[i]; }
	}
	
	return nieuwe_lijn;
}

std::string VindEnVerwijder(std::string lijn, std::string trefwoord)
{
	if(lijn.size() < trefwoord.size()) { return lijn; }
	
	std::string nieuwe_lijn = "";
	for(unsigned long int i = 0; i < lijn.size(); i++)
	{
		if(lijn[i] == trefwoord[0] && lijn[i+trefwoord.size()-1] == trefwoord[trefwoord.size()-1])
		{
			std::string s = GeefDeelString(lijn, i, i+trefwoord.size()-1);
			if(s == trefwoord) { i += trefwoord.size()-1; }
			else { nieuwe_lijn += lijn[i]; }
		}
		else { nieuwe_lijn += lijn[i]; }
	}
	
	return nieuwe_lijn;
}

std::string VoegIn(const std::string &code, const unsigned int &beginpositie, const std::string &invoeging)
{
	std::string fragment = "";
	if(beginpositie == code.size()) { return code + invoeging; }
	for(unsigned long int i = 0; i < code.size(); i++)
	{
		if(i == beginpositie) { fragment += invoeging; }
		fragment += code[i];
	}
	
	return fragment;
}

bool LegeLijn(std::string lijn)
{
	for(unsigned long int i = 0; i < lijn.size(); i++)
	{
		if(lijn[i] != ' ' && lijn[i] != '\t') { return false; }
	}
	return true;
}
