#pragma once
#include <iostream>
#include "tuple.h"
#include "indexqueue.h"
#include "template.h"
#include "typedifference.h"
namespace stl {

    /**
    * @brief parametertype 用来实现函数参数类型替换 原始版本只能进行值绑定
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
    template<size_t N, typename type, typename... T, typename... U>
    struct parameterassisttype<N, type, tuple<T...>, tuple<U...>> {
        static constexpr size_t data_index = tuplefindtype<N, type, tuple<T...>>::get();
        static constexpr size_t arg_index = stl::is_nmber_minus<data_index, 1>::value;
        using data_type = typename tupleelement<N, tuple<T...>>::type;
        using arg_type = typename tupleelement<arg_index, tuple<U...>>::type;
        static constexpr bool use_arg = stl::is_same<data_type, type>::value ? true : false;
        using re_type = typename conditional_selector<use_arg, arg_type, data_type>::type;
    };


    /**
    * @brief  (值类型)获取要替换序列b发生一次拷贝 execute函数入参一次拷贝 返回值一次拷贝
    */

    template<size_t N, typename Type, typename... T, typename... U>
    typename parameterassisttype<N, Type, tuple<T...>, tuple<U...>>::re_type parameter_assist(stl::tuple<T...>& data, stl::tuple<U...>& args) {
        using traits = parameterassisttype<N, Type, tuple<T...>, tuple<U...>>;
        typename traits::data_type b = tuplefindelement<N>(data);
        typename traits::arg_type c = tuplefindelement<traits::arg_index>(args);
        return std::forward<typename traits::re_type>(
            conditional_selector<traits::use_arg, typename traits::arg_type, typename traits::data_type>::execute(
                std::forward<typename traits::arg_type>(c), std::forward<typename traits::data_type>(b)
            )
        );
    }


    template<typename U>
    struct parameterassisted;

    /**
    * @brief _args_tmp 是一个tuple，值类型会发生一次值拷贝
    */

    template<size_t... Is>
    struct parameterassisted<indexqueue<Is...>> {
        template<typename type, typename... M, typename... N>
        static auto run(tuple<M...>& _data, tuple<N...>& _args) {
            constexpr size_t _size_args = tuplesize<tuple<N...>>::size;
            constexpr size_t _index_data = tuplesize<tuple<M...>>::size - 1;
            constexpr size_t placeholderssize = stl::tuplefindtype<_index_data, type, tuple<M...>>::get();
            static_assert(_size_args == placeholderssize, "入参和占位符数量不匹配");
            using types = tuple<typename stl::parameterassisttype<Is, type, tuple<M...>, tuple<N...>>::re_type...>;
            types _args_tmp(std::forward<typename tupleelement<Is, types>::type>(parameter_assist<Is, type>(_data, _args))...);
            return _args_tmp;
        };

    };

    template<typename T,typename U>
    class parametertypeassisteimpl;





    template<typename T, typename U,typename Q>
    class parametertype;

    template<typename Type, typename... T,typename... U>
    class parametertype<Type,typequeue<T...>,typequeue<U...>> {
    public:
        tuple<T...>& _data_1;
        tuple<U...>& _data_2;
        template<typename... P,typename... Q>
        parametertype(tuple<P...>& data_1,tuple<Q...>& data_2) 
            : _data_1(std::forward<tuple<P...>&>(data_1)),_data_2(std::forward<tuple<Q...>&>(data_2)) {};
        parametertype(const parametertype& other) : _data_1(other._data_1) , _data_2(other._data_2) {}
        auto recell(){
            return parameterassisted<class makeindexqueue<tuplesize<tuple<Args...>>::size>::queuedata>::template run<T>(_data_1, _data_2);
        }
    };

    template<typename Type, typename... T>
    class parametertype<Type,typequeue<T...>,typequeue<>> {
    public:
        tuple<T...>& _data_1;
        tuple<U...>& _data_2;
        template<typename... P,typename... Q>
        parametertype(tuple<P...>& data_1,tuple<Q...>& data_2) 
            : _data_1(std::forward<tuple<P...>&>(data_1)),_data_2(std::forward<tuple<Q...>&>(data_2)) {};
        parametertype(const parametertype& other) : _data_1(other._data_1) , _data_2(other._data_2) {}
        auto recell(){
            return _data_1;
        }
    };

    /**
    * @brief 返回值是一个tuple，值类型会发生一次值拷贝 如果值类型发生拷贝parametertype返回有4次拷贝
    */

    template<typename T, typename... Args>
    class parametertype_1;

    template<typename T, typename... Args>
    class parametertype_1<T(Args...)> {
        tuple<Args...> _data;
        template<typename... UArgs>
        parametertype_1(UArgs&&... args) : _data(std::forward<UArgs>(args)...) {};
        parametertype_1(const parametertype& other) : _data(other._data) {}

        template<typename... U>
        auto operator()(U&&... args) {
            tuple<U&&...> _args(std::forward<U>(args)...);
            return parameterassisted<class makeindexqueue<tuplesize<tuple<Args...>>::size>::queuedata>::template run<T>(_data, _args);
        };

        auto operator()() {
            return _data;
        };

    };



}
