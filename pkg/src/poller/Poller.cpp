//
// Created by wave on 2024/7/26.
//

#include "../../include/poller/Poller.h"
#include "../../include/channel/Channel.h"

Poller::Poller(EventLoop *loop)
        : ownerLoop_(loop)
{
}

bool Poller::hasChannel(Channel *channel) const
{
    auto it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel; //first=key; second=value
}