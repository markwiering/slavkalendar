install:
	clang++ -std=c++17 -Wall -o slavprog.uit slavprog.cpp
	
gcc:
	g++ -std=c++17 -Wall -o slavprog.uit slavprog.cpp

run:
	./slavprog.uit
	
draai:
	./slavprog.uit

clean: 
	rm slavprog.uit
	
opschonen: 
	rm slavprog.uit
