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
    }

    FormatedProcess getProcess();
    float cpuUsage();

private:
    FormatedProcess fp;

};

Process::FormatedProcess Process::getProcess()
{
    std::string pid_display = this->fp.pid;
    std::string user_name = ProcessParser::getProcUser(this->fp.pid);
    std::string cpu_usage = ProcessParser::getCpuPercent(this->fp.pid);
    std::string cmd_formated = ProcessParser::getCmd(this->fp.pid);;
    std::string mem_display = ProcessParser::getVmSize(this->fp.pid);

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
    upss << std::setprecision(2) << ProcessParser::getProcUpTime(this->fp.pid);

    Process::FormatedProcess fp_lp;
    fp_lp.cmd = cmd_formated;
    fp_lp.cpu = cpu_usage;
    fp_lp.mem = mem_display;


    fp_lp.pid = this->fp.pid;
    fp_lp.upTime = Util::convertToTime(ProcessParser::getProcUpTime(this->fp.pid));
    fp_lp.user = user_name;

    return fp_lp;
}

float Process::cpuUsage()
{
    try{
        if(this->fp.cpu.empty())
            this->fp.cpu = (ProcessParser::getCpuPercent(this->fp.pid));
        
        
        return stof(this->fp.cpu);
    }catch(...){
        return 0;
    }
}
