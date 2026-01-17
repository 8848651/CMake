#pragma once
#include <iostream>
#include "tuple.h"
#include "indexqueue.h"
#include "template.h"
namespace stl {

    /**
    * @brief parametertype 用来实现函数参数类型替换
    * @return 返回一个可以进行参数类型替换的对象
    */

    template<typename... T>
    struct typequeue;

    template<typename T, typename U>
    struct typequeueadd;

    template<typename T, typename U>
    struct typequeueassisted;


    template<typename... T, typename... U>
    struct typequeueadd<typequeue<T...>, typequeue<U...>> {
        using type = typequeue<T..., U...>;
    };

    template<typename P, typename Q, typename R>
    struct typequeueget;

    template<typename P, typename Q, typename T, typename U>
    struct typequeueget<P, Q, typequeueassisted<T, U>> {
        using type = typename typequeueadd<typequeue<Q>, typename typequeueassisted<T, U>::type>::type;
    };

    template<typename P, typename T, typename U>
    struct typequeueget<P, P, typequeueassisted<T, U>> {
        using type = typename typequeueassisted<T, U>::type;
    };

    template<>
    struct typequeueassisted<typequeue<>, typequeue<>> {
        using type = typequeue<>;
        using previoustyep = typequeue<>;
    };

    template<typename P, typename... T, typename Q, typename... U>
    struct typequeueassisted<typequeue<P, T...>, typequeue<Q, U...>> {
        using previoustyep = typequeueassisted<typequeue<T...>, typequeue<U... >>;
        using type = typename typequeueget<P, Q, previoustyep>::type;
    };




}


