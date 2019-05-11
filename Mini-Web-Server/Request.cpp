#include "Request.h";

bool Request::isValid(string i_method, string i_version) {
	if ((i_method == "GET" ||
		i_method == "PUT" ||
		i_method == "DELETE" ||
		i_method == "OPTIONS" ||
		i_method == "TRACE" ||
		i_method == "HEAD") &&
		i_version == "HTTP/1.1") {
		return true;
	}
	return false;
}

Request::Method Request::methodType() {
	return this->m_method;
}

bool Request::setRequest(string i_method,
						 string i_requestUri,
						 string i_version,
						 vector<pair<string,string>> i_headers,
						 string i_body,
						 string i_httpRequest)
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
	this->m_rawMsg = i_httpRequest;
	return isValid(i_method, i_version);

}

map<string, Request::Method> Request::m_methodMap = {
	{"GET", GET},
	{"PUT", PUT},
	{"OPTIONS", OPTIONS},
	{"DELETE", eDELETE},
	{"TRACE", TRACE},
	{"HEAD",HEAD} };
