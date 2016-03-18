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
#include "waveinfo.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifndef VERSION
#define VERSION 1.0
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
		printf("Unbekannt (0x%04X)", format_tag);
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
		fprintf(stderr, "Geben Sie eine Wave-Datei an: %s [Dateiname]\r\n\t-c zeigt Copyrightinformationen an\r\n\t-w zeigt Gewährleistungs-Informationen an\r\n\
\t-v gibt die Programmversion aus\r\n", argv[0]);
		exit(1);
	}
	// as
	if(strcmp(argv[1], "-w") == 0) {
			printf("This program comes with ABSOLUTELY NO WARRANTY!\r\n");
			exit(0);
	} else if(strcmp(argv[1], "-c") == 0) {
			printf("Copyright (C) 2016 Informatikonline.net / Energetic Tech\r\n");
			exit(0);
	} else if(strcmp(argv[1], "-v") == 0) {
			printf("Programmversion: %s", TOSTRING(VERSION));
			exit(0);
	}
	struct RIFF_header header;
	format_chunk fmt_chunk;
	FILE* ifile = 0x0;
	ifile = fopen(argv[1], "rb");
	if(ifile == 0x0) {
		fprintf(stderr, "Kann die Datei \"%s\" nicht öffnen: %s", argv[1], strerror(errno));
		exit(errno);
	}

	fread(header.chunkId, 1,4, ifile);
	if(strncmp("RIFF", header.chunkId,4) != 0) {
		fprintf(stderr, "Falsches Datei-Format!");
		exit(1);
	}
	fread(&header.chunkSize, 4, 2, ifile);
	if(strncmp("WAVE", header.riffType,4) != 0) {
		fprintf(stderr, "Dies ist keine Wave-Datei!");
		exit(2);
	}
	printf("[INFO] Wave-Header gefunden!\r\n");
	printf("------ HEADER ------\r\n");
	printf("Datei-Format:\tRIFF\r\n");
	wprintf(L"Länge (Header):\t%4.2f kB\r\n",header.chunkSize/ (float)1024);
	printf("RIFF-Type:\tWAVE\r\n");
	printf("--- HEADER - END ---\r\n\r\n");
	if(seek_to_fourcc("fmt ", ifile, 512)) {
		fprintf(stderr, "[ERR] Kein fmt Header gefunden!");
		exit(3);
	}
	fread(&fmt_chunk,2,12, ifile);
	printf("[INFO] fmt-Chunk gefunden!\r\n");
	if(fmt_chunk.chunkSize > 18) printf("[INFO] Wave-Extensible Header gefunden!\r\n");
	printf("--------- fmt - Chunk ---------\r\n");
	printf("ChunkId:\t\tfmt \r\n");
	wprintf(L"Länge (fmt):\t\t%d Bytes\r\n", fmt_chunk.chunkSize);
	printf("Audio-Format:\t\t");
	print_audio_fmt(*((uint16_t*) fmt_chunk.data.wFormatTag));
	wprintf(L"Kanäle:\t\t\t");
	print_channels(fmt_chunk.data.wChannels);
	printf("Sample-Rate:\t\t%d Samples/s\r\n", fmt_chunk.data.dwSamplesPerSec);
	printf("Datenrate:\t\t%d kbit/s\r\n", (fmt_chunk.data.dwAvgBytesPerSec*8)/1024);
	wprintf(L"Frame-Größe:\t\t%d Bytes/Frame\r\n", fmt_chunk.data.wBlockAlign);
	printf("Bit-Tiefe:\t\t%hu Bit\r\n", fmt_chunk.data.wBitsPerSample);
	if(fmt_chunk.chunkSize > 18) {
	fread(&fmt_chunk.data.wValidBitsPerSample, 2,12,ifile);
	printf("Bit-Tiefe (rec):\t%hu Bit\r\n", fmt_chunk.data.wValidBitsPerSample);
	printf("Kanal-Maske:\t\t");
	print_channel_detail(fmt_chunk.data.dwChannelMask);
	printf(" (0x%hx)\r\n",fmt_chunk.data.dwChannelMask);
	}
	printf("------ END - fmt - Chunk ------\r\n\r\n");
	data_chunk data;
	if(seek_to_fourcc("data",ifile,512)) printf("[INFO] Kein Daten-Chunk gefunden!\r\n");
	else {
		printf("[INFO] Data-Chunk gefunden!\r\n");
		fread(&data, 4,2,ifile);

	printf("--------- Data-Chunk ----------\r\n");
	printf("ChunkId:\t\tdata\r\n");
	wprintf(L"Länge:\t\t\t%3.2f kB\r\n", data.chunkSize/ (float) 1024);
	wprintf(L"Daten:\t\t\t(Binärdaten)\r\n");
	printf("Kalk. Abspielzeit:\t%2.2f s\r\n", data.chunkSize/ (float)	fmt_chunk.data.dwAvgBytesPerSec);
	printf("------- END Data -Chunk -------\r\n\r\n");
	}
	fclose(ifile);
	
	return 0;
}