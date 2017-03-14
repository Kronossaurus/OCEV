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
#define MUTATERT 5

bitset<POPSIZE> popbin[POPSIZE];
vector<int>     popint[POPSIZE];
vector<double>  popdou[POPSIZE];

default_random_engine generator;
uniform_real_distribution<double> distribution(RANGEINF, RANGESUP);

int bAlternados(int i, char tipo){
    int fitness=0;
    if(tipo == 'b'){
        for(int j=0; j<POPSIZE-1; j++){
            if(popbin[i][j] != popbin[i][j+1])
                fitness++;
        }
    }
    else if(tipo == 'i'){
        for(int j=0; j<POPSIZE-1; j++){
            // printf("\n%d %d\n", popint[i][j]%2, popint[i][j+1]%2);
            if(popint[i][j]%2 != popint[i][j+1]%2)
                fitness++;
        }
    }
    else
        printf("Tipo incorreto\n");
    return fitness;
}

double x2(int i){
    double fitness=0;
    for(int j=0; j<POPSIZE-1; j++){
        fitness += popdou[i][j]*popdou[i][j];
    }
    return fitness;
}

void mutation(char type){
    if(type == 'b'){
        for(int i=0; i<POPSIZE; i++){
            for (int j = 0; j<ENCSIZE; j++)
            {
                if(rand()%101 < MUTATERT)
                    popbin[i].flip(j);
            }
        }
    }
    if(type == 'i'){
        for(int i=0; i<POPSIZE; i++){
            for(auto j=popint[i].begin(); j<popint[i].end(); j++){
                if(rand()%101 < MUTATERT){
                    popint[i].erase(j);
                    int temp = rand()%(RANGESUP - RANGEINF) + RANGEINF;
                    if(find(popint[i].begin(), popint[i].end(), temp) == popint[i].end()){
                        popint[i].insert(j, temp);
                    }
                }
            }
        }
    }
    else{
        for(int i=0; i<POPSIZE; i++){
            for (int j = 0; j<ENCSIZE; j++)
            {
                if(rand()%101 < MUTATERT)
                    popdou[i][j] = distribution(generator);
            }
        }
    }
}

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
                printf(" %d", (int)popbin[i][j]);
            }
            printf(" Fit = %d\n",bAlternados(i, type));
        }
    }
    else if(type == 'i'){
        int temp;
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                temp = rand()%(RANGESUP - RANGEINF) + RANGEINF;
                if(find(popint[i].begin(), popint[i].end(), temp) == popint[i].end()){
                    popint[i].push_back(temp);
                    printf(" %d", popint[i][j]);
                }
                else
                    j--;
            }
            printf(" Fit = %d\n",bAlternados(i, type));
        }
    }
    else{ //type == 'r'
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                popdou[i].push_back(distribution(generator));
                printf(" %lf", popdou[i][j]);
            }
            printf(" Fit = %lf\n",x2(i));
        }
    }
    return 0;
}
