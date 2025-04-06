#pragma once
#include <stddef.h>
#include "template.h"
namespace stl {


    // <1 <2 <3...>>
    template<class T, class... U>
    class TupleBase {
    public:
        typedef T type;
    public:
        T data;
        TupleBase<U...> base;
        TupleBase() {}
        TupleBase(T _data, U... _base) : data(_data), base(_base...) {}
        TupleBase(const TupleBase<U...>& _base) : data(_base.data), base(_base.base) {}
    };

    template<class T>
    class TupleBase<T> {
    public:
        typedef T type;
    public:
        T data;
        TupleBase() {}
        TupleBase(T _data) : data(_data) {}
        TupleBase(const TupleBase<T>& _base) : data(_base.data) {}
    };

    //-------------------------------------------------------------------------


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

    //-------------------------------------------------------------------------

    template<int Index, class T>
    class TupleFindType {
    public:
        template<class... Types>
        constexpr static int find(TupleBase<Types...>& base) {
            constexpr int value = TupleFindType<Index - 1, T>::find(base.base);
            return is_nmber_add<value, is_same<T, decltype(base.data)>::value ? 1 : 0>::value;
        };
    };

    template<class T>
    class TupleFindType<0, T> {
    public:
        template<class... Types>
        constexpr static int find(TupleBase<Types...>& base) {
            return is_nmber_add<is_same<T, decltype(base.data)>::value ? 1 : 0, 0>::value;
        };
    };

    //-------------------------------------------------------------------------


    template<class... Types>
    class Tuple {
    public:
        TupleBase<Types...> base;
        Tuple() {};
        Tuple(Types... args) : base(args...) {};
        Tuple(const Tuple<Types...>& _base) : base(_base.base) {};
    };


};