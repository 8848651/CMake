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

    template<int N>
    struct parameterassistedhelper {
        template<typename type, typename... T, typename... U>
        constexpr static auto _test(tuple<T...>& _data, tuple<U...>& _args) {
            constexpr int a = tuplefindtype<N, type, tuple<T...>>::Get();
            typename tupleelement<N, tuple<T...>>::type b = tuplefindelement<N>(_data);
            constexpr int value = stl::is_nmber_minus<a, 1>::value;
            typename tupleelement<value, tuple<U...>>::type c = tuplefindelement<value>(_args);
            constexpr bool flag = stl::is_same<decltype(b), type>::value ? true : false;
            return conditional_selector<flag, decltype(c), decltype(b)>::execute(c, b);
        };

    };

    template<int N>
    struct parameterassistedhelper {

        template<typename type>
        struct test_0 {

            template<template T, template U>
            struct test_1;

            template<typename... T, typename... U>
            struct test_1<tuple<T...>, tuple<U...>> {
                constexpr int N = tuplefindtype<N, type, tuple<T...>>::Get();
                constexpr int M = stl::is_nmber_minus<N, 1>::value;
                using type = test_2<N, M>;

                template<size_t N, size_t M>
                struct test_2 {
                    using Ttype = typename tupleelement<N, tuple<T...>>::type;
                    using Utype = typename tupleelement<M, tuple<U...>>::type;
                    constexpr bool flag = stl::is_same<Ttype, type>::value ? true : false;
                    using Rtype = conditional_selector<flag, Utype, Ttype>::type;

                    constexpr static Rtype _test(tuple<T...>& _data, tuple<U...>& _args) {
                        Ttype b = tuplefindelement<N>(_data);
                        Utype c = tuplefindelement<M>(_args);
                        return conditional_selector<flag, Utype, Ttype>::execute(c, b);
                    };
                };

            };

        };
    };


    template<typename U>
    struct parameterassisted;

    template<size_t... Is>
    struct parameterassisted<indexqueue<Is...>> {
        template<typename type, typename... M, typename... N>
        static auto run(tuple<M...>& _data, tuple<N...>& _args) {
            constexpr size_t _size_args = tuplesize<tuple<N...>>::size;
            constexpr size_t _index_data = tuplesize<tuple<M...>>::size - 1;
            constexpr size_t placeholderssize = stl::tuplefindtype<_index_data, type, tuple<M...>>::Get();
            static_assert(_size_args == placeholderssize, "入参和占位符数量不匹配");
            tuple<decltype(parameterassistedhelper<Is>::template _test<type>(_data, _args))...> _args_tmp(parameterassistedhelper<Is>::template _test<type>(_data, _args)...);
            return _args_tmp;
        };

    };

    template<typename T, typename... Args>
    struct parametertype;

    template<typename T, typename... Args>
    struct parametertype<T(Args...)> {
        tuple<Args...> _data;
        parametertype(Args&&... args) : _data(std::forward<Args>(args)...) {};

        template<typename... U>
        auto operator()(U&&... args) {
            tuple<U...> _args{ std::forward<U>(args)... };
            return parameterassisted<class makeindexqueue<tuplesize<tuple<Args...>>::size>::queuedata>::template run<T>(_data, _args);
        };

    };



}
