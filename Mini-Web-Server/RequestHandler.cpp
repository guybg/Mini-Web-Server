#include "RequestHandler.h"

bool RequestHandler::handle(Request i_request, string &o_messege) {
	switch (i_request.methodType()) {
	case Request::GET :
		httpGET(i_request, o_messege);
		break;
	case Request::PUT:
		httpPUT(i_request, o_messege);
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
	bool isValidPath = 0;
	time_t rawtime;
	time(&rawtime);

	isValidPath = getPath(i_request.getRequestUri(), filePath);
	requestLine += VERSION;
	requestLine += ' ';
	isFileFound = fileHandler.readFile(filePath, fileContent);
	if (isValidPath == false) {
		requestLine += BAD_REQUEST;
	}
	else if (isFileFound == false)
	{
		requestLine += NOT_FOUND;
	}
	else 
	{
		requestLine += OK;
	}
	string serverTime = ctime(&rawtime);
	serverTime = serverTime.substr(0, serverTime.length() - 1);
	responseParameters.insert(pair<string, string>(REQUEST_LINE, requestLine));
	responseParameters.insert(make_pair(CONTENT_TYPE_KEY, HTML_CONTENT_TYPE));
	responseParameters.insert(make_pair(CONTENT_LENGTH_KEY, _itoa(fileContent.length(), ctmp, 10)));
	responseParameters.insert(make_pair(DATE_KEY, serverTime));
	responseParameters.insert(make_pair(BODY_KEY, fileContent));

	o_processedMsg = buildAnswer(responseParameters);
	return true;
}
bool RequestHandler::httpPUT(Request i_request, string &o_processedMsg) {
	map<string, string> responseParameters;
	string filePath;
	char ctmp[20];
	bool isValidPath = false;
	string requestLine;
	bool isOperationSuccess = false;
	time_t rawtime;
	time(&rawtime);

	isValidPath = getPath(i_request.getRequestUri(), filePath);
	requestLine += VERSION;
	requestLine += ' ';

	if (isValidPath && fileHandler.isFileExists(filePath)) {
		requestLine += OK;
	}
	else if (isValidPath){
		requestLine += CREATED;
		isOperationSuccess = fileHandler.createFile(filePath);
	}

	if (isValidPath) {
		isOperationSuccess = fileHandler.writeFile(filePath, i_request.getBody());
	}
	if (!isOperationSuccess) {
		int index = requestLine.find(' ');
		requestLine = requestLine.substr(0, index + 1);
		requestLine += NOT_IMPLEMENTED;
	}

	string serverTime = ctime(&rawtime);
	serverTime = serverTime.substr(0, serverTime.length() - 1);
	responseParameters.insert(pair<string, string>(REQUEST_LINE, requestLine));
	responseParameters.insert(make_pair(CONTENT_TYPE_KEY, HTML_CONTENT_TYPE));
	responseParameters.insert(make_pair(DATE_KEY, serverTime));
	responseParameters.insert(make_pair(BODY_KEY, ""));
	responseParameters.insert(make_pair(CONTENT_LENGTH_KEY, _itoa(0, ctmp, 10)));

	o_processedMsg = buildAnswer(responseParameters);
	return isOperationSuccess;
}
bool RequestHandler::httpOPTIONS(Request i_request, string &o_processedMsg) {
	map<string, string> responseParameters;
	char ctmp[20];
	string filePath;
	string fileContent = "";
	string requestLine;
	bool isValidPath = 0;
	int fileLength;
	bool isFileFound = 0;
	time_t rawtime;
	time(&rawtime);
	isValidPath = getPath(i_request.getRequestUri(), filePath);
	requestLine += VERSION;
	requestLine += ' ';
	if (i_request.getRequestUri() == "*") {
		requestLine = NOT_IMPLEMENTED;
		responseParameters.insert(pair<string, string>(ALLOW_KEY, ALLOWED_METHODS));
	}
	else if (isValidPath == false) {
		requestLine += BAD_REQUEST; 
	}
	else
	{
		requestLine += OK;
	}

	string serverTime = ctime(&rawtime);
	serverTime = serverTime.substr(0, serverTime.length() - 1);
	responseParameters.insert(pair<string, string>(REQUEST_LINE, requestLine));
	responseParameters.insert(make_pair(CONTENT_TYPE_KEY, HTML_CONTENT_TYPE));
	responseParameters.insert(make_pair(CONTENT_LENGTH_KEY, _itoa(fileContent.length(), ctmp, 10)));
	responseParameters.insert(make_pair(DATE_KEY, serverTime));
	responseParameters.insert(make_pair(SERVER, SERVER_INFO));
	responseParameters.insert(make_pair(BODY_KEY, fileContent));
	if(isValidPath == false && i_request.getRequestUri() != "*") responseParameters.insert(make_pair(CONNECTION, CONNECTION_CLOSED));

	o_processedMsg = buildAnswer(responseParameters);
	return true;
}
bool RequestHandler::httpDELETE(Request i_request, string &o_processedMsg) {
	map<string, string> responseParameters;
	char ctmp[20];
	string filePath;
	bool isValidPath = 0;
	string requestLine;
	time_t rawtime;

	time(&rawtime);
	isValidPath = getPath(i_request.getRequestUri(), filePath);
	requestLine += VERSION;
	requestLine += ' ';

	if (isValidPath && fileHandler.isFileExists(filePath)) {
		remove(filePath.c_str());
		requestLine += OK;
	}
	else {
		requestLine += NOT_FOUND;
		responseParameters.insert(make_pair(CONTENT_LENGTH_KEY, _itoa(0, ctmp, 10)));
	}
	string serverTime = ctime(&rawtime);
	responseParameters.insert(pair<string, string>(REQUEST_LINE, requestLine));
	serverTime = serverTime.substr(0, serverTime.length() - 1);
	responseParameters.insert(make_pair(DATE_KEY, serverTime));
	o_processedMsg = buildAnswer(responseParameters);

	return true;
}
bool RequestHandler::httpTRACE(Request i_request, string &o_processedMsg) {
	map<string, string> responseParameters;
	char ctmp[20];
	string filePath;
	string fileContent;
	string requestLine;
	string answer = i_request.getRawMsg();
	int answerLength = i_request.getRawMsg().length();
	int fileLength;
	bool isFileFound = 0;
	bool isValidPath = 0;
	time_t rawtime;
	time(&rawtime);

	isValidPath = getPath(i_request.getRequestUri(), filePath);
	requestLine += VERSION;
	requestLine += ' ';
	isFileFound = fileHandler.readFile(filePath, fileContent);
	if (isValidPath == false && i_request.getRequestUri() != "*") {
		requestLine += BAD_REQUEST;
		answerLength = 0;
		answer = "";
	}
	else
	{
		requestLine += OK;
	}
	string test = i_request.getRawMsg();
	string serverTime = ctime(&rawtime);
	serverTime = serverTime.substr(0, serverTime.length() - 1);
	responseParameters.insert(pair<string, string>(REQUEST_LINE, requestLine));
	responseParameters.insert(make_pair(CONTENT_TYPE_KEY, MESSAGE_CONTENT_TYPE));
	responseParameters.insert(make_pair(CONTENT_LENGTH_KEY, _itoa(answerLength, ctmp, 10)));
	responseParameters.insert(make_pair(SERVER, SERVER_INFO));
	responseParameters.insert(make_pair(DATE_KEY, serverTime));
	responseParameters.insert(make_pair(BODY_KEY, answer));

	o_processedMsg = buildAnswer(responseParameters);
	return true;
}
bool RequestHandler::httpHEAD(Request i_request, string &o_processedMsg) {
	httpGET(i_request, o_processedMsg);
	int bodyIndex = o_processedMsg.find("\r\n\r\n");
	if(bodyIndex != -1)
		o_processedMsg = o_processedMsg.substr(0, bodyIndex);
	return true;
}

bool RequestHandler::getPath(string i_requestUri, string &o_path) {
	string filePath = i_requestUri;
	string rootFolder = "www";
	if (filePath.empty())
		return false;
	if (filePath.at(0) == '/') {
		filePath.insert(0, rootFolder);
	}
	else {
		if(filePath.substr(0, 7) == "http://")
			filePath = "www/index.html";
		else {
			return false;
		}
	}

	o_path = filePath;
	return true;
}

string RequestHandler::buildAnswer(map<string, string> i_responseParameters) {
	string answer;
	// adds request line to answer
	answer += i_responseParameters[REQUEST_LINE];
	string body = i_responseParameters[BODY_KEY];
	// adds headers to answer
	i_responseParameters.erase(REQUEST_LINE);
	i_responseParameters.erase(BODY_KEY);
	for (map<string, string>::iterator it = i_responseParameters.begin(); it != i_responseParameters.end(); ++it) {
		answer += "\r\n" + it->first + ": " + it->second;
	}
	// adds body to answer
	if(body.length() > 0)
		answer += "\r\n\r\n" + body;
	return answer;
}

void RequestHandler::httpSendBadRequest(string &o_processedMsg) {
	map<string, string> responseParameters;
	string requestLine;
	time_t rawtime;
	time(&rawtime);

	requestLine += VERSION;
	requestLine += ' ';
	requestLine += BAD_REQUEST;
	string serverTime = ctime(&rawtime);
	serverTime = serverTime.substr(0, serverTime.length() - 1);
	responseParameters.insert(pair<string, string>(REQUEST_LINE, requestLine));
	responseParameters.insert(make_pair(DATE_KEY, serverTime));
	responseParameters.insert(make_pair(SERVER, SERVER_INFO));
	responseParameters.insert(make_pair(CONNECTION, CONNECTION_CLOSED));

	o_processedMsg = buildAnswer(responseParameters);
}