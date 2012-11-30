// Ŭnicode please
#include "stdafx.h"
#include "sora/data_helper.h"
#include "sora/class_attribute.h"
#include "sora/class_dict.h"

#include "sora/data_define.h"
#include "sora/data_init.h"
#include "xml_util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace sora;

namespace sora_test {;

TEST_CLASS(DataHelperTest) {
public:
    TEST_METHOD(DataToXmlNode) {
        init_generated_data();

        DemoStruct_v1 data;
        data.a = 1;
        data.b = 2.0f;
        data.c = true;
        data.d = "asdf";
        data.f.push_back("a");
        data.f.push_back("b");

        TiXmlElement *element = new TiXmlElement("empty");
        DataHelper::DataToXmlNode(&data, element);

        //write as string
        dump_to_stdout(element);

        BaseDataPtr result = DataHelper::XmlNodeToData(element);
        Assert::AreEqual(data.class_hash, result->class_hash);
        Assert::AreEqual(true, DataHelper::Equal(&data, result.get()));

    }

    TEST_METHOD(Equal) {
        init_generated_data();

        DemoStruct_v1 data_a;
        data_a.a = 1;
        data_a.b = 2.0f;
        data_a.c = true;
        data_a.d = "asdf";
        data_a.f.push_back("a");
        data_a.f.push_back("b");

        DemoStruct_v2 data_b;
        data_b.c = "asdf";

        DemoStruct_v1 data_c;
        data_c.a = 4;
        data_c.b = 243.0f;
        data_c.c = true;
        data_c.d = "asdf";
        data_c.f.push_back("a");

        DemoStruct_v1 data_d;
        data_d.a = 1;
        data_d.b = 2.0f;
        data_d.c = true;
        data_d.d = "asdf";
        data_d.f.push_back("a");
        data_d.f.push_back("b");

        Assert::AreEqual(true, DataHelper::Equal(&data_a, &data_a));	//자체 비교
        Assert::AreEqual(false, DataHelper::Equal(&data_a, &data_b));	//다른 타입
        Assert::AreEqual(false, DataHelper::Equal(&data_a, &data_c));	//내용 다름
        Assert::AreEqual(true, DataHelper::Equal(&data_a, &data_d));
    }

    TEST_METHOD(Reset) {
        DemoStruct_v1 data;
        DataHelper::Reset(&data);

        Assert::AreEqual(0, data.a);
        Assert::AreEqual(0.0f, data.b);
        Assert::AreEqual(false, data.c);
        Assert::AreEqual(0, (int)data.d.length());
        Assert::AreEqual(0, (int)data.f.size());
    }

    TEST_METHOD(RestGameHeader) {
        PolyBlockHeader header;
        DataHelper::ResetGameHeader(&header);
        Assert::AreEqual((int)GetStructType<PolyBlockHeader>::value, (int)header.type);

        CoinHeader coin;
        DataHelper::ResetGameHeader(&coin);
        Assert::AreEqual((int)GetStructType<CoinHeader>::value, (int)coin.type);

        BalloonHeader *balloon = new BalloonHeader();
        DataHelper::ResetGameHeader(balloon);
    }

    TEST_METHOD(XmlNodeToData) {
        string xml = "	\
                     <PolyBlockHeader_v1 base='PolyBlockHeader' version='1'>	\
                     <name>block18</name>	\
                     <str_list>139:523:198:772</str_list>	\
                     </PolyBlockHeader_v1>";

        TiXmlDocument doc;
        doc.Parse(xml.c_str());

        BaseDataPtr data = DataHelper::XmlNodeToData(doc.FirstChildElement());
        int type_code = DataHelper::GetStructTypeCode(data.get());
        Assert::AreEqual(PolyBlockHeader_v1::ClassHash(), data->class_hash);
        Assert::AreEqual(PolyBlockHeader_v1::BaseHash(), data->base_hash);

        PolyBlockHeader *header = DataHelper::ClassCast<PolyBlockHeader>(data.get());
        Assert::AreEqual((int)NULL, (int)DataHelper::ClassCast<CoinHeader>(data.get()));	//올바르지 않은거로 캐스팅
        Assert::AreEqual(true, header != NULL);
        Assert::AreEqual("block18", header->name.c_str());

        Assert::AreEqual(4, (int)header->str_list.size());
        Assert::AreEqual("139", header->str_list[0].data());
        Assert::AreEqual("523", header->str_list[1].data());
        Assert::AreEqual("198", header->str_list[2].data());
        Assert::AreEqual("772", header->str_list[3].data());
    }
};
}

