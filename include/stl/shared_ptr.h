#pragma once


template <typename T>
class SharedPtr {
private:
    T* ptr;
    int* ref_count;

public:
    explicit SharedPtr(T* p = nullptr) : ptr(p), ref_count(new int(1)) {}

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), ref_count(other.ref_count) {
        if (ptr) {
            ++(*ref_count);
        }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ptr) {
                ++(*ref_count);
            }
        }
        return *this;
    }

    ~SharedPtr() {
        release();
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
    int use_count() const { return *ref_count; }

private:
    void release() {
        if (ptr && --(*ref_count) == 0) {
            delete ptr;
            delete ref_count;
        }
    }
};