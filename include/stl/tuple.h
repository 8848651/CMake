#pragma once
#include <stddef.h>
namespace stl {


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