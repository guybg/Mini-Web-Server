#include "Request.h";

bool Request::isValid() {
	if(this != NULL)
		return true;
	return false;
}

Request::Method Request::methodType() {
	return this->m_method;
}

bool Request::setRequest(string i_method,
						 string i_requestUri,
						 string i_version,
						 vector<pair<string,string>> i_headers,
						 string i_body)
{
	//check validity
	this->m_method = m_methodMap[i_method];
	this->m_requestUri = i_requestUri;
	this->m_version = i_version;

	for (pair<string, string> header : i_headers) {
		//can check validity
		this->m_headers.insert(header);
	}

	this->m_body = i_body;
	return 0;

}

map<string, Request::Method> Request::m_methodMap = {
	{"GET", GET},
	{"PUSH", PUSH},
	{"OPTIONS", OPTIONS},
	{"DELETE", eDELETE},
	{"TRACE", TRACE},
	{"HEAD",HEAD} };
