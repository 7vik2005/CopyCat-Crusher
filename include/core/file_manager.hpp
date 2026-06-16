#pragma once

#include <filesystem>
#include <string>
#include <vector>

class FileManager{
public:
    explicit FileManager(
        const std::filesystem::path& rootDirectory
    );

    [[nodiscard]]
    std::vector<std::filesystem::path>
    discoverSourceFiles() const;

    [[nodiscard]]
    std::string readFile(
        const std::filesystem::path& filePath
    ) const;

private:
    std::filesystem::path rootDirectory;

    [[nodiscard]]
    bool isSupportedSourceFile(
        const std::filesystem::path& filePath
    ) const;
};
