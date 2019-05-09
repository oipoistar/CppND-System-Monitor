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
    Process(string pid)
    {
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        //TODOs:
        this->mem = ProcessParser::getVmSize(this->pid);
        this->upTime = ProcessParser::getProcUpTime(this->pid);
        this->cpu = ProcessParser::getCpuPercent(this->pid);
        this->cmd = ProcessParser::getCmd(this->pid);
    }

    string getProcess();
    float cpuUsage();
};

string Process::getProcess()
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

    if (cpu_usage.at(1) == '.')
    {
        cpu_usage = " " + cpu_usage.substr(0, 4);
    }
    else
    {
        cpu_usage = cpu_usage.substr(0, 5);
    }

    cpu_usage = cpu_usage + "%";

    if (cmd_formated.size() > 20)
    {
        cmd_formated = cmd_formated.substr(0, 50) + "...";
    }

    // 1234.123123 213.323123 23.23123
    int dec_place = mem_display.find('.');
    if (dec_place != string::npos)
    {
        int diff = 5 - dec_place;
        for (int i = 0; i <= diff; i++)
            mem_display = " " + mem_display;

        dec_place = mem_display.find('.');
        mem_display = mem_display.substr(0, dec_place + 3);
    }else{
        mem_display = "   00.00";
    }

    std::string display_string = pid_display + " " + user_name + " " + cpu_usage + " " + mem_display + "MB " + this->upTime + " " + cmd_formated;
    //std::cout << display_string << "\n";
    LOG_S(INFO) << display_string;
    return display_string;
}

float Process::cpuUsage()
{
    return stof(this->cpu);
}
