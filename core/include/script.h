#pragma once
namespace Core
{
    class BaseScript
    {
    private:
        int _scriptID;

    public:
        int getScriptID() { return _scriptID; }
        void setScriptID(int scriptID) { _scriptID = scriptID; }
        BaseScript() {}
        ~BaseScript() {}
    };

}