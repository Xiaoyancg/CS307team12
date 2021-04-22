#pragma once
#include <string>
#include <nlohmann/json.hpp>
namespace Core
{
    template <typename T>
    T getData(nlohmann::json root, const char *name);
    template int getData<int>(nlohmann::json, const char *);
} // namespace Core
