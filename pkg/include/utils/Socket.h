//
// Created by wave on 2024/7/16.
//

#ifndef LOTUS_SOCKET_H
#define LOTUS_SOCKET_H
#include "Noncopyable.h"
class InetAddress;
class Socket:Noncopyable{
public:
    explicit Socket(int sockfd):sockfd_(sockfd){};
    ~Socket();

    int fd()const{return sockfd_;};
    void bindAddress(const InetAddress& localaddr );
    void listen();
    int accept(InetAddress* peeraddr);

    void shutdownWrite();

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool  on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

private:
    const int sockfd_;
};


#endif //LOTUS_SOCKET_H
