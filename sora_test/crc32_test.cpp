// Ŭnicode please
#include "stdafx.h"
#include "crc32.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace sora;

namespace sora_test {;
TEST_CLASS(CRC32Test) {
public:
    //http://zorc.breitbandkatze.de/crc.html
    //crc 계산기를 이용해서 테스트값을 확인함
    //테스트값
    //CRC order : 32
    //CRC polynom : 4C11DB7
    //initial value : ffffffff
    //final xor value : ffffffff
    TEST_METHOD(GetCRC) {
	    string str = "abc";
	    unsigned int value = CRC32::Get(str);
	    Assert::AreEqual(0x352441c2u, value);

	    str = "asdf1234";
	    value = CRC32::Get(str);
	    Assert::AreEqual(0x9b5345e4u, value);
    }
};
}

