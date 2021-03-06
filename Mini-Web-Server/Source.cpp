#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <iostream>
#include "FileHandler.h"
using namespace std;
// Don't forget to include "Ws2_32.lib" in the library list.
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <map> 
#include "RequestParser.h"
#include "RequestHandler.h"

#define MAX_TIMETOWAIT 120

struct SocketState
{
	SOCKET id;			// Socket handle
	sockaddr_in address;
	int	recv;			// Receiving?
	int	send;			// Sending?
	char buffer[128];
	char currMsg[128];
	int len;
	time_t lastUsed;
};


const int TIME_PORT = 8888;
const int MAX_SOCKETS = 60;
const int EMPTY = 0;
const int LISTEN  = 1;
const int RECEIVE = 2;
const int IDLE = 3;
const int SEND = 4;
const int SEND_TIME = 1;
const int SEND_SECONDS = 2;

bool addSocket(SOCKET id, sockaddr_in socketAddr, int what);
void removeSocket(int index);
void acceptConnection(int index);
void receiveMessage(int index);
void sendMessage(int index);

struct SocketState sockets[MAX_SOCKETS]={0};
int socketsCount = 0;

void main() 
{
	time_t currentTime;

    // Initialize Winsock (Windows Sockets).
	WSAData wsaData; 
    	
	if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &wsaData))
	{
        cout<<"Http Server: Error at WSAStartup()\n";
		return;
	}

	// Server side:
	// Create and bind a socket to an internet address.
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == listenSocket)
	{
        cout<<"Http Server: Error at socket(): "<<WSAGetLastError()<<endl;
        WSACleanup();
        return;
	}

	sockaddr_in serverService;
    serverService.sin_family = AF_INET; 
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(TIME_PORT);

    if (SOCKET_ERROR == bind(listenSocket, (SOCKADDR *) &serverService, sizeof(serverService))) 
	{
		cout<<"Http Server: Error at bind(): "<<WSAGetLastError()<<endl;
        closesocket(listenSocket);
		WSACleanup();
        return;
    }

    // Listen on the Socket for incoming connections.
	// This socket accepts only one connection (no pending connections 
	// from other clients). This sets the backlog parameter.
    if (SOCKET_ERROR == listen(listenSocket, 5))
	{
		cout << "Http Server: Error at listen(): " << WSAGetLastError() << endl;
        closesocket(listenSocket);
		WSACleanup();
        return;
	}
	addSocket(listenSocket, serverService, LISTEN);

    // Accept connections and handles them one by one.
	while (true)
	{
		//remove sockets that past timeout
		for (int i = 1; i < MAX_SOCKETS; i++)
		{
			currentTime = time(0);
			if ((sockets[i].recv != EMPTY || sockets[i].send != EMPTY) &&
				(sockets[i].lastUsed != 0) && (currentTime - sockets[i].lastUsed > MAX_TIMETOWAIT))
			{
				cout << "Time Server: Client " << inet_ntoa(sockets[i].address.sin_addr) << ":" << ntohs(sockets[i].address.sin_port) << " has been disconected (timeout)." << endl;
				removeSocket(i);
			}
		}
		// The select function determines the status of one or more sockets,
		// waiting if necessary, to perform asynchronous I/O. Use fd_sets for
		// sets of handles for reading, writing and exceptions. select gets "timeout" for waiting
		// and still performing other operations (Use NULL for blocking). Finally,
		// select returns the number of descriptors which are ready for use (use FD_ISSET
		// macro to check which descriptor in each set is ready to be used).
		fd_set waitRecv;
		FD_ZERO(&waitRecv);
		for (int i = 0; i < MAX_SOCKETS; i++)
		{
			if ((sockets[i].recv == LISTEN) || (sockets[i].recv == RECEIVE))
				FD_SET(sockets[i].id, &waitRecv);
		}

		fd_set waitSend;
		FD_ZERO(&waitSend);
		for (int i = 0; i < MAX_SOCKETS; i++)
		{
			if (sockets[i].send == SEND)
				FD_SET(sockets[i].id, &waitSend);
		}

		//
		// Wait for interesting event.
		// Note: First argument is ignored. The fourth is for exceptions.
		// And as written above the last is a timeout, hence we are blocked if nothing happens.
		//
		int nfd;
		nfd = select(0, &waitRecv, &waitSend, NULL, NULL);
		if (nfd == SOCKET_ERROR)
		{
			cout <<"Http Server: Error at select(): " << WSAGetLastError() << endl;
			WSACleanup();
			return;
		}

		for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
		{
			if (FD_ISSET(sockets[i].id, &waitRecv))
			{
				nfd--;
				switch (sockets[i].recv)
				{
				case LISTEN:
					acceptConnection(i);
					break;

				case RECEIVE:
					receiveMessage(i);
					break;
				}
			}
		}

		for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
		{
			if (FD_ISSET(sockets[i].id, &waitSend))
			{
				nfd--;
				switch (sockets[i].send)
				{
				case SEND:
					sendMessage(i);
					break;
				}
			}
		}
	}

	// Closing connections and Winsock.
	cout << "Http Server: Closing Connection.\n";
	closesocket(listenSocket);
	WSACleanup();
}

