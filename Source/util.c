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

int seek_to_fourcc(char* fourcc, FILE* file) {
	fseek(file, 0,0);
	//int pos_before = ftell(file);
	char buf[512];
	int offset = 0;
	fread(buf, 4,128,file);
	offset = find_chk_fourcc(fourcc, buf, 512);
	if(offset == -1) {
		return -1;
	}
	fseek(file,offset, 0);
	return 0;
}

void print_channel_detail(uint32_t dwChannelMask) {
	if(SPEAK_MASK_FL & dwChannelMask) printf("front left");
	if(SPEAK_MASK_FR & dwChannelMask) printf(", front right");
	if(SPEAK_MASK_FC & dwChannelMask) printf(", front middle");
	if(SPEAK_MASK_LF & dwChannelMask) printf(", LO-Channel (Subwoofer)");
	if(SPEAK_MASK_BL & dwChannelMask) printf(", surround left");
	if(SPEAK_MASK_BC & dwChannelMask) printf(", surround middle");
	if(SPEAK_MASK_BR & dwChannelMask) printf(", surround right");
	if(SPEAK_MASK_SL & dwChannelMask) printf(", rear left");
	if(SPEAK_MASK_SR & dwChannelMask) printf(", rear right");
}