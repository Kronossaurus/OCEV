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
#define ENCSIZE 16
#define POPSIZE 10
#define RANGEINF -2
#define RANGESUP 20
#define MUTATERT 5

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

void logMedias(int iteration, double value);

#endif