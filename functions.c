#include "local.h"

//To generate a random number between min & max
int getRandomNumber(int min, int max) {
    if (min > max) {
        //printf("Error: min should be less than or equal to max.\n");
        return -1; // Return an error code
    }

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Calculate the range and generate a random number within that range
    int range = max - min + 1;
    int randomNumber = rand() % range + min;

    return randomNumber;
}


//
void readFromFile(const char *filename, int *array, int *numLines) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    *numLines = 0;
    while (fscanf(file, "%*[^0-9]%d", &array[*numLines]) == 1) {
        (*numLines)++;
        if (*numLines >= MAX_LINES) {
            printf("Too many lines in the file. Increase MAX_LINES if needed.\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

void readItemsAndStoreInStorageSharedMemory(const char *filename, int Number_Of_Products) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Create or attach to the shared memory segment
    key_t key = ftok(".", KEY_STORAGE);
    int shmid = shmget(key, sizeof(struct StorageSharedmemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    struct StorageSharedmemory *sharedMemory = shmat(shmid, NULL, 0);
    if (sharedMemory == (struct StorageSharedmemory *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    
    
    
    
    
    
    
    
    // Create or attach to the shared memory segment
    key_t key2 = ftok(".", KEY_SHELVES);
    int shmid2 = shmget(key2, sizeof(struct ShelvesSharedmemory), IPC_CREAT | 0666);
    if (shmid2 == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    struct ShelvesSharedmemory *sharedMemory2 = shmat(shmid2, NULL, 0);
    if (sharedMemory2 == (struct ShelvesSharedmemory *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    
    
    
    
    
    
    
    


    // Read items from the file and store them in shared memory
    int i = 0;
    char name[30];
    while (fscanf(file, "%29[^,],%d,%d,%d,%d,%lf\n", name, &sharedMemory2->ID[i], &sharedMemory->quantity[i], &sharedMemory2->quantity_on_shelves[i],&sharedMemory2->quantity_to_refill[i],&sharedMemory2->price[i]) == 6) {
        
        sharedMemory->ID[i]=sharedMemory2->ID[i];
    	strcpy(sharedMemory->Name[i], name);
    	strcpy(sharedMemory2->Name[i], name);
    	sharedMemory->quantity_on_shelves[i]=sharedMemory2->quantity_on_shelves[i];
        i++;
        if(Number_Of_Products==i){
            break;
        }
        if (i >= MaxItems) {
            printf("Warning: Maximum number of items reached .\n");
            break;
        }
    }

    // Detach from shared memory
    shmdt(sharedMemory);
    shmdt(sharedMemory2);

    // Close the file
    fclose(file);
}
void readAndPrintItemSharedMemory(int Number_Of_Products) {
    key_t key = ftok(".", KEY_STORAGE);
    int shmid = shmget(key, sizeof(struct StorageSharedmemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    struct StorageSharedmemory *sharedMemory = shmat(shmid, NULL, 0);
    if (sharedMemory == (struct StorageSharedmemory *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    
    
    
    
    
    
    
    
    // Create or attach to the shared memory segment
    key_t key2 = ftok(".", KEY_SHELVES);
    int shmid2 = shmget(key2, sizeof(struct ShelvesSharedmemory), IPC_CREAT | 0666);
    if (shmid2 == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    struct ShelvesSharedmemory *sharedMemory2 = shmat(shmid2, NULL, 0);
    if (sharedMemory2 == (struct ShelvesSharedmemory *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    // Print the contents of shared memory
    
    //int skey = getpid();
    //int semid = createSemaphore(1, skey); // Initialize semaphore with a value of 1 to ensure exclusive access
    //lockSemaphore(semid);
    //printf("Storage!\n");
    int Empty=0,All=0;
    
    for (int i = 0; i < Number_Of_Products; i++) { // Assuming you have up to 200 items
        if (sharedMemory->quantity_on_shelves[i] == 0) {
            break; // Stop if the ID is 0 (assuming 0 indicates an empty item)
        }
        /*
        printf("Item %d:\n", i + 1);
        printf("Name: %s\n", sharedMemory->Name[i]);
        printf("ID: %d\n", i+1);
        printf("Quantity: %d\n", sharedMemory->quantity[i]);
        printf("----------------\n");
        */
        if(sharedMemory->quantity[i]==0 && sharedMemory2->quantity_on_shelves[i]==0 && sharedMemory2->Filling[i]==0){Empty++;}
        All++;
        
    }
    if(All==Empty){
        printf("\n==============\nStorage is Empty!\n==============\n");
        usleep(50000);
    	kill(getpid(), SIGINT);
    }
    /*
    for (int i = 0; i < MaxItems; i++) { // Assuming you have up to 200 items
        if (sharedMemory2->quantity_to_refill[i] == 0) {
            break; // Stop if the ID is 0 (assuming 0 indicates an empty item)
        }
        printf("Item %d:\n", i + 1);
        printf("Name: %s\n", sharedMemory2->Name[i]);
        printf("ID: %d\n", sharedMemory2->ID[i]);
        printf("Quantity: %d\n", sharedMemory2->quantity_on_shelves[i]);
        printf("quantity_to_refill: %d\n", sharedMemory2->quantity_to_refill[i]);
        printf("Price: %.2lf\n", sharedMemory2->price[i]);
        printf("----------------\n");
    }
    */
    // Detach from shared memory
    //unlockSemaphore(semid);
    shmdt(sharedMemory);
    shmdt(sharedMemory2);
    
}

int createSemaphore(int initialValue, int SEM_KEY) {
    int semid = semget(ftok(".", SEM_KEY), 1, IPC_CREAT | 0666);
    if (semid == -1) {
        printf("DelteEr1C\n");
        perror("semget");
        
        printf("DelteCrr\n");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    arg.val = initialValue;

    if (semctl(semid, 0, SETVAL, arg) == -1) {
        printf("DelteEr2C\n");
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    return semid;
}
void lockSemaphore(int semid) {
    struct sembuf sops = {0, -1, 0};
    if (semop(semid, &sops, 1) == -1) {
        perror("semop (lock)");
        exit(EXIT_FAILURE);
    }
}

void unlockSemaphore(int semid) {
    struct sembuf sops = {0, 1, 0};
    if (semop(semid, &sops, 1) == -1) {
        perror("semop (unlock)");
        exit(EXIT_FAILURE);
    }
}
void DeleteSharedMemories(){
    int array[2];
    array[0]=KEY_STORAGE;
    array[1]=KEY_SHELVES;
    for (int i=0 ; i<2 ; i++){
	    key_t shm_key = ftok(".", array[i]);  // Use the same key used to create the shared memory
	    int shm_id = shmget(shm_key, 0, 0);

	    if (shm_id == -1) {
		perror("shmget");
		exit(1);
	    }

	    // Remove the shared memory segment
	    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	    }

	    //printf("Shared memory segment removed.\n");
    }
}
void deleteSemaphore(int skey) {
    // Get the semaphore ID
    int semid = semget(ftok(".", skey), 1, 0);
    if (semid == -1) {
        printf("DelteEr\n");
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Delete the semaphore
    if (semctl(semid, 0, IPC_RMID) == -1) {
        printf("DelteEr\n");
        perror("semctl (IPC_RMID)");
        exit(EXIT_FAILURE);
    }

    //printf("Semaphore deleted successfully.\n");
}
// Function to handle termination of child processes
void terminateChildProcesses() {
    // Send a termination signal (e.g., SIGTERM) to all child processes
    // You can also use SIGKILL (kill -9) for forceful termination
    // Here, we use SIGTERM as a graceful termination signal
    kill(0, SIGTERM);
}

void generateCustomerArrivalProcess(int Customers_Arrival_Max, int Customers_Arrival_Min, int Number_Of_Products) {
    pid_t pid = fork();



    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char Customers_Arrival_MaxStr[10];
        char Customers_Arrival_MinStr[10];
        char Number_Of_ProductsStr[5];
        snprintf(Customers_Arrival_MaxStr, sizeof(Customers_Arrival_MaxStr), "%d", Customers_Arrival_Max);
        snprintf(Customers_Arrival_MinStr, sizeof(Customers_Arrival_MinStr), "%d", Customers_Arrival_Min);
        snprintf(Number_Of_ProductsStr, sizeof(Number_Of_ProductsStr), "%d", Number_Of_Products);
        
        char *args[] = {"./customerArrival", Customers_Arrival_MinStr, Customers_Arrival_MaxStr, Number_Of_ProductsStr,NULL};

        execvp(args[0], args);

        // If execvp fails (e.g., file not found), print an error
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
}



void generateCustomerProcess(int Number_Of_Products) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char Number_Of_ProductsStr[5];
        snprintf(Number_Of_ProductsStr, sizeof(Number_Of_ProductsStr), "%d", Number_Of_Products);
        char *args[] = {"./customer", Number_Of_ProductsStr, NULL};
        execvp(args[0], args);
        // If execvp fails (e.g., file not found), print an error
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
}

void OptimizedgetRandomItemsAndCalculateCart(double *cartPrice, int *cart, int Number) {
    // Get the key for the shared memory segment
    key_t key = ftok(".", KEY_SHELVES);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the shared memory segment ID
    int shmid = shmget(key, sizeof(struct ShelvesSharedmemory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    struct ShelvesSharedmemory *sharedMemory = shmat(shmid, NULL, 0);
    if (sharedMemory == (struct ShelvesSharedmemory *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    int skey = getpid();
    int semid = createSemaphore(1, skey); // Initialize semaphore with a value of 1 to ensure exclusive access
    int RandomNumberOfItems = getRandomNumber(1, 120);

    int cartCount = 0;       // Track the cart items
    double cartTotal = 0.0; // Track the cart total price
    while (cartCount < RandomNumberOfItems) {
        int Index = getRandomNumber(0,Number);
        sleep(3);
        lockSemaphore(semid);
        int quantity = sharedMemory->quantity_on_shelves[Index];
        if (quantity <= 0) {
            unlockSemaphore(semid);
            continue;
        }
        int randomQuantity = getRandomNumber(1, quantity); // Ensure quantity is within bounds

        cartTotal += (randomQuantity * sharedMemory->price[Index]);
        cartCount += randomQuantity;
        sharedMemory->quantity_on_shelves[Index] = sharedMemory->quantity_on_shelves[Index]-randomQuantity;
        
        printf ("customer %d bought %d of %s\n", getpid(), randomQuantity, sharedMemory->Name[Index]);
        
        unlockSemaphore(semid);
    }
    
    // Update the cart variables after the loop
    *cartPrice = cartTotal;
    *cart = cartCount;

    // Detach from shared memory
    shmdt(sharedMemory);
    deleteSemaphore(skey);
}




void generateTeamsProcess(int NumberOfEmployees, int Number_Of_Products) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Convert NumberOfEmployees to a string
        char NumberOfEmployeesStr[20]; // Adjust the buffer size as needed
        snprintf(NumberOfEmployeesStr, sizeof(NumberOfEmployeesStr), "%d", NumberOfEmployees);
        char Number_Of_ProductsStr[5]; // Adjust the buffer size as needed
        snprintf(Number_Of_ProductsStr, sizeof(Number_Of_ProductsStr), "%d", Number_Of_Products);
        
        char *args[] = {"./ShelvesTeam", NumberOfEmployeesStr, Number_Of_ProductsStr, NULL};
        execvp(args[0], args);
        
        // If execvp fails (e.g., file not found), print an error
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
}

pid_t generateOpenGl(){
    pid_t childPid = fork();
    char *args[] = {"./myprogram.elf", NULL};
    if (childPid == -1) {
        perror("fork"); // Failed to create child process
        exit(EXIT_FAILURE);
    }

    if (childPid == 0) {
        // This code runs in the child process
        execvp(args[0],args); // Replace the child process with a new program
        perror("execvp"); // Execvp failed only if it returns, so print an error
        exit(EXIT_FAILURE); // Terminate the child process on failure
    } 
    return childPid;
}





//Masalamah
void readSharedMemoryData(int *NumberOfProductsInStorage,int *NumberOfProductsInSuperMarket,int shelves_quantity_on_shelves[MaxItems], int shelves_quantity_to_refill[MaxItems],
                          int storage_quantity[MaxItems], int shelves_id[MaxItems], int storage_id[MaxItems]) {
    key_t storage_key = ftok(".", KEY_STORAGE); // Use the defined keys
    key_t shelves_key = ftok(".", KEY_SHELVES); // Use the defined keys

    if (storage_key == -1 || shelves_key == -1) {
        perror("ftok");
        exit(1);
    }
    int storage_shm_id = shmget(storage_key, sizeof(struct StorageSharedmemory), 0666);
    int shelves_shm_id = shmget(shelves_key, sizeof(struct ShelvesSharedmemory), 0666);

    if (storage_shm_id == -1 || shelves_shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    struct StorageSharedmemory *storage_shm = shmat(storage_shm_id, NULL, 0);
    struct ShelvesSharedmemory *shelves_shm = shmat(shelves_shm_id, NULL, 0);

    if (storage_shm == (struct StorageSharedmemory *)-1 || shelves_shm == (struct ShelvesSharedmemory *)-1) {
        perror("shmat");
        exit(1);
    }int Storage=0;
    int SuperMarket=0;
    //int skey = getpid();
    //int semid = createSemaphore(1, skey); // Initialize semaphore with a value of 1 to ensure exclusive access
    //lockSemaphore(semid);
    //sleep(1);
    for (int i = 0; i < MaxItems; ++i) {
        if (storage_shm->quantity[i] == 0 && shelves_shm->quantity_on_shelves[i] == 0 && shelves_shm->Filling[i]==0){
        	shelves_id[i]= 0;
	    	storage_id[i] = 0;
        	continue;
        }
        else if(storage_shm->quantity[i] == 0){
        	shelves_id[i]= shelves_shm->ID[i];
        	storage_id[i] = 0;
		shelves_quantity_on_shelves[i]=shelves_shm->quantity_on_shelves[i];
	    	shelves_quantity_to_refill[i]=shelves_shm->quantity_to_refill[i];
        
        }
        else{
        
	    	shelves_id[i]=shelves_shm->ID[i];
	    	storage_id[i] =storage_shm->ID[i];
	    	shelves_quantity_on_shelves[i]=shelves_shm->quantity_on_shelves[i];
	    	shelves_quantity_to_refill[i]=shelves_shm->quantity_to_refill[i];
	    	storage_quantity[i]=storage_shm->quantity[i];
		//printf("Item %d:\n", i+1);
		//printf("Shelve ID: %d\n", shelves_id[i]);
		//printf("Storage ID: %d\n", storage_id[i]);
		//printf("Shelve Quantity on Shelves: %d\n", shelves_quantity_on_shelves[i]);
		//printf("Shelve Quantity to Refill: %d\n", shelves_quantity_to_refill[i]);
		//printf("Storage Quantity: %d\n", storage_quantity[i]);
	}
        if (storage_shm->quantity[i]!= 0 || shelves_shm->quantity_on_shelves[i] != 0 || storage_shm->quantity[i]!= 0) {
            SuperMarket++;
        }
        if (storage_shm->quantity[i]!= 0 || storage_shm->quantity[i]!= 0) {
            Storage++;
        }
        
    }
    //unlockSemaphore(semid);
    *NumberOfProductsInStorage=Storage;
    *NumberOfProductsInSuperMarket=SuperMarket; 

    shmdt(storage_shm);
    shmdt(shelves_shm);
        //deleteSemaphore(skey);
}

int countProcesses(const char *processName) {
    char command[256];
    FILE *fp;
    int count = 0;

    // Create the shell command to count healthy processes
    snprintf(command, sizeof(command), "pgrep %s | xargs -I %% ps -o state= -p %% | grep -c '^[^Z]*$'", processName);
    
    // Execute the shell command and read the output
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    // Read the output of the shell command (the count of healthy processes)
    if (fscanf(fp, "%d", &count) != 1) {
        fprintf(stderr, "Error reading process count\n");
        exit(EXIT_FAILURE);
    }

    // Close the file pointer
    pclose(fp);

    return count;
}


