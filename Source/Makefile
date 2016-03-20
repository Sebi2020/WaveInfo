VERSION = 1.0.0
CC = gcc

all: winfo.exe
	
%.exe: main.o util.o
	gcc $^ -o $@ -O3

%.o: %.c waveinfo.h
	$(CC) -c $< -std=c99 -Wall -O3 -I.

.PHONY: clean 
clean:
	del /s *.o *.exe