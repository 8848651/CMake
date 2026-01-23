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

    //类型序列
    template<typename... T>
    struct typequeue;

    //两个类型序列相加
    template<typename T, typename U>
    struct typequeueadd;

    //取A序列与 AB交集 的差集
    template<typename T, typename U>
    struct typequeueassisted;

    //取AB类型相同的引用，AB去掉引用的值类型，以及AB不同B的类型
    template<typename T, typename U>
    struct typequeuereferenceassisted;

    //获取序列第N个类型
    template<size_t N, typename T>
    struct typeelement;

    //获取序列的长度
    template<typename T>
    struct typesize;

    //获取序列中类型T的个数
    template<typename T,typename U>
    struct typequantity;

    //序列中是否有某个值whether
    template<typename T, typename U>
    struct typewhether;


    template<typename T, typename... U>
    struct typewhether<T, typequeue<U...>> {
        constexpr static bool value = typequantity<T, typequeue<U...>>::value ? true : false;
    };


    template<typename T>
    struct typequantity<T, typequeue<>> {
        constexpr static size_t value = 0;
    };
        
    template<typename T, typename U, typename... R>
    struct typequantity<T, typequeue<U, R...>> {
        constexpr static size_t Tvalue = static_cast<bool>(stl::is_same<T, U>::value) ? 1 : 0;
        constexpr static size_t value = typequeue<R...>::value + Tvalue;
    };



    template<size_t N, typename T, typename... U>
    struct typeelement<N, typequeue<T, U...>> : public typeelement<N - 1, tuple<U...>> {};

    template<typename T, typename... U>
    struct typeelement<0, typequeue<T, U...>> {
        using type = T;
    };



    template<typename... T>
    struct typesize<typequeue<T...>> {
        static constexpr size_t size = sizeof...(T);
    };



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


