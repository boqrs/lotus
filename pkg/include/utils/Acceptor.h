//
// Created by wave on 2024/7/17.
//

#ifndef LOTUS_ACCEPTOR_H
#define LOTUS_ACCEPTOR_H
#include "Noncopyable.h"
#include "Socket.h"
#include "../channel/Channel.h"

class InetAddress;
class EventLoop;

class Acceptor: Noncopyable{
public:
    using NewConnectionCallback = std::function<void(int socketfd, const InetAddress&)>;
    Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reusePort);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback& cb){NewConnectionCallback_=cb;}
    bool listenning()const{return listenning_;};
    void listen();

private:
    void handleRead();
    EventLoop *loop_;
    Socket acceptorSocket_;
    Channel acceptorChannel_;
    NewConnectionCallback NewConnectionCallback_;
    bool listenning_;
};

#endif //LOTUS_ACCEPTOR_H
