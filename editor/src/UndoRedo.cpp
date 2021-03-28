#include "UndoRedo.h"

#include <iostream>
#include <vector>
#include <utility>

using namespace std;


int index = 0;
vector<pair<function<void()>, function<void()>>> actionStack;


void pushAction(function<void()> action, function<void()> restore) {
    if (index != actionStack.size()) {
        actionStack.erase(actionStack.begin() + index, actionStack.end());
    }
    auto p = make_pair(action, restore);
    actionStack.push_back(p);
    index++;
}


bool undo() {
    if (actionStack.size() > 0 && index > 0) {
        index--;
        function<void()> undoFunc = actionStack[index].second;
        undoFunc();
        return true;
    }
    return false;
}

bool redo() {
    if (actionStack.size() > 0 && index < actionStack.size()) {
        function<void()> redoFunc = actionStack[index].first;
        redoFunc();
        index++;
        return true;
    }
    return false;
}
