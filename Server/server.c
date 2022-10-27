#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <time.h>

struct ServerStruct
{

    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    char buffer[256];
};

Server createServer(WSADATA WSAData,SOCKET server,SOCKET client,SOCKADDR_IN serverAddr,SOCKADDR_IN clientAddr,char buffer[256])
{

    Server s=malloc (sizeof(struct ServerStruct));

    s->WSAData=WSAData;
    s->server=server;
    s->client=client;
    s->serverAddr=serverAddr;
    s->clientAddr=clientAddr;
    strcpy(s->buffer,buffer);

    return s;
}

void startServer(Server s,int port,int cont)
{

    long res;
    WSADATA WsaAux= s->WSAData;


    res=WSAStartup(MAKEWORD(2,0), &WsaAux);

    if (res==0)
    {

        printf("WSAStartup: succesful\n");
        getDayAndHour();
        printText("==================================");
        getDayAndHour();
        printText("==========INICIA SERVIDOR=========");
        getDayAndHour();
        printText("==================================");
    }
    else
    {

        printf("errpor WSAStartup\n");
    }

    s->server = socket(AF_INET, SOCK_STREAM, 0);

    if (s->server!=INVALID_SOCKET)
    {
        printf("Socket : succesful\n");
    }
    else
    {
        printf("Error Socket\n");
    }

    s->serverAddr.sin_addr.s_addr =inet_addr("192.168.0.203");
    s->serverAddr.sin_family = AF_INET;
    s->serverAddr.sin_port = htons(port);

    getDayAndHour();

    char message[50]="Socket creado.Puerto de escucha:";
    printText(strcat(message,itoa(port,s->buffer,10)));



    res=bind(s->server, (SOCKADDR *)&s->serverAddr, sizeof(s->serverAddr));

    if (res!=SOCKET_ERROR)
    {
        printf("Bind: Succesful\n");
    }
    else
    {
        printf("Error Bind\n");
    }


    res=listen(s->server, 0);

    if (res!=SOCKET_ERROR)
    {
        printf("Listen: Succesful\n");
    }
    else
    {
        printf("Error Listen\n");
    }

    printf("Escuchando conexiones entrantes!\n");


    int clientAddrSize = sizeof(s->clientAddr);

    if((s->client = accept(s->server, (SOCKADDR *)&s->clientAddr, &clientAddrSize)) != INVALID_SOCKET)
    {
        getDayAndHour();
        printText("Conexión Aceptada.");
        printf("Cliente conectado!\n");
    }




}

void receive(Server server)
{


    memset(server->buffer, 0, sizeof(server->buffer));
    recv(server->client, server->buffer, sizeof(server->buffer), 0);


    if ((server->buffer[0]=='c'|| server->buffer[0]=='b'||server->buffer[0]=='d')&& strlen(server->buffer)==1)
    {

        switchOption(server);

    }
    else
    {

        validateInput(server);


    }


}




void switchOption(Server server)
{
    int res=0;
    int clientAddrSize = sizeof(server->clientAddr);

    switch(server->buffer[0])
    {


    case 'b':
        loadBufferWithFileLog(server);


        break;

    case 'c':

        getDayAndHour();
        printText("CLIENTE DESCONECTADO.");
        memset(server->buffer, 0, sizeof(server->buffer));
        closesocket(server->client);

        if((server->client = accept(server->server, (SOCKADDR *)&server->clientAddr, &clientAddrSize)) != INVALID_SOCKET)
        {
            getDayAndHour();
            printText("Conexión Aceptada.");
            printf("Cliente conectado!\n");
        }

        receive(server);


        break;

    case 'd':

        getDayAndHour();
        printText("CLIENTE DESCONECTADO POR INACTIVIDAD.");
        memset(server->buffer, 0, sizeof(server->buffer));
        closesocket(server->client);

        if((server->client = accept(server->server, (SOCKADDR *)&server->clientAddr, &clientAddrSize)) != INVALID_SOCKET)
        {
            getDayAndHour();
            printText("Conexión Aceptada.");
            printf("Cliente conectado!\n");
        }

        receive(server);


        break;


    }

}


void CloseSocket(Server server)
{

    closesocket(server->client);
    closesocket(server->server);
    printf("Socket cerrado, cliente desconectado.\n");
}



