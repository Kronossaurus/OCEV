all:
	g++ -Wall main.cpp -o main -std=c++11 -lm genetic.cpp

clean:
	rm main