#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

struct ClientStruct;

typedef struct ClientStruct *Client;

Client createClient(WSADATA WSAData,SOCKET sClient,SOCKADDR_IN clientAddr,char bufferC[20]);

void startClient(Client client,int port);

void SendM(Client client);

void menu(Client client);

void ReceiveM(Client client);

void readInput(Client client);

#endif // CLIENT_H_INCLUDED
