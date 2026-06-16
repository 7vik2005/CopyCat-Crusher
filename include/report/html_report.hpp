#pragma once

#include "../models/comparison_result.hpp"

#include <string>
#include <string_view>

class HtmlReport{
public:
    explicit HtmlReport(
        const ComparisonResult& result
    );

    bool generate(
        const std::string& outputPath
    ) const;

private:
    ComparisonResult comparisonResult;

    [[nodiscard]]
    std::string_view determineRiskLevel() const;

    [[nodiscard]]
    std::string buildHtml() const;

    [[nodiscard]]
    std::string escapeHtml(
        const std::string& text
    ) const;
};
