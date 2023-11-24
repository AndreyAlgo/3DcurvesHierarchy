default: main

main: main.cpp
	g++ -I/usr/include main.cpp -o main -lm -L/usr/lib -lqt-mt
