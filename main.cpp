#include <cstdio>
#include <bitset>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

//Extra Parameters
#define ENCSIZE 10
#define POPSIZE 10
#define RANGEINF 0
#define RANGESUP 10

bitset<POPSIZE> popbin[POPSIZE];
vector<int>     popint[POPSIZE];
vector<double>  popdou[POPSIZE];

int main(int argc, char **argv){
    if(argc != 2){
        printf("Wrong Parameters\n");
        printf("1- Population Type (b - Binary; i - Integer; r - Real)\n");
        exit(0);
    }
    char type = argv[1][0];
    srand(time(NULL));
    if(type == 'b'){
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                if(rand()%2==0)
                    popbin[i].reset(j);
                else
                    popbin[i].set(j);
            }
        }
    }
    else if(type == 'i'){
        int temp;
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                temp = rand()%(RANGESUP - RANGEINF) + RANGEINF;
                if(find(popint[i].begin(), popint[i].end(), temp) == popint[i].end())
                    popint[i].push_back(temp);
                else
                    j--;
            }
        }
    }
    else{ //type == 'r'
        default_random_engine generator;
        uniform_real_distribution<double> distribution(RANGEINF, RANGESUP);
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                popdou[i].push_back(distribution(generator));
                //printf(" %lf", popdou[i][j]);
            }
            //printf("\n");
        }
    }
    return 0;
}
