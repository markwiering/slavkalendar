/* Dit hoofdbestand is afhankelijk van "drager.hpp" en <string>. */

Drager<std::string> split(const std::string &code, char a = ' ', char b = '\t')
{
	Drager<std::string> v; std::string t = "";
	for(unsigned long long int f = 1; f <= code.size(); f++)
	{
		if(code[f - 1] != a && code[f - 1] != b) { t += code[f - 1]; }
		if((code[f] == a || code[f] == b) && (code[f - 1] != a && code[f - 1] != b)) { v += t; t = "";}
	}
	if(t != "" && t != "\t") { v += t; }
	return v;
}

Drager<std::string> MWPsplit(const std::string &code, char a = ' ', char b = '\t', char c = '=')
{
	Drager<std::string> v; std::string t = "";
	for(unsigned long long int f = 1; f <= code.size(); f++)
	{
		if(code[f - 1] != a && code[f - 1] != b && code[f - 1] != c)
			{ t += code[f - 1]; }
		if((code[f] == a || code[f] == b || code[f] == c) && (code[f - 1] != a && code[f - 1] != b && code[f - 1] != c)) { v += t; t = ""; }
	}
	if(t != "" && t != "\t") { v += t; }
	return v;
}