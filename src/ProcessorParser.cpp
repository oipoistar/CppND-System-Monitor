#include "ProcessParser.h"
#include <string>
#include "ProcStat.h"
#include <sys/resource.h>
#include <pwd.h>
#include <numeric>
#include <regex>
#include <filesystem>

std::map<std::string, ProcessStatusInformation> ProcessParser::pid_map;
std::vector<std::string> ProcessParser::pid_list;

string ProcessParser::getCmd(string pid)
{
    string line;
    ifstream stream = Util::getStream((Path::basePath() + pid + Path::cmdPath()));
    std::getline(stream, line);
    return line;
}

vector<string> ProcessParser::getPidList()
{
    std::string path = Path::basePath();
    std::vector<std::string> pids;
    int procnum = getTotalNumberOfProcesses();
    pids.reserve(procnum);

    for (auto &p : std::filesystem::directory_iterator(path))
    {

        if (std::filesystem::is_directory(p))
        {
            std::string directory_name = p.path().filename().string();

            if(!directory_name.empty() && std::find_if(directory_name.begin(), directory_name.end(),
             [](char c) { return !std::isdigit(c); }) == directory_name.end()){
                pids.emplace_back(directory_name);
            }
        }
    }
    
    pid_list = pids;
    return pids;
}

std::string ProcessParser::getVmSize(std::string pid)
{
    std::string filename = Path::basePath() + pid + Path::statusPath();
    //VmData:	   34052 kB
    regex r("VmData:\\s+(\\d+)\\s+");
    std::string data = Util::PullDataByRegex(r, filename);

    if (data.empty())
    {
        return data;
    }

    float dataSizeInKB = std::stof(data);

    //Scale to MB
    return std::to_string(dataSizeInKB / float(1024));
}

std::string ProcessParser::getCpuPercent(string pid)
{
    std::string filename = Path::basePath() + pid + Path::statPath();
    ProcessStatusInformation psi = Util::ParseStatusFile(filename);

    if (ProcessParser::pid_map.find(pid) == ProcessParser::pid_map.end())
    {
        ProcessParser::pid_map.emplace(pid, psi);
        return "0";
    }
    else
    {
        float utime_after = psi.utime;
        float utime_before = ProcessParser::pid_map[pid].utime;

        float stime_after = psi.stime;
        float stime_before = ProcessParser::pid_map[pid].stime;

        float time_total_after = psi.total_time;
        float time_total_before = ProcessParser::pid_map[pid].total_time;

        float user_util = 100 * (utime_after - utime_before) / (time_total_after - time_total_before);
        float sys_util = 100 * (stime_after - stime_before) / (time_total_after - time_total_before);

        ProcessParser::pid_map[pid] = psi;

        int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
        return std::to_string((user_util + sys_util) * cpu_num);
    }
}

long int ProcessParser::getSysUpTime()
{
    std::string filename = Path::basePath() + Path::upTimePath();
    std::ifstream stream = Util::getStream(filename);
    std::string line;

    if (std::getline(stream, line))
    {
        std::vector<std::string> results;
        auto vec = Util::split(line, ' ');

        return stoi(vec[0]);
    }

    return 0L;
}

std::string ProcessParser::getProcUpTime(string pid)
{
    std::string filename = Path::basePath() + pid + Path::statPath();
    ProcessStatusInformation psi = Util::ParseStatusFile(filename);

    return to_string(float(psi.utime / sysconf(_SC_CLK_TCK)));
}

string ProcessParser::getProcUser(string pid)
{
    std::string filename = Path::basePath() + pid + Path::statusPath();
    int uid = stoi(Util::GetValuesFromFile(filename, "Uid:", '\t'));
    passwd *pwd = getpwuid(uid);
    return string(pwd->pw_name);
}

vector<string> ProcessParser::parseProcStatFile(string coreNumber /*= ""*/)
{
    std::string line;
    std::string cpuname = "cpu" + coreNumber + " ";
    vector<std::string> results;

    ifstream stream = Util::getStream(Path::basePath() + "stat");

    while (std::getline(stream, line))
    {
        if (Util::startsWith(line, cpuname))
        {
            regex reg("\\s\\s");
            line = regex_replace(line, reg, " ");

            results = Util::split(line,' ');
        }
    }

    return results;
}

