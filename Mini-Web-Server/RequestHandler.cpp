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

	//switch 
	return 0;
}

bool RequestHandler::httpGET(Request i_request, string &o_processedMsg) {
	o_processedMsg = "it was a get messege";
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
