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
    AG(type);
    return 0;
}
