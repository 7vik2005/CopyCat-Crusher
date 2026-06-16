#include "../../include/similarity/similarity_engine.hpp"

#include "../../include/algorithms/rolling_hash.hpp"
#include "../../include/algorithms/winnowing.hpp"
#include "../../include/algorithms/suffix_automaton.hpp"
#include "../../include/algorithms/levenshtein.hpp"

#include <algorithm>
#include <unordered_set>

SimilarityEngine::SimilarityEngine(
    const std::vector<std::string>& sourceTokens,
    const std::vector<std::string>& targetTokens
)
    :sourceTokens(sourceTokens),
     targetTokens(targetTokens){}

double SimilarityEngine::computeExactMatchScore(
    const std::vector<std::uint64_t>& sourceHashes,
    const std::vector<std::uint64_t>& targetHashes
) const{
    if(
        sourceHashes.empty()||
        targetHashes.empty()
    ){
        return 0.0;
    }

    std::unordered_set<std::uint64_t> sourceSet(
        sourceHashes.begin(),
        sourceHashes.end()
    );

    std::unordered_set<std::uint64_t> targetSet(
        targetHashes.begin(),
        targetHashes.end()
    );

    std::size_t intersectionCount=0;

    for(const auto hash:sourceSet){
        if(
            targetSet.find(hash)!=
            targetSet.end()
        ){
            ++intersectionCount;
        }
    }

    const std::size_t unionCount=
        sourceSet.size()+
        targetSet.size()-
        intersectionCount;

    if(unionCount==0){
        return 0.0;
    }

    return static_cast<double>(
               intersectionCount
           )/
           static_cast<double>(
               unionCount
           );
}

double SimilarityEngine::computeFingerprintScore(
    const std::vector<Fingerprint>& sourceFingerprints,
    const std::vector<Fingerprint>& targetFingerprints
) const{
    if(
        sourceFingerprints.empty()||
        targetFingerprints.empty()
    ){
        return 0.0;
    }

    std::unordered_set<std::uint64_t> sourceSet;
    std::unordered_set<std::uint64_t> targetSet;

    for(const auto& fingerprint:sourceFingerprints){
        sourceSet.insert(
            fingerprint.hash
        );
    }

    for(const auto& fingerprint:targetFingerprints){
        targetSet.insert(
            fingerprint.hash
        );
    }

    std::size_t intersectionCount=0;

    for(const auto hash:sourceSet){
        if(
            targetSet.find(hash)!=
            targetSet.end()
        ){
            ++intersectionCount;
        }
    }

    const std::size_t unionCount=
        sourceSet.size()+
        targetSet.size()-
        intersectionCount;

    if(unionCount==0){
        return 0.0;
    }

    return static_cast<double>(
               intersectionCount
           )/
           static_cast<double>(
               unionCount
           );
}

double SimilarityEngine::computeLongestMatchScore(
    std::size_t longestBlockLength
) const{
    const std::size_t maximumLength=
        std::max(
            sourceTokens.size(),
            targetTokens.size()
        );

    if(maximumLength==0){
        return 0.0;
    }

    double score=
        static_cast<double>(
            longestBlockLength
        )/
        static_cast<double>(
            maximumLength
        );

    if(longestBlockLength>=100){
        score=std::max(
            score,
            0.50
        );
    }

    return std::min(
        score,
        1.0
    );
}

SimilarityResult SimilarityEngine::analyze() const{
    SimilarityResult result;

    result.sourceTokenCount=
        sourceTokens.size();

    result.targetTokenCount=
        targetTokens.size();

    if(
        sourceTokens.empty()||
        targetTokens.empty()
    ){
        return result;
    }

    constexpr std::size_t rollingWindowSize=8;
    constexpr std::size_t winnowingWindowSize=5;

    RollingHash sourceRollingHash(
        sourceTokens,
        rollingWindowSize
    );

    RollingHash targetRollingHash(
        targetTokens,
        rollingWindowSize
    );

    sourceRollingHash.compute();
    targetRollingHash.compute();

    const auto& sourceHashes=
        sourceRollingHash.getHashes();

    const auto& targetHashes=
        targetRollingHash.getHashes();

    result.exactMatchScore=
        computeExactMatchScore(
            sourceHashes,
            targetHashes
        );

    Winnowing sourceWinnowing(
        sourceHashes,
        winnowingWindowSize
    );

    Winnowing targetWinnowing(
        targetHashes,
        winnowingWindowSize
    );

    sourceWinnowing.compute();
    targetWinnowing.compute();

    result.fingerprintScore=
        computeFingerprintScore(
            sourceWinnowing.getFingerprints(),
            targetWinnowing.getFingerprints()
        );

    SuffixAutomaton automaton;

    automaton.build(
        sourceTokens
    );

    result.longestCommonBlockLength=
        automaton.longestCommonSubstring(
            targetTokens
        );

    result.longestMatchScore=
        computeLongestMatchScore(
            result.longestCommonBlockLength
        );

    Levenshtein levenshtein(
        sourceTokens,
        targetTokens
    );

    result.levenshteinScore=
        levenshtein.computeSimilarity();

    result.finalScore=
        (
            result.exactMatchScore*0.25
        )+
        (
            result.fingerprintScore*0.15
        )+
        (
            result.longestMatchScore*0.35
        )+
        (
            result.levenshteinScore*0.25
        );

    result.finalScore=
        std::min(
            result.finalScore,
            1.0
        );

    return result;
}
