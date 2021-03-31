#include "Logic.h"
#include "PageManager.h"
namespace Core
{
    class LogicManager
    {
    private:
        // all user defined signals/scripts/logics
        // predefined signals are sent without string reference
        // IOW, when event or timer, send a signal with type before compile.
        // when send signal by user defined event, send a string then get the
        // signal by querying the list.
        std::unordered_map<std::string, Signal> msignals;
        std::unordered_map<std::string, Script> mscripts;
        std::unordered_map<std::string, Logic> mlogics;

        // current checking list of signals/ scripts/logics
        // use instance is because signals can be either predefined and user
        // defined. predefined signals has no copy in the msignals, can't pass
        // reference. User defined are in msignals, but can't be delete if
        // passing reference. so have to copy the instance.
        std::unordered_map<std::string, Signal> mcurrSignals;
        std::unordered_map<std::string, Logic *> mcurrKeyLogics;
        // should be multiple current script list for different execute level,
        // so that the script can be executed in order to avoid conflicts.
        // now use only one
        std::unordered_map<std::string, Script *> mcurrEntityScripts;

        PageManager *mpageManager_ptr;

    public:
        /// \brief send signal pointer to current signal list
        /// \param signal
        /// \return int 0 success
        /// \note use instance copy. Predefined signals have scope, user defined
        /// signals are stored in list, can't use pointer here because the
        /// signal can't be deleted. so only copy available
        int sendSignal(SignalType, Signal &signal_ref);

        /// \brief check the signal with corresponding logic list
        /// \param signal_ref
        void check(Signal &signal_ref);

        /// \brief traverse all current signal list
        void signalLoop();

        //int sendScript(Script *);
        int sendScript(ScriptType, Script *);
        /// \brief traverse the all current script list
        void scriptLoop();

        //* ------------------- ANCHOR CREATION ------------------ *//
        // for editor

        // rn only use predefined signal
        //int createSignal(std::string &signalName_ref,
        //                 std::string &LogicName_ref);

        /// \brief Create a specific Logic object and add it to mscripts
        ///
        /// \param logicName
        /// \param signalType
        /// \param scriptType
        /// \param scriptName
        /// \return int 0 success, 1 used name, 2 not found script
        int createKeyLogic(std::string &logicName_ref,
                           SignalType signalType,
                           ScriptType scriptType,
                           std::string &scriptName_ref,
                           SDL_Keycode key,
                           Uint32 keyType);

        // TODO find a better way
        /// \brief Create a Move Entities Constantly Script object and add it to
        /// mscripts
        /// \param scriptName_ref
        /// \param targetList
        /// \param movement
        /// \return int
        int createMoveEntitiesConstantlyScript(
            std::string &scriptName_ref,
            std::vector<std::string> targetList,
            std::string &targetPage_ref,
            glm::vec2 &movement_ref);

        LogicManager() {}
        LogicManager(PageManager *pageManager_ptr)
            : mpageManager_ptr(pageManager_ptr) {}
        ~LogicManager() {}
    };

}