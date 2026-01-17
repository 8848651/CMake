#pragma once
namespace stl {

    template<typename T>
    class referencewrapper;

    template<typename T>
    class referencewrapper<T&> {
    private:
        T* ptr_;  

    public:
        referencewrapper(T& ref): ptr_(&ref) {}

        referencewrapper(T&&) = delete;
        referencewrapper(const referencewrapper&) = default;
        referencewrapper& operator=(const referencewrapper&)   = default;

        //类型转换重载要加const const 对象不能调用非 const 成员函数
        operator T& () const { return *ptr_; }
        T& get() const { return *ptr_; }

        template<typename... Args>
        auto operator()(Args&&... args) const -> decltype((*ptr_)(std::forward<Args>(args)...)) {
            return (*ptr_)(std::forward<Args>(args)...);
        }
    };

} 