all:
	g++ -Wall main.cpp -o main -std=c++11 -lm genetic.cpp

plot:
	gnuplot plot.sh

clean:
	rm main *.png data.txt