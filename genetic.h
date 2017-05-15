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

//Extra Parameters
#define ENCSIZE 6
#define POPSIZE 10
#define RANGEINF 1
#define RANGESUP ENCSIZE
#define ELITISM 0
#define MUTATERT 0
#define CROSSRT 0
#define MAXGENS 4

#define MODCONST 100

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

void AG(char type);
void Fitness(char type);
#endif