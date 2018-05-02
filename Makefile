CC = gcc
py = python #python3

all: Analysis.c Analysis.h main.c
	clear
	${CC} Analysis.c Analysis.h main.c -o main
	python main.py

clear: main.exe main
	rm main.exe
	rm main
