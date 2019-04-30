#ifndef REQUEST_H

#include <string.h>
#include <map> 
using namespace std;

struct Request {
	string method;
	string requestUri;
	string version;
	map<string, string> headers;
	string body;
};


#endif
