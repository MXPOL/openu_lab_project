maman14: main.o passManager.o firstPass.o commonFuncs.o directivesManager.o firstPassCommandsManager.o firstPassCommandsManager.o
	gcc -m32  -Wall -ansi -pedantic main.o commonFuncs.o passManager.o firstPass.o directivesManager.o -o maman14


main.o:	main.c
	gcc -m32 -c -Wall -ansi -pedantic main.c -o main.o

passManager.o: passManager.c
	gcc -m32 -c -ansi -Wall -pedantic passManager.c -o passManager.o

firstPass.o: firstPass.c
	gcc -m32 -c -ansi -Wall -pedantic firstPass.c -o firstPass.o

commonFuncs.o: commonFuncs.c
	gcc -m32  -c -ansi -Wall -pedantic commonFuncs.c -o commonFuncs.o

directivesManager.o: directivesManager.c
	gcc -m32 -c -ansi -Wall -pedantic directivesManager.c -o directivesManager.o

firstPassCommandsManager.o: firstPassCommandsManager.c
	gcc -c -ansi -Wall -pedantic firstPassCommandsManager.c -o firstPassCommandsManager.o
