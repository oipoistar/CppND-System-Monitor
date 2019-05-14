#pragma once

#include <string>

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
    unsigned int flags;
    unsigned long int minflt;
    unsigned long int cminflt;
    unsigned long int majflt;
    unsigned long int cmajflt;
    unsigned long int utime;
    unsigned long int stime;
    long cutime;
    long cstime;
    long priority;
    long nice;
    long num_threads;
    long itrealvalue;
    unsigned long long starttime;
    unsigned long int vsize;
    long rss;
    unsigned long int rsslim;
    unsigned long int startcode;
    unsigned long int endcode;
    unsigned long int startstack;
    unsigned long int kstkesp;
    unsigned long int kstkeip;
    unsigned long int signal;
    unsigned long int blocked;
    unsigned long int sigignore;
    unsigned long int sigcatch;
    unsigned long int wchan;
    unsigned long int nswap;
    unsigned long int cnswap;
    int exit_signal;
    int processor;
    unsigned int rt_priority;
    unsigned int policy;
    unsigned long long delayacct_blkio_ticks;
    unsigned long int guest_time;
    long cguest_time;
    unsigned long int start_data;
    unsigned long int end_data;
    unsigned long int start_brk;
    unsigned long int arg_start;
    unsigned long int arg_end;
    unsigned long int env_start;
    unsigned long int env_end;
    int exit_code;
    float total_time;
};



