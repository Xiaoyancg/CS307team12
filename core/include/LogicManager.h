#pragma once
#include <vector>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "Logic.h"
#include "Page.h"
namespace Core
{

    //* ---------------- ANCHOR LOGIC MANAGER ---------------- *//

    /// \brief the Logic Manager handle the storage and execution of signals,
    /// logics and scripts all in one go
    ///
    class LogicManager
    {
        //* ----------------------- SIGNAL ----------------------- *//
    private:
        /// \brief current signal list
        ///
        std::vector<Signal> _currSignalList;

        /// \brief the signal storage
        ///
        std::vector<Signal> _signalList;

    public:
        /// \brief send the signal to current signal list
        /// For system-defined signal
        ///
        void sendSignal(Signal signal);

        /// \brief send Signal to current signal list by id
        /// For script and initialize.
        ///
        /// \param signalId
        void sendSignal(int signalId);

        /// \brief remove signal from current signal List by id
        /// TODO: need more logic life span specification
        /// RN except signal, everything is permanent
        ///
        /// \param signalId
        void removeSignal(int signalId);

        /// \brief Create a default Signal object and add it to List
        /// *For editor, used in Game::createSignal()
        ///
        /// \return Signal*
        Signal *createSignal();

        /// \brief delete a signal from list by id
        /// *For editor, used in Game::deleteSignal(int), binding with delete
        /// button
        ///
        /// \param signalId
        void deleteSignal(int signalId);

        /// \brief Get the pointer of the signals storage
        ///
        /// \return std::vector<Signal> *
        std::vector<Signal> *getSignalList();

        /// \brief traverse the current signal list and call the corresponding
        /// logic
        ///
        void checkCurrSignalList();

        //* ------------------------ LOGIC ----------------------- *//
    private:
        std::vector<Logic *> _currKeyLogicList;
        std::vector<Logic *> _currCustomLogicList;
        std::vector<Logic> _logicList;

    public:
        /// \brief send logic to corresponding current list by id
        ///
        /// \param logicId
        void sendLogic(int logicId);

        /// \brief remove logic from all current list by id
        ///
        /// \param logicId
        void removeLogic(int logicId);

        /// \brief Create a default Logic object and add it to list
        /// *For editor, used in Game::createLogic() binding to create button in
        /// Logic editor
        ///
        /// \return Logic*
        Logic *createLogic();

        /// \brief delete a logic from List by id
        /// *For editor, used in Game::deleteLogic(int) binding to delete button
        /// in Logic editor.
        /// only used in editor, no need to check the curr list for nullptr
        ///
        /// \param logicId
        void deleteLogic(int logicId);

        /// \brief Get the pointer of the list containing all Logic object
        ///
        /// \return std::vector<Logic>*
        std::vector<Logic> *getLogicList();
        //* ----------------------- SCRIPT ----------------------- *//

    private:
        std::vector<Script *> _currEntityScriptList;
        std::vector<Script *> _currPageScriptList;
        std::vector<Script *> _currGameScriptList;
        std::vector<Script> _scriptList;

    public:
        /// \brief send script to corresponding current list by id
        ///
        /// \param scriptId
        void sendScript(int scriptId);

        /// \brief remove script from corresponding current list by id
        /// TODO: need to implement better way to add lifespan feature
        ///
        /// \param scriptId
        void removeScript(int scriptId);

        /// \brief Create a default Script object
        /// *For editor, used in Game::createScript(), binding with create
        /// button in script editor
        ///
        /// \param name
        /// \return Script*
        Script *createScript(std::string name);

        /// \brief delete a script from list by id
        ///
        /// \param scriptId
        void deleteScript(int scriptId);

        /// \brief Get the pointer of the Script List with all scripts
        ///
        /// \return std::vector<Script>*
        std::vector<Script> *getScriptList();

        /// \brief traverse and run all current script in the order
        ///
        void runCurrScriptList();

        /// \brief execution function for script type MoveConstantly
        ///
        /// \param script
        void runMoveConstantly(ScriptMoveConstantly script);

        /// \brief execution function for script type custom
        ///
        /// \param script
        void runCustom(ScriptCustom script);
        //* -------------------- LOGICMANAGER -------------------- *//
    private:
        std::vector<Page *> *_pageList;
        Page **_currPage;

    public:
        /// \brief load all signals, logics and scripts to the corresponding
        /// unordered_map
        ///
        /// \param js
        LogicManager parse(nlohmann::json root);
        LogicManager();
        LogicManager(std::vector<Page *> *pageList);
        ~LogicManager();
    };

}