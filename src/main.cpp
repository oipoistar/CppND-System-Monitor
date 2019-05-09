#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <ncurses.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include "util.h"
#include "SysInfo.h"
#include "ProcessContainer.h"
#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"


using namespace std;

char *getCString(std::string str)
{
    char *cstr = new char[str.length() + 1];
    std::strcpy(cstr, str.c_str());
    return cstr;
}

void writeSysInfoToConsole(SysInfo sys, WINDOW *sys_win)
{
    sys.setAttributes();

    mvwprintw(sys_win, 2, 2, getCString(("OS: " + sys.getOSName())));
    mvwprintw(sys_win, 3, 2, getCString(("Kernel version: " + sys.getKernelVersion())));
    mvwprintw(sys_win, 4, 2, getCString(("Memory: ")));
    wattron(sys_win, COLOR_PAIR(1));
    wprintw(sys_win, getCString(Util::getProgressBar(sys.getMemPercent())));
    wattroff(sys_win, COLOR_PAIR(1));
    mvwprintw(sys_win, 5, 2, getCString(("Total Processes:" + sys.getTotalProc())));
    mvwprintw(sys_win, 6, 2, getCString(("Running Processes:" + sys.getRunningProc())));
    mvwprintw(sys_win, 7, 2, getCString(("Up Time: " + Util::convertToTime(sys.getUpTime()))));
    wrefresh(sys_win);
}

void writeCPUInfoToConsole(SysInfo sys, WINDOW *sys_win)
{
    mvwprintw(sys_win, 2, 2, getCString("CPU: "));
    wattron(sys_win, COLOR_PAIR(1));
    wprintw(sys_win, getCString(Util::getProgressBar(sys.getCpuPercent())));
    wattroff(sys_win, COLOR_PAIR(1));
    mvwprintw(sys_win, 3, 2, getCString(("Other cores:")));
    wattron(sys_win, COLOR_PAIR(1));
    std::vector<std::string> val = sys.getCoresStats();
    for (int i = 0; i < val.size(); i++)
    {
        mvwprintw(sys_win, (4 + i), 2, getCString(val[i]));
    }
    wattroff(sys_win, COLOR_PAIR(1));
}

void getProcessListToConsole(std::vector<Process::FormatedProcess> processes, WINDOW *win)
{

    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 1, 2, "PID:");
    mvwprintw(win, 1, 9, "User:");
    mvwprintw(win, 1, 20, "CPU[%%]:");
    mvwprintw(win, 1, 35, "RAM[MB]:");
    mvwprintw(win, 1, 45, "Uptime:");
    mvwprintw(win, 1, 55, "CMD:");
    wattroff(win, COLOR_PAIR(2));
    for (int i = 0; i < processes.size(); i++)
    {
        mvwprintw(win, 2 + i, 2, getCString(processes[i].pid));
        mvwprintw(win, 2 + i, 9, getCString(processes[i].user));
        mvwprintw(win, 2 + i, 20, getCString(processes[i].cpu));
        mvwprintw(win, 2 + i, 35, getCString(processes[i].mem));
        mvwprintw(win, 2 + i, 45, getCString(processes[i].upTime));
        mvwprintw(win, 2 + i, 55, getCString(processes[i].cmd));
    }
}

void printMain(SysInfo sys, ProcessContainer procs)
{
    initscr();     /* Start curses mode 		  */
    noecho();      // not printing input values
    cbreak();      // Terminating on classic ctrl + c
    start_color(); // Enabling color change of text
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax); // getting size of window measured in lines and columns(column one char length)
    WINDOW *sys_win = newwin(17, ((int)(xMax / 2) - 1), 0, 0);
    WINDOW *sys_overview = newwin(17, ((int)(xMax / 2) - 1), 0, ((int)xMax / 2));
    WINDOW *proc_win = newwin(28, xMax - 1, 18, 0);
    
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    while (1)
    {
        box(sys_win, 0, 0);
        wCenterTitle(sys_win, "System Overview");
        box(proc_win, 0, 0);
        box(sys_overview, 0, 0);
        procs.refreshList();
        std::vector<Process::FormatedProcess> processes = procs.getList();
        writeSysInfoToConsole(sys, sys_win);
        writeCPUInfoToConsole(sys, sys_overview);
        getProcessListToConsole(processes, proc_win);
        wrefresh(sys_win);
        wrefresh(proc_win);
        wrefresh(sys_overview);
        refresh();
        sleep(1);
    }

    endwin();
}


int main(int argc, char *argv[])
{
    loguru::g_stderr_verbosity = loguru::Verbosity_OFF;

    loguru::init(argc, argv);
    loguru::add_file("output.log", loguru::Append, loguru::Verbosity_MAX);
    
    loguru::set_fatal_handler([](const loguru::Message& message){
	    throw std::runtime_error(std::string(message.prefix) + message.message);
    });

    //Object which contains list of current processes, Container for Process Class
    ProcessContainer procs;
    // Object which containts relevant methods and attributes regarding system details
    SysInfo sys;

    printMain(sys, procs);
    return 0;
}
