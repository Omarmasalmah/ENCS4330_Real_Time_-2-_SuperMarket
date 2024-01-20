#include "local.h"
//The main file
// Flag to indicate if a child process has sent a SIGINT signal
volatile sig_atomic_t childSignaled = 0;

// Signal handler function to handle signals from child processes
void ParentSigintHandler(int signum) {
    
    usleep(50000);
    childSignaled = 1;
    //terminateChildProcesses();
}


int main(int argc, char *argv[]) {
    //to check if the arguments number are the right arguments [the files]
    if (argc != 3) {
        printf("Usage: %s <UserDefinedNumbersFile> <ItmesFile>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (signal(SIGINT, ParentSigintHandler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    const char *UserDefinedNumbers = argv[1]; 
    const char *ItemsFile = argv[2];
    
    
    int numberArray[MAX_LINES];
    int numLines;


    //to read from User defined numbers file (filename.txt)
    readFromFile(UserDefinedNumbers, numberArray, &numLines);
    
    //the values from the file
    int Number_Of_Products = numberArray[0];
    int Number_Of_Shelving_Teams = numberArray[1];
    int Number_Of_Employes_Per_Team = numberArray[2];
    int Customers_Arrival_Max = numberArray[3];
    int Customers_Arrival_Min = numberArray[4];
    int MaxTime = numberArray[5]*60;
    
    //to read and store the items in the shared memoris storage and shelves
    readItemsAndStoreInStorageSharedMemory(ItemsFile,Number_Of_Products);
    
    readAndPrintItemSharedMemory(Number_Of_Products);
    


    


    
    //this loop to generate the shelving teams
    for(int i=0;i< Number_Of_Shelving_Teams;i++){
    	sleep(1);
    	generateTeamsProcess(Number_Of_Employes_Per_Team, Number_Of_Products);
    	
    }
    //to generate the customer arrival process which make the customers arrive at a range
    generateCustomerArrivalProcess(Customers_Arrival_Max, Customers_Arrival_Min, Number_Of_Products);
    //to generate the customer arrival process which make the customers arrive at a range
    pid_t childPid = generateOpenGl(); // Create a new child process
    //to keep checking until a SIGINT arrived or the storage is empty or the time is done
    while (!childSignaled) {
        //sleep(MaxTime);
    	//sleep(10);
    	sleep(1);
    	readAndPrintItemSharedMemory(Number_Of_Products);
    	MaxTime--;
    	if(MaxTime==0){
    		printf("\n========\nTime out!\n========\n");
    		usleep(1000);
    		kill(getpid(),SIGINT);
    		}
    	if (kill(childPid, SIGALRM) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }

    }

    printf("Received SIGINT signal from a customer. Terminating all processes...\n");
    
    DeleteSharedMemories();
    printf("\n================\nDone\n================\n");
    terminateChildProcesses();

    // Wait for all child processes to exit
    int status;
    while (wait(&status) > 0) {
    
        // Continue waiting until all children have exited
    }
    return 0;
}


