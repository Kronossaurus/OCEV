all:
	g++ -Wall main.cpp -o main -std=c++11 -lm -fopenmp genetic.cpp

plot:
	gnuplot plot.sh

calc:
	gcc -Wall mutatCalc.c -o calc -lm

clean:
	rm main *.png data.txt calc