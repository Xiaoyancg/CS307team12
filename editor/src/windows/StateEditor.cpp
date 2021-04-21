#include "windows/StateEditor.h"
#include "UndoRedo.h"


void StateEditor::draw() {
    if (visible)
    {
        // possibly implement a new function here for readability purposes
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("State Editor", &visible))
        {

        }
        ImGui::End();
    }
}