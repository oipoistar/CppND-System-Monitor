#pragma once

using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process
{

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

    Process(string pid) : fp()
    {
        this->fp.pid = pid;
        std::string pid_display = this->fp.pid;
        std::string user_name = ProcessParser::getProcUser(this->fp.pid);
        int cpu_usage = ProcessParser::getCpuPercent(this->fp.pid);
        std::string cmd_formated = ProcessParser::getCmd(this->fp.pid);

        float mem_used = ProcessParser::getVmSize(this->fp.pid);
        char s[32] = "";
        std::string mem_display(Util::FormatBytes(mem_used, s));

        this->fp.cmd = cmd_formated;
        this->fp.cpu = std::to_string(cpu_usage);
        this->fp.mem = mem_display;

        this->fp.pid = this->fp.pid;
        this->fp.upTime = Util::convertToTime(ProcessParser::getProcUpTime(this->fp.pid));
        this->fp.user = user_name;
    }

    Process(const Process &p2) {
            fp.pid = p2.fp.pid;
            fp.user = p2.fp.user;
            fp.cmd = p2.fp.cmd;
            fp.cpu = p2.fp.cpu;
            fp.mem = p2.fp.mem;
            fp.upTime = p2.fp.upTime;
            
        } 

    int cpuUsage();
    FormatedProcess fp;

private:
};

int Process::cpuUsage()
{
    return stoi(this->fp.cpu);
}
