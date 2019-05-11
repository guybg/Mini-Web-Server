#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#define _CRT_SECURE_NO_WARNINGS
#include "Request.h"
#include "FileHandler.h"
#include <time.h>

#define OK "200 OK"
#define CREATED "201 Created"
#define NO_CONTENT "204 No Content"
#define NOT_FOUND "404 Not Found"
#define BAD_REQUEST "400 Bad Request"
#define PRECONDITION_FAILED "412 Precondition Failed"
#define NOT_IMPLEMENTED "501 Not Implemented"
#define VERSION "HTTP/1.1"
#define SERVER_INFO "MiniWebServer (Windows)"
#define CONNECTION_CLOSED "close"

#define MESSAGE_CONTENT_TYPE "message/http"
#define HTML_CONTENT_TYPE "text/html"
#define ALLOWED_METHODS "HEAD,GET,TRACE,PUT,DELETE,OPTIONS"
#define CONTENT_LENGTH_KEY "Content-Length"
#define DATE_KEY "Date"
#define CONTENT_TYPE_KEY "Content-Type"
#define BODY_KEY "Body"
#define ALLOW_KEY "Allow"
#define METHOD_KEY "Method"
#define PATH_KEY "Path"
#define HTTP_VER_KEY "HTTP-Version"
#define STATUS_CODE_KEY "Status"
#define REQUEST_LINE "request-line"
#define SERVER "Server"
#define CONNECTION "Connection"

static class RequestHandler {
public:
	bool handle(Request i_request, string &o_messege);
	void httpSendBadRequest(string &o_processedMsg);
private:
	bool httpGET(Request i_request, string &o_processedMsg);
	bool httpPUT(Request i_request, string &o_processedMsg);
	bool httpOPTIONS(Request i_request, string &o_processedMsg);
	bool httpDELETE(Request i_request, string &o_processedMsg);
	bool httpTRACE(Request i_request, string &o_processedMsg);
	bool httpHEAD(Request i_request, string &o_processedMsg);
	string buildAnswer(map<string, string> i_responseParameters);
	bool getPath(string i_requestUri, string &o_path);
	FileHandler fileHandler;
};

#endif