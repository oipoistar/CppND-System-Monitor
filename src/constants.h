#pragma once

#include <string>

using namespace std;

enum CPUStates{
	S_USER = 1,
	S_NICE,
	S_SYSTEM,
	S_IDLE,
	S_IOWAIT,
	S_IRQ,
	S_SOFTIRQ,
	S_STEAL,
	S_GUEST,
	S_GUEST_NICE
};

class Path{

public:
    static string basePath() {
        return "/proc/"s;
    }
    static string cmdPath(){
        return "/cmdline"s;
    }
    static string statusPath(){
        return "/status"s;
    }
    static string statPath(){
        return "/stat"s;
    }
    static string upTimePath(){
        return "uptime"s;
    }
    static string memInfoPath(){
        return "meminfo"s;
    }
    static string versionPath(){
        return "version"s;
    }
};
