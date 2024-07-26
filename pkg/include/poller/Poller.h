//
// Created by wave on 2024/7/15.
//

#ifndef LOTUS_POLLER_H
#define LOTUS_POLLER_H
#include <vector>
#include <unordered_map>

#include "../utils/Timestamp.h"
#include "../channel/Channel.h"

class EventLoop;
class Poller {
public:
    using ChannelList = std::vector<Channel*>;
    Poller(EventLoop* loop);
    virtual ~Poller()=default;

    virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;

    bool hasChannel(Channel* channel)const;

    static Poller* newDefaultPoller(EventLoop* loop);

protected:
    //TODO: 这里是的保护性数据有用到吗？
    using ChannelMap = std::unordered_map<int, Channel *>;
    ChannelMap channels_;
private:
    EventLoop* ownerLoop_;
};


#endif //LOTUS_POLLER_H