float ProcessParser::getSysRamPercent()
{
    std::string filename = Path::basePath() + Path::memInfoPath();
    ifstream stream = Util::getStream(filename);

    const string MEM_AVAILABLE_STR = "MemAvailable:";
    const string MEM_FREE_STR = "MemFree:";
    const string MEM_BUFERS_STR = "Buffers:";

    float total_mem = 0;
    float free_mem = 0;
    float buffers = 0;

    std::string line;
    regex r("\\s+(\\d+)\\s\\D\\D");
    std::smatch m;

    while (std::getline(stream, line))
    {
        if (Util::startsWith(line, MEM_AVAILABLE_STR))
        {
            if (regex_search(line, m, r))
            {
                total_mem = stof(m.str(1));
            }
        }

        if (Util::startsWith(line, MEM_FREE_STR))
        {
            if (regex_search(line, m, r))
            {
                free_mem = stof(m.str(1));
            }
        }

        if (Util::startsWith(line, MEM_BUFERS_STR))
        {
            if (regex_search(line, m, r))
            {
                buffers = stof(m.str(1));
            }
        }
    }

    return float(100.0 * (1 - (free_mem / (total_mem - buffers))));
}

string ProcessParser::getSysKernelVersion()
{
    //Linux version 5.0.0-13-generic
    std::string filename = Path::basePath() + Path::versionPath();
    ifstream stream = Util::getStream(filename);

    std::string line;

    if (getline(stream, line))
    {
        regex r("Linux\\sversion\\s(\\S+)\\s");
        std::smatch m;

        if (regex_search(line, m, r))
        {
            return m.str(1);
        }
    }

    return "";
}

int ProcessParser::getTotalThreads()
{
    if(pid_list.empty())
        getPidList();

    return std::accumulate(pid_list.begin(), pid_list.end(),0, [](int t, std::string pid){
            std::string filename = Path::basePath() + pid + Path::statusPath();
            try{
                return t + stoi(Util::GetValuesFromFile(filename, "Threads:", '\t'));
            }catch(...){return t;}
        });
}

int ProcessParser::getTotalNumberOfProcesses()
{
    std::string filename = Path::basePath() + Path::statPath();
    return stoi(Util::GetValuesFromFile(filename, "processes", ' '));
}

int ProcessParser::getNumberOfRunningProcesses()
{
    std::string filename = Path::basePath() + Path::statPath();
    return stoi(Util::GetValuesFromFile(filename, "procs_running", ' '));
}

string ProcessParser::getOSName()
{
    std::string filename = "/etc/os-release";
    ifstream stream = Util::getStream(filename);

    std::string line;

    while (getline(stream, line))
    {

        if (Util::startsWith(line, "PRETTY_NAME"))
        {
            //PRETTY_NAME="Ubuntu 19.04"
            regex r("PRETTY_NAME=\"(.+)\"");
            std::smatch m;

            if (regex_search(line, m, r))
            {
                return m.str(1);
            }
        }
    }

    return "";
}

float get_sys_active_cpu_time(std::vector<std::string> values)
{
    return (stof(values[S_USER]) +
            stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) +
            stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL]) +
            stof(values[S_GUEST]) +
            stof(values[S_GUEST_NICE]));
}

float get_sys_idle_cpu_time(vector<string> values)
{
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}

std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2)
{
    float active_time = get_sys_active_cpu_time(values2) - get_sys_active_cpu_time(values1);
    float idle_time = get_sys_idle_cpu_time(values2) - get_sys_idle_cpu_time(values1);
    float total_time = active_time + idle_time;
    float result = 100.0 * (active_time / total_time);
    return to_string(result);
}

bool ProcessParser::isPidExisting(string pid)
{
    if(pid_list.empty())
        getPidList();
    
    if (std::find(pid_list.begin(), pid_list.end(), pid) != pid_list.end())
    {
        return true;
    }

    return false;
}
