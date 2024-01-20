#include "local.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ShelveNeedRefill = PTHREAD_COND_INITIALIZER;
pthread_cond_t ShelvesFull = PTHREAD_COND_INITIALIZER;
int Number_Of_Products;
int Cart, ID;
volatile int should_exit = 0; // Shared flag
void sigint_handler(int signum) {
    should_exit = 1;
}
mqd_t mq; // Message queue descriptor

struct mq_attr attr;

void* TeamManager(void *args) {
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)args;
    struct ShelvesSharedmemory *shelves = threadArgs->shelves;
    struct StorageSharedmemory *storage = threadArgs->storage;
    int semid = threadArgs->semid;

    while (!should_exit) {
	pthread_mutex_lock(&mutex);
	while (Cart != 0) { 
	    pthread_cond_wait(&ShelveNeedRefill, &mutex);
	}
	if(ID!=getpid()){shelves->Filling[ID]=0;}
	int amount_to_refill=0;
	
	for (int i = 0; i < Number_Of_Products; i++) {
	    if (shelves->quantity_on_shelves[i] <= shelves->quantity_to_refill[i]) {
	    
	    //printf("\n\n================\nTM here12\n\n===================\n");
		if (storage->quantity[i] == 0 || shelves->Filling[i]!=0) {
		    continue;
		}
		
	    //printf("\n\n================\nTM here11\n\n\n");
	        
    	        lockSemaphore(semid);
		amount_to_refill = storage->quantity_on_shelves[i] - shelves->quantity_on_shelves[i];
		ID = i;
		shelves->Filling[i]=1;
		// Check if there's enough quantity in StorageSharedmemory
		if (storage->quantity[i] > amount_to_refill) {
		    storage->quantity[i] -= amount_to_refill;
		    printf("Refilled %s on cart with id %d with %d items.\n", shelves->Name[i], shelves->ID[i], amount_to_refill);
		    break;
		} else if (storage->quantity[i] <= amount_to_refill) {
		    printf("Not enough quantity of %s in storage to refill.\n", shelves->Name[i]);
		    amount_to_refill = storage->quantity[i];
		    storage->quantity[i] = 0;
		    //storage->ID[i]=0;
		    
		    break;
		}
		
	    }
	    unlockSemaphore(semid);
	}
	sleep(getRandomNumber(1,3));

	Cart = amount_to_refill;
	pthread_cond_broadcast(&ShelvesFull);
	pthread_mutex_unlock(&mutex);
	
    }
    pthread_exit(NULL);

    return NULL;
}

void* Employes(void *args) {
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)args;
    struct ShelvesSharedmemory *shelves = threadArgs->shelves;
    int semid = threadArgs->semid;

    while (!should_exit) {
    
	
	pthread_mutex_lock(&mutex);
	
	usleep(getRandomNumber(250000,500000));
	while (Cart == 0) { // Wait if buffer is empty
	    pthread_cond_wait(&ShelvesFull, &mutex);
	}
	

	if (Cart > 0) {
	    
	    
	    lockSemaphore(semid);
	    //printf("Refilling %s on Shelve.\n", shelves->Name[ID]);
	    shelves->Filling[ID]=2;
	    shelves->quantity_on_shelves[ID]++;
	    unlockSemaphore(semid);
	    Cart--;
	}
	pthread_cond_signal(&ShelveNeedRefill);
	pthread_mutex_unlock(&mutex);
	
	
}
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Number of Employees> <Number_Of_Products>\n", argv[0]);
        return 1;
    }
    int skey = getpid();
    int semid = createSemaphore(1, skey);
    int number = atoi(argv[1]);
    Number_Of_Products = atoi(argv[2]);
    pthread_t TeamMang;
    pthread_t Emp[number];
    key_t key_shelves = ftok(".", KEY_SHELVES);
    ID=getpid();
    if (key_shelves == -1) {
        perror("ftok shelves");
        exit(EXIT_FAILURE);
    }

    // Get the shared memory segment ID for shelves
    int shmid_shelves = shmget(key_shelves, sizeof(struct ShelvesSharedmemory), 0666);

    if (shmid_shelves == -1) {
        perror("shmget shelves");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment for shelves
    struct ShelvesSharedmemory *shelves = shmat(shmid_shelves, NULL, 0);

    if (shelves == (struct ShelvesSharedmemory *)-1) {
        perror("shmat shelves");
        exit(EXIT_FAILURE);
    }

    key_t key_storage = ftok(".", KEY_STORAGE);

    if (key_storage == -1) {
        perror("ftok storage");
        exit(EXIT_FAILURE);
    }

    // Get the shared memory segment ID for storage
    int shmid_storage = shmget(key_storage, sizeof(struct StorageSharedmemory), 0666);

    if (shmid_storage == -1) {
        perror("shmget storage");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment for storage
    struct StorageSharedmemory *storage = shmat(shmid_storage, NULL, 0);

    if (storage == (struct StorageSharedmemory *)-1) {
        perror("shmat storage");
        exit(EXIT_FAILURE);
    }

    struct ThreadArgs Args;
    Args.shelves = shelves;
    Args.storage = storage;
    Args.semid = semid;
    Cart=0;
    pthread_cond_signal(&ShelveNeedRefill);
    // Detach from the shared memory segments
    pthread_create(&TeamMang, NULL, TeamManager, (void *)&Args);

    signal(SIGINT, sigint_handler);
    for (int i = 0; i < number; i++) {
        pthread_create(&Emp[i], NULL, Employes, (void *)&Args);
    }
    
    pthread_join(TeamMang, NULL);
    // Join Team Manager thread
    
    for (int i = 0; i < number; i++) {
        pthread_join(Emp[i], NULL);
    }

    
    
    
    shmdt(shelves);
    shmdt(storage);

    return 0;
}

