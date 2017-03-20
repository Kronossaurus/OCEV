#ifndef GENETIC_H
#define GENETIC_H

#include <bitset>
#include <vector>
#include <algorithm>
#include <random>
using std::bitset;
using std::vector;
using std::default_random_engine;
using std::uniform_real_distribution;

//Extra Parameters
#define ENCSIZE 10
#define POPSIZE 10
#define RANGEINF 0
#define RANGESUP 10
#define MUTATERT 5

int bAlternados(int i, char tipo);
double x2(int i);
void mutation(char type);
void crossover(char tipo, int i, int j);
void printGen(char tipo);
void init(char tipo);

#endif