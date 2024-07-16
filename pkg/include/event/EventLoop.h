//
// Created by wave on 2024/7/13.
//

#ifndef LOTUS_EVENTLOOP_H
#define LOTUS_EVENTLOOP_H
#include "../channel/Channel.h"
#include "../utils/Noncopyable.h"
#include "../utils/Timestamp.h"
#include "../thread/CurrentThread.h"

class EventLoop: Noncopyable{
public:
    using Functor = std::function<void()>;
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();



    Timestamp pollReturnTime()const{return pollReturnTime_;};
    void runInLoop(Functor cb); //TODO: 这两个有问题吧
    void queueInLoop(Functor cb);

    void wakeup();

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

    bool isInLoopThread()const{return threadId_==CurrentThread::tid();};
private:
    void handleRead();
    void doPendingFunctors();
    using ChannelList = std::vector<Channel*>; //缓存的是当前epoll中的所有通道

    std::atomic_bool  looping_;
    std::atomic_bool  quit_;

    const pid_t threadId_;
    Timestamp pollReturnTime_;
    std::unique_ptr<Poller> poller_;

    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_; //具体的类型不需要写指针

    ChannelList activeChannels_;
    std::atomic_bool callingPendingFunctors_;
    std::vector<Functor> pendingFunctors_;
    std::mutex mutex_; //TODO: eventLoop在单独的线程里面,互斥锁只需要锁定最小的范围了
};
#endif //LOTUS_EVENTLOOP_H
