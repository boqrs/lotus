//
// Created by wave on 2024/7/13.
//

#ifndef LOTUS_CHANNEL_H
#define LOTUS_CHANNEL_H
#include <functional>
#include <memory>
#include "../utils/Noncopyable.h"
#include "../utils/Timestamp.h"


class EventLoop;

class Channel: Noncopyable{
public:
    using  EventCallback = std::function<viod>;
    using  ReadEventCallback = std::function<Timestamp>;

    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);

    void setReadCallback(ReadEventCallback cb){readCallback_= std::move(cb)};
    void setWriteCallback(EventCallback cb){writeCallback_ = std::move(cb)};
    void setCloseCallback(EventCallback cb){closeCallback_ = std::move(cb)};
    void setErrorCallback(EventCallback cb){errorCallback_ = std::move(cb)};

    void tie(const std::shared_ptr<void>&);

    int fd(){return fd_;};
    int events()const{return events_;};
    void set_revents(int rent){rEvents_ = rent;};
    void remove();
    EventLoop* ownerLoop(){return loop_;};
    int index(){return index_;};
    void setIndex(int idx){index_ = idx; };

    void enableReading(){events_ |= kReadEvent; update();};
    void disableReading(){events_ &= ~kReadEvent; update();};
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableWriting() { events_ &= ~kWriteEvent; update(); }
    void disableAll() { events_ = kNoEvent; update(); }


private:
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    static  const int kNoEvent;
    static  const int kReadEvent;
    static  const int kWriteEvent;

    EventLoop* loop_; //TODO: channel是绑定了loop的,这种绑定本质上是fd
    const int fd_; //这里本质上就是客户端

    int events_; //这是注册的时候绑定的事件
    int rEvents_;  //上报的事件类型
    int index_;

    std::weak_ptr<void> tie_;
    bool tied_;

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventCallback closeCallback_;
};

#endif //LOTUS_CHANNEL_H
