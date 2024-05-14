#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define BUFFER_SIZE 5
#define MAX_ITERATIONS 20

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int totalProduced = 0;
int totalConsumed = 0;
HANDLE emptySemaphore , fullSemaphore, mutex;

void printBuffer(){
    printf("[");
    for(int i = 0; i < BUFFER_SIZE; i++){
        printf("%d ", buffer[i]);
    }
    printf("]");
    printf("\n");
}

DWORD WINAPI producer(LPVOID lpParam){
    int item;
    while(totalProduced < MAX_ITERATIONS){
        item = rand()%20;
        WaitForSingleObject(emptySemaphore, INFINITE);
        WaitForSingleObject(mutex, INFINITE);

        buffer[in] = item;
        printf("Produced %d at position%d\n", item, in);
        printBuffer();

        in = (in+1) % BUFFER_SIZE;
        totalProduced++;

        ReleaseSemaphore(mutex,1,NULL);
        ReleaseSemaphore(fullSemaphore, 1, NULL);
        Sleep(1000);
    }
    printf("Producer finished\n");
    return 0 ;

}

DWORD WINAPI consumer(LPVOID lpParam){
    int item ;
    while(totalConsumed < MAX_ITERATIONS){
        WaitForSingleObject(fullSemaphore, INFINITE);
        WaitForSingleObject(mutex, INFINITE);

        item = buffer[out];
        printf("Consumed %d from position %d\n", item, out);
        buffer[out] = 0;
        printBuffer();

        out = (out+1) % BUFFER_SIZE;
        totalConsumed++;

        ReleaseSemaphore(mutex, 1, NULL);
        ReleaseSemaphore(emptySemaphore, 1, NULL);

        sleep(2000);
    }
    printf("Consumer has finished conusmig\n");
    return 0;

}
int main(){
    emptySemaphore = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    fullSemaphore = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
    mutex = CreateSemaphore(NULL, 1,1,NULL);
    HANDLE PRODUCERThread = CreateThread(NULL,0,producer,NULL,0,NULL);
    HANDLE consumerThread = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

    WaitForSingleObject(PRODUCERThread, INFINITE);
    WaitForSingleObject(consumerThread, INFINITE);

    CloseHandle(PRODUCERThread);
    CloseHandle(consumerThread);

    CloseHandle(emptySemaphore);
    CloseHandle(fullSemaphore);
    CloseHandle(mutex);

    return 0;
}