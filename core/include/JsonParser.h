#pragma once
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
namespace Core
{
    template <typename T>
    static T getData(nlohmann::json root, const char *name)
    {
        try
        {
            return root.at(name).get<T>();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return T();
    }
} // namespace Core
