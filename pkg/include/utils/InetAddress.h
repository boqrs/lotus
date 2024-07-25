//
// Created by wave on 2024/7/16.
//

#ifndef LOTUS_INETADDRESS_H
#define LOTUS_INETADDRESS_H
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

class InetAddress {
public:
    explicit InetAddress(uint16_t port=0, std::string ip="127.0.0.1");
    explicit InetAddress(const sockaddr_in &addr):addr_(addr){};

    std::string toIp()const;
    std::string toIpPort()const;
    std::string toPort()const;

    const sockaddr_in* getSockAddr(){return &addr_;};
    void setSockAddr(const sockaddr_in &addr){addr_ = addr;};
private:
    sockaddr_in addr_;

};





#endif //LOTUS_INETADDRESS_H
