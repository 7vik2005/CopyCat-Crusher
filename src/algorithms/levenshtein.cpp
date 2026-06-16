#include "../../include/algorithms/levenshtein.hpp"

#include <algorithm>

Levenshtein::Levenshtein(
    const std::vector<std::string>& sourceTokens,
    const std::vector<std::string>& targetTokens
)
    :sourceTokens(sourceTokens),
     targetTokens(targetTokens){}

std::size_t Levenshtein::computeDistance() const{
    const auto* shorterSequence=&sourceTokens;
    const auto* longerSequence=&targetTokens;

    if(sourceTokens.size()>targetTokens.size()){
        shorterSequence=&targetTokens;
        longerSequence=&sourceTokens;
    }

    std::vector<std::size_t> previousRow(
        shorterSequence->size()+1
    );

    std::vector<std::size_t> currentRow(
        shorterSequence->size()+1
    );

    for(std::size_t column=0;
        column<=shorterSequence->size();
        ++column){
        previousRow[column]=column;
    }

    for(std::size_t row=1;
        row<=longerSequence->size();
        ++row){

        currentRow[0]=row;

        for(std::size_t column=1;
            column<=shorterSequence->size();
            ++column){

            std::size_t substitutionCost=
                (*longerSequence)[row-1]==
                (*shorterSequence)[column-1]
                ?0
                :1;

            currentRow[column]=std::min(
                {
                    previousRow[column]+1,
                    currentRow[column-1]+1,
                    previousRow[column-1]+substitutionCost
                }
            );
        }

        previousRow.swap(currentRow);
    }

    return previousRow.back();
}

double Levenshtein::computeSimilarity() const{
    const std::size_t maxLength=
        std::max(
            sourceTokens.size(),
            targetTokens.size()
        );

    if(maxLength==0){
        return 1.0;
    }

    const std::size_t distance=
        computeDistance();

    return 1.0-
        (
            static_cast<double>(distance)/
            static_cast<double>(maxLength)
        );
}
