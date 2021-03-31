#include "Logic.h"
namespace Core
{
    class LogicManager
    {
    private:
        std::unordered_map<int, Signal> signalList;
        std::unordered_map<int, Script> scriptList;
        std::vector<Logic *> keyLogicList;

    public:
        LogicManager(/* args */);
        ~LogicManager();
    };

    LogicManager::LogicManager(/* args */)
    {
    }

    LogicManager::~LogicManager()
    {
    }

}