//
// Created by wave on 2024/7/15.
//

#include "../../include/thread/CurrentThread.h"

namespace CurrentThread{
    __thread int t_cachedTid=0; //

    void cacheTid(){
        if (t_cachedTid==0){
            t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }
};
