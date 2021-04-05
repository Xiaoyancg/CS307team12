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
    private:
        /// \brief current signal list
        ///
        std::vector<Signal> _currSignals;

        /// \brief the signal storage
        ///
        std::vector<Signal> _signals;

    public:
        /// \brief send the signal to current signal list
        ///
        void sendSignal(Signal signal)
        {
            _currSignals.push_back(signal);
        }

        /// \brief Get the pointer of the signals storage
        ///
        /// \return std::vector<Signal> *
        std::vector<Signal> *getSignals()
        {
            return &_signals;
        }

        /// \brief traverse the current signal list and call the corresponding
        /// logic
        ///
        void checkSignals();

        /// \brief load all signals, logics and scripts to the corresponding
        /// unordered_map
        ///
        /// \param js
        LogicManager parse(nlohmann::json root);

        /////// \brief Create a Signal object
        ///////
        ////Signal *createSignal(int id, SignalType type, SignalUnion signal);

        /// \brief Create a Signal object for Editor
        /// Create a default Signal and add it to the signal List, then return
        /// the address of the stored Signal
        ///
        Signal *createSignal();

        LogicManager()
        {
        }
        ~LogicManager() {}
    };

}