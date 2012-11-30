// Ŭnicode please
#pragma once

#include <string>

//http://blog.daum.net/aswip/4538253
//문자열 해쉬함수 모음 (String Hash Function) 
namespace sora {;
struct RSHash {
    static unsigned int Hash(const char *str, int len);
    static unsigned int Hash(const std::string &str) {
        return Hash(str.c_str(), str.length());
    }
};

struct JSHash {
    static unsigned int Hash(const char *str, int len);
    static unsigned int Hash(const std::string &str) {
        return Hash(str.c_str(), str.length());
    }
};

// P. J. Weinberger Hash Function
struct PJWHash {
    static unsigned int Hash(const char *str, int len);
    static unsigned int Hash(const std::string &str) {
        return Hash(str.c_str(), str.length());
    }
};

struct ELFHash {
    static unsigned int Hash(const char *str, int len);
    static unsigned int Hash(const std::string &str) {
        return Hash(str.c_str(), str.length());
    }
};


struct BKDRHash {
    static unsigned int Hash(const char *str, int len);
    static unsigned int Hash(const std::string &str) {
        return Hash(str.c_str(), str.length());
    }
};

struct SDBMHash {
    static unsigned int Hash(const char *str, int len);
    static unsigned int Hash(const std::string &str) {
        return Hash(str.c_str(), str.length());
    }
};

struct DJBHash {
    static unsigned int Hash(const char *str, int len);
    static unsigned int Hash(const std::string &str) {
        return Hash(str.c_str(), str.length());
    }
};

struct DEKHash {
    static unsigned int Hash(const char *str, int len);
    static unsigned int Hash(const std::string &str) {
        return Hash(str.c_str(), str.length());
    }
};

struct APHash {
    static unsigned int Hash(const char *str, int len);
    static unsigned int Hash(const std::string &str) {
        return Hash(str.c_str(), str.length());
    }
};
}