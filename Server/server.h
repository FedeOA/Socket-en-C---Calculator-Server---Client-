#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED
#include <winsock2.h>

struct ServerStruct ;

typedef struct ServerStruct *Server;

Server createServer(WSADATA WSAData,SOCKET server,SOCKET client,SOCKADDR_IN serverAddr,SOCKADDR_IN clientAddr,char buffer[256]);

void startServer(Server s,int port,int cont);

void CloseSocket(Server server);

void receive(Server server);

void validateInput(Server server);

void getDayAndHour();

void printText(char text[50]);

void calculate(Server server);

char* searchOperator(Server server);

int searchPositionOperator(Server server);

void switchOption(Server server);

void makeResponse(Server server,int i,char messageDefault[100]);

void loadBufferWithFileLog(Server server);
#endif // SERVER_H_INCLUDED
