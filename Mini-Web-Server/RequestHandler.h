#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#include "Request.h"
class RequestHandler {
public:
	bool handle(Request i_request, string &o_messege);
private:
	bool httpGET(Request i_request, string &o_processedMsg);
	bool httpPUSH(Request i_request, string &o_processedMsg);
	bool httpOPTIONS(Request i_request, string &o_processedMsg);
	bool httpDELETE(Request i_request, string &o_processedMsg);
	bool httpTRACE(Request i_request, string &o_processedMsg);
	bool httpHEAD(Request i_request, string &o_processedMsg);
};

#endif