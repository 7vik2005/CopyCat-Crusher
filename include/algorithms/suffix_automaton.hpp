#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstddef>
struct SamState{
    int length=0;
    int suffixLink=-1;

    std::unordered_map<int,int> transitions;
};

class SuffixAutomaton{
public:
    SuffixAutomaton();

    void build(
        const std::vector<std::string>& tokens
    );

    [[nodiscard]]
    std::size_t longestCommonSubstring(
        const std::vector<std::string>& tokens
    ) const;

private:
    std::vector<SamState> states;

    int lastState;

    std::unordered_map<std::string,int>
    tokenDictionary;

    int nextTokenId;

    int getOrCreateTokenId(
        const std::string& token
    );

    int getExistingTokenId(
        const std::string& token
    ) const;

    void extend(int symbol);
};
