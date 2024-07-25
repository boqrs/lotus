//
// Created by wave on 2024/7/17.
//

#ifndef LOTUS_BUFFER_H
#define LOTUS_BUFFER_H
#include <vector>
#include <string>
#include <algorithm>
#include <stddef.h>

class Buffer{
public:
    static const size_t kCheapPrepend = 8; //header len
    static const size_t kInitialSize = 1024; //body len

    explicit Buffer(size_t initSize=kInitialSize)
    :buffer_(kCheapPrepend+initSize),
    readIndex_(kCheapPrepend),
    writeIndex_(kCheapPrepend){

    }

    size_t prependableBytes(){return readIndex_;} //已经读取的数据
    size_t readableBytes(){return writeIndex_-readIndex_;};
    size_t writeableBytes(){return buffer_.size()-writeIndex_;}

    const char* peek()const{return begin()+readIndex_;};
    void retrieve(size_t len)    {
        if (len<readableBytes()){
            readIndex_+=len;
        } else{
            retrieveAll();
        }
    }

    void retrieveAll(){
        readIndex_ = kCheapPrepend;
        writeIndex_ = kCheapPrepend;
    }

    std::string retrieveAsString(size_t len){
        std::string result(peek(),len);
        retrieve(len);
        return result;
    }

    void ensureWritableBytes(size_t len){
        if (writeableBytes()<len){
            makeSpace(len);
        }
    }

    void append(const char *data, size_t len){
        ensureWritableBytes(len);
        std::copy(data, data+len, beginWrite());
        writerIndex_ += len;
    }

    char *beginWrite() { return begin() + writerIndex_; }
    const char *beginWrite() const { return begin() + writerIndex_; }

    ssize_t readFd(int fd, int *saveErrno);
    ssize_t writeFd(int fd, int *saveErrno);
private:

    char* begin(){return &*buffer_.begin();};
    const char* begin()const{return &*buffer_.begin();};

    void makeSpace(size_t len){
        if (writeableBytes()+readableBytes()< len+kCheapPrepend){
            buffer_.resize(writeIndex_+len); //扩容len长度的空间
        } else{
            //如果len长度小于可写空间
            //复制write-read的内容到kCheapPrepend之后,writeIndex=kCheapPrepend+write-read readIndex=kCheapPrepend;
            size_t dataLen = readableBytes;
            std::copy(buffer_.begin()+readIndex_, buffer_.begin()+writeIndex_, buffer_.begin()+kCheapPrepend);
            readIndex_=kCheapPrepend;
            writeIndex_=readIndex_+dataLen;
        }
    };

    std::vector<char> buffer_;
    size_t readIndex_;
    size_t writerIndex_;
};
#endif //LOTUS_BUFFER_H
