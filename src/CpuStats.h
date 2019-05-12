#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iterator>

#include "constants.h"
#include "util.h"
#include "ProcessParser.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/param.h>

const double ticks = (double)sysconf( _SC_CLK_TCK );

class CpuStat{

    struct CPU_CORE{
        bool is_main;
        int id;
        long user;
        long nice;
        long system;
        long idle;
        long iowait;
        long irq;
        long softirq;
        long steal;
        long guest;
        long guest_nice;
    };

    private:
        std::vector<CPU_CORE> lastParsed;
        std::vector<CPU_CORE> current;
        void fillCPUCOREStruct(CPU_CORE& core, std::vector<std::string> str);
        float calculateUsage(CPU_CORE& old, CPU_CORE& current) const;
    public:
        int getNumberOfCores();
        float getPercentageForCore(int core);
        float getTotalCpuUsage();
        void refreshReading();

        CpuStat(): lastParsed(), current() {}
};

void CpuStat::fillCPUCOREStruct(CPU_CORE& core, std::vector<std::string> str){
   core.user = stol(str[1]);
   core.nice = stol(str[2]);
   core.system = stol(str[3]);
   core.idle = stol(str[4]);
   core.iowait = stol(str[5]);
   core.irq = stol(str[6]);
   core.softirq = stol(str[7]);
   core.steal = stol(str[8]);
   core.guest = stol(str[9]);
   core.guest_nice = stol(str[10]);
}

void CpuStat::refreshReading(){
    this->lastParsed = this->current;
    current.clear();

    int numOfCores = getNumberOfCores();

    //get total cpu
    CPU_CORE main_core;
    main_core.is_main = true;
    
    auto vec = ProcessParser::parseProcStatFile();
    fillCPUCOREStruct(main_core, vec);
    this->current.emplace_back(main_core);

    // Now parse remaining cores
    for(int i = 0; i<numOfCores; i++){
        CPU_CORE core;
        core.is_main = false;
        core.id = i;
        vec = ProcessParser::parseProcStatFile(std::to_string(i));
        fillCPUCOREStruct(core, vec);
        this->current.emplace_back(core);
    }
}

float CpuStat::calculateUsage(CPU_CORE& old, CPU_CORE& current) const{
    float total_time_old = old.user + old.nice + old.system + old.idle + old.iowait + old.irq + old.softirq + old.steal + old.guest;
    float total_time_current = current.user + current.nice + current.system + current.idle + current.iowait + current.irq + current.softirq + current.steal + current.guest;

    float total_idle_time_old = old.idle + old.iowait;
    float total_idle_time_current = current.idle + current.iowait;

    float diff_total = total_time_current - total_time_old;
    float diff_idle = total_idle_time_current - total_idle_time_old;
    // diff_total = cpu_delta
    // diff_idle = cpu_idle
    // cpu_usage = diff_total - diff_idle
    float used = diff_total - diff_idle;

    float usage = ((used / diff_total)) * ticks;

    return usage;
}

float CpuStat::getTotalCpuUsage(){
    if(!lastParsed.empty()){
        CPU_CORE cur_core;
        CPU_CORE old_core;
        for(auto& entry : this->current){
            if(entry.is_main)
                cur_core = entry;
        }

        for(auto& entry : this->lastParsed){
            if(entry.is_main)
                old_core = entry;
        }
        float result = calculateUsage(old_core, cur_core);
        return result;
    }else{
        return 0;
    }
}

float CpuStat::getPercentageForCore(int core){
    if(!lastParsed.empty()){
        CPU_CORE cur_core;
        CPU_CORE old_core;
        for(auto& entry : this->current){
            if(entry.id == core)
                cur_core = entry;
        }

        for(auto& entry : this->lastParsed){
            if(entry.id == core)
                old_core = entry;
        }

        return calculateUsage(old_core, cur_core);
    }else{
        return 0;
    }
}

int CpuStat::getNumberOfCores()
{
    std::string line;
    std::string name = "cpu cores";
    std::ifstream stream = Util::getStream((Path::basePath() + "cpuinfo"));
    while (std::getline(stream, line))
    {
        if (line.compare(0, name.size(), name) == 0)
        {
            std::istringstream buf(line);
            istream_iterator<std::string> beg(buf), end;
            vector<string> values(beg, end);
            return stoi(values[3]);
        }
    }

    return 0;
}