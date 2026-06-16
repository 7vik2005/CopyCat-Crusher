#include "../../include/algorithms/rolling_hash.hpp"

#include <functional>

RollingHash::RollingHash(
    const std::vector<std::string>& tokens,
    std::size_t windowSize
)
    :tokens(tokens),
     windowSize(windowSize){}

const std::vector<std::uint64_t>&
RollingHash::getHashes() const{
    return hashes;
}

std::uint64_t RollingHash::tokenValue(
    const std::string& token
) const{
    return static_cast<std::uint64_t>(
        std::hash<std::string>{}(token)
    )%modulus;
}

void RollingHash::compute(){
    hashes.clear();

    if(tokens.empty()){
        return;
    }

    if(windowSize==0){
        return;
    }

    if(tokens.size()<windowSize){
        return;
    }

    std::uint64_t highestPower=1;

    for(
        std::size_t index=1;
        index<windowSize;
        ++index
    ){
        highestPower=
            (highestPower*base)%modulus;
    }

    std::uint64_t currentHash=0;

    for(
        std::size_t index=0;
        index<windowSize;
        ++index
    ){
        currentHash=
            (
                currentHash*base+
                tokenValue(tokens[index])
            )%modulus;
    }

    hashes.emplace_back(
        currentHash
    );

    for(
        std::size_t right=windowSize;
        right<tokens.size();
        ++right
    ){
        const std::size_t left=
            right-windowSize;

        const std::uint64_t outgoingValue=
            tokenValue(
                tokens[left]
            );

        currentHash=
            (
                currentHash+
                modulus-
                (
                    outgoingValue*
                    highestPower
                )%modulus
            )%modulus;

        currentHash=
            (
                currentHash*base+
                tokenValue(
                    tokens[right]
                )
            )%modulus;

        hashes.emplace_back(
            currentHash
        );
    }
}
