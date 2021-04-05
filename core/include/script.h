#pragma once
#include <vector>
#include <glm/glm.hpp>
namespace Core
{
    enum class ScriptType
    {
        MoveEntityConstantly,  // 0
        MoveEntitiesConstantly // 1
    };

    /// \brief There will only be user defined script
    ///
    class BaseScript
    {
    private:
        int _ID;
        ScriptType _type;

    public:
        int getScriptID() { return _ID; }
        void setScriptID(int scriptID) { _ID = scriptID; }
        ScriptType getScriptType() { return _type; }
        void setScriptType(ScriptType type) { _type = type; }

        virtual void run() = 0;

        BaseScript() : _ID(-1) {}
        ~BaseScript() {}
    };

    class MoveEntityConstantlyScript : public BaseScript
    {
    private:
        int _targetEntityID;
        glm::vec2 _movement;

    public:
        int getTargetEntityID() { return _targetEntityID; }
        void setTargetEntityID(int targetEntityID)
        {
            _targetEntityID = targetEntityID;
        }
        glm::vec2 getMovement() { return _movement; }
        void setMovement(glm::vec2 movement) { _movement = movement; }
        void run();
        MoveEntityConstantlyScript() {}
        ~MoveEntityConstantlyScript() {}
    };

    class MoveEntitiesConstantlyScript : public MoveEntityConstantlyScript
    {
    private:
        std::vector<int> _targetEntityIDList;

    public:
        void run();
        MoveEntitiesConstantlyScript();
        ~MoveEntitiesConstantlyScript();
    };

    union Script
    {
        MoveEntityConstantlyScript mec;
        MoveEntitiesConstantlyScript mesc;
    };
}