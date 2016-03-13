#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "waveinfo.h"


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

void print_channel_detail(uint32_t dwChannelMask) {
	if(SPEAK_MASK_FL & dwChannelMask) printf("Vorne links");
	if(SPEAK_MASK_FR & dwChannelMask) printf(", vorne rechts");
	if(SPEAK_MASK_FC & dwChannelMask) printf(", vorne mitte");
	if(SPEAK_MASK_LF & dwChannelMask) printf(", LO-Kanal (Subwoofer)");
	if(SPEAK_MASK_BL & dwChannelMask) printf(", hinten links");
	if(SPEAK_MASK_BC & dwChannelMask) printf(", hinten mitte");
	if(SPEAK_MASK_BR & dwChannelMask) printf(", hinten rechts");
	if(SPEAK_MASK_SL & dwChannelMask) printf(", seitlich links");
	if(SPEAK_MASK_SR & dwChannelMask) printf(", seitlich rechts");
}