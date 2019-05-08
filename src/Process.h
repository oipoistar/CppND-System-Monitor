#pragma once

#include <string>

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
    std::string cpu_usage = this->cpu;
    std::string cmd_formated = this->cmd;

    if(cpu_usage.at(1) == '.'){
        cpu_usage = cpu_usage.substr(0, 4);
    }else{
        cpu_usage = cpu_usage.substr(0, 5);
    }

    if(cmd_formated.size() > 20){
        cmd_formated = cmd_formated.substr(0,50) + "...";
    }

    return (this->pid + " " + this->user + " " + cpu_usage + "% " + this->mem  + " " + this->upTime + " " + cmd_formated);
}

float Process::cpuUsage(){
    return stof(this->cpu);
}
