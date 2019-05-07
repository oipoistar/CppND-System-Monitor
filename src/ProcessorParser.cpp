#include "ProcessParser.h"
#include <string>
#include "stat.h"
#include <sys/resource.h>

string ProcessParser::getCmd(string pid)
{
}

vector<string> ProcessParser::getPidList()
{
}

std::string ProcessParser::getVmSize(std::string pid)
{
    std::string filename = Path::basePath() + pid + Path::statusPath();
    //VmData:	   34052 kB
    regex r("VmData:\\s+(\\d+)\\s+");
    float dataSizeInKB = std::stof(Util::PullDataByRegex(r, filename));

    //Scale to MB
    return std::to_string(dataSizeInKB / float(1024));
}

std::string ProcessParser::getCpuPercent(string pid)
{
    std::string filename = Path::basePath() + pid + Path::statPath();
    ProcessStatusInformation psi = Util::ParseStatusFile(filename);

    float freq = sysconf(_SC_CLK_TCK);
    float uptime = psi.utime / freq;

    float total_time = psi.utime + psi.stime + psi.cutime + psi.cstime;
    
    float seconds = uptime - (psi.starttime/freq);
    float result = 100.0*((total_time/freq)/seconds);

    return to_string(result);
}

long int ProcessParser::getSysUpTime()
{
}

std::string ProcessParser::getProcUpTime(string pid)
{
    std::string filename = Path::basePath() + pid + Path::statPath();
    ProcessStatusInformation psi = Util::ParseStatusFile(filename);

    return to_string(float(psi.utime / sysconf(_SC_CLK_TCK)));
}

string ProcessParser::getProcUser(string pid)
{
}

vector<string> ProcessParser::getSysCpuPercent(string coreNumber /*= ""*/)
{
}

float ProcessParser::getSysRamPercent()
{
}

string ProcessParser::getSysKernelVersion()
{
}

int ProcessParser::getTotalThreads()
{
}

int ProcessParser::getTotalNumberOfProcesses()
{
}

int ProcessParser::getNumberOfRunningProcesses()
{
}

string ProcessParser::getOSName()
{
}

std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2)
{
}

bool ProcessParser::isPidExisting(string pid)
{
}
