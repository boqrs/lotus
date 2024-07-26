//
// Created by wave on 2024/7/13.
//

#include <string>

#include "../pkg/include/tcp/Server.h"
#include "../pkg/include/utils/Logger.h"

class EchoServer
{
public:
    EchoServer(EventLoop *loop, const InetAddress &addr, const std::string &name)
            : server_(loop, addr, name)
            , loop_(loop)
    {
        // 注册回调函数
        server_.setConnectionCallback(
                std::bind(&EchoServer::onConnection, this, std::placeholders::_1));

        server_.setMessageCallback(
                std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        // 设置合适的subloop线程数量
        server_.setThreadNum(150);
    }
    void start()
    {
        server_.start();
    }
private:
    // 连接建立或断开的回调函数
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_INFO("Connection UP : %s", conn->getPeer().toIpPort().c_str());
        }
        else
        {
            LOG_INFO("Connection DOWN : %s", conn->getPeer().toIpPort().c_str());
        }
    }

    // 可读写事件回调
    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
    {
        std::string msg = buf->retrieveAllAsString();
        conn->send(msg);
        // conn->shutdown();   // 关闭写端 底层响应EPOLLHUP => 执行closeCallback_
    }

    EventLoop *loop_;
    TcpServer server_;
};

int main(){
    std::cout<<"server start with port 8003"<<std::endl;
    EventLoop loop;
    std::cout<<"start 1"<<std::endl;
    InetAddress addr(8003);
    std::cout<<"start 2"<<std::endl;
    EchoServer server(&loop, addr, "EchoServer");
    std::cout<<"start 3"<<std::endl;
    server.start();
    std::cout<<"start 4"<<std::endl;
    loop.loop();
    std::cout<<"start 5"<<std::endl;
    return 0;
}