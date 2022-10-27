#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include "client.h"
#include <time.h>
int main()
{

    int port=0;
    WSADATA WSAData;
    SOCKET sClient;
    SOCKADDR_IN clientAddr;
    char bufferClient[256];



    Client client=createClient(WSAData,sClient,clientAddr,bufferClient);
    printf(":::::::::::::::::::::::::::::::::::::::::Bienvenido :::::::::::::::::::::::::::::::::::::::::\n\nSeleccione el puerto por el cual desea conectarse\n");
    scanf("%d",&port);
    startClient(client,port);
    Sleep(1000);
    system("cls");



    while (1)
    {

        menu(client);
        system("cls");
        SendM(client);
        system("cls");
        ReceiveM(client);


    }


    return 0;
}
