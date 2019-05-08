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
    std::vector<std::string> getList();
};

void ProcessContainer::refreshList(){
    std::vector<std::string> pidList = ProcessParser::getPidList();
    
    this->_list.clear();
    this->_list.reserve(pidList.size());
    
    for( auto pid : pidList){
        this->_list.emplace_back(pid);
    }
}

std::string ProcessContainer::printList(){
    std::string result="";
    std::stringstream ss;

    for(auto p : _list){
        ss << p.getProcess();
    }

    return ss.str();
}

std::vector<std::string> ProcessContainer::getList(){
    std::vector<std::string> values;

    std::sort(_list.begin(), _list.end(),
        [](Process a, Process b){return a.cpuUsage() > b.cpuUsage();}
     );

    for(auto entry : _list){
        values.push_back(entry.getProcess());

        if(values.size() == 24)
            break;
    }

   return values;
}
