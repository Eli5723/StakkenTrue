#pragma once

#include <filesystem>

namespace Engine
{
    namespace File
    {
        std::string slurp(const char *filename);
        char* slurpBytes(const char* filename);
        std::filesystem::path ExecutablePath();
        std::filesystem::path ExecutablePath();
    }
}