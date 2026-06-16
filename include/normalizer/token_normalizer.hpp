#pragma once

#include "../tokenizer/clang_tokenizer.hpp"

#include <string>
#include <vector>

class TokenNormalizer{
public:
    explicit TokenNormalizer(const std::vector<Token>& tokens);

    void normalize();

    [[nodiscard]]
    const std::vector<std::string>& getNormalizedTokens() const;

private:
    const std::vector<Token>& originalTokens;
    std::vector<std::string> normalizedTokens;

    bool isNumericLiteral(const std::string& token) const;
    bool isStringLiteral(const std::string& token) const;
    bool isCharacterLiteral(const std::string& token) const;
    bool isFunctionIdentifier(std::size_t currentIndex) const;
};
