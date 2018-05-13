CC = gcc
py = python #python3

all:
	clear
	${CC} Analysis.h Analysis_Hugo.c main.c -o main
	python Analysis.py

clear: main.exe main
	rm main.exe
	rm main
