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
#define ENCSIZE 10
#define POPSIZE 2000
#define RANGEINF -5
#define RANGESUP 5
#define MUTATERT 5
#define CROSSRT 80
#define MAXGENS 400

// Fitness Functions
int bAlternados(int i, char tipo);
double x2(int i);
double x2_2(int i);
double customFunc1(int i);


void mutation(char type);
void crossover1p(char tipo);
void crossunif(char tipo);
void printGen(char tipo);
void printInt(char tipo);
void init(char tipo);
int binToDec(int i);
void roulette(char tipo);

void logMedias(int iteration);

void AG(char type);
void Fitness(char type);
#endif