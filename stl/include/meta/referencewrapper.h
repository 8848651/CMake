#pragma once
namespace stl {

    template<typename T>
    class referencewrapper {
    private:
        T* ptr_;  

    public:
        referencewrapper(T& ref): ptr_(&ref) {}

        referencewrapper(T&&) = delete;
        referencewrapper(const referencewrapper&) = default;
        referencewrapper& operator=(const referencewrapper&)   = default;

        /*类型转换重载要加const const 对象不能调用非 const 成员函数
        * int b=10;
        * int& a=referencewrapper<int> rw1{b}; int& a=rw1.operator T& ();
        * const referencewrapper<int> rw2{b}
        * int& a=rw2.operator T& (); 报错因为rb2是const类型
        */
        operator T& () const { return *ptr_; }
        T& get() const { return *ptr_; }

        template<typename... Args>
        auto operator()(Args&&... args) -> decltype((*ptr_)(std::forward<Args>(args)...)) {
            return (*ptr_)(std::forward<Args>(args)...);
        }
    };

    template<typename T>
    referencewrapper<T> ref(T& t)   {
        return referencewrapper<T>(t);
    }

    template<typename T>
    void ref(T&&) = delete;

} 