#pragma once
#include <cstddef>
#include <cstdint>
namespace stl {

    //按一次8个字节进行复制
    void* my_memcpy(void* dest, const void* src, size_t count) {
        uint8_t* d = static_cast<uint8_t*>(dest);
        const uint8_t* s = static_cast<const uint8_t*>(src);

        //对齐地址，让地址为8的倍数
        // 64 位系统 一次可以取8个字节 64bit 如果分在0-7内一次即可取完 如果分布在 3-11 取两次 0-7 8-15
        while (count && (reinterpret_cast<uintptr_t>(d) % 8)) {
            *d++ = *s++;
            --count;
        }

        // 处理主要的对齐部分，每次复制8个字节
        uint64_t* d64 = reinterpret_cast<uint64_t*>(d);
        const uint64_t* s64 = reinterpret_cast<const uint64_t*>(s);
        while (count >= 8) {
            *d64++ = *s64++;
            count -= 8;
        }

        // 处理剩余的未对齐部分
        d = reinterpret_cast<uint8_t*>(d64);
        s = reinterpret_cast<const uint8_t*>(s64);
        while (count--) {
            *d++ = *s++;
        }

        return dest;
    }

}