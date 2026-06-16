#include "../../include/algorithms/winnowing.hpp"

#include <deque>

Winnowing::Winnowing(
    const std::vector<std::uint64_t>& hashes,
    std::size_t windowSize
)
    :hashes(hashes),
     windowSize(windowSize){}

const std::vector<Fingerprint>&
Winnowing::getFingerprints() const{
    return fingerprints;
}

void Winnowing::compute(){
    fingerprints.clear();

    if(hashes.empty()){
        return;
    }

    if(windowSize==0){
        return;
    }

    if(windowSize>hashes.size()){
        return;
    }

    std::deque<std::size_t> candidates;

    std::size_t lastSelectedPosition=
        static_cast<std::size_t>(-1);

    for(
        std::size_t index=0;
        index<hashes.size();
        ++index
    ){
        while(
            !candidates.empty() &&
            candidates.front()+windowSize<=index
        ){
            candidates.pop_front();
        }

        while(
            !candidates.empty() &&
            hashes[candidates.back()]>=
            hashes[index]
        ){
            candidates.pop_back();
        }

        candidates.push_back(index);

        if(index+1<windowSize){
            continue;
        }

        const std::size_t minimumPosition=
            candidates.front();

        if(
            minimumPosition!=
            lastSelectedPosition
        ){
            fingerprints.push_back(
                {
                    hashes[minimumPosition],
                    minimumPosition
                }
            );

            lastSelectedPosition=
                minimumPosition;
        }
    }
}
