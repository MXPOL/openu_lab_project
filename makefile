maman14: main.o 
	gcc -m32  -Wall -ansi -pedantic main.o  -o maman14


main.o:	main.c
	gcc -m32 -c -Wall -ansi -pedantic main.c -o main.o