#include "ProcessParser.h"
#include <string>
#include "stat.h"
#include <sys/resource.h>
#include <pwd.h>
#include <boost/filesystem.hpp>
#include <regex>

string ProcessParser::getCmd(string pid)
{
}

vector<string> ProcessParser::getPidList()
{
    std::string path = Path::basePath();
    std::vector<std::string> pids;

    pids.reserve(100);
    
    regex r("^[0-9]+$");
    std::smatch m;

    for(auto & p : boost::filesystem::directory_iterator( path )){

        if(boost::filesystem::is_directory(p)){
            std::string directory_name = p.path().filename().string();

            if (regex_search(directory_name, m, r))
            {
                pids.emplace_back(directory_name);
            }
        }
    }

    return pids;
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
    std::string filename = Path::basePath() + Path::upTimePath();
    std::ifstream stream = Util::getStream(filename);
    std::string line;

    if(std::getline(stream, line)){
        std::vector<std::string> results;
        boost::split(results, line, [](char c) { return c == ' '; });

        return stoi(results[0]);
    }
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
    ifstream stream = Util::getStream(filename);
    std::string line;

    while(std::getline(stream, line)){
        if(boost::starts_with(line, "Uid:")){
            vector<std::string> results;
            boost::split(results, line, [](char c) { return c == '\t'; });

            int uid = stoi(results[1]);
            passwd* pwd = getpwuid(uid);
            return string(pwd->pw_name);
        }
    }

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
