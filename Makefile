all : main.o utils.o
	clang++ --std=c++20 main.o utils.o -o table

main.o : source/main.cpp
	clang++ --std=c++20 -c source/main.cpp -o main.o

utils.o : source/utils.cpp
	clang++ --std=c++20 -c source/utils.cpp -o utils.o

clean : 
	rm *.o table