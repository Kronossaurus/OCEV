#include "genetic.h"

bitset<ENCSIZE> *popbin, *popbInt = NULL, outBin;//current and intermediate populations, output individual
vector<int>     *popint, *popiInt = NULL, outInt;
vector<double>  *popdou, *popdInt = NULL, outDou;

//global to reduce processing complexity
double fit[POPSIZE], sum = 0, maior = 0, outFit = 0;
FILE *file = NULL;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
uniform_real_distribution<double> distribution(RANGEINF, RANGESUP);
uniform_real_distribution<double> percentage(0, 100);

int bAlternados(int i, char tipo){
    int fitness=0;
    if(tipo == 'b'){
        for(int j=0; j<ENCSIZE-1; j++){
            if(popbin[i][j] != popbin[i][j+1])
                fitness++;
        }
    }
    else if(tipo == 'i'){
        for(int j=0; j<ENCSIZE-1; j++){
            // printf("\n%d %d\n", popint[i][j]%2, popint[i][j+1]%2);
            if(popint[i][j]%2 != popint[i][j+1]%2)
                fitness++;
        }
    }
    else
        printf("Tipo incorreto para bAlternados\n");
    return fitness;
}

double x2(int i){
    double fitness=0;
    for(int j=0; j<ENCSIZE; j++){
        fitness += popdou[i][j]*popdou[i][j];
    }
    return fitness;
}

double x2_2(int i){//aula do dia 17/04
    int j;
    double fitness=0;
    double scale = abs(RANGESUP) > abs(RANGEINF)? abs(RANGESUP) : abs(RANGEINF);
    // #pragma omp parallel for default(shared) private(j) reduction (+:fitness)
    for(j=0; j<ENCSIZE; j++){
        fitness += scale*scale - (popdou[i][j]-2)*(popdou[i][j]-2);
    }
    return fitness/ENCSIZE/scale/scale;
}

double customFunc1(int i){
    double scale = -2 + ((2+2)/(pow(2,ENCSIZE)-1))*binToDec(i);
    // printf(" binToDec: %d, scale: %lf ", binToDec(i), scale);
    double fx = cos(20*scale) - fabs(scale)/2 + scale*scale*scale/4;
	
    // int precision = 4;
    // char buf[256];
    // sprintf(buf, "%.*lf", precision, fx);
    // fx = atof(buf);
    // printf(" precision: %lf\n", fx);
	return fx + 4;
}

int pattern(int i){
    int size = 6;
    int goal[] = {  1, 0, 1, 1, 1, 1,
                    1, 0, 1, 0, 0, 1,
                    1, 0, 1, 0, 1, 1,
                    1, 1, 1, 1, 0, 1,
                    1, 0, 0, 0, 1, 1,
                    1, 1, 1, 1, 0, 1 };

    int dist = 0;
    for (int j = 0; j < size*size; ++j){
        if(goal[j] != popbin[i][j])
            dist++;
    }

    int fitness = 36 - dist;
    return fitness;
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
            if(rand()%101 < MUTATERT){
                int a = rand()%ENCSIZE, b;
                b = a;
                while(b==a)
                    b = rand()%ENCSIZE;
                int aux = popint[i][a];
                popint[i][a] = popint[i][b];
                popint[i][b] = aux;
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

void deltaMutation(char type){
    if(type != 'r'){
        printf("Wrong type for deltaMutation\n");
        exit(0);
    }
    uniform_real_distribution<double> deltaDist(0, 0.2);
    for(int i=0; i<POPSIZE; i++){
        for(int j=0; j<ENCSIZE; j++){
            if(rand()%101 < MUTATERT){
                popdou[i][j] += rand()%2 == 0? deltaDist(generator) : -deltaDist(generator);
            }
        }
    }
}

void crossover1p(char tipo){
    for(int i=0; i<POPSIZE; i+=2){
        if(rand()%101 < CROSSRT){
            int corte = rand()%(ENCSIZE-1) + 1;
            for(int j=0; j<ENCSIZE; j++){
                if(j<corte){
                    if(tipo == 'b'){
                        popbin[i][j] = popbInt[i][j];
                        popbin[i+1][j] = popbInt[i+1][j];
                    }
                    else if(tipo == 'i'){
                        popint[i][j] = popiInt[i][j];
                        popint[i+1][j] = popiInt[i+1][j];
                    }
                    else{
                        popdou[i][j] = popdInt[i][j];
                        popdou[i+1][j] = popdInt[i+1][j];
                    }

                }
                else{
                    if(tipo == 'b'){
                        popbin[i][j] = popbInt[i+1][j];
                        popbin[i+1][j] = popbInt[i][j];
                    }
                    else if(tipo == 'i'){
                        popint[i][j] = popiInt[i+1][j];
                        popint[i+1][j] = popiInt[i][j];
                    }
                    else{
                        popdou[i][j] = popdInt[i+1][j];
                        popdou[i+1][j] = popdInt[i][j];
                    }
                }
            }
        }
        else{
            if(tipo == 'b'){
                popbin[i] = popbInt[i];
                popbin[i+1] = popbInt[i+1];
            }
            else if(tipo == 'i'){
                popint[i] = popiInt[i];
                popint[i+1] = popiInt[i+1];
            }
            else{
                popdou[i] = popdInt[i];
                popdou[i+1] = popdInt[i+1];
            }
        }
    }
}

void crossunif(char tipo){
    for(int i=0; i<POPSIZE; i+=2){
        if(rand()%101 < CROSSRT){
            for(int j=0; j<ENCSIZE; j++){
                if(rand()%2 == 0){
                    if(tipo == 'b'){
                        popbin[i][j] = popbInt[i+1][j];
                        popbin[i+1][j] = popbInt[i][j];
                    }
                    else if(tipo == 'i'){
                        popint[i][j] = popiInt[i+1][j];
                        popint[i+1][j] = popiInt[i][j];
                    }
                    else{
                        popdou[i][j] = popdInt[i+1][j];
                        popdou[i+1][j] = popdInt[i][j];
                    }
                }
                else{
                    if(tipo == 'b'){
                        popbin[i][j] = popbInt[i][j];
                        popbin[i+1][j] = popbInt[i+1][j];
                    }
                    else if(tipo == 'i'){
                        popint[i][j] = popiInt[i][j];
                        popint[i+1][j] = popiInt[i+1][j];
                    }
                    else{
                        popdou[i][j] = popdInt[i][j];
                        popdou[i+1][j] = popdInt[i+1][j];
                    }
                }
            }
        }
        else{
            if(tipo == 'b'){
                popbin[i] = popbInt[i];
                popbin[i+1] = popbInt[i+1];
            }
            else if(tipo == 'i'){
                popint[i] = popiInt[i];
                popint[i+1] = popiInt[i+1];
            }
            else{
                popdou[i] = popdInt[i];
                popdou[i+1] = popdInt[i+1];
            }
        }
    }
}

void roulette(char tipo){
    double fstpercentages[POPSIZE], sndpercentages[POPSIZE];
    for(int i=0; i<POPSIZE; i++)
        fstpercentages[i] = fit[i]/sum;

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
                if(tipo == 'b')
                    popbInt[i] = popbin[j];
                else if(tipo == 'i')
                    popiInt[i] = popint[j];
                else
                    popdInt[i] = popdou[j];

                lastj = j;
                break;
            }
        }
    }
}

