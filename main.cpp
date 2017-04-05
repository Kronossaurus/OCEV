#include <cstdio>
#include "genetic.h"
int main(int argc, char **argv){
    if(argc != 2){
        printf("Wrong Parameters\n");
        printf("1- Population Type (b - Binary; i - Integer; r - Real)\n");
        exit(0);
    }
    char type = argv[1][0];
    srand(time(NULL));
    init(type);
    printGen(type);
    printf("After Roulette:\n");
    roulette(type);
    printInt(type);
    crossover(type);
    printf("After Crossover:\n");
    printGen(type);
    // printf("After Mutation:\n");
    // mutation(type);
    // printGen(type);
    return 0;
}
