// Ŭnicode please
#pragma once

#define SR_PROPERTY(TYPE, NAME, CLASS_NAME, GETTER, SETTER)  \
    template< \
    typename T, \
    typename ConType, \
    T (ConType::*GetFunc)(),  \
    T (ConType::*SetFunc)(const T &) \
    > \
class Property_##CLASS_NAME##_##NAME {  \
public: \
    Property_##CLASS_NAME##_##NAME() {} \
    Property_##CLASS_NAME##_##NAME(const T i) : value(i) {} \
    T &operator=(const T &i) {  \
    unsigned long offset = offsetof(CLASS_NAME, NAME); \
    ConType *parent = (ConType*)((unsigned long)(this) - offset);  \
    value = (parent->*SetFunc)(i);  \
    return value; \
    } \
    operator T() const {  \
    unsigned long offset = offsetof(CLASS_NAME, NAME); \
    ConType *parent = (ConType*)((unsigned long)(this) - offset);  \
    T result = (parent->*GetFunc)(); \
    return result;  \
    } \
private:  \
    friend class CLASS_NAME;  \
    T value;  \
};  \
    Property_##CLASS_NAME##_##NAME <TYPE, CLASS_NAME, &CLASS_NAME::GETTER, &CLASS_NAME::SETTER> NAME


#define SR_PROPERTY_GET(TYPE, NAME, CLASS_NAME, GETTER)  \
    template< \
    typename T, \
    typename ConType, \
    T (ConType::*GetFunc)()  \
    > \
class Property_##CLASS_NAME##_##NAME {  \
public: \
    Property_##CLASS_NAME##_##NAME() {} \
    Property_##CLASS_NAME##_##NAME(const T i) : value(i) {} \
    operator T() const {  \
    unsigned long offset = offsetof(CLASS_NAME, NAME); \
    ConType *parent = (ConType*)((unsigned long)(this) - offset);  \
    T result = (parent->*GetFunc)(); \
    return result;  \
    } \
private:  \
    friend class CLASS_NAME;  \
    T value;  \
};  \
    Property_##CLASS_NAME##_##NAME <TYPE, CLASS_NAME, &CLASS_NAME::GETTER> NAME


#define SR_PROPERTY_SET(TYPE, NAME, CLASS_NAME, SETTER)  \
    template< \
    typename T, \
    typename ConType, \
    T (ConType::*SetFunc)(const T &) \
    > \
class Property_##CLASS_NAME##_##NAME {  \
public: \
    Property_##CLASS_NAME##_##NAME() {} \
    Property_##CLASS_NAME##_##NAME(const T i) : value(i) {} \
    T &operator=(const T &i) {  \
    unsigned long offset = offsetof(CLASS_NAME, NAME); \
    ConType *parent = (ConType*)((unsigned long)(this) - offset);  \
    value = (parent->*SetFunc)(i);  \
    return value; \
    } \
private:  \
    friend class CLASS_NAME;  \
    T value;  \
};  \
    Property_##CLASS_NAME##_##NAME <TYPE, CLASS_NAME, &CLASS_NAME::SETTER> NAME
