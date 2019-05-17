#pragma once

#include <string>
#include <fstream>
#include <regex>

#include "ProcStat.h"
// Classic helper functions
class Util
{
public:
    static std::string convertToTime(long int input_seconds);
    static std::string getProgressBar(std::string percent, int width);
    static std::ifstream getStream(std::string path);
    static std::string PullDataByRegex(std::regex r, std::string file);
    static ProcessStatusInformation ParseStatusFile(std::string file);
    static std::string GetValuesFromFile(std::string filename, std::string value, char separator);
    static std::string getProgressBarShortened(std::string id, std::string percent, int width);
    static bool startsWith(std::string& str,const std::string& part);
    static std::vector<std::string> split(const std::string& to_separate, char separator);
    static bool isAllDigit(std::string& checkifdigit);
};

