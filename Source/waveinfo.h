// Copyright Informatikonline.net / Energetic-Tech 2016
// This file is part of Waveinfo.
//
//	Waveinfo is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	Waveinfo is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
// 	You should have received a copy of the GNU General Public License
// 	along with Waveinfo. If not, see <http://www.gnu.org/licenses/>.

#include <stdint.h>

struct RIFF_header{
	char chunkId[4];			// Format-Identifizierer (RIFF)
	uint32_t chunkSize;			// Datei-Gräße
	char riffType[4];			// Container-Format (WAVE)
};

struct fmt_chunk_head {
	char fmt_head[4];
	uint32_t fmt_length;
} __attribute__((packed));

typedef struct fmt_chunk_head fmt_chk_head;
struct format_chunk {
	fmt_chk_head head;
	uint8_t wFormatTag[2];			// Audio-Format
	uint16_t wChannels;				// Kanäle
	uint32_t dwSamplesPerSec;		// Sample-Rate
	uint32_t dwAvgBytesPerSec;		// Durchschnittliche (nötige) Übertragungsbandbreite
	uint16_t wBlockAlign;			// Größe eines Frames in Byte
	uint16_t wBitsPerSample;		// Auflösung (Quantisierung)
	union {
		uint16_t wValidBitsPerSample;
		uint16_t wSamplesPerBlock;
		uint16_t wReserved;
	} Samples;
	uint32_t dwChannelMask;
	uint8_t subtype[16];
} __attribute__((packed));

typedef struct wav_header wav_header;
typedef struct format_chunk format_chunk;

#define DEFINE_WAVEFORMATEX_GUID(x) \
    (uint8_t)(x),0x0,0x0,0x0,0x0,0x0, 0x10, 0x0,0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71

#define WAVEFORMAT_WAVE DEFINE_WAVEFORMATEX_GUID(0x1)
