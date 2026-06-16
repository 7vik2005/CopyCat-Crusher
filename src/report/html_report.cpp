#include "../../include/report/html_report.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

HtmlReport::HtmlReport(
    const ComparisonResult& result
)
    :comparisonResult(result){}

std::string HtmlReport::escapeHtml(
    const std::string& text
) const{
    std::string escaped;

    escaped.reserve(
        text.size()
    );

    for(char character:text){
        switch(character){
        case '&':
            escaped+="&amp;";
            break;

        case '<':
            escaped+="&lt;";
            break;

        case '>':
            escaped+="&gt;";
            break;

        case '"':
            escaped+="&quot;";
            break;

        case '\'':
            escaped+="&#39;";
            break;

        default:
            escaped.push_back(
                character
            );
        }
    }

    return escaped;
}

std::string_view HtmlReport::determineRiskLevel() const{
    const double score=
        comparisonResult
            .similarityResult
            .finalScore;

    if(score>=0.80){
        return "CRITICAL";
    }

    if(score>=0.60){
        return "HIGH";
    }

    if(score>=0.30){
        return "MEDIUM";
    }

    return "LOW";
}

std::string HtmlReport::buildHtml() const{
    std::ostringstream html;

    auto percentage=[](double value){
        std::ostringstream stream;

        stream
            <<std::fixed
            <<std::setprecision(2)
            <<value*100.0;

        return stream.str();
    };

    const auto& result=
        comparisonResult
            .similarityResult;

    std::string riskColor;

    if(result.finalScore>=0.80){
        riskColor="#dc2626";
    }else if(result.finalScore>=0.60){
        riskColor="#ea580c";
    }else if(result.finalScore>=0.30){
        riskColor="#ca8a04";
    }else{
        riskColor="#16a34a";
    }

    html
    <<R"(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>CopyCat Crusher Report</title>

<style>
*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:Inter,Segoe UI,sans-serif;
}

body{
background:#0f172a;
color:#e2e8f0;
padding:40px;
}

.container{
max-width:1200px;
margin:auto;
}

.header{
margin-bottom:30px;
}

.title{
font-size:2.5rem;
font-weight:700;
margin-bottom:8px;
}

.subtitle{
color:#94a3b8;
}

.file-grid{
display:grid;
grid-template-columns:1fr 1fr;
gap:20px;
margin-top:25px;
margin-bottom:30px;
}

.file-card{
background:#1e293b;
padding:20px;
border-radius:12px;
}

.file-label{
font-size:0.9rem;
color:#94a3b8;
margin-bottom:8px;
}

.file-name{
font-size:1.1rem;
font-weight:600;
word-break:break-word;
}

.summary{
background:#1e293b;
padding:24px;
border-radius:12px;
margin-bottom:30px;
}

.risk-badge{
display:inline-block;
padding:10px 18px;
border-radius:999px;
font-weight:700;
margin-top:15px;
color:white;
}

.metrics{
display:grid;
grid-template-columns:repeat(auto-fit,minmax(220px,1fr));
gap:20px;
}

.metric-card{
background:#1e293b;
padding:22px;
border-radius:12px;
}

.metric-title{
color:#94a3b8;
font-size:0.9rem;
margin-bottom:10px;
}

.metric-value{
font-size:1.8rem;
font-weight:700;
}

.metric-small{
margin-top:10px;
color:#94a3b8;
font-size:0.9rem;
}
</style>
</head>

<body>

<div class="container">

<div class="header">
<div class="title">CopyCat Crusher</div>
<div class="subtitle">
Advanced Source Code Plagiarism Analysis Report
</div>
</div>
)";

    html
    <<R"(
<div class="file-grid">

<div class="file-card">
<div class="file-label">Source File</div>
<div class="file-name">)"
    <<escapeHtml(
        comparisonResult
            .sourceFileName
    )
    <<R"(</div>
</div>

<div class="file-card">
<div class="file-label">Target File</div>
<div class="file-name">)"
    <<escapeHtml(
        comparisonResult
            .targetFileName
    )
    <<R"(</div>
</div>

</div>
)";

    html
    <<R"(
<div class="summary">

<h2>Comparison Summary</h2>

<br>

<p>
<strong>Final Similarity Score:</strong>
 )"
    <<percentage(result.finalScore)
    <<R"(%</p>

<p>
<strong>Source Tokens:</strong>
 )"
    <<result.sourceTokenCount
    <<R"(</p>

<p>
<strong>Target Tokens:</strong>
 )"
    <<result.targetTokenCount
    <<R"(</p>

<p>
<strong>Longest Copied Block:</strong>
 )"
    <<result.longestCommonBlockLength
    <<R"( tokens</p>

<div
class="risk-badge"
style="background:)"
    <<riskColor
    <<R"(;">
)"
    <<determineRiskLevel()
    <<R"(
</div>

</div>
)";

    html
    <<R"(
<div class="metrics">

<div class="metric-card">
<div class="metric-title">
Exact Match Score
</div>
<div class="metric-value">)"
    <<percentage(result.exactMatchScore)
    <<R"( %</div>
<div class="metric-small">
Rolling Hash + Jaccard Similarity
</div>
</div>

<div class="metric-card">
<div class="metric-title">
Fingerprint Score
</div>
<div class="metric-value">)"
    <<percentage(result.fingerprintScore)
    <<R"( %</div>
<div class="metric-small">
Winnowing Fingerprint Overlap
</div>
</div>

<div class="metric-card">
<div class="metric-title">
Longest Match Score
</div>
<div class="metric-value">)"
    <<percentage(result.longestMatchScore)
    <<R"( %</div>
<div class="metric-small">
Suffix Automaton Analysis
</div>
</div>

<div class="metric-card">
<div class="metric-title">
Levenshtein Score
</div>
<div class="metric-value">)"
    <<percentage(result.levenshteinScore)
    <<R"( %</div>
<div class="metric-small">
Approximate Similarity Detection
</div>
</div>

</div>

</div>

</body>
</html>
)";

    return html.str();
}

bool HtmlReport::generate(
    const std::string& outputPath
) const{
    std::ofstream outputFile(
        outputPath,
        std::ios::out|
        std::ios::trunc
    );

    if(!outputFile.is_open()){
        return false;
    }

    outputFile<<buildHtml();

    outputFile.flush();

    return outputFile.good();
}
