#include "../../include/core/batch_analyzer.hpp"

#include "../../include/core/file_manager.hpp"

#include "../../include/tokenizer/clang_tokenizer.hpp"
#include "../../include/normalizer/token_normalizer.hpp"

#include "../../include/similarity/similarity_engine.hpp"
#include "../../include/report/html_report.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <system_error>
#include <utility>

BatchAnalyzer::BatchAnalyzer(
    const std::filesystem::path& submissionsDirectory,
    const std::filesystem::path& reportsDirectory
)
    :submissionsDirectory(
        submissionsDirectory
    ),
     reportsDirectory(
        reportsDirectory
    ){}

std::vector<ComparisonResult>
BatchAnalyzer::analyze() const{
    FileManager fileManager(
        submissionsDirectory
    );

    const auto sourceFiles=
        fileManager.discoverSourceFiles();
    std::vector<Submission>
    submissions;

    submissions.reserve(
        sourceFiles.size()
    );

    for(const auto& filePath:sourceFiles){
        ClangTokenizer tokenizer(
            filePath.string()
        );

        if(
            !tokenizer.tokenize()
        ){
            continue;
        }

        TokenNormalizer normalizer(
            tokenizer.getTokens()
        );

        normalizer.normalize();

        const auto& normalizedTokens=
            normalizer.getNormalizedTokens();

        if(
            normalizedTokens.empty()
        ){
            continue;
        }

        Submission submission;

        submission.filePath=
            filePath;

        submission.fileName=
            filePath.filename()
                    .string();

        submission.normalizedTokens=
            normalizedTokens;

        submissions.emplace_back(
            std::move(
                submission
            )
        );
    }

    std::vector<ComparisonResult>
    results;

    const std::size_t comparisonCount=
        submissions.size()<2
        ? 0
        : (
            submissions.size()*
            (
                submissions.size()-1
            )
        )/2;

    results.reserve(
        comparisonCount
    );

    std::error_code errorCode;

    if(
        !std::filesystem::exists(
            reportsDirectory,
            errorCode
        )
    ){
        std::filesystem::create_directories(
            reportsDirectory,
            errorCode
        );
    }

    std::size_t reportCounter=0;

    for(
        std::size_t left=0;
        left<submissions.size();
        ++left
    ){
        for(
            std::size_t right=
                left+1;
            right<submissions.size();
            ++right
        ){
            if(
                submissions[left]
                    .normalizedTokens==
                submissions[right]
                    .normalizedTokens
            ){
                ComparisonResult result;

                result.sourceFileName=
                    submissions[left]
                        .fileName;

                result.targetFileName=
                    submissions[right]
                        .fileName;

                result.similarityResult
                    .sourceTokenCount=
                        submissions[left]
                            .normalizedTokens
                            .size();

                result.similarityResult
                    .targetTokenCount=
                        submissions[right]
                            .normalizedTokens
                            .size();

                result.similarityResult
                    .exactMatchScore=1.0;

                result.similarityResult
                    .fingerprintScore=1.0;

                result.similarityResult
                    .longestMatchScore=1.0;

                result.similarityResult
                    .levenshteinScore=1.0;

                result.similarityResult
                    .finalScore=1.0;

                result.similarityResult
                    .longestCommonBlockLength=
                        submissions[left]
                            .normalizedTokens
                            .size();

                results.emplace_back(
                    result
                );
            }
            else{
                SimilarityEngine
                similarityEngine(
                    submissions[left]
                        .normalizedTokens,
                    submissions[right]
                        .normalizedTokens
                );

                ComparisonResult result;

                result.sourceFileName=
                    submissions[left]
                        .fileName;

                result.targetFileName=
                    submissions[right]
                        .fileName;

                result.similarityResult=
                    similarityEngine
                        .analyze();

                results.emplace_back(
                    result
                );
            }

            HtmlReport report(
                results.back()
            );

            const std::filesystem::path
            reportPath=
                reportsDirectory/
                (
                    "comparison_" +
                    std::to_string(
                        reportCounter++
                    ) +
                    ".html"
                );

            if(
                !report.generate(
                    reportPath.string()
                )
            ){
                std::cerr
                    <<"Failed to generate report: "
                    <<reportPath.string()
                    <<"\n";
            }
        }
    }

    std::sort(
        results.begin(),
        results.end()
    );

    return results;
}
