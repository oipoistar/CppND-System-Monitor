#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <ncurses.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "util.h"
#include "SysInfo.h"
#include "ProcessContainer.h"



using namespace std;

char *getCString(std::string str)
{
    char *cstr = new char[str.length() + 1];
    std::strcpy(cstr, str.c_str());
    return cstr;
}

std::string getFormatedStringBySize(std::string str, int currentPos, int xMax){
    size_t available_size = xMax - currentPos - 2;

    if(str.size() > available_size)
        return str.substr(0, available_size);

    return str;
}

void decideColorShemeOnPercentage(const std::string& percentage, WINDOW* win){
    float perc = stof(percentage);

    if(perc > 30 && perc < 60){
        wattron(win, COLOR_PAIR(4));
    }else if(perc > 59){
        wattron(win, COLOR_PAIR(5));
    }else{
        wattron(win, COLOR_PAIR(3));
    }
}


void writeSysInfoToConsole(SysInfo& sys, WINDOW *sys_win, int width)
{
    sys.setAttributes();

    mvwprintw(sys_win, 1, 2, getCString(("OS: ")));
    mvwprintw(sys_win, 1, 20, getCString((sys.getOSName())));

    mvwprintw(sys_win, 2, 2, getCString(("Kernel: ")));
    mvwprintw(sys_win, 2, 20, getCString((sys.getKernelVersion())));
    
    mvwprintw(sys_win, 3, 2, getCString(("Total Processes:")));
    mvwprintw(sys_win, 3, 20, getCString((sys.getTotalProc())));

    mvwprintw(sys_win, 4, 2, getCString(("Running Processes:")));
    mvwprintw(sys_win, 4, 20, getCString((sys.getRunningProc())));

    mvwprintw(sys_win, 5, 2, getCString(("Up Time:")));
    mvwprintw(sys_win, 5, 20, getCString((Util::convertToTime(sys.getUpTime()))));

    std::string percentage = sys.getMemPercent();

    mvwprintw(sys_win, 6, 2, getCString(("Memory: ")));
    decideColorShemeOnPercentage(percentage, sys_win);
    wprintw(sys_win, getCString(Util::getProgressBar(percentage, (width - std::string("Memory: ").size() - 15) )));
    wattroff(sys_win, COLOR_PAIR(1)); 
    wrefresh(sys_win);
}

void writeCPUInfoToConsole(SysInfo& sys, WINDOW *sys_win, int width)
{
    mvwprintw(sys_win, 1, 2, getCString("CPU: "));
    std::string cpu_percentage = sys.getCpuPercent();
    decideColorShemeOnPercentage(cpu_percentage, sys_win);
    wprintw(sys_win, getCString(Util::getProgressBar(cpu_percentage, (width - std::string("CPU: ").size() - 15))));
    wattroff(sys_win, COLOR_PAIR(1));
    mvwprintw(sys_win, 2, 2, getCString(("Most utilized cores:")));
    wattron(sys_win, COLOR_PAIR(1));
    
    std::vector<std::string> val = sys.getCoresStats();
    for (size_t i = 0; i < val.size(); i++)
    {
        mvwprintw(sys_win, (3 + i), 2, getCString(val[i]));
    }
    wattroff(sys_win, COLOR_PAIR(1));
}

void getProcessListToConsole(std::vector<Process::FormatedProcess>& processes, WINDOW *win, size_t max_entries, int maxX)
{

    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 1, 2, "PID:");
    mvwprintw(win, 1, 9, "User:");
    mvwprintw(win, 1, 20, "CPU[%%]:");
    mvwprintw(win, 1, 30, "RAM[MB]:");
    mvwprintw(win, 1, 40, "Uptime:");
    mvwprintw(win, 1, 50, "CMD:");

    wattroff(win, COLOR_PAIR(2));
    for (size_t i = 0; i < processes.size(); i++)
    {
        mvwprintw(win, 2 + i, 2, getCString(processes[i].pid));
        mvwprintw(win, 2 + i, 9, getCString(processes[i].user));
        mvwprintw(win, 2 + i, 20, getCString(processes[i].cpu));
        mvwprintw(win, 2 + i, 30, getCString(processes[i].mem));
        mvwprintw(win, 2 + i, 40, getCString(processes[i].upTime));
        mvwprintw(win, 2 + i, 50, getCString(getFormatedStringBySize(processes[i].cmd, 50, maxX)));
    }
}

[[noreturn]] void printMain(SysInfo& sys, ProcessContainer& procs)
{
    initscr();     /* Start curses mode 		  */
    noecho();      // not printing input values
    cbreak();      // Terminating on classic ctrl + c
    start_color(); // Enabling color change of text
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax); // getting size of window measured in lines and columns(column one char length)
    int half_screen_x = ((int)(xMax / 2) - 1);
    WINDOW *sys_win = newwin(8, half_screen_x, 0, 0);
    WINDOW *sys_overview = newwin(8, half_screen_x, 0, ((int)xMax / 2));
    WINDOW *proc_win = newwin(yMax - 8, xMax - 1, 8, 0);
    
    size_t max_processes = yMax - 11;

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_YELLOW);
    init_pair(5, COLOR_WHITE, COLOR_RED);

    while (1)
    {
        box(sys_win, 0, 0);
        box(proc_win, 0, 0);
        box(sys_overview, 0, 0);
        std::vector<Process::FormatedProcess> processes = procs.getList(max_processes);
        writeSysInfoToConsole(sys, sys_win, half_screen_x);
        writeCPUInfoToConsole(sys, sys_overview, half_screen_x);
        getProcessListToConsole(processes, proc_win, max_processes,xMax);
        wrefresh(sys_win);
        wrefresh(proc_win);
        wrefresh(sys_overview);
        refresh();
        sleep(1);
        procs.refreshList();
    }

    endwin();
}


int main(int argc, char *argv[])
{
    ProcessContainer procs;
    SysInfo sys;

    printMain(sys, procs);
    return 0;
}
