#include "genetic.h"

vector<bitset<ENCSIZE> >popbin, popbInt; //current and intermediate populations
vector<vector<int> >    popint, popiInt;
vector<vector<double> > popdou, popdInt;

bitset<ENCSIZE> outBin;//output
vector<int>     outInt;
vector<double>  outDou;

//global to reduce processing complexity
double fit[POPSIZE], sum = 0, maior = 0, menor = RAND_MAX, outFit = 0;
int gap = GENGAP0;
FILE *file = NULL;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
uniform_real_distribution<double> distribution(RANGEINF, RANGESUP);
uniform_real_distribution<double> percentage(0, 100);

void elitism(char tipo){
    for(int i=0; i<ELITISM; i++)
        if(tipo == 'i')
            popint[i] = outInt;
        else if(tipo == 'b')
            popbin[i] = outBin;
        else
            popdou[i] = outDou;
}

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

int deceptive(int i){
    int blocksize = 4, cont;
    double fitness = 0;
    if(ENCSIZE%blocksize != 0){
        printf("Wrong ENCSIZE for deceptive\n");
        exit(0);
    }
    for(int k=0; k<ENCSIZE/blocksize; k++){
        cont = 0;
        for(int j=k*blocksize; ; j++){
            // printf("%d\n", j);
            if(popbin[i][j] == 1)
                cont++;
            if(j%blocksize==blocksize-1)
                break;
        }
        fitness += cont == 0? cont + 1 : cont;
    }
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

double nDamas(int i){
    // #define RANGEINF 1
    // #define RANGESUP ENCSIZE
    double fitness = ENCSIZE;
    for(int j = 0; j<ENCSIZE; j++){
        for(int k = j+1; k<ENCSIZE; k++){
            if(k-j == abs(popint[i][j]-popint[i][k])){
                fitness--;
                break;
            }
        }
    }
    return fitness/ENCSIZE;
}

double f3(int i){
    double fitness = 0;
    if(ENCSIZE%3 != 0){
        printf("Wrong ENCSIZE for f3\n");
        exit(0);
    }
    int b[3];
    for(int j = 0; j < ENCSIZE; j+=3){
        b[0] = (int)popbin[i][j];
        b[1] = (int)popbin[i][j+1];
        b[2] = (int)popbin[i][j+2];
        int dec = binToDecf3(b);
        if(dec == 0)
            fitness += 28;
        else if(dec == 1)
            fitness += 26;
        else if(dec == 2)
            fitness += 22;
        else if(dec == 3 || dec == 5 || dec == 6)
            fitness += 0;
        else if(dec == 4)
            fitness += 26;
        else if(dec == 7)
            fitness += 30;
    }
    return fitness/(10*ENCSIZE);
}
double f3S(int i){
    double fitness = 0;
    return fitness;
}

int binToDec(int i){
	int sum=0;
	for(int j=0; j<ENCSIZE; j++){
		sum += popbin[i][j]*pow(2, ENCSIZE-j-1);
	}
	return sum;
}

int binToDecf3(int b[]){
    int sum=0;
    for(int i=0; i<3; i++)
        sum += b[i]*pow(2,2-i);
    return sum;
}

void mutation(char type){
    if(type == 'b'){
        for(int i=gap; i<POPSIZE; i++){
            for (int j = 0; j<ENCSIZE; j++){
                if(rand()%MODCONST < MUTATERT)
                    popbin[i].flip(j);
            }
        }
    }
    else if(type == 'i'){
        for(int i=gap; i<POPSIZE; i++){
            if(rand()%MODCONST < MUTATERT){
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
        for(int i=gap; i<POPSIZE; i++){
            for (int j = 0; j<ENCSIZE; j++)
            {
                if(rand()%MODCONST < MUTATERT)
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
    for(int i=gap; i<POPSIZE; i++){
        for(int j=0; j<ENCSIZE; j++){
            if(rand()%MODCONST < MUTATERT){
                popdou[i][j] += rand()%2 == 0? deltaDist(generator) : -deltaDist(generator);
            }
        }
    }
}

void swapPosition(char type){
    if (type != 'i'){
        printf("Wrong type for swapPosition\n");
        exit(0);
    }
    for(int i=gap; i<POPSIZE; i++){
        for(int j=0; j<ENCSIZE; j++){
            if(rand()%MODCONST < MUTATERT){
                int target = j;
                while(target == j){
                    target = rand()%ENCSIZE;
                }
                int aux = popint[i][j];
                popint[i][j] = popint[i][target];
                popint[i][target] = aux;
            }
        }
    }
}

void crossover1p(char tipo){
    for(int i=gap; i<POPSIZE; i+=2){
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
    for(int i=gap; i<POPSIZE; i+=2){
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

void PMX(char tipo){
    if (tipo != 'i'){
        printf("Wrong type for PMX\n");
        exit(0);
    }
    int cut1, cut2;
    for(int i=gap; i<POPSIZE; i+=2){
        cut1 = rand()%(ENCSIZE-1);
        cut2 = cut1 + 1 +rand()%(ENCSIZE-cut1-1);
        for(int j=cut1; j<=cut2; j++){
            popint[i][j] = popiInt[i+1][j];
            popint[i+1][j] = popiInt[i][j];
        }
        for(int j=0; j<ENCSIZE; j++){
            if(j<cut1 || j>cut2){
                popint[i][j] = popiInt[i][j];
                for(int k=cut1; k<=cut2; k++){
                    if(popint[i][j] == popint[i][k]){
                        popint[i][j] = popint[i+1][k];
                        k=cut1-1;
                    }
                }
            }
        }
        for(int j=0; j<ENCSIZE; j++){
            if(j<cut1 || j>cut2){
                popint[i+1][j] = popiInt[i+1][j];
                for(int k=cut1; k<=cut2; k++){
                    if(popint[i+1][j] == popint[i+1][k]){
                        popint[i+1][j] = popint[i][k];
                        k=cut1-1;
                    }
                }
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
    popbin.reserve(POPSIZE);
    popbInt.reserve(POPSIZE);
    if(tipo =='b'){
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
        popint.reserve(POPSIZE);
        popiInt.reserve(POPSIZE);
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
        popdou.reserve(POPSIZE);
        popdInt.reserve(POPSIZE);
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
    // int cont = 0;
    for (int i = 0; i < POPSIZE; i++){
        for (int j = i+1; j < POPSIZE; j++){
            for (int k = 0; k < ENCSIZE; k++){
                if(type == 'b')
                    div += sqrt((popbin[i][k] - popbin[j][k]) * (popbin[i][k] - popbin[j][k]));
                else if(type == 'i')
                    div += sqrt((popint[i][k] - popint[j][k]) * (popint[i][k] - popint[j][k]));
                else
                    div += sqrt((popdou[i][k] - popdou[j][k]) * (popdou[i][k] - popdou[j][k]));
            }
            // cont++;
        }
    }
    // printf("cont = %d real = %d", cont,(POPSIZE-1)*POPSIZE/2);
    return div/((POPSIZE-1)*POPSIZE/2);
}

void Fitness(char type){//this function fills the fit vector and the sum variable
    maior = 0;
    sum = 0;
    menor = RAND_MAX;

    for(int i=0; i<POPSIZE; i++){

        if(type == 'b'){
            // fit[i] = pattern(i);
            fit[i] = deceptive(i);
            // fit[i] = f3(i);
            if(fit[i] > outFit){
                outFit = fit[i];
                outBin = popbin[i];
            }
        }

        else if(type == 'i'){
            fit[i] = nDamas(i);
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
        if(fit[i] < menor)
            menor = fit[i];
        sum += fit[i];
    }
}

void FitScaling(int i){
    double C = 0;
    // double C = (2 - 1.2) * i / MAXGENS + 1.2; //linear crescente
    // double C = (2 - 1.2) * (MAXGENS - i) / MAXGENS + 1.2; //linear decrescente
    // double C = 2.0 * pow(1.2 / 2.0, 1 - (double) i / MAXGENS); //cooling schedule 1
    // double C = 3.0 - pow(MAXGENS - i + 1, log(3.0 - 1.2) / (double) log(MAXGENS)); //cooling schedule 3
    // printf("%lf\n", C);
    double media = sum / POPSIZE;
    double alpha = 0, beta = 0;
    if(menor > (C * media - maior) / (C - 1)){
        alpha = (media * (C - 1)) / (maior - menor);
        beta = (media * (maior - C * media)) / (maior - media);
    }
    else{
        alpha = media / (media - menor);
        beta = (-menor * media) / (media - menor);
    }
    // printf("alpha: %lf, beta: %lf\n", alpha, beta);
    for (int j = 0; j < POPSIZE; ++j)
        fit[j] = alpha * fit[j] + beta;
}

void genShuffle(char type){
    if(type == 'b'){
        random_shuffle(popbin.begin(), popbin.end());
    }
    else if(type == 'i'){
        random_shuffle(popint.begin(), popint.end());
    }
    else{
        random_shuffle(popdou.begin(), popdou.end());
    }
}

void gapUpdate(char type, int i){
    if(gap == 0)
        return;
    static int space = (MAXGENS)/(GENGAP0+1)*2;
    if(i%space == 0 && i <= MAXGENS)
        gap -= 2;
    // printf("it: %d, gap: %d, space: %d\n", i, gap, space);
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
        if(i%(MAXGENS/10) == 0)
            printf("%d0%%\n", i/(MAXGENS/10));
        // printf("\nGen %d: \n", i-1);
        // printGen(type); //Escalonado

        // selection
        // printf("Roleta\n");
        roulette(type);

        //crossover
        // printInt(type);
        // printf("Crossover\n");
        // crossover1p(type);
        crossunif(type);
        // PMX(type);


        //mutation
        // printf("Mutation\n");
        mutation(type);
        // deltaMutation(type);
        // swapPosition(type);
        
        //elitism
        elitism(type);
        gapUpdate(type, i);

        //fitness update
        // printf("Fitness\n");
        Fitness(type);
        // printGen(type); //Nao Escalonado
        FitScaling(i);

        //log
        // printf("Log\n");
        div = diversity(type);
        logMedias(i, div);
        
        //shuffle
        genShuffle(type);
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