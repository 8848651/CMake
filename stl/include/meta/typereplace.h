#pragma once
#include <iostream>
#include "tuple.h"
#include "indexqueue.h"
#include "template.h"
#include "typedifference.h"
namespace stl {

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
    struct parameterassisttype<N, type, typequeue<T...>, typequeue<U...>> {
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
        using traits = parameterassisttype<N, Type, typequeue<T...>, typequeue<U...>>;
        typename traits::data_type b = tuplefindelement<N>(data);
        typename traits::arg_type c = tuplefindelement<traits::arg_index>(args);
        return std::forward<typename traits::re_type>(
            conditional_selector<traits::use_arg, typename traits::arg_type, typename traits::data_type>::execute(
                std::forward<typename traits::arg_type>(c), std::forward<typename traits::data_type>(b)
            )
        );
    }


    template<typename type,typename U,typename P,typename Q>
    struct parameterassistedtype;

    template<typename type,size_t... Is,typename... T,typename... U>
    struct parameterassistedtype<type,indexqueue<Is...>,typequeue<T...>, typequeue<U...>>{
        using types = tuple<typename stl::parameterassisttype<Is, type, tuple<T...>, tuple<U...>>::re_type...>;
    };

    template<typename type,size_t... Is,typename... T,typename... U>
    typename parameterassistedtype<type,indexqueue<Is...>,typequeue<T...>, typequeue<U...>>::types 
        parameter_args_tmp(tuple<T...>& _data, tuple<U...>& _args,indexqueue<Is...>){
            using types = typename parameterassistedtype<type,indexqueue<Is...>,typequeue<T...>, typequeue<U...>>::types;
            constexpr size_t _size_args = tuplesize<tuple<T...>>::size;
            constexpr size_t _index_data = tuplesize<tuple<U...>>::size - 1;
            constexpr size_t placeholderssize = stl::tuplefindtype<_index_data, type, tuple<U...>>::get();
            static_assert(_size_args == placeholderssize, "入参和占位符数量不匹配");
            types _args_tmp(std::forward<typename tupleelement<Is, types>::type>(parameter_assist<Is, type>(_data, _args))...);
            return _args_tmp;
    };


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
            return parameter_args_tmp<Type>(_data_1,_data_2,makeindexqueue<tuplesize<tuple<T...>>::size>::queuedata());
        }
    };

    template<typename Type, typename... T>
    class parametertype<Type,typequeue<T...>,typequeue<>> {
    public:
        tuple<T...>& _data_1;
        tuple<>& _data_2;
        template<typename... P,typename... Q>
        parametertype(tuple<P...>& data_1,tuple<Q...>& data_2) 
            : _data_1(std::forward<tuple<P...>&>(data_1)),_data_2(std::forward<tuple<Q...>&>(data_2)) {};
        parametertype(const parametertype& other) : _data_1(other._data_1) , _data_2(other._data_2) {}
        auto recell(){
            return _data_1;
        }
    };




}
