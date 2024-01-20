#include "local.h"
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s Number_Of_Products\n", argv[0]);
        return 1;
    }
    int Number_Of_Products = atoi(argv[1]);
    printf("Customer %d went inside the supermarket!\n",getpid());
    double cartPrice;
    int cart;
    //sleep(getRandomNumber(SHOOPINGTIMEMIN, SHOPPINGTIMEMAX));
    OptimizedgetRandomItemsAndCalculateCart(&cartPrice, &cart, Number_Of_Products);
    printf("\nCustomer %d finished with cart price: $%.2lf and with %d items\n",getpid(), cartPrice,cart);

    sleep(getRandomNumber(SHOOPINGTIMEMIN, SHOPPINGTIMEMAX));
    exit(1);
}

