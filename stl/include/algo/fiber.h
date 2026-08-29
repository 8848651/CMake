#pragma once
#include <functional>
#include <vector>
#include <ucontext.h>   
#include <memory>
namespace stl {
    class fiber {
public:
    // 构造函数：传入要执行的函数
    template<typename Func>
    explicit fiber(Func&& f) : fn_(std::forward<Func>(f)), stack_(65536) {
        //获取当前上下文
        getcontext(&ctx_);
        //初始化新上下文到栈
        ctx_.uc_stack.ss_sp = stack_.data(); 
        ctx_.uc_stack.ss_size = stack_.size(); 
        ctx_.uc_link = nullptr;  

        //设置新栈入口函数
        makecontext(&ctx_, (void(*)()) &fiber::trampoline, 1, this);
        done_ = false;
    }

    void resume() {
        if (done_) return;
        //保存并切换，保存当前上下文到caller_并切换ctx_
        swapcontext(&caller_, &ctx_);
    }

    void yield() {
        swapcontext(&ctx_, &caller_);
    }

private:
    //ucontext_t保存上下文结构体，有各种寄存器
    ucontext_t ctx_;    
    ucontext_t caller_; 
    //模拟纤程的栈
    std::vector<char> stack_; 
    std::function<void()> fn_;
    bool done_ = false;

    static void trampoline(fiber* self) {
        self->fn_();            
        self->done_ = true;     
        swapcontext(&self->ctx_, &self->caller_);
    }
};

}