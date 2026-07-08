#pragma once
#include <cstddef>



namespace stl {

    /**
      * @brief 用来编译期创建序列类型 0,2,3,4...
      * @param N 给定的整数
      * @return 返回一个从0-N的序列 IndexQueue<0,1,2...,N>
    */


    template<size_t... I>
    struct indexqueue{};

    template<typename A, typename B>
    struct indexqueueconcat;

    template<size_t... A, size_t... B>
    struct indexqueueconcat<indexqueue<A...>, indexqueue<B...>> {
        using type = indexqueue<A..., B...>;
    };

    template<size_t N>
    struct makeindexqueue {
        using queuedata = typename indexqueueconcat<typename makeindexqueue<N - 1>::queuedata, indexqueue<N - 1>>::type;
    };

    // 递归终止
    template<>
    struct makeindexqueue<0> {
        using  queuedata = indexqueue<>;
    };

    //reverse order
    template<size_t N>
    struct makereverseindexqueue {
        using queuedata = typename indexqueueconcat<indexqueue<N - 1>,typename makereverseindexqueue<N - 1>::queuedata>::type;
    };

    // 递归终止
    template<>
    struct makereverseindexqueue<0> {
        using  queuedata = indexqueue<>;
    };

}


