#include "RequestHandler.h"

bool RequestHandler::handle(Request i_request, string &o_messege) {
	bool validRequest;
	string processedMsg;
	validRequest = i_request.isValid();

	if (!validRequest) {
		return 1;
	}

	switch (i_request.methodType()) {
	case Request::GET :
		httpGET(i_request, processedMsg);
		break;
	case Request::PUSH:
		httpPUSH(i_request, processedMsg);
		break;
	case Request::OPTIONS:
		httpOPTIONS(i_request, processedMsg);
		break;
	case Request::eDELETE:
		httpDELETE(i_request, processedMsg);
		break;
	case Request::TRACE:
		httpTRACE(i_request, processedMsg);
		break;
	case Request::HEAD:
		httpHEAD(i_request, processedMsg);
		break;
	}

	//switch 
	return 0;
}

bool RequestHandler::httpGET(Request i_request, string &o_processedMsg) {
	return true;
}
bool RequestHandler::httpPUSH(Request i_request, string &o_processedMsg) {
	return true;
}
bool RequestHandler::httpOPTIONS(Request i_request, string &o_processedMsg) {
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
