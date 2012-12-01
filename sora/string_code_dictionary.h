// Ŭnicode please
#ifndef BASE_SORA_STRING_CODE_DICTIONARY_H_
#define BASE_SORA_STRING_CODE_DICTIONARY_H_

#include <vector>
#include <string>
#include <algorithm>

namespace sora {;

template<typename T>  class StringCodeDictionary;
typedef StringCodeDictionary<int> StringIntCodeDictionary;

template<typename T>
class StringCodeDictionary {
public:
    typedef T CodeType;
    typedef std::string NameType;
public:
    StringCodeDictionary() {}
    ~StringCodeDictionary() {}
    T Register(const std::string &name) {
        using std::vector;
        using std::string;
        using std::find;
        using std::distance;
        vector<string> &name_list = str_list_;
        vector<string>::iterator found = find(name_list.begin(),
            name_list.end(), name);
        if (found == name_list.end()) {
            str_list_.push_back(name);
            return str_list_.size() - 1;
        } else {
            return distance(name_list.begin(), found);
        }
    }
    const std::string &CodeToName(T code) {
        using std::vector;
        using std::string;
        static std::string empty("");
        vector<string> &name_list = str_list_;
        if (code < 0 || code >= (int)name_list.size()) {
            return empty;
        } else {
            return name_list[code];
        }
    }
    T NameToCode(const std::string &name) {
        using std::vector;
        using std::string;
        using std::find;
        using std::distance;
        vector<string> &name_list = str_list_;
        vector<string>::iterator found = find(name_list.begin(),
            name_list.end(), name);
        if (found == name_list.end()) {
            return -1;
        } else {
            return distance(name_list.begin(), found);
        }
    }
private:
    std::vector<std::string> str_list_;
};
}

#endif  // BASE_SORA_STRING_CODE_DICTIONARY_H_
