//
// Created by wave on 2024/7/16.
//

#ifndef LOTUS_EVENTLOOPTHREAD_H
#define LOTUS_EVENTLOOPTHREAD_H
#include <functional>
#include <mutex>
#include <condition_variable>
#include <string>

#include "../utils//Noncopyable.h"
#include "../thread/Thread.h"
class EventLoop;
class EventLoopThread: Noncopyable{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;
    EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(),
                    const std::string &name = std::string()); //这里为什么要初始化ThreadInitCallback呢？

    ~EventLoopThread();
    EventLoop* startLoop();
private:
    void threadFunc();
    EventLoop* loop_;
    bool exiting_;
    Thread thread_;
    std::mutex mutex_;
    std::condition_variable() cond_;
    ThreadInitCallback callback_;
};


#endif //LOTUS_EVENTLOOPTHREAD_H
