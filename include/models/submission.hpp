#pragma once

#include <filesystem>
#include <string>
#include <vector>

struct Submission{
    std::filesystem::path filePath;

    std::string fileName;

    std::vector<std::string> normalizedTokens;
};
