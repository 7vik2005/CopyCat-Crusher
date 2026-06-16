#include "../../include/algorithms/suffix_automaton.hpp"

#include <algorithm>

SuffixAutomaton::SuffixAutomaton()
    :lastState(0),
     nextTokenId(1){
    states.emplace_back();
}

int SuffixAutomaton::getOrCreateTokenId(
    const std::string& token
){
    auto iterator=
        tokenDictionary.find(token);

    if(iterator!=tokenDictionary.end()){
        return iterator->second;
    }

    int assignedId=nextTokenId++;

    tokenDictionary[token]=assignedId;

    return assignedId;
}

int SuffixAutomaton::getExistingTokenId(
    const std::string& token
) const{
    auto iterator=
        tokenDictionary.find(token);

    if(iterator==tokenDictionary.end()){
        return -1;
    }

    return iterator->second;
}

void SuffixAutomaton::extend(
    int symbol
){
    int currentState=
        static_cast<int>(states.size());

    states.emplace_back();

    states[currentState].length=
        states[lastState].length+1;

    int parentState=lastState;

    while(
        parentState!=-1 &&
        states[parentState]
            .transitions.find(symbol)==
        states[parentState]
            .transitions.end()
    ){
        states[parentState]
            .transitions[symbol]=currentState;

        parentState=
            states[parentState]
                .suffixLink;
    }

    if(parentState==-1){
        states[currentState]
            .suffixLink=0;
    }else{
        int nextState=
            states[parentState]
                .transitions[symbol];

        if(
            states[parentState]
                .length+1==
            states[nextState]
                .length
        ){
            states[currentState]
                .suffixLink=
                nextState;
        }else{
            int cloneState=
                static_cast<int>(
                    states.size()
                );

            states.emplace_back(
                states[nextState]
            );

            states[cloneState]
                .length=
                states[parentState]
                    .length+1;

            while(
                parentState!=-1 &&
                states[parentState]
                    .transitions[symbol]==
                nextState
            ){
                states[parentState]
                    .transitions[symbol]=
                    cloneState;

                parentState=
                    states[parentState]
                        .suffixLink;
            }

            states[nextState]
                .suffixLink=
                cloneState;

            states[currentState]
                .suffixLink=
                cloneState;
        }
    }

    lastState=currentState;
}

void SuffixAutomaton::build(
    const std::vector<std::string>& tokens
){
    states.clear();

    states.emplace_back();

    lastState=0;

    tokenDictionary.clear();

    nextTokenId=1;

    for(const auto& token:tokens){
        extend(
            getOrCreateTokenId(
                token
            )
        );
    }
}

std::size_t SuffixAutomaton::longestCommonSubstring(
    const std::vector<std::string>& tokens
) const{
    if(
        states.empty()||
        tokens.empty()
    ){
        return 0;
    }

    int currentState=0;
    int currentLength=0;
    int bestLength=0;

    for(const auto& token:tokens){
        int symbol=
            getExistingTokenId(
                token
            );

        if(symbol==-1){
            currentState=0;
            currentLength=0;
            continue;
        }

        while(
            currentState!=0 &&
            states[currentState]
                .transitions.find(symbol)==
            states[currentState]
                .transitions.end()
        ){
            currentState=
                states[currentState]
                    .suffixLink;

            currentLength=
                states[currentState]
                    .length;
        }

        auto transition=
            states[currentState]
                .transitions.find(
                    symbol
                );

        if(
            transition!=
            states[currentState]
                .transitions.end()
        ){
            currentState=
                transition->second;

            ++currentLength;
        }else{
            currentState=0;
            currentLength=0;
        }

        bestLength=
            std::max(
                bestLength,
                currentLength
            );
    }

    return static_cast<std::size_t>(
        bestLength
    );
}
