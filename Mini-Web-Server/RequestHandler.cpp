#include "RequestHandler.h"

bool RequestHandler::handle(Request i_request, string &o_messege) {
	bool validRequest;
	validRequest = i_request.isValid();

	if (!validRequest) {
		return 1;
	}

	switch (i_request.methodType()) {
	case Request::GET :
		httpGET(i_request, o_messege);
		break;
	case Request::PUSH:
		httpPUSH(i_request, o_messege);
		break;
	case Request::OPTIONS:
		httpOPTIONS(i_request, o_messege);
		break;
	case Request::eDELETE:
		httpDELETE(i_request, o_messege);
		break;
	case Request::TRACE:
		httpTRACE(i_request, o_messege);
		break;
	case Request::HEAD:
		httpHEAD(i_request, o_messege);
		break;
	}

	return 0;
}

bool RequestHandler::httpGET(Request i_request, string &o_processedMsg) {
	map<string, string> responseParameters;
	char ctmp[20];
	string filePath;
	string fileContent;
	string requestLine;
	int fileLength;
	bool isFileFound = 0;
	time_t rawtime;
	time(&rawtime);

	filePath = getPath(i_request.getRequestUri());
	requestLine += VERSION;
	requestLine += ' ';
	isFileFound = fileHandler.readFile(filePath, fileContent);
	
	if (isFileFound == false)
	{
		requestLine += NOT_FOUND;
	}
	else 
	{
		requestLine += OK;
	}

	responseParameters.insert(pair<string, string>(REQUEST_LINE, requestLine));
	responseParameters.insert(make_pair(CONTENT_TYPE_KEY, HTML_CONTENT_TYPE));
	responseParameters.insert(make_pair(CONTENT_LENGTH_KEY, _itoa(fileContent.length(), ctmp, 10)));
	responseParameters.insert(make_pair(DATE_KEY, ctime(&rawtime)));
	responseParameters.insert(make_pair(BODY_KEY, fileContent));

	o_processedMsg = buildAnswer(responseParameters);
	return true;
}
bool RequestHandler::httpPUSH(Request i_request, string &o_processedMsg) {
	return true;
}
bool RequestHandler::httpOPTIONS(Request i_request, string &o_processedMsg) {
	o_processedMsg = "it was a options messege";
	return true;
}
bool RequestHandler::httpDELETE(Request i_request, string &o_processedMsg) {
	return true;
}
bool RequestHandler::httpTRACE(Request i_request, string &o_processedMsg) {
	return true;
}
bool RequestHandler::httpHEAD(Request i_request, string &o_processedMsg) {
	return true;
}

string RequestHandler::getPath(string i_requestUri) {
	string filePath = i_requestUri;
	string rootFolder = "www";
	if (filePath.at(0) == '/') {
		filePath.insert(0, rootFolder);
	}
	else {
		filePath = "www/index.html";
	}

	return filePath;
}

string RequestHandler::buildAnswer(map<string, string> i_responseParameters) {
	string answer;
	// adds request line to answer
	answer += i_responseParameters[REQUEST_LINE];
	answer += '\n';
	string body = i_responseParameters[BODY_KEY];
	// adds headers to answer
	i_responseParameters.erase(REQUEST_LINE);
	i_responseParameters.erase(BODY_KEY);
	for (map<string, string>::iterator it = i_responseParameters.begin(); it != i_responseParameters.end(); ++it) {
		answer += "\r\n" + it->first + ": " + it->second;
	}
	// adds body to answer
	answer += "\r\n" + body;
	return answer;
}