#pragma once

#include "../algorithms/winnowing.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct SimilarityResult{
    double exactMatchScore=0.0;
    double fingerprintScore=0.0;
    double longestMatchScore=0.0;
    double levenshteinScore=0.0;
    double finalScore=0.0;

    std::size_t longestCommonBlockLength=0;

    std::size_t sourceTokenCount=0;
    std::size_t targetTokenCount=0;
};

class SimilarityEngine{
public:
    SimilarityEngine(
        const std::vector<std::string>& sourceTokens,
        const std::vector<std::string>& targetTokens
    );

    [[nodiscard]]
    SimilarityResult analyze() const;

private:
    const std::vector<std::string>& sourceTokens;
    const std::vector<std::string>& targetTokens;

    [[nodiscard]]
    double computeExactMatchScore(
        const std::vector<std::uint64_t>& sourceHashes,
        const std::vector<std::uint64_t>& targetHashes
    ) const;

    [[nodiscard]]
    double computeFingerprintScore(
        const std::vector<Fingerprint>& sourceFingerprints,
        const std::vector<Fingerprint>& targetFingerprints
    ) const;

    [[nodiscard]]
    double computeLongestMatchScore(
        std::size_t longestBlockLength
    ) const;
};
