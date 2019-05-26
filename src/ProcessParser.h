#pragma once

#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <map>
#include "constants.h"
#include "util.h"
#include "ProcStat.h"

using namespace std;

class ProcessParser
{
private:
    std::ifstream stream;

public:
    static std::map<std::string, ProcessStatusInformation> pid_map;
    static vector<string> pid_list;

    static string getCmd(string pid);
    static vector<string> getPidList();
    static float getVmSize(std::string pid);
    static int getCpuPercent(string pid);
    static long int getSysUpTime();
    static long int getProcUpTime(string pid);
    static string getProcUser(string pid);
    static vector<string> parseProcStatFile(string coreNumber = "");
    static float getSysRamPercent();
    static string getSysKernelVersion();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static string getOSName();
    static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2);
    static bool isPidExisting(string pid);
};

// TODO: Define all of the above functions below:
