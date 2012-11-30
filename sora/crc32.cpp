// Ŭnicode please
#include "sora_stdafx.h"
#include "crc32.h"

SR_NS_BEGIN(sora)

CRC32::CRC32() { 
    GenerateTable(); 
}
CRC32::~CRC32() { }

void CRC32::GenerateTable() {
	unsigned int crc, poly;
	int i, j;

	poly = 0xEDB88320L;
	for (i = 0; i < 256; i++) {
		crc = i;
		for (j = 8; j > 0; j--) {
			if (crc & 1) {
				crc = (crc >> 1) ^ poly;
			} else {
				crc >>= 1;
			}
			crc_table[i] = crc;
		}
	}
}

unsigned int CRC32::GetCRC(const std::string &str) const {
	unsigned int crc = 0xFFFFFFFF;
	for(size_t i = 0 ; i < str.size() ; i++) {
		int ch = static_cast<int>(str[i]);
		crc = (crc>>8) ^ crc_table[ (crc^ch) & 0xFF ];
	}     
	return( crc^0xFFFFFFFF );
}

unsigned int CRC32::Get(const std::string &str) {
	return GetInstance().GetCRC(str);
}

SR_NS_END(sora)