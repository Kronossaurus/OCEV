#include <stdio.h>
#include <math.h>
// Calculates the chance of at least one mutation occur in an individual
// given its encoding size and the mutation rate

int main(){
	printf("Provide the encoding size and the mutation rate\n");
	printf("Size: int\nRate: [0, 1]\n");
	int size;
	double rate, result;
	scanf("%d %lf", &size, &rate);

	result = 100 * (1 - pow(1-rate, size));

	printf("\n%lf%%\n", result);
	return 0;
}