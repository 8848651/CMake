#pragma once
#include <functional>
#include <vector>
#include <ucontext.h>   
#include <memory>
#include <queue>
#include <unordered_map>
#include <random>
namespace muduo {
    class fiber : public std::enable_shared_from_this<fiber> {
    public:
        template<typename F>
        explicit fiber(F&& f) : fn_(std::forward<F>(f)), stack_(65536) {
            getcontext(&ctx_);
            ctx_.uc_stack.ss_sp = stack_.data();
            ctx_.uc_stack.ss_size = stack_.size();
            ctx_.uc_link = nullptr;
            makecontext(&ctx_, (void(*)()) & fiber::trampoline, 1, this);
            done_ = false;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> distrib(1, 1000000);
            hashcode = distrib(gen);
        }

        void resume() {
            if (done_) return;
            currentptr_ = shared_from_this();
            swapcontext(&caller_, &ctx_);
        }

        void yield() {
            swapcontext(&ctx_, &caller_);
        }

        void newhashcode() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> distrib(1, 1000000);
            hashcode = distrib(gen);
        }

    public:
        ucontext_t ctx_;
        ucontext_t caller_;
        std::vector<char> stack_;
        std::function<void()> fn_;
        bool done_ = false;
        size_t hashcode;

        using task = std::function<void()>;
        static std::shared_ptr<std::queue<task>> queue1_;
        static std::unordered_map<int, task> queue2_;
        static std::weak_ptr<fiber> currentptr_;


        static void trampoline(fiber* self) {
            self->fn_();
            self->done_ = true;
            swapcontext(&self->ctx_, &self->caller_);
        }
    };

}

