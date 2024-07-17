//
// Created by wave on 2024/7/17.
//
#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>

#include "../../include/utils/Buffer.h"

ssize_t Buffer::readFd(int fd, int *saveErrno) {
    char extrabuf[65535] = {0};
    /*
    struct iovec {
        ptr_t iov_base; // iov_base指向的缓冲区存放的是readv所接收的数据或是writev将要发送的数据
        size_t iov_len; // iov_len在各种情况下分别确定了接收的最大长度以及实际写入的长度
    };
    */

    struct iovec vec[2];
    const size_t writable = writableBytes();

    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;

    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    const int iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
    /* 这里vec是一个结构体数组,如果可写区域小于栈中的空间则两个一起使用否则只使用可写区*/
    const ssize_t n = ::readv(fd, vec, iovcnt);
    if (n <0){
        *saveErrno = errno;
    } else if (n <= writable){
        writerIndex_ += n;
    } else{
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }

    return n;
}

ssize_t Buffer::writeFd(int fd, int *saveErrno) {
    ssize_t n = ::write(fd, peek(), readableBytes());
    if (n < 0)
    {
        *saveErrno = errno;
    }
    return n;
}