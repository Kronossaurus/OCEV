#ifndef GENETIC_H
#define GENETIC_H

#include <bitset>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <chrono>
using std::bitset;
using std::vector;
using std::default_random_engine;
using std::uniform_real_distribution;

//Extra Parameters
#define ENCSIZE 10
#define POPSIZE 500
#define RANGEINF -2
#define RANGESUP 4
#define MUTATERT 1
#define CROSSRT 80
#define MAXGENS 100

// Fitness Functions
int bAlternados(int i, char tipo);
double x2(int i);
double customFunc1(int i);


void mutation(char type);
void crossover(char tipo);
void printGen(char tipo);
void printInt(char tipo);
void init(char tipo);
int binToDec(int i);
void roulette(char tipo);

void logMedias(int iteration);

void AG(char type);
void Fitness(char type);
#endif