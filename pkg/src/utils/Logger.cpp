//
// Created by wave on 2024/7/13.
//
#include "../../include/utils/Timestamp.h"
#include "../../include/utils/Logger.h"

Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::setLogLevel(int level) {
    logLevel_ = level;
}

void Logger::log(std::string msg) {
    std::string pre = "";
    switch (logLevel_) {
        case INFO:
            pre = "INFO: ";
            break;
        case ERROR:
            pre = "ERROR: ";
            break;
        case DEBUG:
            pre = "DEBUG: ";
            break;
        case FATAL:
            pre = "FATAL: ";
            break;
    }

    std::cout << pre + Timestamp::now().toString() << " : " << msg << std::endl;
}