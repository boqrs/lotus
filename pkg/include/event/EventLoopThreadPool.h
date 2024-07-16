//
// Created by wave on 2024/7/16.
//

#ifndef LOTUS_EVENTLOOPTHREADPOOL_H
#define LOTUS_EVENTLOOPTHREADPOOL_H
#include <functional>
#include <string>
#include <vector>
#include <memory>

#include "../utils/Noncopyable.h"
class EventLoop;
class EventLoopThread;

class EventLoopThreadPool{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;
    EventLoopThreadPool(EventLoop* baseLoop, const std::string nameArg&);
    ~EventLoopThreadPool();

    void setThreadNum(int num){numThread_ = num;};
    void start(const ThreadInitCallback& cb = ThreadInitCallback());

    EventLoop* getNextLoop();
    std::vector<EventLoop*> getAllLoops();

    bool started(){return started_;};
    const std::string name()const{return name_;}; //输出的const有什么用

private:
    EventLoop* baseLoop_;
    std::string name_;
    bool started_;
    int numThread_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_; //这个容器内保存的是loopthread的指针
    std::vector<EventLoop*> loops_; //保存所有的loop信息

};
#endif //LOTUS_EVENTLOOPTHREADPOOL_H
