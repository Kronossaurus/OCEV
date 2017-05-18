#ifndef GENETIC_H
#define GENETIC_H

#include <bitset>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <chrono>
#include <omp.h>
using std::bitset;
using std::vector;
using std::default_random_engine;
using std::uniform_real_distribution;
using std::random_shuffle;

//Extra Parameters
#define ENCSIZE 256
#define POPSIZE 50
#define RANGEINF 1
#define RANGESUP ENCSIZE
#define ELITISM 1
#define GENGAP0 0
#define MUTATERT 2
#define CROSSRT 100
#define MAXGENS 2000

#define MODCONST 1000

// Fitness Functions
int bAlternados(int i, char tipo);
double x2(int i);
double x2_2(int i);
double customFunc1(int i);
int pattern(int i);
double nDamas(int i);

void elitism(char tipo);
void mutation(char type);
void crossover1p(char tipo);
void crossunif(char tipo);
void printGen(char tipo);
void printInt(char tipo);
void init(char tipo);
int binToDec(int i);
void roulette(char tipo);
void FitScaling(int i);

double diversity(char type);
void logMedias(int iteration, double div);
void genShuffle(char type);

void AG(char type);
void Fitness(char type);
#endif