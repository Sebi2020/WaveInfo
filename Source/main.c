// Copyright Informatikonline.net / Energetic Tech 2016
// This file is part of Waveinfo.
//
//	Waveinfo is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.

//	Waveinfo is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.

//	You should have received a copy of the GNU General Public License
//	along with Waveinfo.  If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <locale.h>

#ifndef _WIN32
#define _UNICODE
#define UNICODE
#include <errno.h>
#endif

#include "waveinfo.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef CMAKE_BUILD
#include "WaveInfoConfig.h"
#endif

#ifndef VERSION
#define VERSION 1.01
#endif

int find_data_chk(FILE* file) {
	char ibuf[512];
	int cur_pos = ftell(file);
	printf("Position before: %d\r\n", cur_pos);
	fread(ibuf, 1,5,file);
	printf("Position after: %ld\r\n", ftell(file));
	char* pos = strstr(ibuf, "data");
	printf("ibuf: \"%s\"\r\n", ibuf);
	printf("strstr: \"%s\"\r\n", pos);
	return cur_pos;
}
void print_audio_fmt(int format_tag) {
	switch(format_tag) {
		case WAVE_FMT_PCM:
		printf("Pulsecode Modulation (PCM)");
		break;
		case WAVE_FMT_DOLBY_AC2:
		printf("Dolby Digital AC2");
		break;
		case WAVE_FMT_ADPCM:
		printf("Microsoft Adaptive PCM");
		break;
		case WAVE_FMT_MPEG:
		printf("MPEG Audio");
		break;
		case WAVE_FMT_YM_ADPCM:
		printf("Yamaha Adaptive PCM");
		break;
		case WAVE_FMT_CT_ADPCM:
		printf("Creative Adaptive PCM");
		break;
		case WAVE_FMT_MS_ADPCM:
		printf("Mediaspace Adaptive PCM");
		break;
		case WAVE_FMT_ALAW:
		printf("Microsoft A-Law Audio");
		break;
		default:
		printf("Unkown (0x%04X)", format_tag);
	}
	printf("\r\n");
}
void print_channels(int channel_num) {
		switch(channel_num) {
		case 1:
		printf("Mono (1)\r\n");
		break;
		case 2:
		printf("Stereo (2)\r\n");
		break;
		case 3:
		printf("2.1 (3)\r\n");
		break;
		case 4:
		printf("Quadrophonie (4)\r\n");
		break;
		case 5:
		printf("Surround (5)\r\n");
		break;
		case 6:
		printf("5.1 Surround (6)\r\n");
		break;
		case 7:
		printf("7.0 Surround (7)\r\n");
		break;
		case 8:
		printf("7.1 Surround (8)\r\n");
		break;
		default:
		printf("%d\r\n", channel_num);
	}
}
int main(int argc, char** argv) {
	setlocale(LC_ALL, "");
	if(argc < 2) {
		fprintf(stderr, "Specify a wave file: %s [Dateiname]\r\n\t-c show Copyright information\r\n\t-w shows warranty information\r\n\
\t-v prints programm version\r\n", argv[0]);
		exit(1);
	}

	if(strcmp(argv[1], "-w") == 0) {
			printf("This program comes with ABSOLUTELY NO WARRANTY!\r\n");
			exit(0);
	} else if(strcmp(argv[1], "-c") == 0) {
			printf("Copyright (C) 2016 Informatikonline.net / Energetic Tech\r\n");
			exit(0);
	} else if(strcmp(argv[1], "-v") == 0) {
			printf("Programmversion: %s\r\n", TOSTRING(VERSION));
			#ifdef _WIN32
			printf(" Windows Build\r\n");
			#endif
			exit(0);
	}
	struct RIFF_header header;
	format_chunk fmt_chunk;
	FILE* ifile = 0x0;
	ifile = fopen(argv[1], "rb");
	if(ifile == 0x0) {
		fprintf(stderr, "Cannot open file \"%s\": %s", argv[1], strerror(errno));
		exit(errno);
	}

	fread(header.chunkId, 1,4, ifile);
	if(strncmp("RIFF", header.chunkId,4) != 0) {
		fprintf(stderr, "Wrong file format!");
		exit(1);
	}
	fread(&header.chunkSize, 4, 2, ifile);
	if(strncmp("WAVE", header.riffType,4) != 0) {
		fprintf(stderr, "This is not a wave file!");
		exit(2);
	}
	printf("[INFO] Wave header found!\r\n");
	printf("------ HEADER ------\r\n");
	printf("File-Format:\tRIFF\r\n");
	printf("Length (header):\t%4.2f kB\r\n",header.chunkSize/ (float)1024);
	printf("RIFF-Type:\tWAVE\r\n");
	printf("--- HEADER - END ---\r\n\r\n");
	if(seek_to_fourcc("fmt ", ifile)) {
		fprintf(stderr, "[ERR] No fmt header present!");
		exit(3);
	}
	fread(&fmt_chunk,2,12, ifile);
	printf("[INFO] Found fmt-chunk!\r\n");
	if(fmt_chunk.chunkSize > 18) printf("[INFO] Wave-Extensible header found!\r\n");
	printf("--------- fmt - Chunk ---------\r\n");
	printf("ChunkId:\t\tfmt \r\n");
	printf("Length (fmt):\t\t%d Bytes\r\n", fmt_chunk.chunkSize);
	printf("Audioformat:\t\t");
	print_audio_fmt(fmt_chunk.chunk.data.wFormatTag);
	printf("Channels:\t\t");
	print_channels(fmt_chunk.chunk.data.wChannels);
	printf("Sample-rate:\t\t%d Samples/s\r\n", fmt_chunk.chunk.data.dwSamplesPerSec);
	printf("Data-rate:\t\t%d kbit/s\r\n", (fmt_chunk.chunk.data.dwAvgBytesPerSec*8)/1024);
	printf("Frame-Size:\t\t%d Bytes/Frame\r\n", fmt_chunk.chunk.data.wBlockAlign);
	printf("Bit-Depth:\t\t%hu Bit\r\n", fmt_chunk.chunk.data.wBitsPerSample);

	if(fmt_chunk.chunkSize > 18) {
	fread(&fmt_chunk.chunk.data_ex.wValidBitsPerSample, 2,12,ifile);
	printf("Bit-Depth (rec):\t%hu Bit\r\n", fmt_chunk.chunk.data_ex.wValidBitsPerSample);
	printf("Channel-Mask:\t\t");
	print_channel_detail(fmt_chunk.chunk.data_ex.dwChannelMask);
	printf(" (0x%hx)\r\n",fmt_chunk.chunk.data_ex.dwChannelMask);
	}
	printf("------ END - fmt - Chunk ------\r\n\r\n");

	data_chunk data;
	if(seek_to_fourcc("data",ifile)) printf("[INFO] No Data-Chunk present!\r\n");
	else {
		printf("[INFO] Data-Chunk present!\r\n");
		fread(&data, 4,2,ifile);

	printf("--------- Data-Chunk ----------\r\n");
	printf("ChunkId:\t\tdata\r\n");
	printf("Length:\t\t\t%3.2f kB\r\n", data.chunkSize/ (float) 1024);
	printf("Data:\t\t\t(Binarydata)\r\n");
	printf("Calc. play duration:\t%2.2f s\r\n", data.chunkSize/ (float)	fmt_chunk.chunk.data.dwAvgBytesPerSec);
	printf("------- END Data -Chunk -------\r\n\r\n");
	}
	fclose(ifile);
	return 0;
}
