#ifndef PARSER_H
#define PARSER_H

#include "Request.h"
#include "Trim.h"
#include <Vector>
class RequestParser {
public:
	static bool Parse(Request &o_request, string i_httpRequest);
private:
	static vector<string> splitLine(char i_charToSplitBy, string i_line);
	static size_t findCaseInsensitive(string i_data, string i_toSearch, size_t i_pos = 0);
};


#endif
