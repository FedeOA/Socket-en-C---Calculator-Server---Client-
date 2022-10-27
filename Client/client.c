#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include "client.h"
#include <time.h>
struct ClientStruct
{

    WSADATA wsaData;
    SOCKET sClient;
    SOCKADDR_IN clientAddr;
    char bufferClient[256];

};

Client createClient(WSADATA wsaData,SOCKET sClient,SOCKADDR_IN clientAddr,char bufferC[256])
{

    Client c=malloc (sizeof(struct ClientStruct));

    c->wsaData=wsaData;
    c->sClient=sClient;
    c->clientAddr=clientAddr;
    strcpy(c->bufferClient,bufferC);

    return c;

}

void startClient(Client client,int port)
{

    long response;

    response=WSAStartup(MAKEWORD(2,0),&client->wsaData);

    if (response==0)
    {
        printf("WSAStartup()\t\t successful\n");
    }
    else
    {
        printf("error WSAStartup():\n",WSAGetLastError());
    }

    client->sClient=socket(AF_INET,SOCK_STREAM,0);

    if (client->sClient!= INVALID_SOCKET)
    {
        printf("socket() \t\t successful\n");
    }
    else
    {
        printf("error socket():\n",WSAGetLastError());
    }

    client->clientAddr.sin_addr.s_addr=inet_addr("192.168.0.203");
    client->clientAddr.sin_family = AF_INET;
    client->clientAddr.sin_port=htons(port);


    response= connect(client->sClient,(SOCKADDR*)&client->clientAddr,sizeof(client->clientAddr));

    if (response != SOCKET_ERROR)
    {
        printf("connect() \t\t successful\n");
    }
    else
    {
        printf("not connected(): \n",WSAGetLastError());
    }

}

void menu(Client client)
{

    time_t t;

    t=time(NULL);

    do
    {

        printf("Seleccione una opcion:\n");
        printf("a.Realizar calculo\nb.Ver registro de actividades\nc.Cerrar sesion.\n");
        scanf("%s",client->bufferClient);
        if (time(NULL)>t+10)
        {

            client->bufferClient[0]='d';
            SendM(client);

            closesocket(client->sClient);

            printf("Cliente desconectado por inactividad.\n");
            Sleep(1000);
            system("cls");
            main();

        }
        t=time(NULL);

        if (client->bufferClient[0]!='a' && client->bufferClient[0]!='b' && client->bufferClient[0]!='c' || strlen(client->bufferClient)!=1)
        {

            system("cls");
            printf("Debe seleccionar una opcion correcta\n");
            Sleep(1000);
            system("cls");

        }


    }
    while (client->bufferClient[0]!='a' && client->bufferClient[0]!='b' && client->bufferClient[0]!='c' || strlen(client->bufferClient)!=1);


    system("cls");
    if (client->bufferClient[0]=='a'&& strlen(client->bufferClient)==1)
    {
        printf("Ingrese la operacion que desea realizar:\n");

        readInput(client);

        if (time(NULL)>t+10)
        {

            client->bufferClient[0]='d';
            SendM(client);
            closesocket(client->sClient);
            printf("Cliente desconectado por inactividad.\n");
            Sleep(1000);
            system("cls");
            return main();


        }
        t=time(NULL);
        if (strcmp(client->bufferClient,"volver")==0)
        {
            system("cls");
            menu(client);
        }


    }

    if (client->bufferClient[0]=='b' && strlen(client->bufferClient)==1)



    {

        printf("Respuesta del servidor: \n\n");
        while (strlen(client->bufferClient)!=0)
        {


            SendM(client);


            recv(client->sClient,client->bufferClient,sizeof(client->bufferClient),0);

            printf("%s",client->bufferClient);

            if (strlen(client->bufferClient)!=0)
            {
                strcpy(client->bufferClient,"b");
            }
        }

        if (strlen(client->bufferClient)==0)
        {



            Sleep(1000);
            system("cls");
            menu(client);
        }
    }

    if (client->bufferClient[0]=='c' && strlen(client->bufferClient)==1)
    {
        SendM(client);
        closesocket(client->sClient);
        exit(0);

    }

}

void SendM(Client client)
{
    send(client->sClient, client->bufferClient, sizeof(client->bufferClient), 0);
    memset(client->bufferClient, 0, sizeof(client->bufferClient));
    Sleep(1000);

}


void ReceiveM(Client client)
{

    memset(client->bufferClient, 0, sizeof(client->bufferClient));
    recv(client->sClient,client->bufferClient,sizeof(client->bufferClient),0);
    printf("Respuesta del servidor: \n");
    printf("%s",client->bufferClient);
    memset(client->bufferClient, 0, sizeof(client->bufferClient));

    Sleep(5000);
    system("cls");


}

void readInput(Client client)
{
    if(client->bufferClient[strlen(client->bufferClient) - 1] != '\n')
    {
        //Limpiamos el búfer
        while(getchar() != '\n');
    }
    fgets(client->bufferClient, 256, stdin);
    if ((strlen(client->bufferClient) > 0) && (client->bufferClient[strlen(client->bufferClient) - 1] == '\n'))
    {
        client->bufferClient[strlen(client->bufferClient) - 1] = '\0';
    }

}