bool addSocket(SOCKET id, sockaddr_in socketAddr, int what)
{
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (sockets[i].recv == EMPTY)
		{
			sockets[i].id = id;
			sockets[i].recv = what;
			sockets[i].address = socketAddr;
			sockets[i].send = IDLE;
			sockets[i].len = 0;
			sockets[i].lastUsed = time(0);
			socketsCount++;
			return (true);
		}
	}
	return (false);
}

void removeSocket(int index)
{
	sockets[index].recv = EMPTY;
	sockets[index].send = EMPTY;
	socketsCount--;
}

void acceptConnection(int index)
{
	SOCKET id = sockets[index].id;
	struct sockaddr_in from;		// Address of sending partner
	int fromLen = sizeof(from);

	SOCKET msgSocket = accept(id, (struct sockaddr *)&from, &fromLen);
	if (INVALID_SOCKET == msgSocket)
	{ 
		 cout << "Http Server: Error at accept(): " << WSAGetLastError() << endl; 		 
		 return;
	}
	cout << "Http Server: Client "<<inet_ntoa(from.sin_addr)<<":"<<ntohs(from.sin_port)<<" is connected." << endl;

	//
	// Set the socket to be in non-blocking mode.
	//
	unsigned long flag=1;
	if (ioctlsocket(msgSocket, FIONBIO, &flag) != 0)
	{
		cout<<"Http Server: Error at ioctlsocket(): "<<WSAGetLastError()<<endl;
	}

	if (addSocket(msgSocket, from, RECEIVE) == false)
	{
		cout<<"\t\tToo many connections, dropped!\n";
		closesocket(id);
	}
	return;
}

void receiveMessage(int index)
{
	SOCKET msgSocket = sockets[index].id;

	int len = sockets[index].len;
	int bytesRecv = recv(msgSocket, &sockets[index].buffer[len+1], sizeof(sockets[index].buffer) - len + 1, 0);
	sockets[index].buffer[len] = bytesRecv;

	if (SOCKET_ERROR == bytesRecv)
	{
		cout << "Http Server: Error at recv(): " << WSAGetLastError() << endl;
		closesocket(msgSocket);			
		removeSocket(index);
		return;
	}
	if (bytesRecv == 0)
	{
		closesocket(msgSocket);			
		removeSocket(index);
		return;
	}
	else
	{
		sockets[index].buffer[len + 1 + bytesRecv] = '\0'; //add the null-terminating to make it a string
		cout<<"\nHttp Server: Recieved: "<<bytesRecv<<" bytes of \n\""<<&sockets[index].buffer[len + 1]<<"\" message.\n";

		//***************testing************
		sockets[index].len += bytesRecv + 1;
		
		if (sockets[index].len > 0)
		{
			int msgLength = sockets[index].buffer[0];
			sockets[index].send = SEND;
			memcpy(sockets[index].currMsg, &sockets[index].buffer[1], msgLength + 1 + 1);
			memcpy(sockets[index].buffer, &sockets[index].buffer[msgLength + 1], sockets[index].len - msgLength + 1);
			sockets[index].len -= msgLength + 1;
		}
		//************************************
	}

}

void sendMessage(int index)
{
	int bytesSent = 0;
	char sendBuff[1024];

	SOCKET msgSocket = sockets[index].id;
	sockets[index].lastUsed = time(0);

	RequestParser parser;
	RequestHandler handler;
	Request request;
	bool isParsed;
	string messegeToClient;

	isParsed = parser.Parse(request, sockets[index].currMsg);
	if (isParsed == true)
		handler.handle(request, messegeToClient);
	else
		handler.httpSendBadRequest(messegeToClient);
	strcpy(sendBuff, messegeToClient.c_str());

	bytesSent = send(msgSocket, sendBuff, (int)strlen(sendBuff), 0);
	if (SOCKET_ERROR == bytesSent)
	{
		cout << "Http Server: Error at send(): " << WSAGetLastError() << endl;	
		return;
	}

	cout<<"\n\nHttp Server: Sent: "<<bytesSent<<"\\"<<strlen(sendBuff)<<" bytes of \n\""<<sendBuff<<"\" message.\n";	

	sockets[index].send = IDLE;
}


