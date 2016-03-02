VERSION = 1.0.0

all: winfo.exe
.PHONY: clean all
winfo.exe: main.c waveinfo.h
	gcc -Wall -std=c99 main.c -o winfo.exe -DVERSION=$(VERSION)
clean:
	del /s *.o *.exe