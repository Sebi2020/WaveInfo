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


// Defines

// RIFF - Format - CHUNK - Types for info table -- see below
#define RIFF_CHUNK_TYPE_RIFF 1
#define RIFF_CHUNK_TYPE_LIST 2
#define RIFF_CHUNK_TYPE_FMT 3
#define RIFF_CHUNK_TYPE_DATA 4

// Typedefs
typedef struct chunk_info_entry chunk_info_entry;
typedef struct RIFF_Header RIFF_header;
typedef struct fmt_chunk_data fmt_chunk_data;
typedef struct format_chunk format_chunk;
typedef struct data_chunk data_chunk;

// Structs

struct chunk_info_entry {
	uint8_t type;
	uint32_t length;
	uint32_t offset;
	const char* hr_name;
};

struct RIFF_header{
	char chunkId[4];			// Format-Identifizierer (RIFF)
	uint32_t chunkSize;			// Datei-Gräße
	char riffType[4];			// Container-Format (WAVE)
};

struct fmt_chunk_data {
	uint8_t wFormatTag[2];			// Audio-Format
	uint16_t wChannels __attribute__((packed));		// Kanäle
	uint32_t dwSamplesPerSec __attribute__((packed));		// Sample-Rate
	uint32_t dwAvgBytesPerSec __attribute__((packed));		// Durchschnittliche (nötige) Übertragungsbandbreite
	uint16_t wBlockAlign __attribute__((packed));		// Größe eines Frames in Byte
	uint16_t wBitsPerSample __attribute__((packed));		// Auflösung (Quantisierung)
	uint16_t wValidBitsPerSample __attribute__((packed));	// Aufnahme-Auflösung
	uint16_t cb_size;
	uint32_t dwChannelMask __attribute__((packed));			// Kanal-Maske (siehe unten)
	uint8_t subtype[16];			// Audio-Daten-Typ GUID (Leitet sich vom Format-Tag ab)
} __attribute__((packed));				// wird von Microsoft verwendet für RIFF-Format Erweiterungen
										// und von MS-Windows Miniport-Treibern
										
struct format_chunk {
	char chunkId[4];			// fmt-Chunk Identifizierer (fmt )
	uint32_t chunkSize;		// Länge des fmt (Format)-Chunks	
	fmt_chunk_data data;
} __attribute__((packed));

struct data_chunk {
	char chunkId[4];
	uint32_t chunkSize;
	uint8_t* data;
};

// Macros
// Subtype GUIDs (MS Extension)
#define DEFINE_WAVEFORMATEX_GUID(x) \
    (uint8_t)(x),0x00,0x00,0x00,0x00,0x00, 0x10, 0x00,0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71

#define WAVEFORMAT_WAVE DEFINE_WAVEFORMATEX_GUID(0x01)

// Function declarations
int find_chk_fourcc(char* chunk, char* data, int len);
int seek_to_fourcc(char* chunk, FILE* file, int maxlength);