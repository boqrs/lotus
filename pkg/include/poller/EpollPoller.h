//
// Created by wave on 2024/7/15.
//

#ifndef LOTUS_EPOLLPOLLER_H
#define LOTUS_EPOLLPOLLER_H
#include <vector>
#include <sys/epoll.h>

#include "../utils/Timestamp.h"
#include "Poller.h"

class Channel;

class EpollPoller: public Poller{
public:
    EPollPoller(EventLoop *loop);
    ~EPollPoller() override;

    Timestamp poll(int timeoutMs, ChannelList *activeChannels) override;
    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;

private:
    static const int kInitEventListSize = 16;
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
    void update(int operation, Channel *channel);
    using EventList = std::vector<epoll_event>;

    int epollfd_;
    EventList events_;
};



#endif //LOTUS_EPOLLPOLLER_H
