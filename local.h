#ifndef __LOCAL_H_
#define __LOCAL_H_

/*
 * Common header file: parent, producer and consumer
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>
#include <signal.h>
#include <time.h>
#include <GL/glut.h>
#include <pthread.h>
#include <mqueue.h>
#include <errno.h>

#define MAX_LINES 100
#define KEY_STORAGE 4444
#define KEY_SHELVES 5555
#define MaxItems 300
#define SHOPPINGTIMEMAX 10
#define SHOOPINGTIMEMIN 3
#define MAXITEMS2BUY 50
int countProcesses(const char *processName);
void generateCustomerProcess(int Number_Of_Products);
void terminateChildProcesses();
int getRandomNumber(int min, int max);
void generateCustomerArrivalProcess(int Customers_Arrival_Max, int Customers_Arrival_Min, int Number_Of_Products);
void readFromFile(const char *filename, int *array, int *numLines);
void readItemsAndStoreInStorageSharedMemory(const char *filename, int Number_Of_Products);
void readAndPrintItemSharedMemory(int Number_Of_Products);
int createSemaphore(int initialValue, int SEM_KEY);
void lockSemaphore(int semid);
void unlockSemaphore(int semid);
void DeleteSharedMemories();
void deleteSemaphore(int skey);
void OptimizedgetRandomItemsAndCalculateCart(double *cartPrice, int *cart, int Number);
void GetNumbers(int *CahsiersNumber, int *CustomersNumber, int *leftCahsiersNumber,int *leftCustomersNumber);
pid_t generateOpenGl();
void generateTeamsProcess(int NumberOfEmployees, int Number_Of_Products);
void readSharedMemoryData(int *NumberOfProductsInStorage,int *NumberOfProductsInSuperMarket,int shelves_quantity_on_shelves[MaxItems], int shelves_quantity_to_refill[MaxItems],
                          int storage_quantity[MaxItems], int shelves_id[MaxItems], int storage_id[MaxItems]);

struct StorageSharedmemory {
    char Name[MaxItems][30];
    int ID[MaxItems];
    int quantity[MaxItems];
    int emptyAll;
    int quantity_on_shelves[MaxItems];
};
struct ShelvesSharedmemory {
    char Name[MaxItems][30];
    int ID[MaxItems];
    int quantity_on_shelves[MaxItems];
    int quantity_to_refill[MaxItems];
    double price[MaxItems];
    int Filling[MaxItems];
};
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
struct ThreadArgs {
    struct ShelvesSharedmemory *shelves;
    struct StorageSharedmemory *storage;
    int semid;
};

#endif



