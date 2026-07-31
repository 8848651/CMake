#pragma once
#include <cstddef>
#include "indexqueue.h"

namespace stl {

    // 版本一  注意T是二进制的长度位不是数组的长度位

    template<size_t T, size_t U>
    struct binaryreversal {
        constexpr static size_t data = ((T & 1) << U) + binaryreversal<T/2, U-1>::data;
    };

    template<size_t T>
    struct binaryreversal<T, 0> {
        constexpr static size_t data = T & 1;
    };


    // 编译期计算 ceil(log2(N))，即表示 N 需要的 bit 位数
    template<size_t N>
    struct bit_width {
        constexpr static size_t value = bit_width<N / 2>::value + 1;
    };

    template<>
    struct bit_width<0> { constexpr static size_t value = 0; };

    template<>
    struct bit_width<1> { constexpr static size_t value = 1; };



    // 版本二
    template<size_t T>
    struct valuetobits {
        constexpr static size_t size = T & 1;
        using queue = typename stl::indexqueueconcat<stl::indexqueue<size>, typename valuetobits<T/2>::queue>::type;
    };

    template<>
    struct valuetobits<1> {
        using queue = stl::indexqueue<1>;
    };

    template<>
    struct valuetobits<0> {
        using queue = stl::indexqueue<0>;
    };

    template<typename T, typename U>
    struct padbits {};

    template<size_t... T, size_t M, size_t... U, size_t N>
    struct padbits<stl::indexqueue<M, T...>, stl::indexqueue<N, U...>> {
        using queue = typename stl::indexqueueconcat<
            stl::indexqueue<M>,
            typename padbits<stl::indexqueue<T...>, stl::indexqueue<U...>>::queue
        >::type;
    };

    template<size_t... U, size_t N>
    struct padbits<stl::indexqueue<>, stl::indexqueue<N, U...>> {
        using queue = typename stl::indexqueueconcat<
            stl::indexqueue<0>,
            typename padbits<stl::indexqueue<>, stl::indexqueue<U...>>::queue
        >::type;
    };

    template<>
    struct padbits<stl::indexqueue<>, stl::indexqueue<>> {
        using queue = stl::indexqueue<>;
    };

    template<typename T, typename U>
    struct bitstovalue {};

    template<size_t... T, size_t M, size_t... U, size_t N>
    struct bitstovalue<stl::indexqueue<M, T...>, stl::indexqueue<N, U...>> {
        constexpr static size_t data = (M << N) + bitstovalue<stl::indexqueue<T...>, stl::indexqueue<U...>>::data;
    };

    template<>
    struct bitstovalue<stl::indexqueue<>, stl::indexqueue<>> {
        constexpr static size_t data = 0;
    };

    //T是二进制的长度，U是要进行反转的下标
    template<size_t T, size_t U>
    struct bitreverse {
    private:
        using queue_1 = typename stl::makereverseindexqueue<T>::queuedata;
        using queue_2 = typename valuetobits<U>::queue;
        using queue_3 = typename padbits<queue_2, queue_1>::queue;
    public:
        constexpr static size_t data = bitstovalue<queue_3, queue_1>::data;
    };



    template<size_t T, typename U>
    struct bitrev_table {};

    template<size_t T, size_t... U>
    struct bitrev_table<T, stl::indexqueue<U...>> {
        constexpr static auto table = {stl::binaryreversal<U, (stl::bit_width<T>::value - 1) - 1>::data...};
        //constexpr static auto table = {stl::bitreverse<stl::bit_width<T>::value-1,U>::data...};
    };

}
