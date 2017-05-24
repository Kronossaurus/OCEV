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
#define ENCSIZE 20*3
#define POPSIZE 50
#define RANGEINF 1
#define RANGESUP ENCSIZE
#define ELITISM 1
#define GENGAP0 12
#define MUTATERT 1
#define CROSSRT 98
#define MAXGENS 3000

#define MODCONST 100

// Fitness Functions
int bAlternados(int i, char tipo);
int deceptive(int i);
double x2(int i);
double x2_2(int i);
double customFunc1(int i);
int pattern(int i);
double nDamas(int i);
double f3(int i);
double f3S(int i);

void elitism(char tipo);
void mutation(char type);
void crossover1p(char tipo);
void crossunif(char tipo);
void printGen(char tipo);
void printInt(char tipo);
void init(char tipo);
int binToDec(int i);
int binToDecf3(int b[]);
void roulette(char tipo);
void FitScaling(int i);

double diversity(char type);
void logMedias(int iteration, double div);
void genShuffle(char type);

void AG(char type);
void Fitness(char type);
#endif