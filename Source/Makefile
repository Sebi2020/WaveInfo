VERSION = 1.0.0

all: winfo.exe
.PHONY: clean all
winfo.exe: main.c util.c waveinfo.h
	gcc -Wall -std=c99 main.c util.c -o winfo.exe -DVERSION=$(VERSION) -O0
clean:
	del /s *.o *.exe