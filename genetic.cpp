#include "genetic.h"

bitset<ENCSIZE> *popbin;
vector<int>     *popint;
vector<double>  *popdou;

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
            for (int j = 0; j<ENCSIZE; j++){
                if(rand()%101 < MUTATERT)
                    popbin[i].flip(j);
            }
        }
    }
    else if(type == 'i'){
        for(int i=0; i<POPSIZE; i++){
            for(auto j=popint[i].begin(); j<popint[i].end(); j++){
                if(rand()%101 < MUTATERT){
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

void crossover(char tipo, int i, int j){
    int corte = rand()%(ENCSIZE-1);
    if(tipo == 'b'){
        for(int k=0; k<ENCSIZE; k++){
            if(k<=corte){
                //aux[k] = popbin[i][k];
            }
            else{
                //aux[k] = popbin[j][k];
            }
        }
        //return aux;
    }
    else if(tipo == 'i'){
    }
    else{
    }
}

void printGen(char tipo){
    switch(tipo){
        case 'b':
            for(int i=0; i<POPSIZE; i++){
                for(int j=0; j<ENCSIZE; j++){
                    printf(" %d", (int)popbin[i][j]);
                }
                printf(" Fit = %d\n",bAlternados(i, tipo));
            }
        break;

        case 'i':
            for(int i=0; i<POPSIZE; i++){
                for(int j=0; j<ENCSIZE; j++){
                    printf(" %d", popint[i][j]);
                }
                printf(" Fit = %d\n",bAlternados(i, tipo));
            }
        break;

        case 'r':
            for(int i=0; i<POPSIZE; i++){
                for(int j=0; j<ENCSIZE; j++){
                    printf(" %lf", popdou[i][j]);
                }
                printf(" Fit = %lf\n",x2(i));
            }
        break;
    }
}

void init(char tipo){
    if(tipo =='b'){
        popbin = (bitset<ENCSIZE>*)malloc(sizeof(bitset<ENCSIZE>)*POPSIZE);
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                if(rand()%2==0)
                    popbin[i].reset(j);
                else
                    popbin[i].set(j);
            }
        }
    }
    else if(tipo == 'i'){
        popint = (vector<int>*)malloc(sizeof(vector<int>)*POPSIZE);
        int temp;
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                temp = rand()%(RANGESUP - RANGEINF) + RANGEINF;
                if(find(popint[i].begin(), popint[i].end(), temp) == popint[i].end()){
                    popint[i].push_back(temp);
                }
                else
                    j--;
            }
        }
    }
    else{
        popdou = (vector<double>*)malloc(sizeof(vector<double>)*POPSIZE);
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                popdou[i].push_back(distribution(generator));
            }
        }
    }
}