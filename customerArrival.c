#include "local.h"
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s Customers_Arrival_Min Customers_Arrival_Max Number_Of_Products\n", argv[0]);
        return 1;
    }

    int Customers_Arrival_Min = atoi(argv[1]);
    int Customers_Arrival_Max = atoi(argv[2]);
    int Number_Of_Products = atoi (argv[3]);
    if (Customers_Arrival_Min > Customers_Arrival_Max) {
        fprintf(stderr, "Error: Customers_Arrival_Min should be less than or equal to Customers_Arrival_Max.\n");
        return 1;
    }
    while (1) {
        // Generate a random arrival time between Customers_Arrival_Min and Customers_Arrival_Max
        int arrivalTime = getRandomNumber(Customers_Arrival_Min, Customers_Arrival_Max);

        usleep(arrivalTime*1000); // Wait for the random arrival time

        generateCustomerProcess(Number_Of_Products);
    }

    return 0;
}

