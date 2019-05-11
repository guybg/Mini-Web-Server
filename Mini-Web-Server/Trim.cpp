#include "Trim.h"


const string WHITESPACE = " \n\r\t\f\v";

string Trim::leftTrim(const string& i_string)
{
	size_t start = i_string.find_first_not_of(WHITESPACE);
	return (start == string::npos) ? "" : i_string.substr(start);
}

string Trim::rightTrim(const string& i_string)
{
	size_t end = i_string.find_last_not_of(WHITESPACE);
	return (end == string::npos) ? "" : i_string.substr(0, end + 1);
}

string Trim::trim(const string& i_string)
{
	return rightTrim(leftTrim(i_string));
}

string Trim::trimAll(const string i_string) {
	string newString;
	for (int i = 0; i < i_string.length(); i++) {
		if (!(i_string[i] == '\n' || i_string[i] == '\r' || i_string[i] == '\t' || i_string[i] == '\f' || i_string[i] == '\v' || i_string[i] == ' '))
		{
			newString += i_string[i];
		}
	}
	return newString;
}