void validateInput(Server server)
{

    char* messageCharacter[2];
    char messageDefault[100]="";
    int contOperators=0;

    //Validamos que la operacion comience con un numero entero positivo

    if ((server->buffer[0]!='1' && server->buffer[0]!='2' && server->buffer[0]!='3' && server->buffer[0]!='4' &&server->buffer[0]!='5' &&server->buffer[0]!='6' &&server->buffer[0]!='7' &&
            server->buffer[0]!='8' && server->buffer[0]!='9' && server->buffer[0]!='0')&& (server->buffer[0]=='/' || server->buffer[0]=='+'
                    || server->buffer[0]=='*'|| server->buffer[0]=='-' ||server->buffer[0]=='^')|| server->buffer[0]=='!')
    {

         makeResponse(server,0,messageDefault);

        // Validamos el formato de la operacion


    }
    else if (server->buffer[strlen(server->buffer)-1]=='/' || server->buffer[strlen(server->buffer)-1]=='+'
             || server->buffer[strlen(server->buffer)-1]=='*'|| server->buffer[strlen(server->buffer)-1]=='-' ||server->buffer[strlen(server->buffer)-1]=='^')
    {
        char aux[3];
        aux[0]=server->buffer[strlen(server->buffer)-2];
        aux[1]=server->buffer[strlen(server->buffer)-1];

        strcpy(messageDefault," No se pudo realizar la operacion, la operacion esta mal formada: ");
        strcpy(server->buffer,strcat(messageDefault,aux));



    }

    else if (strlen(server->buffer)>=20 || strlen(server->buffer)<1)
    {

        strcpy(messageDefault,"La operacion debe tener entre 1 y 20 caracteres.\n");
        strcpy(server->buffer,messageDefault);

    }
    else
    {

        int i=0;
        for (i; i<strlen(server->buffer); i++)
        {

            // Validamos que el caracter del factorial sea el ultimo en caso que exista dicho caracter
            if (server->buffer[i]=='!' && strlen(server->buffer)-1 != i)
            {
                makeResponse(server,i,messageDefault);
                i=strlen(server->buffer);
            }
            else if (server->buffer[i]=='+' || server->buffer[i]=='-' || server->buffer[i]=='*' || server->buffer[i]=='/' || server->buffer[i]=='!' ||
                     server->buffer[i]=='^')
            {

                contOperators++;

                if(contOperators>1)
                {
                    strcpy(messageDefault,"No pueden existir mas de dos operadores\n");
                    strcpy(server->buffer,messageDefault);
                    i=strlen(server->buffer);

                }
                else if(server->buffer[i+1]=='+' || server->buffer[i+1]=='-' || server->buffer[i+1]=='*' || server->buffer[i+1]=='/' || server->buffer[i+1]=='!' ||
                        server->buffer[i+1]=='^')
                {

                    makeResponse(server,i,messageDefault);
                    i=strlen(server->buffer);
                }
                // Validamos que todos los caracteres sean correctos
            }
            else if (server->buffer[i]!='0' && server->buffer[i]!='1' && server->buffer[i]!='2' && server->buffer[i]!='3' && server->buffer[i]!='4' &&server->buffer[i]!='5' &&server->buffer[i]!='6' &&server->buffer[i]!='7' &&

                     server->buffer[i]!='8' && server->buffer[i]!='9' && server->buffer[i]!='+' && server->buffer[i]!='-' && server->buffer[i]!='*' && server->buffer[i]!='/' && server->buffer[i]!='!' &&
                     server->buffer[i]!='^' )
            {

                messageCharacter[0]=server->buffer[i];

                strcpy(messageDefault,"No se pudo realizar la operación, se encontro un caracter no contemplado: ");
                strcpy(server->buffer,strcat(messageDefault,messageCharacter));

                i=strlen(server->buffer);

            }


        }

    }

    //si pasa las validaciones calcula; sino envia respuesta de error
    if (strlen(messageDefault)==0)
    {
        calculate(server);
    }
    else
    {
        send(server->client, server->buffer, sizeof(server->buffer), 0);
        memset(server->buffer, 0, sizeof(server->buffer));
    }
}

void getDayAndHour()
{

    int hours, minutes, seconds, day, month, year;

    time_t now;

    time(&now);

    struct tm *local = localtime(&now);

    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;

    FILE *punteroArchivo;
    punteroArchivo=fopen ("server.log.txt","a");

    fprintf(punteroArchivo,"%02d/%02d/%d_", day, month, year);
    if (hours < 12)      // antes del mediodia
    {
        fprintf(punteroArchivo,"%02d:%02d:%02dam :", hours, minutes, seconds);
    }
    else      // Después de mediodía
    {
        fprintf(punteroArchivo,"%02d:%02dpm: ",hours,minutes);
    }

    fclose(punteroArchivo);

}

