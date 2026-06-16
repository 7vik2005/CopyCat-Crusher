#pragma once

#include "../models/comparison_result.hpp"
#include "../models/submission.hpp"

#include <filesystem>
#include <vector>

class BatchAnalyzer{
public:
    BatchAnalyzer(
        const std::filesystem::path& submissionsDirectory,
        const std::filesystem::path& reportsDirectory
    );

    [[nodiscard]]
    std::vector<ComparisonResult>
    analyze() const;

private:
    std::filesystem::path submissionsDirectory;
    std::filesystem::path reportsDirectory;
};
