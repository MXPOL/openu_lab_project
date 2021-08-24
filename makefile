maman14: main.o passManager.o firstPass.o commonFuncs.o directivesManager.o firstPassCommandsManager.o firstPassFunctions.o	outputManager.o secondPassManager.o secondPassCommandManager.o 
	gcc   -Wall -ansi -pedantic main.o commonFuncs.o passManager.o firstPass.o directivesManager.o firstPassCommandsManager.o firstPassFunctions.o	outputManager.o secondPassManager.o secondPassCommandManager.o  -o maman14

main.o:	main.c
	gcc  -c -Wall -ansi -pedantic main.c -o main.o

passManager.o: passManager.c
	gcc  -c -ansi -Wall -pedantic passManager.c -o passManager.o

firstPass.o: firstPass.c
	gcc  -c -ansi -Wall -pedantic firstPass.c -o firstPass.o

commonFuncs.o: commonFuncs.c
	gcc   -c -ansi -Wall -pedantic commonFuncs.c -o commonFuncs.o

directivesManager.o: directivesManager.c
	gcc  -c -ansi -Wall -pedantic directivesManager.c -o directivesManager.o

firstPassCommandsManager.o: firstPassCommandsManager.c
	gcc   -c -ansi -Wall -pedantic firstPassCommandsManager.c -o firstPassCommandsManager.o

firstPassFunctions.o:	firstPassFunctions.c
	gcc  -c -ansi -Wall -pedantic firstPassFunctions.c -o firstPassFunctions.o	

outputManager.o:	outputManager.c
	gcc  -c -Wall -ansi -pedantic outputManager.c -o outputManager.o

secondPassManager.o:	secondPassManager.c
	gcc  -c -Wall -ansi -pedantic secondPassManager.c -o secondPassManager.o

secondPassCommandManager.o:	secondPassCommandManager.c
	gcc  -c -Wall -ansi -pedantic secondPassCommandManager.c -o secondPassCommandManager.o

