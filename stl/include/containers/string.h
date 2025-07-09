#pragma once
#include <iostream>
#include "tuple.h"
#include "too.h"
namespace stl {



    // TODO: 实现一个简单的string类
    class string {

    private:
        char* data;
        size_t size;

    public:
        string() : data(new char[1] {'\0'}), size(0) {};
        string(const char* str);
        string(const string& other) : data(new char[other.size + 1]), size(other.size) {
            my_memcpy((void*)data, (void*)other.data, size);
        }
        ~string() { delete[] data; };

    public:
        bool operator==(const string& other) const;
        bool operator==(const char*& other) const;

    public:
        size_t length() const { return size; };
        const char* c_str() const { return data; };
        string substr(const char str);
        void print(std::ostream& os) const { os.write(data, size); };
    };


    string::string(const char* str) {
        size_t len = 0;
        while (str[len] != '\0') {
            len++;
        }
        size = len;
        data = new char[len + 1];
        my_memcpy((void*)data, (void*)str, size);
    };

    bool string::operator==(const string& other) const {
        if (size != other.length()) { return false; }
        char* s1 = data;
        char* s2 = other.data;
        while (*s1 && *s2) {
            if (*s1 != *s2) {
                return false;
            }
            s1++;
            s2++;
        }
        return !(*s1 || *s2);
    }

    bool string::operator==(const char*& other) const {
        if (size != str_len(other)) { return false; }
        return str_same(data, other);
    }


    //字符串切割 123,456 -> 123 456 
    string string::substr(const char str) {
        size_t i = 0;
        char* _data = data;
        for (; i < size + 1; i++) {
            _data++;
            if (data[i] == str) { break; }
        }
        if (i == size + 1) { return string(); }
        if (i == 0) {
            string temp(++data);
            data = new char[1] {'\0'};
            size = 0;
            return temp;
        }
        size_t len = size - i + 1;
        char* _new_data = new char[len];
        my_memcpy((void*)_new_data, (void*)_data, size);
        size = i;
        *(data + i) = '\0';
        return string(_new_data);

    }


    std::ostream& operator<<(std::ostream& os, const string& s) {
        //获取私有成员变量
        s.print(os);
        return os;
    }
}