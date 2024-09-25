#pragma once
#include <stddef.h>
namespace stl {


    // <1 <2 <3...>>
    template<class T, class... Types>
    class TupleBase {
    public:
        T head;
        TupleBase<Types...> tail;
        TupleBase(T head, TupleBase<Types...> tail) : head(std::move(head)), tail(std::move(tail)) {};
    };

    template<class T>
    struct TupleBase<T> {
        T head;
        TupleBase(T head) : head(std::move(head)) {}
    };

    template<size_t Index, class T, class... Types>
    struct TupleAccessor {
        static auto& get(TupleBase<T, Types...>& tuple) {
            return TupleAccessor<Index - 1, Types...>::get(tuple.tail);
        }
    };

    template<class T, class... Types>
    struct TupleAccessor<0, T, Types...> {
        static T& get(TupleBase<T, Types...>& tuple) {
            return tuple.head;
        }
    };

    template<class... Types>
    class tuple {
    public:
        TupleBase<Types...> base;
        tuple() {}
        tuple(Types... args) : base(args...) {}

        auto& operator[](size_t index) {
            return TupleAccessor<index, Types...>::get(base);
        }
    };


}