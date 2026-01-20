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

    template<typename T, typename U>
    struct typequeuereferenceassisted;


    template<typename... T, typename... U>
    struct typequeueadd<typequeue<T...>, typequeue<U...>> {
        using type = typequeue<T..., U...>;
    };


    /**
    * @brief typequeueget用来做分支匹配两个类型不同取第一个类型
    */

    template<typename P, typename Q, typename R>
    struct typequeueget;

    template<typename P, typename Q, typename T, typename U>
    struct typequeueget<P, Q, typequeueassisted<T, U>> {
        using type = typename typequeueadd<typequeue<P>, typename typequeueassisted<T, U>::type>::type;
    };

    template<typename P, typename T, typename U>
    struct typequeueget<P, P, typequeueassisted<T, U>> {
        using type = typename typequeueassisted<T, U>::type;
    };

    /**
    * @brief typequeueassisted 序列循环体
    */

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


    /**
    * @brief P Q去掉引用以及P P去掉引用是否相同，相同取P,不同取Q
    */

    template<typename P, typename Q, typename R>
    struct typequeuereferenceget;

    template<typename P, typename Q, typename T, typename U>
    struct typequeuereferenceget<P, Q, typequeuereferenceassisted<T, U>> {
        static constexpr bool Pvalue = stl::is_same<typename stl::remove_reference<P>::type, P>::value;
        static constexpr bool Qvalue = stl::is_same<typename stl::remove_reference<Q>::type, typename stl::remove_reference<P>::type>::value;
        using Rtype = typename stl::conditional_selector<Pvalue&& Qvalue, P, Q>::type;
        using type = typename typequeueadd<typequeue<Rtype>, typename typequeuereferenceassisted<T, U>::type>::type;
    };

    template<typename P, typename T, typename U>
    struct typequeuereferenceget<P, P, typequeuereferenceassisted<T, U>> {
        using type = typename typequeueadd<typequeue<P>, typename typequeuereferenceassisted<T, U>::type>::type;;
    };

    template<>
    struct typequeuereferenceassisted<typequeue<>, typequeue<>> {
        using type = typequeue<>;
        using previoustyep = typequeue<>;
    };

    template<typename P, typename... T, typename Q, typename... U>
    struct typequeuereferenceassisted<typequeue<P, T...>, typequeue<Q, U...>> {
        using previoustyep = typequeuereferenceassisted<typequeue<T...>, typequeue<U... >>;
        using type = typename typequeuereferenceget<P, Q, previoustyep>::type;
    };




}


