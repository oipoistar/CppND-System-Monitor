#include "util.h"
#include <iostream>

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
std::string Util::getProgressBar(std::string percent)
{
    std::string result = "0% ";
    int _size = 50;
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

    result += " " + percent.substr(0, 5) + " /100%";
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

ProcessStatusInformation Util::ParseStatusFile(std::string file)
{
    std::ifstream stream = Util::getStream(file);
    std::string line;
    std::vector<std::string> results;
    ProcessStatusInformation psi;

    if (std::getline(stream, line))
    {
        boost::split(results, line, [](char c) { return c == ' '; });

        int i = 0;
        psi.pid = std::stoi(results[i++]);
        psi.comm = results[i++];
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
    }

    return psi;
}