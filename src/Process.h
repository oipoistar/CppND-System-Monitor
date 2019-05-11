#pragma once
#define LOGURU_WITH_STREAMS 1

#include <string>
#include "loguru.hpp"

using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process
{
private:
    string pid;
    string user;
    string cmd;
    string cpu;
    string mem;
    string upTime;

public:
    struct FormatedProcess
    {
        string pid;
        string user;
        string cmd;
        string cpu;
        string mem;
        string upTime;
    };

    Process(string pid)
    {
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        this->mem = ProcessParser::getVmSize(this->pid);
        this->upTime = ProcessParser::getProcUpTime(this->pid);
        this->cpu = ProcessParser::getCpuPercent(this->pid);
        this->cmd = ProcessParser::getCmd(this->pid);
    }

    FormatedProcess getProcess();
    float cpuUsage();
};

Process::FormatedProcess Process::getProcess()
{
    std::string pid_display = this->pid;
    std::string user_name = this->user;
    std::string cpu_usage = this->cpu;
    std::string cmd_formated = this->cmd;
    std::string mem_display = this->mem;

    if (pid_display.size() < 6)
    {
        int diff = 6 - pid_display.size();
        for (int i = 0; i <= diff; i++)
            pid_display += " ";
    }

    if (user_name.size() < 12)
    {
        int diff = 12 - user_name.size();
        for (int i = 0; i <= diff; i++)
            user_name += " ";
    }

    if (cpu_usage.find('.', 0) != std::string::npos)
    {
        if (cpu_usage.at(1) == '.')
        {
            cpu_usage = " " + cpu_usage.substr(0, 4);
        }
        else
        {
            cpu_usage = cpu_usage.substr(0, 5);
        }

        
    }else{
        cpu_usage = "00.00";
    }

    cpu_usage = cpu_usage + "%";

    if (cmd_formated.size() > 20)
    {
        cmd_formated = cmd_formated.substr(0, 50) + "...";
    }

    // 1234.123123 213.323123 23.23123
    if (!mem_display.empty())
    {
        std::stringstream ss;
        ss << std::setprecision(4) << stof(mem_display);
        mem_display = ss.str() + "MB";
    }
    else
    {
        mem_display = "N/A";
    }

    std::stringstream upss;
    upss << std::setprecision(2) << this->upTime;

    Process::FormatedProcess fp;
    fp.cmd = cmd_formated;
    fp.cpu = cpu_usage;
    fp.mem = mem_display;
    fp.pid = pid;
    fp.upTime = upss.str();
    fp.user = user_name;

    return fp;
}

float Process::cpuUsage()
{
    return stof(this->cpu);
}