void printText(char text[256])
{

    FILE *punteroArchivo;
    punteroArchivo=fopen ("server.log.txt","a");
    fprintf(punteroArchivo,"%s\n",text);
    fclose(punteroArchivo);


}

char* searchOperator(Server server)
{

    for (int i=0; i<strlen(server->buffer); i++)
    {

        if(server->buffer[i]=='+' || server->buffer[i]=='-' || server->buffer[i]=='*' || server->buffer[i]=='/' || server->buffer[i]=='!' ||
                server->buffer[i]=='^')
        {
            return server->buffer[i];

        }
    }
}

int searchPositionOperator(Server server)
{

    for (int i=0; i<strlen(server->buffer); i++)
    {

        if(server->buffer[i]=='+' || server->buffer[i]=='-' || server->buffer[i]=='*' || server->buffer[i]=='/' || server->buffer[i]=='!' ||
                server->buffer[i]=='^')
        {
            return i;

        }
    }
}


void calculate(Server server)
{

    char option;
    option=searchOperator(server);
    int positionOperator=searchPositionOperator(server);
    int c=0;
    char aux[10];
    char aux2[10];

    for (int i=0; i<positionOperator; i++)
    {

        aux[i]=server->buffer[i];

    }


    for (int i=positionOperator+1; i<strlen(server->buffer); i++)
    {

        aux2[i-(positionOperator+1)]=server->buffer[i];

    }


    int firstN=atoi(aux);
    int secondN=atoi(aux2);

    int result=0;

    switch (option)
    {

    case '+':
        result=firstN+secondN;
        strcpy(server->buffer,itoa(result,server->buffer,10));
        break;

    case '-':
        result=firstN-secondN;
        strcpy(server->buffer,itoa(result,server->buffer,10));
        break;

    case '*':
        result=firstN*secondN;
        strcpy(server->buffer,itoa(result,server->buffer,10));
        break;

    case '/':
        result=firstN/secondN;
        strcpy(server->buffer,itoa(result,server->buffer,10));
        break;

    case '^':
        result=pow(firstN,secondN);
        strcpy(server->buffer,itoa(result,server->buffer,10));
        break;

    case '!':
        result=1;
        for (c = 1; c <=firstN; c++)
        {
            result = result * c;
        }
        strcpy(server->buffer,itoa(result,server->buffer,10));
        break;
    }
    send(server->client, server->buffer, sizeof(server->buffer), 0);
    memset(server->buffer, 0, sizeof(server->buffer));



}


void loadBufferWithFileLog(Server server)
{

    FILE* punteroArchivo=fopen ("server.log.txt","r");

    while (!feof(punteroArchivo))
    {
        char aux[256]=" ";


        if (server->buffer[0]=='b')
        {
            memset(server->buffer, 0, sizeof(server->buffer));
            fgets(aux,256,punteroArchivo);
            strcpy(server->buffer,aux);
            send(server->client, server->buffer, sizeof(server->buffer), 0);

            recv(server->client, server->buffer, sizeof(server->buffer), 0);


        }


    }
    memset(server->buffer, 0, sizeof(server->buffer));
    send(server->client, server->buffer, sizeof(server->buffer), 0);

    fclose(punteroArchivo);


}


void makeResponse(Server server,int i,char messageDefault[100])
{


    char messageCharacter2[5]="";

    if
        (server->buffer[0]=='/' || server->buffer[0]=='+'
            || server->buffer[0]=='*'|| server->buffer[0]=='-' ||server->buffer[0]=='^' || server->buffer[0]=='!')
    {
        if (strlen(server->buffer)!=1){
        messageCharacter2[0]=server->buffer[i];
        messageCharacter2[1]=server->buffer[i+1];
        }else{

         messageCharacter2[0]=server->buffer[i];

        }
    }
    else if((server->buffer[i+1]=='/' || server->buffer[i+1]=='+'
            || server->buffer[i+1]=='*'|| server->buffer[i+1]=='-' ||server->buffer[i+1]=='^' || server->buffer[i+1]=='!'))

    {

        messageCharacter2[0]=server->buffer[i];
        messageCharacter2[1]=server->buffer[i+1];
        messageCharacter2[2]=server->buffer[i+2];

    }




        else
        {

            messageCharacter2[0]=server->buffer[i-1];
            messageCharacter2[1]=server->buffer[i];
            messageCharacter2[2]=server->buffer[i+1];

        }
    strcpy(messageDefault,"No se pudo realizar la operacion, la operacion esta mal formada:\t\t");
    strcpy(server->buffer,strcat(messageDefault,messageCharacter2));
    }




















