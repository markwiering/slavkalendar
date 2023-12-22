run:
	g++ -std=c++17 -Wall -o slavprog.uit slavprog.cpp
	
runclang:
	clang++ -std=c++17 -Wall -o slavprog.uit slavprog.cpp
	
rungcc:
	gcc -std=c++17 -Wall -o slavprog.uit slavprog.cpp

clean: 
	rm slavprog.uit
	
opschonen: 
	rm slavprog.uit
