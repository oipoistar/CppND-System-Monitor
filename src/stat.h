#pragma once

#include <string>
#include <boost/algorithm/string.hpp>

struct ProcessStatusInformation
{
    int pid;
    std::string comm;
    char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    u_int flags;
    u_long minflt;
    u_long cminflt;
    u_long majflt;
    u_long cmajflt;
    u_long utime;
    u_long stime;
    long cutime;
    long cstime;
    long priority;
    long nice;
    long num_threads;
    long itrealvalue;
    unsigned long long starttime;
    u_long vsize;
    long rss;
    u_long rsslim;
    u_long startcode;
    u_long endcode;
    u_long startstack;
    u_long kstkesp;
    u_long kstkeip;
    u_long signal;
    u_long blocked;
    u_long sigignore;
    u_long sigcatch;
    u_long wchan;
    u_long nswap;
    u_long cnswap;
    int exit_signal;
    int processor;
    uint rt_priority;
    uint policy;
    unsigned long long delayacct_blkio_ticks;
    u_long guest_time;
    long cguest_time;
    u_long start_data;
    u_long end_data;
    u_long start_brk;
    u_long arg_start;
    u_long arg_end;
    u_long env_start;
    u_long env_end;
    int exit_code;
    float total_time;
};



