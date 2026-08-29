#pragma once
#include <functional>
#include <vector>
#include <ucontext.h>   
#include <memory>
#include <queue>
namespace muduo {
    class fiber {
    public:
        template<typename F>
        explicit fiber(F&& f) : fn_(std::forward<F>(f)), stack_(65536) {
            getcontext(&ctx_);
            ctx_.uc_stack.ss_sp = stack_.data(); 
            ctx_.uc_stack.ss_size = stack_.size(); 
            ctx_.uc_link = nullptr;  
            makecontext(&ctx_, (void(*)()) &fiber::trampoline, 1, this);
            done_ = false;
        }

        void resume() {
            if (done_) return;
            swapcontext(&caller_, &ctx_);
        }

        void yield() {
            swapcontext(&ctx_, &caller_);
        }

    public:
        ucontext_t ctx_;    
        ucontext_t caller_; 
        std::vector<char> stack_; 
        std::function<void()> fn_;
        bool done_ = false;

        using task = std::function<void()>;
        static std::shared_ptr<std::queue<task>> queue1_;

        static void trampoline(fiber* self) {
            self->fn_();            
            self->done_ = true;     
            swapcontext(&self->ctx_, &self->caller_);
        }
    };

}

