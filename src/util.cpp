#include "util.h"
#include <iostream>
#include <numeric>
#include "ProcessParser.h"

std::string Util::convertToTime(long int input_seconds)
{
    long minutes = input_seconds / 60;
    long hours = minutes / 60;
    long seconds = int(input_seconds % 60);
    minutes = int(minutes % 60);

    
    std::string result = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    return result;
}

// constructing string for given percentage
// 50 bars is uniformly streched 0 - 100 %
// meaning: every 2% is one bar(|)
std::string Util::getProgressBar(std::string percent, int width)
{
    std::string result = "0% [";
    int _size = width;
    int boundaries = (stof(percent) / 100) * _size;

    for (int i = 0; i < _size; i++)
    {
        if (i <= boundaries)
        {
            result += "|";
        }
        else
        {
            result += " ";
        }
    }

    result += "] 100%";
    return result;
}

std::string Util::getProgressBarShortened(std::string id, std::string percent, int width)
{
    if(id.size() < 2)
        id = " " + id;

    std::string result = id + "[";
    int _size = width;
    int boundaries = (stof(percent) / 100) * _size;

    for (int i = 0; i < _size; i++)
    {
        if (i <= boundaries)
        {
            result += "|";
        }
        else
        {
            result += " ";
        }
    }

    result += "]";
    return result;
}

// wrapper for creating streams
std::ifstream Util::getStream(std::string path)
{
    std::ifstream stream(path);
    if (!stream)
    {
        throw std::runtime_error("Non - existing PID");
    }
    return stream;
}

std::string Util::PullDataByRegex(std::regex r, std::string file)
{
    std::ifstream stream = Util::getStream(file);
    std::string line;
    std::smatch m;

    while (std::getline(stream, line))
    {
        if (regex_search(line, m, r))
        {
            return m.str(1);
        }
    }

    return std::string();
}

std::string Util::GetValuesFromFile(std::string filename, std::string value, char separator)
{
    std::ifstream stream = Util::getStream(filename);
    std::string line;

    while (getline(stream, line))
    {
        if (Util::startsWith(line, value))
        {
            
            auto results = Util::split(line, separator);
            //expecting name value or name: value
            return results[1];
        }
    }

    return std::string();
}

ProcessStatusInformation Util::ParseStatusFile(std::string file)
{
    std::ifstream stream = Util::getStream(file);
    std::string line;
    std::vector<std::string> results;
    ProcessStatusInformation psi;

    if (std::getline(stream, line))
    {
        results = Util::split(line,' ');

        // comm section contains whitespace so lets join it
        if (results.size() > 52)
        {
            int diff = results.size() - 52;

            std::vector<std::string>::iterator begin;
            std::vector<std::string>::iterator end;
            begin = results.begin() + 1;
            end = begin + diff;

            std::string comm;
            for (int i = 1; i <= (diff + 1); i++)
            {
                comm.append(results[i]);

                if (i < (diff + 1))
                    comm.append(" ");
            }

            psi.comm = comm;
            results.erase(begin, end + 1);
        }
        else
        {
            psi.comm = results[1];

            std::vector<std::string>::iterator begin;
            begin = results.begin() + 1;

            results.erase(begin);
        }

        int i = 0;
        psi.pid = std::stoi(results[i++]);

        psi.state = results[i++].c_str()[0];

        psi.ppid = std::stoi(results[i++]);
        psi.pgrp = std::stoi(results[i++]);
        psi.session = std::stoi(results[i++]);
        psi.tty_nr = std::stoi(results[i++]);
        psi.tpgid = std::stoi(results[i++]);

        psi.flags = std::stoul(results[i++]);
        psi.minflt = std::stoul(results[i++]);
        psi.cminflt = std::stoul(results[i++]);
        psi.majflt = std::stoul(results[i++]);
        psi.cmajflt = std::stoul(results[i++]);
        psi.utime = std::stoul(results[i++]);
        psi.stime = std::stoul(results[i++]);

        psi.cutime = std::stol(results[i++]);
        psi.cstime = std::stol(results[i++]);
        psi.priority = std::stol(results[i++]);
        psi.nice = std::stol(results[i++]);
        psi.num_threads = std::stol(results[i++]);
        psi.itrealvalue = std::stol(results[i++]);

        psi.starttime = std::stoull(results[i++]);

        psi.vsize = std::stoul(results[i++]);

        psi.rss = std::stol(results[i++]);

        psi.rsslim = std::stoul(results[i++]);
        psi.startcode = std::stoul(results[i++]);
        psi.endcode = std::stoul(results[i++]);
        psi.startstack = std::stoul(results[i++]);
        psi.kstkesp = std::stoul(results[i++]);
        psi.kstkeip = std::stoul(results[i++]);
        psi.signal = std::stoul(results[i++]);
        psi.blocked = std::stoul(results[i++]);
        psi.sigignore = std::stoul(results[i++]);
        psi.sigcatch = std::stoul(results[i++]);
        psi.wchan = std::stoul(results[i++]);
        psi.nswap = std::stoul(results[i++]);
        psi.cnswap = std::stoul(results[i++]);

        psi.exit_signal = std::stoi(results[i++]);
        psi.processor = std::stoi(results[i++]);

        psi.rt_priority = std::stoul(results[i++]);
        psi.policy = std::stoul(results[i++]);

        psi.delayacct_blkio_ticks = std::stoull(results[i++]);

        psi.guest_time = std::stoul(results[i++]);

        psi.cguest_time = std::stol(results[i++]);

        psi.start_data = std::stoul(results[i++]);
        psi.end_data = std::stoul(results[i++]);
        psi.start_brk = std::stoul(results[i++]);
        psi.arg_start = std::stoul(results[i++]);
        psi.arg_end = std::stoul(results[i++]);
        psi.env_start = std::stoul(results[i++]);
        psi.env_end = std::stoul(results[i++]);

        psi.exit_code = std::stoi(results[i++]);

        std::vector<std::string> cpu_list = ProcessParser::parseProcStatFile();
        cpu_list.erase(cpu_list.begin());

        psi.total_time = std::accumulate(cpu_list.begin(), cpu_list.end(),0, [](int t, std::string cpu_str){
            return t + stof(cpu_str);
        });
    }

    return psi;
}

bool Util::startsWith(std::string& str,const std::string& part){
    if(str.rfind(part, 0) == 0)
        return true;

    return false;
}

std::vector<std::string> Util::split(const std::string& s, char delimiter)
{
    std::vector<std::string> result;

    std::size_t current = 0;
    std::size_t p = s.find_first_of(delimiter, 0);

    while (p != std::string::npos)
    {
        result.emplace_back(s, current, p - current);
        current = p + 1;
        p = s.find_first_of(delimiter, current);
    }

    result.emplace_back(s, current);

    return result;
}

bool Util::isAllDigit(std::string& checkifdigit){
    for(auto c : checkifdigit)
        if(!std::isdigit(c))
            return false;
    
    return true;
}
