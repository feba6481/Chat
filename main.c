#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

void sendMode();

int calcKey(int sock , int mode);
int recieveB(int A, int mode, int sock);
void encrypt(char inputString[], int n, int key[], int sock);
void recieveMode();
void decrypt(char inputString[], int n, int key[]);
int getP();
int getG();

void sendKey(int key[], int n, int sock, int mode);
int establishConnectionToServer(char ip[]);
int startServer();

int main(void)
{
    srand(time(NULL));
    int mode = 0;
    printf("MODE: ");
    scanf("%d", &mode);
    if(mode)
        sendMode();
    else
        recieveMode();
    return 0;
}

//SENDMODE BEGIN

void sendMode()
{
    int sock = establishConnectionToServer("127.0.0.1");
    char inputString[256];
    printf("Bitte zu verschluesselnde Nachricht eingeben: ");
    scanf("%s", inputString);
    int n = strlen(inputString);
    int key[n];
    char tempi[1];
    tempi[0] = n;
    if(!send(sock , tempi, 1, 0))
        printf("PROBLEMO");
    sendKey(key, n, sock, 1);

    encrypt(inputString, n, key, sock);
    puts(inputString);
    close(sock);
}

void sendKey(int key[], int n, int sock, int mode)
{
    int i = 0;
    for(i = 0; i<n; i++)
    {
        key[i] = calcKey(sock, mode);
        printf("Key: %d\n", key[i]);
    }
}

int establishConnectionToServer(char ip[])
{
    int sock;
    struct sockaddr_in server;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    return sock;
}


int calcKey(int sock, int mode)
{
    int p = getP(), g = getG();
    int a = rand()%10+2;
    int A = (long)pow(g, a)%p;
    printf("A: %d = %d^%d mod %d\n",A, g, a, p);
    int B = recieveB(A, mode, sock);
    int key = (long)pow(B, a)%p;
    printf("K: %d = %d^%d mod %d\n", key, B, a, p);
    return key;
}

int recieveB(int A, int mode, int sock)
{
    int B = 0;
    if(mode)
    {
        char tempi[1];
        tempi[0]= A;
        send(sock , tempi, strlen(tempi), 0);
        printf("Sent: %d\n", A);
        if(!recv(sock , tempi, 2000 , 0))
            printf("Error Recieving");
        B = tempi[0];
        printf("Recieved: %d\n", B);
    }
    else
    {
        char tempi[1];
        recv(sock , tempi, 2000 , 0);
        B = tempi[0];
        tempi[0]= A;
        if(!send(sock , tempi, strlen(tempi), 0))
            printf("Error Sending");
        printf("Sent: %d\n", A);

    }
    return B;
}

void encrypt(char inputString[], int n, int key[], int sock)
{
    int i = 0;
    for(i = 0; i<n; i++)
    {
        inputString[i] = inputString[i] + key[i]; //TODO: actual encryption
    }
    send(sock , inputString, strlen(inputString), 0);
}

//SENDMODE END

//RECIEVER

void recieveMode()
{
    int sock = startServer();
    char inputString[256];
    char tempi[1];
    recv(sock, tempi, 2000, 0);
    int n = tempi[0];
    int key[n];
    sendKey(key, n, sock , 0);
    if(!recv(sock , inputString, 2000 , 0))
        printf("Error Recieving");
    decrypt(inputString, n, key);
    printf("%s\n", inputString);
    close(sock);
}

int startServer()
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    return client_sock;
}


void decrypt(char inputString[], int n, int key[])
{
    int i = 0;
    for(i = 0; i<n; i++)
    {
        //TODO: Recieve B
        inputString[i] = inputString[i] - key[i]; //TODO: actual encryption

    }
}

//RECIEVER END


int getP()
{
    return 13;
}

int getG()
{
    return 2;
}
