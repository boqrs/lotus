//
// Created by wave on 2024/7/16.
//
#include "../../include/event/EventLoopThread.h"
#include "../../include/event/EventLoop.h"

EventLoopThread::EventLoopThread(const EventLoopThread::ThreadInitCallback &cb,
                                 const std::string &name):
                                 loop_(nullptr),
                                 exiting_(false),
                                 thread_(std::bind(&EventLoopThread::threadFunc, this), name),
                                 mutex_(),
                                 cond_(),
                                 callback_(cb){

}

EventLoopThread::~EventLoopThread(){
    exiting_ = true;
    if (loop_ != nullptr){
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop(){
    thread_.start();
    EventLoop *loop = nullptr;
    {
        std::unique_lock<std::mutex> lock(mutex_); //这里互斥锁锁的是什么资源
        while(loop_ == nullptr)
        {
            cond_.wait(lock);
        }
        loop = loop_; //执行到这一步的时候loop_已经被赋值了
    }
    return loopl;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    if (callback_){

        callback_(loop_); //这里的callback是什么时候注册进来的?

    }

    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = &loop;
        cond_.notify_one();
    }
    loop.loop(); //这是一个死循环
    std::unique_lock<std::mutex> lock(mutex_); //互斥锁锁定的是下方的所有代码吧～～～
    loop_ = nullptr;
}
