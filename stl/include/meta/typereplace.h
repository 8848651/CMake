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
            auto b = tuplefindelement<N>(_data);
            //必须保证stl::is_nmber_minus<a, 1>::value>=0
            auto c = tuplefindelement<stl::is_nmber_minus<a, 1>::value>(_args);
            constexpr bool flag = stl::is_same<decltype(b), type>::value ? true : false;
            return conditional_selector<flag, decltype(c), decltype(b)>::execute(c, b);
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
