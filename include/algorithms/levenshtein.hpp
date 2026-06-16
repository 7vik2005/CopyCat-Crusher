#pragma once

#include <string>
#include <vector>
#include <cstddef>
class Levenshtein{
public:
    Levenshtein(
        const std::vector<std::string>& sourceTokens,
        const std::vector<std::string>& targetTokens
    );

    [[nodiscard]]
    std::size_t computeDistance() const;

    [[nodiscard]]
    double computeSimilarity() const;

private:
    const std::vector<std::string>& sourceTokens;
    const std::vector<std::string>& targetTokens;
};
