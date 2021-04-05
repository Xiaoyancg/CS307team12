#pragma once
#include "mySignal.h"
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>
#include "Script.h"
#include <any>
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
        ///
        void sendSignal(Signal signal);

        void sendSignal(int signalId);

        /// \brief Create a Signal object for Editor
        /// Create a default Signal and add it to the signal List, then return
        /// the address of the stored Signal
        ///
        Signal *createSignal();

        /// \brief Get the pointer of the signals storage
        ///
        /// \return std::vector<Signal> *
        std::vector<Signal> *getSignals();

        /// \brief traverse the current signal list and call the corresponding
        /// logic
        ///
        void checkSignals();

        //* ------------------------ LOGIC ----------------------- *//
    private:
    public:
        //* ----------------------- SCRIPT ----------------------- *//

    private:
        std::vector<Script *> _currScriptList;
        std::vector<Script> _scriptList;

    public:
        Script *createScript();
        void sendScript(int scriptId);

        //* -------------------- LOGICMANAGER -------------------- *//

        /// \brief load all signals, logics and scripts to the corresponding
        /// unordered_map
        ///
        /// \param js
        LogicManager parse(nlohmann::json root);

        LogicManager();
        ~LogicManager();
    };

}