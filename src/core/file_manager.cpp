#include "../../include/core/file_manager.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <system_error>
#include <unordered_set>

FileManager::FileManager(
    const std::filesystem::path& rootDirectory
)
    :rootDirectory(rootDirectory){}

bool FileManager::isSupportedSourceFile(
    const std::filesystem::path& filePath
) const{
    static const std::unordered_set<std::string>
    supportedExtensions{
        ".c",
        ".cc",
        ".cpp",
        ".cxx",
        ".h",
        ".hh",
        ".hpp",
        ".hxx"
    };

    std::string extension=
        filePath.extension().string();

    std::transform(
        extension.begin(),
        extension.end(),
        extension.begin(),
        [](unsigned char character){
            return static_cast<char>(
                std::tolower(character)
            );
        }
    );

    return
        supportedExtensions.find(
            extension
        )!=supportedExtensions.end();
}

std::vector<std::filesystem::path>
FileManager::discoverSourceFiles() const{
    std::vector<std::filesystem::path>
    sourceFiles;

    std::error_code errorCode;

    if(
        !std::filesystem::exists(
            rootDirectory,
            errorCode
        )
    ){
        return sourceFiles;
    }

    if(
        !std::filesystem::is_directory(
            rootDirectory,
            errorCode
        )
    ){
        return sourceFiles;
    }

    std::filesystem::recursive_directory_iterator
    iterator(
        rootDirectory,
        std::filesystem::directory_options::
            skip_permission_denied,
        errorCode
    );

    std::filesystem::recursive_directory_iterator
    endIterator;

    while(
        iterator!=endIterator &&
        !errorCode
    ){
        const auto& entry=
            *iterator;

        if(
            entry.is_regular_file(
                errorCode
            )
        ){
            const auto& filePath=
                entry.path();

            const std::string fileName=
                filePath.filename().string();

            if(
                !fileName.empty() &&
                fileName.front()=='.'
            ){
                iterator.increment(
                    errorCode
                );
                continue;
            }

            if(
                isSupportedSourceFile(
                    filePath
                )
            ){
                sourceFiles.emplace_back(
                    std::filesystem::absolute(
                        filePath,
                        errorCode
                    )
                );
            }
        }

        iterator.increment(
            errorCode
        );
    }

    std::sort(
        sourceFiles.begin(),
        sourceFiles.end()
    );

    sourceFiles.erase(
        std::unique(
            sourceFiles.begin(),
            sourceFiles.end()
        ),
        sourceFiles.end()
    );

    return sourceFiles;
}

std::string FileManager::readFile(
    const std::filesystem::path& filePath
) const{
    std::ifstream inputFile(
        filePath,
        std::ios::binary
    );

    if(!inputFile){
        return {};
    }

    std::ostringstream buffer;

    buffer<<inputFile.rdbuf();

    return buffer.str();
}
