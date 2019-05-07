#ifndef REQUEST_H
#define REQUEST_H

#include <string.h>
#include <map> 
#include <vector>
using namespace std;


class Request {
public:
	enum Method {
		GET = 0,
		PUSH,
		OPTIONS,
		eDELETE,
		TRACE,
		HEAD };
private:
	Method m_method;
	string m_requestUri;
	string m_version;
	map<string, string> m_headers;
	string m_body;
	string m_rawMsg;
	static map<string, Method> m_methodMap;

public:
	bool isValid();
	bool setRequest(string i_method,
		string i_requestUri,
		string i_version,
		vector<pair<string,string>> i_headers,
		string i_body,
		string i_httpRequest);
	Method methodType();
	string getRequestUri() { return m_requestUri; }
	string getVersion() { return m_version; }
	string getBody() { return m_body; }
	string getHeaderValue(string i_headerName) { return m_headers[i_headerName]; }
	string getRawMsg() { return m_rawMsg; }
};

#endif
