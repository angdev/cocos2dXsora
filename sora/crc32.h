// Ŭnicode please
//http://www.redwiki.net/wiki/wiki.php/CRC32
#pragma once

#include "singleton.h"
#include <string>

SR_NS_BEGIN(sora)

class CRC32 : public Singleton<CRC32> {
public:
    CRC32();
	~CRC32();
	///@brief calculate the crc value
	unsigned int GetCRC(const std::string &str) const ;
	static unsigned int Get(const std::string &str);
	static unsigned int Hash(const std::string &str) {
		return GetInstance().GetCRC(str);
	}

private:
	///@brief build the crc table 
	void GenerateTable();
	unsigned int crc_table[256];
};

SR_NS_END(sora)