#pragma once
#include <cstddef>
#include "indexqueue.h"

namespace stl {

    /** 版本一
      * @brief 
      * @param 
      * @return 
    */

    template<size_t T, size_t U>
    struct binaryreversal {
        constexpr static size_t tempdata = ((T & 1) << U) + binaryreversal<T/2, U-1>::tempdata;
    };

    template<size_t T>
    struct binaryreversal<T, 0> {
        constexpr static size_t tempdata = T & 1;
    };


    // 版本二
    template<size_t T>
    struct ValueToBits {
        constexpr static size_t size = T & 1;
        using queue = typename stl::indexqueueconcat<stl::indexqueue<size>, typename ValueToBits<T/2>::queue>::type;
    };

    template<>
    struct ValueToBits<1> {
        using queue = stl::indexqueue<1>;
    };

    template<>
    struct ValueToBits<0> {
        using queue = stl::indexqueue<0>;
    };

    template<typename T, typename U>
    struct PadBits {};

    template<size_t... T, size_t M, size_t... U, size_t N>
    struct PadBits<stl::indexqueue<M, T...>, stl::indexqueue<N, U...>> {
        using queue = typename stl::indexqueueconcat<
            stl::indexqueue<M>,
            typename PadBits<stl::indexqueue<T...>, stl::indexqueue<U...>>::queue
        >::type;
    };

    template<size_t... U, size_t N>
    struct PadBits<stl::indexqueue<>, stl::indexqueue<N, U...>> {
        using queue = typename stl::indexqueueconcat<
            stl::indexqueue<0>,
            typename PadBits<stl::indexqueue<>, stl::indexqueue<U...>>::queue
        >::type;
    };

    template<>
    struct PadBits<stl::indexqueue<>, stl::indexqueue<>> {
        using queue = stl::indexqueue<>;
    };

    template<typename T, typename U>
    struct BitsToValue {};

    template<size_t... T, size_t M, size_t... U, size_t N>
    struct BitsToValue<stl::indexqueue<M, T...>, stl::indexqueue<N, U...>> {
        constexpr static size_t data = (M << N) + BitsToValue<stl::indexqueue<T...>, stl::indexqueue<U...>>::data;
    };

    template<>
    struct BitsToValue<stl::indexqueue<>, stl::indexqueue<>> {
        constexpr static size_t data = 0;
    };

    template<size_t T, size_t U>
    struct BitReverse {
    private:
        using queue_1 = typename stl::makereverseindexqueue<T>::queuedata;
        using queue_2 = typename ValueToBits<U>::queue;
        using queue_3 = typename PadBits<queue_2, queue_1>::queue;
    public:
        constexpr static size_t data = BitsToValue<queue_3, queue_1>::data;
    };

}
