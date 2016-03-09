#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>


int find_chk_fourcc(char* chunk, char* data, int len) {
	char match = 0;
	int i = 0;
	for(i=0; i < len-4; i++) {
		if(data[i]==chunk[0] && data[i+1]==chunk[1] && data[i+2] == chunk[2] && data[i+3] == chunk[3]) match = 1;
		if(match == 1) break;
	}
	if(match == 0) return -1;
	return i;
}

int seek_to_fourcc(char* fourcc, FILE* file, int maxlength) {
	int pos_before = ftell(file);
	char buf[maxlength];
	int offset = 0;
	fread(buf, 4,128,file);
	offset = find_chk_fourcc(fourcc, buf, maxlength);
	if(offset == -1) return -1;
	fseek(file,pos_before + offset, 0);
	return 0;
}