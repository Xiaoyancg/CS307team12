#pragma once
#include <SDL.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
namespace Core
{
    union SignalUnion;
    class Signal;

    //* --------------------- SIGNAL TYPE -------------------- *//

    enum class SignalType
    {
        Custom = 0,
        Key = 1,
    };

    //* --------------- ANCHOR TYPES OF SIGNALS -------------- *//

    //* ----------------------- CUSTOM ----------------------- *//

    /// \brief custom signal contains only the target script ID List
    /// when sending custom signal, it means the user want to evoke the target
    /// script. There will be no criteria to check in logic.
    /// Should be the default signal in editor.
    ///
    class SignalCustom // : public BaseSignal
    {
    private:
        std::vector<int> _targetScriptList;

    public:
        std::vector<int> getTargetScriptList();
        void setTargetScriptList(std::vector<int> targetScriptList);
        static SignalCustom parse(nlohmann::json root);
        SignalCustom();
        SignalCustom(std::vector<int> targetScriptList);
        ~SignalCustom();
    };

    //* ------------------------- KEY ------------------------ *//

    /// \brief Key signals handles key event from SDL.
    /// Sends the key code and key type ( press / release ).
    /// System defined signal.
    ///
    class SignalKey
    {
    private:
        /// \brief SDL key code
        ///
        SDL_Keycode _key;

        /// \brief key type ( SDL_PRESSED or SDL_RELEASED )
        ///
        Uint32 _keyType;

    public:
        SignalKey();

        /// \brief Construct a new Key Signal object with event
        ///
        /// \param event
        SignalKey(SDL_KeyboardEvent event);

        ~SignalKey();
    };

    //* -------------------- SIGNAL UNION -------------------- *//

    union SignalUnion
    {
        SignalKey keySignal;
        SignalCustom customSignal;
        SignalUnion &operator=(const SignalUnion &other);
        SignalUnion();
        SignalUnion(const SignalUnion &);
        SignalUnion(SignalKey keySignal);
        SignalUnion(SignalCustom customSignal);
        ~SignalUnion();
    };

    //* -------------------- ANCHOR SIGNAL ------------------- *//

    /// \brief the signal union contains all kind of signal and id and type
    ///
    class Signal
    {
    private:
        int _signalId;
        SignalType _signalType;
        SignalUnion _signal;
        std::string _signalName;

        int getSignalId();
        void setSignalId(int signalId);
        SignalType getSignalType();
        void setSignalType(SignalType signalType);
        std::string getSignalName();
        void setSignalName(std::string signalName);
        SignalUnion getSignal();
        void setSignal(SignalUnion signal);

    public:
        /// \brief Parse function
        ///
        /// \param root
        /// \return Signal
        static Signal parse(nlohmann::json root);

        /// \brief update all information of Signal
        /// *For editor, binding with the update button in signal editor
        ///
        /// \param signalType
        /// \param ... id, name, rest are signal variable. List of different type:
        /// \param Custom: std::vector<int>
        ///
        void updateSignal(SignalType signalType, ...);

        /// \brief Construct a new Signal object by default.
        /// Id is -1, type is custom, name is "".
        /// *For editor, used in Game::createSignal() binding with create button
        /// in signal editor
        ///
        Signal();

        /// \brief Construct a new Signal object
        ///
        /// \param signalId
        /// \param signalType
        /// \param signal
        Signal(int signalId, SignalType signalType, std::string signalName,
               SignalUnion signal);

        Signal(const Signal &);
        ~Signal();
    };

}