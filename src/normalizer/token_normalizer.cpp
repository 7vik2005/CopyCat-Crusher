#include "../../include/normalizer/token_normalizer.hpp"

#include <cctype>
#include <cstddef>
#include <unordered_set>

TokenNormalizer::TokenNormalizer(
    const std::vector<Token>& tokens
)
    :originalTokens(tokens){}

const std::vector<std::string>&
TokenNormalizer::getNormalizedTokens() const{
    return normalizedTokens;
}

bool TokenNormalizer::isNumericLiteral(
    const std::string& token
) const{
    if(token.empty()){
        return false;
    }

    if(
        token.size()>2 &&
        token[0]=='0' &&
        (
            token[1]=='x'||
            token[1]=='X'
        )
    ){
        return true;
    }

    if(
        token.size()>2 &&
        token[0]=='0' &&
        (
            token[1]=='b'||
            token[1]=='B'
        )
    ){
        return true;
    }

    bool digitSeen=false;

    for(char character:token){
        if(
            std::isdigit(
                static_cast<unsigned char>(
                    character
                )
            )
        ){
            digitSeen=true;
            continue;
        }

        switch(character){
        case '.':
        case 'e':
        case 'E':
        case '+':
        case '-':
        case 'f':
        case 'F':
        case 'l':
        case 'L':
        case 'u':
        case 'U':
            break;

        default:
            return false;
        }
    }

    return digitSeen;
}

bool TokenNormalizer::isStringLiteral(
    const std::string& token
) const{
    return
        token.size()>=2 &&
        token.front()=='"' &&
        token.back()=='"';
}

bool TokenNormalizer::isCharacterLiteral(
    const std::string& token
) const{
    return
        token.size()>=3 &&
        token.front()=='\'' &&
        token.back()=='\'';
}

bool TokenNormalizer::isFunctionIdentifier(
    std::size_t currentIndex
) const{
    if(
        currentIndex+1>=
        originalTokens.size()
    ){
        return false;
    }

    const auto& currentToken=
        originalTokens[currentIndex];

    const auto& nextToken=
        originalTokens[currentIndex+1];

    return
        currentToken.category==
        TokenCategory::Identifier &&
        nextToken.value=="(";
}

void TokenNormalizer::normalize(){
    normalizedTokens.clear();

    normalizedTokens.reserve(
        originalTokens.size()
    );

    static const std::unordered_set<std::string>
    primitiveTypes{
        "int",
        "float",
        "double",
        "char",
        "bool",
        "void",
        "long",
        "short",
        "signed",
        "unsigned",
        "size_t",
        "wchar_t",
        "char8_t",
        "char16_t",
        "char32_t"
    };

    for(
        std::size_t index=0;
        index<originalTokens.size();
        ++index
    ){
        const auto& token=
            originalTokens[index];

        if(
            token.category==
            TokenCategory::Keyword
        ){
            if(
                primitiveTypes.find(
                    token.value
                )!=primitiveTypes.end()
            ){
                normalizedTokens.emplace_back(
                    "TYPE"
                );
            }else{
                std::string keyword=
                    token.value;

                for(
                    char& character:
                    keyword
                ){
                    character=
                        static_cast<char>(
                            std::toupper(
                                static_cast<
                                    unsigned char
                                >(
                                    character
                                )
                            )
                        );
                }

                normalizedTokens.emplace_back(
                    keyword
                );
            }

            continue;
        }

        if(
            isNumericLiteral(
                token.value
            )
        ){
            normalizedTokens.emplace_back(
                "NUM_LITERAL"
            );

            continue;
        }

        if(
            isStringLiteral(
                token.value
            )
        ){
            normalizedTokens.emplace_back(
                "STRING_LITERAL"
            );

            continue;
        }

        if(
            isCharacterLiteral(
                token.value
            )
        ){
            normalizedTokens.emplace_back(
                "CHAR_LITERAL"
            );

            continue;
        }

        if(
            token.category==
            TokenCategory::Identifier
        ){
            if(
                isFunctionIdentifier(
                    index
                )
            ){
                normalizedTokens.emplace_back(
                    "FUNC"
                );
            }else{
                normalizedTokens.emplace_back(
                    "VAR"
                );
            }

            continue;
        }

        normalizedTokens.emplace_back(
            token.value
        );
    }
}
