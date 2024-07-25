//
// Created by wave on 2024/7/16.
//

#include "../../include/event/EventLoopThread.h"
#include "../../include/event/EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop, const std::string& nameArg):
baseLoop_(baseLoop),
name_(nameArg),
started_(false),
numThread_(0),
next_(0){}

EventLoopThreadPool::~EventLoopThreadPool() {}

void EventLoopThreadPool::start(const EventLoopThreadPool::ThreadInitCallback &cb) {
    started_ = true;

    for (int i = 0; i < numThread_ ; ++i) {
        char buf[name_.size() + 32]; //numThread_ may be int32, actually threadnum max = 150 is better
        snprintf(buf, sizeof buf, "%s%d", name_.c_str(), i);
        EventLoopThread *et = new EventLoopThread(cb, buf);//能不能用make_unique_ptr
        threads_.push_back(std::unique_ptr<EventLoopThread>(et));
        loops_.push_back(et->startLoop());
    }

    if (numThread_==0 && cb){
        cb(baseLoop_); //there is one thread in process
    }
}

EventLoop *EventLoopThreadPool::getNextLoop() {
    EventLoop *loop=baseLoop_;

    if (!loops_.empty()){
        loop = loops_[next_];
        ++next_;
        if (next_>=loops_.size()){
            next_ = 0;
        }
    }

    return loop;
}

std::vector<EventLoop *> EventLoopThreadPool::getAllLoops() {
    if (loops_.empty()){
        return std::vector<EventLoop*>(1,baseLoop_); //vector的初始化
    }
    return loops_;
}