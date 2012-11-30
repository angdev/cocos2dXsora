// Ŭnicode please 
#pragma once

#ifndef NULL
#define NULL ((void*)0)
#endif

namespace sora {;
template<typename T>
class SharedObject {
public:
	static T &GetInstance() {
		if (ctx_ == NULL) {
			ctx_ = new T;
		}
		return *ctx_;
	}
	static void DestoryInstance() {
		if (ctx_ != NULL) {
			delete(ctx_);
			ctx_ = NULL;
		}
	}
	static bool IsCreated() { return (ctx_ != NULL); }

	SharedObject() {}
	~SharedObject() {}

private:
	static T *ctx_;
};

template<typename T>
T* SharedObject<T>::ctx_ = NULL;
}
