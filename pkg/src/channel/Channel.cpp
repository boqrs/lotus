//
// Created by wave on 2024/7/15.
//
#include <sys/epoll.h>

#include "../../include/channel/Channel.h"
#include "../../include/event/EventLoop.h"
#include "../../include/utils/Logger.h"

#define invalidChannelIndex -1

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;;
const int Channel::kWriteEvent = EPOLLOUT;


Channel::Channel(EventLoop *loop, int fd)
:loop_(loop)
,fd_(fd)
,index_(invalidChannelIndex),
events_(0)
,revents_(0)
,tied_(false){};

Channel::~Channel(){};

void  Channel::tie(const std::shared_ptr<void> & obj) {
    tie_ = obj; //这里链接的ptr会转化成为一个weak ptr,在channel生命周期内一直持有这个weak_ptr保证了,
    // channel一定先于connection释放掉
    tied_ = true;
}

void Channel::update() {
    loop_->updateChannel(this);
}

void Channel::remove() {
    loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime) {

    if (tied_){
        //if guard == nullptr, means connection is destoryed~~~
        std::shared_ptr<void> guard = tie_.lock();
        if (guard){
            handleEventWithGuard(receiveTime);
        }
    } else{
        //TODO: just print log here, if no connection bind here, must no data~~~~
    }
}

void Channel::handleEventWithGuard(Timestamp receiveTime) {
    LOG_INFO("channel handleEvent revents:%d\n", revents_);

    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if (closeCallback_)
        {
            closeCallback_();
        }
    }

    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
        {
            errorCallback_();
        }
    }

    if (revents_ & (EPOLLIN | EPOLLPRI))
    {
        if (readCallback_)
        {
            readCallback_(receiveTime);
        }
    }

    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_)
        {
            writeCallback_();
        }
    }
}