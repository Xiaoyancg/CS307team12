#pragma once
#include "mySignal.h"
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>
namespace Core
{

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

        std::vector<Signal> getSignals()
        {
            std::vector<Signal *> r;
            for (auto signalpair : _signals)
            {
                r.push_back(&signalpair.second);
            }
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

        LogicManager(/* args */)
        {
        }
        ~LogicManager() {}
    };

}