#pragma once
#include "Signal.h"

namespace Core
{
    class SignalHandler
    {
    private:
        // current signals
        std::vector<Signal> msignals;

    public:
        SignalHandler() {}
        ~SignalHandler() {}

        ///
        /// @brief push the signal to the signal list waiting for game to check
        ///
        /// @param signal
        ///
        void sendSignal(Signal signal);

        ///
        /// @brief used by game loop. go through the signal List and call
        /// corresponding logic to check
        ///
        ///
        void check();
    };

}