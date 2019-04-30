#include "Trim.h"


const string WHITESPACE = " \n\r\t\f\v";

string Trim::leftTrim(const std::string& i_string)
{
	size_t start = i_string.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : i_string.substr(start);
}

string Trim::rightTrim(const std::string& i_string)
{
	size_t end = i_string.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : i_string.substr(0, end + 1);
}

string Trim::trim(const std::string& i_string)
{
	return rightTrim(leftTrim(i_string));
}
