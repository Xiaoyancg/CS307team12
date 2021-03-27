#pragma once
#include "signal.h"

namespace Core
{
    // forwar declaration
    enum class EventType;
    class Signal;

    // *classes list
    class KeyLogic;

    //* --------------------------- different response -------------------------- */
    class KeyLogic
    {
    private:
        // based on SDL
        // key code
        SDL_KeyCode mkey;
        // press or release
        Uint32 mtype;

    public:
        void setKey(SDL_KeyCode key) { mkey = key; }
        SDL_KeyCode getKey() { return mkey; }
        void setType(Uint32 type) { mtype = type; }
        Uint32 getType() { return mtype; }

        // check for KeyLogic
        // singal is KeySignal
        bool check(Core::Signal signal);
        KeyLogic() {}
        ~KeyLogic() {}
    };

    typedef std::variant<KeyLogic> ResponseVariant;
    //* ----------------------------- Entry Response ----------------------------- */

    // should be used in the logic list waiting for signals
    // e.g, for each (signal in signalList) {
    //          switch (signal.type) {
    //              case key:
    //                  for each (logic in inputLogicList){
    //                      logic.check(signal);
    //                  }
    //                  break;
    //          }
    //      }
    //
    class Logic
    {
    private:
        EventType mtype;

        // name starts with m for member
        union
        {
            Core::KeyLogic mkr;
            // add more here
        };
        //bool mready = false;

    public:
        EventType getType() { return mtype; }
        void setType(EventType type) { mtype = type; }
        //bool isReady() { return mready; }
        //void setReady(bool ready) { mready = ready; }
        // must call after settype

        bool check(Signal signal);
        Logic() {}
        ~Logic() {}
    };

}