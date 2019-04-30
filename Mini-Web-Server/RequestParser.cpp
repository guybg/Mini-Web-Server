#include "RequestParser.h"
#define SPACE 32

bool RequestParser::Parse(Request &o_request, string i_httpRequest) {
	int index;
	Trim trimer;
	string requestLine;
	string value;
	vector<string> requestLineParameters;
	index = i_httpRequest.find('\n', 0);
	requestLine = i_httpRequest.substr(0, index);
	requestLineParameters = splitLine(' ', requestLine);
	
	o_request.method = requestLineParameters[0];
	o_request.requestUri = requestLineParameters[1];
	o_request.version = requestLineParameters[2];
	
	i_httpRequest = i_httpRequest.substr(index + 1);
	while (i_httpRequest.length())
	{
		string headerLine;
		string headerName;
		string headerValue;

		index = i_httpRequest.find('\n', 0);
		headerLine = i_httpRequest.substr(0, index);
		requestLineParameters = splitLine(':', headerLine);
		headerName = requestLineParameters[0];
		headerValue = requestLineParameters[1];
		o_request.headers.insert(pair<string, string>(headerName, headerValue));
		if (index == -1)
			break;
		i_httpRequest = i_httpRequest.substr(index);
		i_httpRequest = trimer.trim(i_httpRequest);
	}
	return true;
}


vector<string> RequestParser::splitLine(char i_charToSplitBy, string i_line) {
	vector<string> lines;
	string value;
	Trim trimer;
	int index = 0;
	int linesIndex = 0;
	while (i_line.length()) {
		if (i_charToSplitBy == ' ') {
			i_line = trimer.trim(i_line);
			index = 0;
			while (index != i_line.length()) {
				if (std::isspace(i_line.at(index))) {
					break;
				}
				index++;
			}
		}
		else {
			index = i_line.find(i_charToSplitBy, 0);
		}
		value = i_line.substr(0, index);
		value = trimer.trim(value);
		lines.push_back(value);
		if (index == i_line.length())
			break;
		i_line = i_line.substr(index + 1);
		i_line = trimer.trim(i_line);
		if (index == -1)
			i_line.clear();
	}
	return lines;
}
