//
// Created by wave on 2024/7/13.
//

#ifndef LOTUS_EVENTLOOP_H
#define LOTUS_EVENTLOOP_H
#include "../channel/Channel.h"
#include "../utils/Noncopyable.h"


class EventLoop: Noncopyable{
    public:
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

};
#endif //LOTUS_EVENTLOOP_H
