#ifndef TRIM_H
#include <algorithm> 
#include <cctype>
#include <locale>
using namespace std;
class Trim
{
private:
	static string rightTrim(const string&);
	static string leftTrim(const string&);
public:
	static string trim(const std::string&);
};

#endif

