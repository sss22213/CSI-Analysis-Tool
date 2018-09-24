CC = gcc
py = python #python3

all:
	clear
	${CC} Analysis.h Analysis_Hugo.c main.c -o main

clear: main.exe main
	rm main.exe
	rm main

socket:
	clear
	${CC} Analysis.h Analysis_Hugo.c server.c server.h -lwsock32 -o main
	main 
 
debug:
	clear
	${CC} Analysis.h Analysis_Hugo.c main.c -o main -g -Wall
	gdb -q main
