#pragma once
#include "mySignal.h"
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>
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
        std::unordered_map<int, Signal> _currSignals;

        /// \brief the signal storage
        ///
        std::unordered_map<int, Signal> _signals;

    public:
        /// \brief send the signal to current signal list
        ///
        void sendSignal(Signal);

        /// \brief traverse the current signal list and call the corresponding
        /// logic
        ///
        void checkSignals();

        /// \brief Get all Signal pointers in vector
        ///
        /// \return std::vector<Signal *>
        std::vector<Signal *> getSignals()
        {
            std::vector<Signal *> r;
            for (auto &signalpair : _signals)
            {
                r.push_back(&signalpair.second);
            }
            return r;
        }

        /// \brief load all signals, logics and scripts to the corresponding
        /// unordered_map
        ///
        /// \param js
        void
        parse(nlohmann::json js);

        /// \brief Create a Signal object
        /// TODO add api
        void createSignal();

        LogicManager()
        {
        }
        ~LogicManager() {}
    };

}