#pragma once

#include "template_lib.h"
#include "logger.h"

namespace sora {;
typedef short i16;
typedef int i32;

enum {
	kMagicNumber = 0x1234,
	kNoTag = 0
};

// low level alloc, support only simple log
void *BasicMalloc(size_t size);
void BasicFree(void *ptr);
void *BasicCalloc( size_t num, size_t size );
void *BasicRealloc( void *memblock, size_t size );

// add header alloc
void *Malloc(size_t size);
void Free(void *ptr);
void *TagMalloc(size_t size, i32 tag);
void TagFree(i32 tag);

// memory alloc stat
struct AllocState {
	i32 bytes;
	i32 count;
};

struct AllocHeader {
	AllocHeader()
		: prev(NULL),
		next(NULL),
		magic(kMagicNumber),
		tag(0),
		size(0) {
			prev = this;
			next = this;
	}

	AllocHeader *prev;
	AllocHeader *next;
	i16 magic;
	i16 tag;
	i32 size;
};
//전역변수형태로 구현됨.
//너무 c스러운가?
extern AllocHeader g_mem_chain;
extern i32 g_alloc_bytes;  // 할당받은 총 용량
extern i32 g_alloc_count;  // 할당받은 조각의 갯수


void AllocStat(AllocState *data);

}

#define MM_MALLOC(X) sora::BasicMalloc(X)
#define MM_FREE(X)  sora::BasicFree(X)
#define MM_CALLOC(NUM, SIZE)  sora::BasicCalloc(NUM, SIZE)
#define MM_REALLOC(MEMBLOCK, SIZE)  sora::BasicRealloc(MEMBLOCK, SIZE)

#define SR_MALLOC(X) sora::Malloc(X)
#define SR_FREE(X)  sora::Free(X)
#define SR_TAG_MALLOC(SIZE, TAG)  sora::TagMalloc(SIZE, TAG)
#define SR_TAG_FREE(TAG)  sora::TagFree(TAG)


namespace sora {;
//mem의 tag기능을 써서 메모리 new/delete를 처리하는 클래스
//메모리 누수 감지용으로 바꿀때 쓰면 적절하다
//태그는 활성화 범위 자체로 쓰면됨
//메모리 디버깅과 관련된 잡탕 기능을 제공함
template<int Tag, typename TargetType>
class TagAllocator {
public:
	void* operator new(size_t x);
	void operator delete(void *ptr);
	
	//helper method
public:
	//해당 tag로 연결된 모든 메모리를 해제
	static void DeleteAll();
};

template<int Tag, typename TargetType>
void* TagAllocator<Tag, TargetType>::operator new(size_t x)
{
	static_assert(Tag != kNoTag, "Tag must not kNoTag");
	IUASSERT(sizeof(TargetType) == x);
	void *ptr = SR_TAG_MALLOC(x, Tag);
	return ptr;
}

template<int Tag, typename TargetType>
void TagAllocator<Tag, TargetType>::operator delete(void *ptr)
{
	SR_FREE(ptr);
}

//해당 tag로 연결된 모든 메모리를 해제
template<int Tag, typename TargetType>
void TagAllocator<Tag, TargetType>::DeleteAll()
{
	int leak_occur_count = 0;
	//TAG_FREE(Tag);
	//그냥 FREE를 사용해서 flush하면 소멸자가 불리지 않는다. tag free의 코드를 수정해서 쓴다
	AllocHeader *header = g_mem_chain.next;
	AllocHeader *next = NULL;
	for ( ; header != &g_mem_chain ; header = next) {
		next = header->next;
		if (header->tag == Tag) {
			void *alloc_ptr = (void*)((long)header + sizeof(AllocHeader));
			//강제 소멸자 호출
			sora::CallDestructor((TargetType*)alloc_ptr);
			SR_FREE(alloc_ptr);
			leak_occur_count++;
		}
	}

	if(leak_occur_count == 0) {
		TAG_LOGD("Mem", "Tag %d is no leak occur", Tag);
	} else {
		TAG_LOGE("Mem", "may be mem leak occur");
		TAG_LOGD("Mem", "Tag %d's Leak count is %d", Tag, leak_occur_count);
	}
}
}