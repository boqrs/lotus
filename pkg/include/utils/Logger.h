//
// Created by wave on 2024/7/13.
//

#ifndef LOTUS_LOGGER_H
#define LOTUS_LOGGER_H
#include <iostream>
#include <string>
#include "Noncopyable.h"

//宏定义一般都是以do while的形式书写，划分模块
#define LOG_INFO(logmsgFormat, ...) \
    do {                            \
      Logger &logger = Logger::instance(); \
    logger.setLogLevel(INFO);      \
       char buf[1024] = {0};                             \
          snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                           \
             logger.log(buf);                         \
                                    \
    } while(0);


#define LOG_ERROR(logmsgFormat, ...) \
do {                                 \
      Logger &logger = Logger::instance(); \
    logger.setLogLevel(ERROR);       \
       char buf[1024] = {0};         \
          snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                           \
             logger.log(buf);                         \
             }while(0);


#define LOG_FATAL(logmsgFormat, ...) \
do {                                 \
      Logger &logger = Logger::instance(); \
    logger.setLogLevel(FATAL);       \
       char buf[1024] = {0};         \
          snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                           \
             logger.log(buf);        \
             exit(-1);                        \
             }while(0);

#ifdef MUDEBUG
#define LOG_DEBUG(logmsgFormat, ...) \
  do                                 \
  {                               \
    Logger &logger = Logger::instance(); \
    logger.setLogLevel(DEBUG);       \
            char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);   \
        //__VA_ARGS__ 这是一个可变参数的宏定义
        logger.log(buf);                                  \
  }while(0);
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif


// 定义日志的不同级别, INFO ERROR FATAL DEBUG
enum LogLevel {
    INFO,
    ERROR,
    FATAL,
    DEBUG,
};

class Logger: Noncopyable{
public:
    //唯一的日志对象, 单例
    static Logger &instance(); //这个应该在刚开始的位置初始化～～～
    void setLogLevel(int level);
    void log(std::string msg);

private:
    int logLevel_;
};


#endif //LOTUS_LOGGER_H
