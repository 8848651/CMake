#pragma once
#include <iostream>
#include "tuple.h"
#include "indexqueue.h"
#include "template.h"
#include "typedifference.h"
namespace stl {

    template<size_t N, typename... T>
    struct argumentassisttype;

    template<size_t N, typename... R, typename... T, typename... U>
    struct argumentassisttype<N, typequeue<R...>, typequeue<T...>, typequeue<U...>> {
        static constexpr size_t data_index = stl::typetwodifference<N, typequeue<R...>, typequeue<T...>>::value;
        static constexpr size_t arg_index = stl::is_nmber_minus<data_index, 1>::value;
        using data_type = typeelement<N, typequeue<T...>>::type;
        using arg_type = typeelement<N, typequeue<U...>>::type;
        using re_type = typename typeelement<N, typequeue<R...>>::type;
        static constexpr bool use_arg = static_cast<bool>(stl::is_same<data_type, re_type>::value);
    };


    template<size_t N, typename... R, typename... T, typename... U>
    typename argumentassisttype<N, typequeue<R...>, typequeue<T...>, typequeue<U...>>::re_type
        argument_assist(stl::tuple<T...>& data, stl::tuple<U...>& args) {
        using traits = argumentassisttype<N, typequeue<R...>, typequeue<T...>, typequeue<U...>>;
        typename traits::data_type b = tuplefindelement<N>(data);
        typename traits::arg_type c = tuplefindelement<traits::arg_index>(args);
        return static_cast<typename traits::re_type>(
            conditional_selector<traits::use_arg, typename traits::data_type, typename traits::arg_type>::execute(b, c)
            );
    }


    template<typename... R, size_t... Is, typename... T, typename... U>
    tuple<R...> argument_args_tmp(tuple<T...>& _data, tuple<U...>& _args, indexqueue<Is...>) {
        tuple<R...> _args_tmp(static_cast<typename tupleelement<Is, tuple<R...>>::type>(argument_assist<Is, type>(_data, _args))...);
        return _args_tmp;
    };


    template<typename... T>
    class argumenttype;

    template<typename... T, typename... U, typename... R>
    class argumenttype<typequeue<T...>, typequeue<U...>, typequeue<R...>> {
    public:
        tuple<T...>& _data_1;
        tuple<U...>& _data_2;
        template<typename... P, typename... Q>
        argumenttype(tuple<P...>& data_1, tuple<Q...>& data_2) : _data_1(data_1), _data_2(data_2) {};
        argumenttype(const argumenttype& other) : _data_1(other._data_1), _data_2(other._data_2) {}
        auto recell() {
            using type = typename makeindexqueue<typesize<typequeue<T...>>::size>::queuedata;
            return argument_args_tmp<typequeue<R...>>(_data_1, _data_2, type());
        }
    };

    template<typename Type, typename... T, typename... R>
    class argumenttype<Type, typequeue<T...>, typequeue<>, typequeue<R...>> {
    public:
        tuple<T...>& _data_1;
        tuple<>& _data_2;
        template<typename... P, typename... Q>
        argumenttype(tuple<P...>& data_1, tuple<Q...>& data_2) : _data_1(data_1), _data_2(data_2) {};
        argumenttype(const argumenttype& other) : _data_1(other._data_1), _data_2(other._data_2) {}
        auto recell() {
            return _data_1;
        }
    };




}
