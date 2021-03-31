#include "Logic.h"
#include "PageManager.h"
namespace Core
{
    class LogicManager
    {
    private:
        // all signals/scripts/logics
        std::unordered_map<std::string, Signal> msignals;
        std::unordered_map<std::string, Script> mscripts;
        std::unordered_map<std::string, Logic> mlogics;

        // current checking list of signals/ scripts/logics
        std::unordered_map<std::string, Signal> mcurrSignals;
        std::unordered_map<std::string, Logic> mcurrLogics;
        // should be multiple current script list for different execute level,
        // so that the script can be executed in order to avoid conflicts.
        // now use only one
        std::unordered_map<std::string, Script> mcurrScripts;

        PageManager mpageManager;

    public:
        void sendSignal(Signal signal);
        void check();
        LogicManager() {}
        LogicManager(PageManager &pageManager) : mpageManager(pageManager) {}
        ~LogicManager() {}
    };

}