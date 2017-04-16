#include "genetic.h"

bitset<ENCSIZE> *popbin, *popbInt = NULL;//current and intermediate populations
vector<int>     *popint, *popiInt = NULL;
vector<double>  *popdou, *popdint = NULL;
FILE *file = NULL;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
uniform_real_distribution<double> distribution(RANGEINF, RANGESUP);
uniform_real_distribution<double> percentage(0, 100);

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

double customFunc1(int i){
    double scale = -2 + ((2+2)/(pow(2,ENCSIZE)-1))*binToDec(i);
    // printf(" binToDec: %d, scale: %lf ", binToDec(i), scale);
    double fx = cos(20*scale) - fabs(scale)/2 + scale*scale*scale/4;
	
    int precision = 4;
    char buf[256];
    sprintf(buf, "%.*lf", precision, fx);
    fx = atof(buf);
    // printf(" precision: %lf\n", fx);
	return fx + 4;
}

int binToDec(int i){
	int sum=0;
	for(int j=0; j<ENCSIZE; j++){
		sum += popbin[i][j]*pow(2, ENCSIZE-j-1);
	}
	return sum;
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

void crossover(char tipo){
    int corte = rand()%(ENCSIZE-1) + 1;
    if(tipo == 'b'){
        for(int i=0; i<POPSIZE; i+=2){
            for(int j=0; j<ENCSIZE; j++){
                if(j<corte){
                    popbin[i][j] = popbInt[i][j];
                    popbin[i+1][j] = popbInt[i+1][j];
                }
                else{
                    popbin[i][j] = popbInt[i+1][j];
                    popbin[i+1][j] = popbInt[i][j];
                }
            }
        }
    }
    else if(tipo == 'i'){
    }
    else{
    }
}

void roulette(char tipo){
    double sum=0, fit[POPSIZE];
    double fstpercentages[POPSIZE], sndpercentages[POPSIZE];
    if(tipo == 'b'){
        for(int i=0; i<POPSIZE; i++){
            fit[i] = customFunc1(i);
            sum += fit[i];
        }
        for(int i=0; i<POPSIZE; i++){
            fstpercentages[i] = fit[i]/sum;
        }
        if(popbInt == NULL){
            popbInt = (bitset<ENCSIZE>*)malloc(sizeof(bitset<ENCSIZE>)*POPSIZE);
        }

        //roleta
        int lastj;
        for(int i=0; i<POPSIZE; i++){
            double accum=0;
            double random = percentage(generator);
            // printf("random = %lf\n", random);
            double sum2=0;
            //sndpercentages
            if(i%2 == 1){
                sum2 = sum - fit[lastj];
                for(int k2=0; k2<POPSIZE; k2++){
                    sndpercentages[k2] = k2==lastj? 0 : fit[k2]/sum2;
                }
            }

            for(int j=0; j<POPSIZE; j++){
                accum += i%2==0? fstpercentages[j] : sndpercentages[j];
                // printf("accum*100 = %lf\n", accum*100);
                if(random < accum*100){
                    // printf("%d\n", j);
                    popbInt[i] = popbin[j];
                    lastj = j;
                    break;
                }
            }
        }
    }
    else if(tipo == 'i'){
        
    }
    else{//tipo == 'r'
    }
}

void printGen(char tipo){
    switch(tipo){
        case 'b':
            for(int i=0; i<POPSIZE; i++){
                for(int j=0; j<ENCSIZE; j++){
                    printf(" %d", (int)popbin[i][j]);
                }
                // printf(" Fit = %d\n",bAlternados(i, tipo));

                //Aula 22/mar/17
                printf(" Fit = %lf\n",customFunc1(i));
                //end Aula
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

void printInt(char tipo){
    switch(tipo){
        case 'b':
            for(int i=0; i<POPSIZE; i++){
                for(int j=0; j<ENCSIZE; j++){
                    printf(" %d", (int)popbInt[i][j]);
                }
                printf("\n");
                // printf(" Fit = %d\n",bAlternados(i, tipo));
                // printf(" Fit = %lf\n",customFunc1(i));
            }
        break;

        case 'i':
            for(int i=0; i<POPSIZE; i++){
                for(int j=0; j<ENCSIZE; j++){
                    printf(" %d", popiInt[i][j]);
                }
                printf("\n");
                // printf(" Fit = %d\n",bAlternados(i, tipo));
            }
        break;

        case 'r':
            for(int i=0; i<POPSIZE; i++){
                for(int j=0; j<ENCSIZE; j++){
                    printf(" %lf", popdint[i][j]);
                }
                printf("\n");
                // printf(" Fit = %lf\n",x2(i));
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
        if(RANGESUP - RANGEINF + 1 < ENCSIZE){
            printf("Insufficient range\n");
            exit(0);
        }
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                temp = rand()%(RANGESUP - RANGEINF + 1) + RANGEINF;
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

void logMedias(int iteration, double value){
    if(file == NULL){
        file = fopen("data.txt", "w");
    }
    fprintf(file, "%d %lf\n", iteration, value);
}