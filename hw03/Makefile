gpio-utils.o: gpio-utils.h gpio-utils.c
	g++ -c -o gpio-utils.o gpio-utils.c

Button.o: Button.h Button.cpp
	g++ -c -o Button.o Button.cpp

Main.o: Main.cpp
	g++ -c -o Main.o Main.cpp

i2cbusses.o: i2cbusses.h i2cbusses.c i2c-dev.h
	g++ -c -o i2cbusses.o i2cbusses.c

game: Button.o Main.o gpio-utils.o i2cbusses.o
	g++ Button.o Main.o gpio-utils.o i2cbusses.o -o game



clear:
	rm *.o