//
// Created by wave on 2024/7/15.
//
#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <memory>

#include "../../include/event/EventLoop.h"
#include "../../include/utils/Logger.h"
#include "../../include/poller/Poller.h"

__thread EventLoop* t_threadInThisLoop = nullptr;

const int kPollTimeMs = 10000;

int createEventFd(){
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_FATAL("eventfd error:%d\n", errno);
    }
    return evtfd;
}


EventLoop::EventLoop()
:looping_(false),
quit_(false),
callingPendingFunctors_(false),
threadId_(CurrentThread::tid()),
poller_(Poller::newDefaultPoller(this)),
wakeupFd_(createEventFd()),
wakeupChannel_(new Channel(this, wakeupFd_))
{
    std::cout<<"here~~~"<<endl;
    if(t_threadInThisLoop){
        LOG_FATAL("Another EventLoop %p exists in this thread %d\n", t_threadInThisLoop, threadId_);
    }else{
        t_threadInThisLoop = this;
    }

    wakeupChannel_->setReadCallback(
            std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
};

EventLoop::~EventLoop() {
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();

    ::close(wakeupFd_);
    t_threadInThisLoop= nullptr;
}

void EventLoop::loop() {
    looping_ = true;
    quit_ = false;

    while (!quit_){
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        for(Channel* channel: activeChannels_){
            channel->handleEvent(pollReturnTime_);
        };
        doPendingFunctors();
    };
    LOG_INFO("EventLoop %p stop looping.\n", this);
    looping_ = false;
};

void EventLoop::quit() {
    quit_ = true;

    if(!isInLoopThread()){ //TODO: 任意一个非本线程的线程都需要负责唤醒loop线程
        wakeup();
    }
};

void EventLoop::runInLoop(EventLoop::Functor cb) {
    if(isInLoopThread()){
        cb;
    } else{
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(EventLoop::Functor cb) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        pendingFunctors_.emplace_back(cb);
    }

    if (!isInLoopThread() || callingPendingFunctors_) //TODO: callingPendingFunctors_ 这个有什么用,为什么要这样用~~~~
    {
        wakeup(); // 唤醒loop所在线程
    }
}

//loop的主函数中poll会让线程进入阻塞状态, 通过wakeup将本线程唤醒,继续执行～～～
void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR("EventLoop::handleRead() reads %lu bytes instead of 8\n", n);
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR("EventLoop::wakeup() writes %lu bytes instead of 8\n", n);
    }
}

void EventLoop::updateChannel(Channel *channel) {
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel) {
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel) {
    return poller_->hasChannel(channel);
}

void EventLoop::doPendingFunctors(){
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for (Functor f :functors){
        f();
    }

    callingPendingFunctors_ = false;
}