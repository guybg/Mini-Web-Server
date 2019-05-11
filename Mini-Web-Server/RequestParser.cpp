#include "RequestParser.h"

bool RequestParser::Parse(Request &o_request, string i_httpRequest) {
	int index;
	Trim trimer;
	string requestLine;
	string value;
	string rawRequest = i_httpRequest;
	vector<string> requestLineParams;
	vector<string> headerParams;
	string requestBody = "";
	bool isValid = true;
	i_httpRequest.append("\n");
	index = i_httpRequest.find('\n', 0);
	requestLine = i_httpRequest.substr(0, index);
	requestLineParams = splitLine(' ', requestLine);
	i_httpRequest = i_httpRequest.substr(index + 1);

	index = i_httpRequest.find("\r\n\r\n", 0);
	if (index != -1)
	{
		requestBody = i_httpRequest.substr(index, i_httpRequest.length() - index);
		requestBody = trimer.trim(requestBody);
	}
	i_httpRequest = i_httpRequest.substr(0, index);

	vector<pair<string, string>> headerList;
	if (findCaseInsensitive(trimer.trimAll(i_httpRequest), "Host:") == -1 || requestLineParams.size() != 3)
		isValid = false;
	while (i_httpRequest.length() && isValid == true)
	{
		string headerLine;
		string headerName;
		string headerValue;
		index = i_httpRequest.find('\n', 0);
		headerLine = i_httpRequest.substr(0, index);
		headerParams = splitLine(':', headerLine);
		headerName = headerParams[0];
		headerValue = headerParams[1];
		headerList.insert(headerList.end(), pair<string, string>(headerName, headerValue));
		
		i_httpRequest = i_httpRequest.substr(index + 1);
		i_httpRequest = trimer.trim(i_httpRequest);
		if (index == -1)
			i_httpRequest.clear();
	}
	if(isValid == true)
		isValid = o_request.setRequest(requestLineParams[0], requestLineParams[1], requestLineParams[2], headerList, requestBody, rawRequest);
	return isValid;
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

/*
* Find Case Insensitive Sub String in a given substring
*/
size_t RequestParser::findCaseInsensitive(string i_data, string i_toSearch, size_t i_pos)
{
	// Convert complete given String to lower case
	transform(i_data.begin(), i_data.end(), i_data.begin(), ::tolower);
	// Convert complete given Sub String to lower case
	transform(i_toSearch.begin(), i_toSearch.end(), i_toSearch.begin(), ::tolower);
	// Find sub string in given string
	return i_data.find(i_toSearch, i_pos);
}