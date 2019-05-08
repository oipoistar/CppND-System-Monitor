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
    std::vector<std::vector<std::string>> getList();
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

std::vector<std::vector<std::string> > ProcessContainer::getList(){
    std::vector<std::vector<std::string>> values;
    std::vector<std::string> stringifiedList;
    for(int i=0; i<ProcessContainer::_list.size(); i++){
        stringifiedList.push_back(ProcessContainer::_list[i].getProcess());
    }
    int lastIndex = 0;
    for (int i=0; i<stringifiedList.size();i++){
        if(i %10 == 0 && i > 0){
          std::vector<std::string>  sub(&stringifiedList[i-10], &stringifiedList[i]);
          values.push_back(sub);
          lastIndex = i;
        }
        if(i == (ProcessContainer::_list.size() - 1) && (i-lastIndex)<10){
            std::vector<std::string> sub(&stringifiedList[lastIndex],&stringifiedList[i+1]);
            values.push_back(sub);
        }
   }

   return values;
}
