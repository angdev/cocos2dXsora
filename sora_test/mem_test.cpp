/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "stdafx.h"
#include "sora/mem.h"

using namespace sora;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace sora_test {;
TEST_CLASS(MemoryTest) {
public:
    TEST_METHOD(Malloc_Free) {

        AllocState alloc_state;

        AllocStat(&alloc_state);
        Assert::AreEqual(0, alloc_state.bytes);
        Assert::AreEqual(0, alloc_state.count);

        void *ptr1 = SR_MALLOC(4);
        AllocStat(&alloc_state);
        Assert::AreEqual(4, alloc_state.bytes);
        Assert::AreEqual(1, alloc_state.count);

        void *ptr2 = SR_MALLOC(8);
        AllocStat(&alloc_state);
        Assert::AreEqual(4+8, alloc_state.bytes);
        Assert::AreEqual(2, alloc_state.count);

        void *ptr3 = SR_MALLOC(16);
        AllocStat(&alloc_state);
        Assert::AreEqual(4+8+16, alloc_state.bytes);
        Assert::AreEqual(3, alloc_state.count);

        void *ptr4 = SR_MALLOC(32);
        AllocStat(&alloc_state);
        Assert::AreEqual(4+8+16+32, alloc_state.bytes);
        Assert::AreEqual(4, alloc_state.count);

        SR_FREE(ptr1);  // 4byte
        AllocStat(&alloc_state);
        Assert::AreEqual(8+16+32, alloc_state.bytes);
        Assert::AreEqual(3, alloc_state.count);

        SR_FREE(ptr4);  // 32byte
        AllocStat(&alloc_state);
        Assert::AreEqual(8+16, alloc_state.bytes);
        Assert::AreEqual(2, alloc_state.count);

        SR_FREE(ptr2);  // 8byte
        AllocStat(&alloc_state);
        Assert::AreEqual(16, alloc_state.bytes);
        Assert::AreEqual(1, alloc_state.count);

        SR_FREE(ptr3);  // 16byte
        AllocStat(&alloc_state);
        Assert::AreEqual(0, alloc_state.bytes);
        Assert::AreEqual(0, alloc_state.count);
    }

    TEST_METHOD(Malloc_Free_1) {
        AllocState alloc_state;

        AllocStat(&alloc_state);
        Assert::AreEqual(0, alloc_state.bytes);
        Assert::AreEqual(0, alloc_state.count);

        void *ptr1 = SR_MALLOC(4);
        AllocStat(&alloc_state);
        Assert::AreEqual(4, alloc_state.bytes);
        Assert::AreEqual(1, alloc_state.count);

        void *ptr2 = SR_MALLOC(8);
        AllocStat(&alloc_state);
        Assert::AreEqual(4+8, alloc_state.bytes);
        Assert::AreEqual(2, alloc_state.count);

        ///////////
        SR_FREE(ptr1);  // 4byte
        AllocStat(&alloc_state);
        Assert::AreEqual(8, alloc_state.bytes);
        Assert::AreEqual(1, alloc_state.count);

        void *ptr3 = SR_MALLOC(16);
        AllocStat(&alloc_state);
        Assert::AreEqual(8+16, alloc_state.bytes);
        Assert::AreEqual(2, alloc_state.count);

        void *ptr4 = SR_MALLOC(32);
        AllocStat(&alloc_state);
        Assert::AreEqual(8+16+32, alloc_state.bytes);
        Assert::AreEqual(3, alloc_state.count);

        SR_FREE(ptr3);  // 16byte
        AllocStat(&alloc_state);
        Assert::AreEqual(8+32, alloc_state.bytes);
        Assert::AreEqual(2, alloc_state.count);

        SR_FREE(ptr2);  // 8byte
        AllocStat(&alloc_state);
        Assert::AreEqual(32, alloc_state.bytes);
        Assert::AreEqual(1, alloc_state.count);

        SR_FREE(ptr4);  // 32byte
        AllocStat(&alloc_state);
        Assert::AreEqual(0, alloc_state.bytes);
        Assert::AreEqual(0, alloc_state.count);
    }

    TEST_METHOD(TagMalloc_TagFree) {
        AllocState alloc_state;

        void *ptr1 = SR_TAG_MALLOC(4, 1);
        void *ptr2 = SR_TAG_MALLOC(8, 2);
        void *ptr3 = SR_TAG_MALLOC(16, 1);
        void *ptr4 = SR_TAG_MALLOC(32, 2);

        AllocStat(&alloc_state);
        Assert::AreEqual(4+8+16+32, alloc_state.bytes);
        Assert::AreEqual(4, alloc_state.count);

        SR_TAG_FREE(2);
        AllocStat(&alloc_state);
        Assert::AreEqual(4+16, alloc_state.bytes);
        Assert::AreEqual(2, alloc_state.count);

        SR_TAG_FREE(1);
        AllocStat(&alloc_state);
        Assert::AreEqual(0, alloc_state.bytes);
        Assert::AreEqual(0, alloc_state.count);
    }

};


class TagAllocatorSample : public TagAllocator<1234, TagAllocatorSample> {
public:
	TagAllocatorSample() { cnt++; }
	~TagAllocatorSample() { cnt--; }
	static int cnt;
};
int TagAllocatorSample::cnt = 0;

TEST_CLASS(TagAllocatorTest) {
public:
    TEST_METHOD(tag_allocator_test) {
        Assert::AreEqual(0, TagAllocatorSample::cnt);

        TagAllocatorSample *obj1 = new TagAllocatorSample();
        TagAllocatorSample *obj2 = new TagAllocatorSample();
        TagAllocatorSample *obj3 = new TagAllocatorSample();
        TagAllocatorSample *obj4 = new TagAllocatorSample();
        Assert::AreEqual(4, TagAllocatorSample::cnt);

        delete(obj2);
        delete(obj3);
        Assert::AreEqual(2, TagAllocatorSample::cnt);

        //시스템 완료후 강제로 메모리 해제
        TagAllocatorSample::DeleteAll();
        Assert::AreEqual(0, TagAllocatorSample::cnt);

        //두번호출해도 문제 없이 돌아갈것
        TagAllocatorSample::DeleteAll();
        Assert::AreEqual(0, TagAllocatorSample::cnt);
    }

};

}
