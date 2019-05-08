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
};

string Process::getProcess()
{
    if (!ProcessParser::isPidExisting(this->pid))
        return "";
        
    return (this->pid + "   " + this->user + "   "+ this->mem  + "   " + this->cpu  + "   " + this->upTime + "   " + this->cmd);
}
