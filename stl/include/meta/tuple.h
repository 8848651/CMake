#pragma once
#include <cstddef>
#include <utility>
#include "template.h"
namespace stl {

    /**
    * @brief tuple 用来实现编译期的元组类型
    * @return 返回一个可以存储多种类型的元组类型
    */
    //这个tupledata必须要有，不然get类型无法转换

    template<size_t N, typename T, typename R = void>
    class tupledata {
    private:
        T data;
    public:
        tupledata() = delete;
        tupledata(T&& _data) : data(std::forward<T>(_data)) {};
        tupledata(const tupledata<N, T>& _base) : data(_base.get()) {};
        T get() const { return std::forward<T>(data); };
    };

    template<size_t N, typename T>
    class tupledata<N, T, typename stl::void_type<stl::is_same<T, typename stl::remove_reference<T>::type>::value>::type> {
    public:
        tupledata() = delete;
        tupledata(T&& _data) : data(std::forward<T>(_data)) {};
        tupledata(const tupledata<N, T>& _base) : data(_base.get()) {};
        tupledata(tupledata<N, T>&& _base) : data(std::move(_base.get())) {};
        T get() const { return data; }
    private:
        T data;
    };

    //-------------------------------------------------------------------------

    template<size_t N, typename... T>
    class tuplebase;

    template<size_t N>
    class tuplebase<N> {};

    // <1 <2 <3...>>
    template<size_t N, typename T, typename... U>
    class tuplebase<N, T, U...> : public tupledata<N, T>, public tuplebase<N + 1, U...> {
    public:
        using data = tupledata<N, T>;
        using base = tuplebase<N + 1, U...>;
    public:
        tuplebase(T&& _data, U&&... _base) : data(std::forward<T>(_data)), base(std::forward<U>(_base)...) {};
        tuplebase(const tuplebase<N, T, U...>& _base) :data(_base), base(_base) {};
        tuplebase(tuplebase<N, T, U...>&& _base) :data(std::move(_base)), base(std::move(_base)) {};
    };

    //根据容器的法则中，容器不允许存引用类型如tuple<size_t&>
    template<typename... T>
    class tuple : public tuplebase<0, T...> {
    public:
        using base = tuplebase<0, T...>;
        tuple(T&&... args) : base(std::forward<T>(args)...) {}
        tuple(const tuple<T...>& _base) : base(_base) {};
        tuple(tuple<T...>&& _base) : base(std::move(_base)) {};

    };

    //-------------------------------------------------------------------------

    template<size_t N, typename T>
    struct tupleelement;

    template<size_t N, typename T, typename... U>
    struct tupleelement<N, tuple<T, U...>> : public tupleelement<N - 1, tuple<U...>> {};

    template<typename T, typename... U>
    struct tupleelement<0, tuple<T, U...>> {
        using type = T;
    };

    //-------------------------------------------------------------------------

    template<typename T>
    struct tuplesize;

    template<typename... T>
    struct tuplesize<tuple<T...>> {
        static constexpr size_t size = sizeof...(T);
    };

    //-------------------------------------------------------------------------

    template<size_t N, typename... U>
    typename tupleelement<N, tuple<U...>>::type tuplefindelement(tuple<U...>& base) {
        return static_cast<tupledata<N, typename tupleelement<N, tuple<U...>>::type>>(base).get();
    };

    //-------------------------------------------------------------------------

    template<size_t N, typename... T>
    struct tuplefindtype;

    template<typename T, typename... U>
    struct tuplefindtype<0, T, tuple<U...>> {
        static constexpr size_t value = static_cast<bool>(stl::is_same<T, typename tupleelement<0, tuple<U...>>::type>::value) ? 1 : 0;
        static constexpr size_t get() {
            return value;
        }
    };

    template<size_t N, typename T, typename... U>
    struct tuplefindtype<N, T, tuple<U...>> {
        static constexpr size_t value = static_cast<bool>(stl::is_same<T, typename tupleelement<N, tuple<U...>>::type>::value) ? 1 : 0;
        static constexpr size_t get() {
            return tuplefindtype<N - 1, T, tuple<U...>>::get() + value;
        }
    };

    //-------------------------------------------------------------------------

};

