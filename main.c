#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

void sendMode();
int calcKey();
int recieveB(int A);
void encrypt(char inputString[], int n, int key);
void recieveMode();
void decrypt(char inputString[], int n, int key);
int getP();
int getG();

int main(void)
{
    srand(time(NULL));

    //TODO: Chatroom
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
    char inputString[256];
    int n = 3;
    int key = calcKey();
    printf("Key: %d Bitte zu verschluesselnde Nachricht eingeben: ", key);
    scanf("%s", inputString);
    encrypt(inputString, n, key);
    puts(inputString);
}

int calcKey()
{
    int p = getP(), g = getG();
    int a = rand()%45+1;
    int A = (int)pow(g, a)%p;
    int B = recieveB(A);
    int key = (int)pow(B, a)%p;
    return key;
}

int recieveB(int A)
{
    //TODO send A
    printf("%d\n", A);
    int B = 0; //TODO: get B
    printf("B: ");
    scanf("%d", &B);

    return B;
}

void encrypt(char inputString[], int n, int key)
{
    int i = 0;
    for(i = 0; i<n; i++)
    {
        inputString[i] = inputString[i] + key; //TODO: actual encryption
        //TODO send tempi
    }
}

//SENDMODE END

//RECIEVER

void recieveMode()
{
    char inputString[256];
    int n = 3;
    int key = calcKey();
    printf("Key: %d Zu entschluesselnde Nachricht eingeben: ", key);
    scanf("%s", inputString);
    decrypt(inputString, n, key);
    printf("%s", inputString);
}

void decrypt(char inputString[], int n, int key)
{
    int i = 0;
    for(i = 0; i<n; i++)
    {
        //TODO: Recieve B
        inputString[i] = inputString[i] - key; //TODO: actual encryption

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

