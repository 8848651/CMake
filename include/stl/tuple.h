#pragma once
#include <stddef.h>
namespace stl {


    // <1 <2 <3...>>


    // template<size_t Index, class T, class... Types>
    // struct TupleAccessor {
    //     static auto& get(TupleBase<T, Types...>& tuple) {
    //         return TupleAccessor<Index - 1, Types...>::get(tuple.tail);
    //     }
    // };

    // template<class T, class... Types>
    // struct TupleAccessor<0, T, Types...> {
    //     static T& get(TupleBase<T, Types...>& tuple) {
    //         return tuple.head;
    //     }
    // };


    // <1 <2 <3...>>
    template<class T, class... U>
    class TupleBase {
    public:
        T data;
        TupleBase<U...> next;
        TupleBase(T data, TupleBase<U...> next) : data(data), next(next) {}
    };

    template<class T>
    class TupleBase<T> {
    public:
        T data;
        TupleBase(T data) : data(data) {}
    };


    template<class... Types>
    class Tuple {
    public:
        TupleBase<Types...> base;
        Tuple() {}
        Tuple(Types... args) : base(args...) {}
    };


}