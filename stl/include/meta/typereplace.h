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

    /**
     * @brief 类型模板主要用于替换类型
     * @param N 被替换序列的索引
     * @param type 要被替换的类型
     * @param datatype 要被替换的序列
     * @param argutype 替换序列
     */
    template<size_t N, typename type, typename datatype, typename argutype>
    struct parameterassisttype;


    /**
     * @param data_index 寻找datatype序列的前N个中有几个type类型，减一为argutype要替换索引
     * @param arg_index argutype中的索引，去替换datatype序列
     * @param data_type datatype中要被替换的类型，与type比较是否相同
     * @param arg_type argutype中要去替换的类型
     * @param type 最终的类型
     */
    template<int N, typename type, typename... T, typename... U>
    struct parameterassisttype<N, type, tuple<T...>, tuple<U...>> {
        static constexpr int data_index = tuplefindtype<N, type, tuple<T...>>::Get();
        static constexpr int arg_index = stl::is_nmber_minus<data_index, 1>::value;
        using data_type = typename tupleelement<data_index, tuple<T...>>::type;
        using arg_type = typename tupleelement<arg_index, tuple<U...>>::type;
        static constexpr bool use_arg = stl::is_same<data_type, type>::value ? true : false;
        using re_type = typename conditional_selector<use_arg, arg_type, data_type>::type;
    };

    template<int N, typename Type, typename... T, typename... U>
    typename parameterassisttype<N, Type, tuple<T...>, tuple<U...>>::re_type parameter_assist(stl::tuple<T...>& data, stl::tuple<U...>& args) {
        using traits = parameterassisttype<N, Type, tuple<T...>, tuple<U...>>;
        typename traits::data_type b = tuplefindelement<traits::data_index>(data);
        typename traits::arg_type c = tuplefindelement<traits::arg_index>(args);
        return conditional_selector<traits::use_arg, typename traits::arg_type, typename traits::data_type>::execute(c, b);
    }





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
