test_input: test_input.c
	gcc test_input.c -o test_input -lgpiod 

test_cpp: Button.o test.o
	g++ Button.o test.o -o test_cpp -lgpiod 

Button.o: Button.h Button.cpp
	g++ -c -o Button.o Button.cpp -lgpiod

test.o: test.cpp
	g++ -c -o test.o test.cpp -lgpiod

Main.o: Main.cpp
	g++ -c -o Main.o Main.cpp -lgpiod

game: Button.o Main.o
	g++ Button.o Main.o -o game -lgpiod

clear:
	rm *.o