#include "Process.h"
#include <vector>
class ProcessContainer{

private:
    std::vector<Process>_list;
public:
    ProcessContainer(){
        this->refreshList();
    }
    void refreshList();
    std::string printList();
    std::vector<Process::FormatedProcess> getList();
};

void ProcessContainer::refreshList(){
    std::vector<std::string> pidList = ProcessParser::getPidList();
    
    this->_list.clear();
    this->_list.reserve(pidList.size());
    
    for( auto &pid : pidList){
        try{
            this->_list.emplace_back(pid);
        }catch(...){
            //cout << "Removed PID: " << pid;
        }
    }
}

std::string ProcessContainer::printList(){
    std::string result="";
    std::stringstream ss;

    for(auto &p : _list){
        ss << p.getProcess().cmd;
    }

    return ss.str();
}

std::vector<Process::FormatedProcess> ProcessContainer::getList(){
    std::vector<Process::FormatedProcess> values;
    values.reserve(25);

    std::sort(_list.begin(), _list.end(),
        [](Process a, Process b){return a.cpuUsage() > b.cpuUsage();}
     );

    for(auto &entry : _list){
        values.push_back(entry.getProcess());

        if(values.size() == 24)
            break;
    }

   return values;
}
