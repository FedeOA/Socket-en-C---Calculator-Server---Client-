#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <winsock2.h>

int main()
{





    WSADATA WSAData;
    SOCKET _server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    char bufferr[256]="";
    int cont=0;
    Server server=createServer(WSAData,_server,client,serverAddr,clientAddr,bufferr);

    startServer(server,5000,cont);


    while(1)
    {


     receive(server);

    }


    return 0;

}
