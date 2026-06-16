#include "../../include/tokenizer/clang_tokenizer.hpp"

#include <clang-c/Index.h>

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_set>
#include <utility>

ClangTokenizer::ClangTokenizer(
    std::string sourceFile
)
    :sourceFilePath(
        std::move(sourceFile)
    ){}

const std::vector<Token>&
ClangTokenizer::getTokens() const{
    return tokens;
}

TokenCategory ClangTokenizer::classifyToken(
    const std::string& tokenText
) const{
    static const std::unordered_set<std::string>
    keywords{
        "alignas","alignof","asm","auto","bool","break","case","catch",
        "char","char8_t","char16_t","char32_t","class","const","constexpr",
        "consteval","constinit","continue","default","delete","do","double",
        "else","enum","explicit","export","extern","false","float","for",
        "friend","goto","if","inline","int","long","mutable","namespace",
        "new","noexcept","nullptr","operator","private","protected","public",
        "register","reinterpret_cast","requires","return","short","signed",
        "sizeof","static","static_assert","struct","switch","template",
        "this","thread_local","throw","true","try","typedef","typename",
        "union","unsigned","using","virtual","void","volatile","while"
    };

    if(
        keywords.find(tokenText)!=
        keywords.end()
    ){
        return TokenCategory::Keyword;
    }

    if(
        !tokenText.empty() &&
        std::isdigit(
            static_cast<unsigned char>(
                tokenText.front()
            )
        )
    ){
        return TokenCategory::Literal;
    }

    static const std::unordered_set<std::string>
    operators{
        "+","-","*","/","%","=","==","!=",
        "<",">","<=",">=","&&","||","!",
        "&","|","^","~","<<",">>",
        "+=","-=","*=","/=","%=",
        "++","--","->",".","::"
    };

    if(
        operators.find(tokenText)!=
        operators.end()
    ){
        return TokenCategory::Operator;
    }

    static const std::unordered_set<std::string>
    punctuations{
        "(",")","{","}","[","]",
        ",",";","?",":"
    };

    if(
        punctuations.find(tokenText)!=
        punctuations.end()
    ){
        return TokenCategory::Punctuation;
    }

    if(
        !tokenText.empty() &&
        (
            std::isalpha(
                static_cast<unsigned char>(
                    tokenText.front()
                )
            ) ||
            tokenText.front()=='_'
        )
    ){
        return TokenCategory::Identifier;
    }

    return TokenCategory::Unknown;
}

bool ClangTokenizer::tokenize(){
    tokens.clear();

    const char* clangArguments[]={
        "-x",
        "c++",
        "-std=c++17"
    };

    constexpr unsigned clangArgumentsCount=3U;

    CXIndex index=
        clang_createIndex(
            0,
            0
        );

    if(index==nullptr){
        return false;
    }

    CXTranslationUnit translationUnit=
    clang_parseTranslationUnit(
        index,
        sourceFilePath.c_str(),
        clangArguments,
        clangArgumentsCount,
        nullptr,
        0,
        CXTranslationUnit_SkipFunctionBodies|
        CXTranslationUnit_KeepGoing
    );

    if(translationUnit==nullptr){
        clang_disposeIndex(index);
        return false;
    }

    CXCursor rootCursor=
        clang_getTranslationUnitCursor(
            translationUnit
        );

    CXSourceRange sourceRange=
        clang_getCursorExtent(
            rootCursor
        );

    CXToken* clangTokens=nullptr;

    unsigned int tokenCount=0;

    clang_tokenize(
        translationUnit,
        sourceRange,
        &clangTokens,
        &tokenCount
    );

    if(
        clangTokens==nullptr||
        tokenCount==0
    ){
        if(clangTokens!=nullptr){
            clang_disposeTokens(
                translationUnit,
                clangTokens,
                tokenCount
            );
        }

        clang_disposeTranslationUnit(
            translationUnit
        );

        clang_disposeIndex(
            index
        );

        return false;
    }

    tokens.reserve(tokenCount);

    for(
        unsigned int indexToken=0;
        indexToken<tokenCount;
        ++indexToken
    ){
        const CXTokenKind tokenKind=
            clang_getTokenKind(
                clangTokens[indexToken]
            );

        if(tokenKind==CXToken_Comment){
            continue;
        }

        CXString spelling=
            clang_getTokenSpelling(
                translationUnit,
                clangTokens[indexToken]
            );

        const char* tokenCString=
            clang_getCString(
                spelling
            );

        std::string tokenText=
            tokenCString!=nullptr
            ? tokenCString
            : "";

        CXSourceLocation location=
            clang_getTokenLocation(
                translationUnit,
                clangTokens[indexToken]
            );

        unsigned int line=0;
        unsigned int column=0;

        clang_getSpellingLocation(
            location,
            nullptr,
            &line,
            &column,
            nullptr
        );

        tokens.push_back(
            {
                tokenText,
                classifyToken(
                    tokenText
                ),
                line,
                column
            }
        );

        clang_disposeString(
            spelling
        );
    }

    clang_disposeTokens(
        translationUnit,
        clangTokens,
        tokenCount
    );

    clang_disposeTranslationUnit(
        translationUnit
    );

    clang_disposeIndex(
        index
    );

    return !tokens.empty();
}
