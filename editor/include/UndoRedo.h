#pragma once

#include <functional>

void pushAction(std::function<void()> action, std::function<void()> restore);
bool undo();
bool redo();
