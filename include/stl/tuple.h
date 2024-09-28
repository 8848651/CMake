#pragma once
#include <stddef.h>
namespace stl {


    // <1 <2 <3...>>
    template<class T, class... U>
    class TupleBase {
    public:
        T data;
        TupleBase<U...> base;
        TupleBase(T _data, U... _base) : data(_data), base(_base...) {}
    };

    template<class T>
    class TupleBase<T> {
    public:
        T data;
        TupleBase(T _data) : data(_data) {}
    };


    template<int Index>
    class TupleFindElement {
    public:
        template<class... Types>
        static auto find(TupleBase<Types...>& base) {
            return TupleFindElement<Index - 1>::find(base.base);
        };
    };

    template<>
    class TupleFindElement<0> {
    public:
        template<class... Types>
        static auto find(TupleBase<Types...>& base) {
            return base.data;
        };
    };


    template<class... Types>
    class Tuple {
    public:
        TupleBase<Types...> base;
        Tuple() {};
        Tuple(Types... args) : base(args...) {};
    };


};