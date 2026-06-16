#pragma once

#include <string>
#include <vector>

enum class TokenCategory{
    Keyword,
    Identifier,
    Literal,
    Operator,
    Punctuation,
    Comment,
    Unknown
};

struct Token{
    std::string value;
    TokenCategory category;
    unsigned int line;
    unsigned int column;
};

class ClangTokenizer{
public:
    explicit ClangTokenizer(std::string sourceFile);

    bool tokenize();

    [[nodiscard]]
    const std::vector<Token>& getTokens() const;

private:
    std::string sourceFilePath;
    std::vector<Token> tokens;

    TokenCategory classifyToken(const std::string& tokenText) const;
};
