#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "ProcessParser.h"
#include "CpuStats.h"

class SysInfo
{
private:
    float memPercent;
    std::string OSname;
    std::string kernelVer;
    long upTime;
    int totalProc;
    int runningProc;
    int threads;
    CpuStat cpu_stat;

public:
    SysInfo() : cpu_stat()
    {
        /*
    Getting initial info about system
    Initial data for individual cores is set
    System data is set
    */
        this->setAttributes();
        this->OSname = ProcessParser::getOSName();
        this->kernelVer = ProcessParser::getSysKernelVersion();  
    }

    void setAttributes();
    std::string getMemPercent() const;
    long getUpTime() const;
    std::string getThreads() const;
    std::string getTotalProc() const;
    std::string getRunningProc() const;
    std::string getKernelVersion() const;
    std::string getOSName() const;
    std::string getCpuPercent() ;
    std::vector<std::string> getCoresStats() ;
};


void SysInfo::setAttributes()
{
    cpu_stat.refreshReading();
    // getting parsed data
    this->memPercent = ProcessParser::getSysRamPercent();
    this->upTime = ProcessParser::getSysUpTime();
    this->totalProc = ProcessParser::getTotalNumberOfProcesses();
    this->runningProc = ProcessParser::getNumberOfRunningProcesses();
    this->threads = ProcessParser::getTotalThreads();
}

std::string SysInfo::getMemPercent() const
{
    return to_string(this->memPercent);
}
long SysInfo::getUpTime() const
{
    return this->upTime;
}
std::string SysInfo::getKernelVersion() const
{
    return this->kernelVer;
}
std::string SysInfo::getTotalProc() const
{
    return to_string(this->totalProc);
}
std::string SysInfo::getRunningProc() const
{
    return to_string(this->runningProc);
}
std::string SysInfo::getThreads() const
{
    return to_string(this->threads);
}
std::string SysInfo::getOSName() const
{
    return this->OSname;
}

std::string SysInfo::getCpuPercent(){
    return std::to_string(this->cpu_stat.getTotalCpuUsage());
}

std::vector<std::string> SysInfo::getCoresStats(){
    return std::vector<std::string>();
}