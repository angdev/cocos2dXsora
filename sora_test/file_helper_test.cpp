// Ŭnicode please
#include "stdafx.h"
#include "sora/file_helper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace sora;
using namespace std;

namespace sora_test {;
TEST_CLASS(FileHelperTest) {
public:
    TEST_METHOD(RemoveSuffix)
    {
	    {
		    const char *hd_str = "ani/shark-hd.png";
		    const char *ipadhd_str = "ani/shark-ipadhd.png";
		    const char *normal_str = "ani/shark.png";
		    Assert::AreEqual(normal_str, FileHelper::RemoveSuffix(hd_str).c_str());
		    Assert::AreEqual(normal_str, FileHelper::RemoveSuffix(ipadhd_str).c_str());
		    Assert::AreEqual(normal_str, FileHelper::RemoveSuffix(normal_str).c_str());
	    }
	    {
		    const char *hd_str = "ani/red_mice-basic-hd.xml";
		    const char *ipadhd_str = "ani/red_mice-basic-ipadhd.xml";
		    const char *normal_str = "ani/red_mice-basic.xml";
		    Assert::AreEqual(normal_str, FileHelper::RemoveSuffix(hd_str).c_str());
		    Assert::AreEqual(normal_str, FileHelper::RemoveSuffix(ipadhd_str).c_str());
		    Assert::AreEqual(normal_str, FileHelper::RemoveSuffix(normal_str).c_str());
	    }
    }

};
}