void printGen(char tipo){
    for(int i=0; i<POPSIZE; i++){
        for(int j=0; j<ENCSIZE; j++){
            if(tipo == 'b')
                printf(" %d", (int)popbin[i][j]);
            else if(tipo == 'i')
                printf(" %d", popint[i][j]);
            else
                printf(" %lf", popdou[i][j]);
        }
        printf(" Fit = %lf\n",fit[i]);
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
                    printf(" %lf", popdInt[i][j]);
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
        popbInt = (bitset<ENCSIZE>*)malloc(sizeof(bitset<ENCSIZE>)*POPSIZE);
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                if(rand()%2==0)
                    popbin[i].reset(j);
                else
                    popbin[i].set(j);
            }
        }
        for(int i=0; i<POPSIZE; i++)
            popbin[0][i] = 1;
    }
    else if(tipo == 'i'){
        popint = (vector<int>*)malloc(sizeof(vector<int>)*POPSIZE);
        popiInt = (vector<int>*)malloc(sizeof(vector<int>)*POPSIZE);
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
        popdInt = (vector<double>*)malloc(sizeof(vector<double>)*POPSIZE);
        for(int i=0; i<POPSIZE; i++){
            for(int j=0; j<ENCSIZE; j++){
                popdou[i].push_back(distribution(generator));
            }
        }
    }
}

void logMedias(int iteration, double div){
    if(file == NULL){
        file = fopen("data.txt", "w");
    }
    fprintf(file, "%d %lf %lf %lf\n", iteration, sum/POPSIZE, maior, div);
}

double diversity(char type){
    double div = 0;
    if(type == 'b'){
        for (int i = 0; i < POPSIZE; i++){
            for (int j = i+1; j < POPSIZE; j++){
                for (int k = 0; k < ENCSIZE; k++){
                    if(popbin[i][k] != popbin[j][k])
                        div++;
                }
            }
        }
    }
    return div/POPSIZE/POPSIZE;
}

void Fitness(char type){//this function fills the fit vector and the sum variable
    maior = 0;
    sum = 0;
    for(int i=0; i<POPSIZE; i++){

        if(type == 'b'){
            fit[i] = pattern(i);
            if(fit[i] > outFit){
                outFit = fit[i];
                outBin = popbin[i];
            }
        }

        else if(type == 'i'){
            fit[i] = bAlternados(i, type);
            if(fit[i] > outFit){
                outFit = fit[i];
                outInt = popint[i];
            }
        }

        else{
            fit[i] = x2_2(i);
            if(fit[i] > outFit){
                outFit = fit[i];
                outDou = popdou[i];
            }
        }
        if(fit[i] > maior)
            maior = fit[i];
        sum += fit[i];
    }
}

void AG(char type){
    init(type);
    //fitness update
    Fitness(type);
    //log
    outFit = maior;
    double div = 0;
    div = diversity(type);
    logMedias(0, div);
    for(int i=1; i<=MAXGENS; i++){
        // printf("\nGen %d: \n", i-1);
        // printGen(type);

        //selection
        // printf("Roleta\n");
        roulette(type);

        //crossover
        // printInt(type);
        // printf("Crossover\n");
        // crossover(type);
        crossunif(type);

        //mutation
        // printf("Mutation\n");
        mutation(type);
        // deltaMutation(type);

        //fitness update
        // printf("Fitness\n");
        Fitness(type);

        //log
        // printf("Log\n");
        div = diversity(type);
        logMedias(i, div);
    }
    printf("Maior Fitness = %.4lf\n", outFit);
    printf("Genótipo do melhor indivíduo =");
    if(type == 'b')
        for(int i=0; i<ENCSIZE; i++)
            printf(" %d", (int)outBin[i]);
    else if(type == 'i')
        for(int i=0; i<ENCSIZE; i++){
            printf(" %i", outInt[i]);
        }
    else
        for(int i=0; i<ENCSIZE; i++){
            printf(" %.4lf", outDou[i]);
        }
    printf("\n");
}