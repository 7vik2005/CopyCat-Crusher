#pragma once

#include "../similarity/similarity_engine.hpp"

#include <string>

struct ComparisonResult{
    std::string sourceFileName;
    std::string targetFileName;

    SimilarityResult similarityResult;

    bool operator<(
        const ComparisonResult& other
    ) const{
        return similarityResult.finalScore>
               other.similarityResult.finalScore;
    }
};
