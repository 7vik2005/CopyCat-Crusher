#include "../include/core/batch_analyzer.hpp"

#include <exception>
#include <iomanip>
#include <iostream>
int main(
    int argc,
    char* argv[]
){
    if(argc!=3){
        std::cerr
            <<"Usage: copycat_crusher "
            <<"<submissions_directory> "
            <<"<reports_directory>\n";

        return 1;
    }

    try{
        BatchAnalyzer analyzer(
            argv[1],
            argv[2]
        );

        const auto results=
            analyzer.analyze();

        if(results.empty()){
            std::cout
                <<"No comparisons generated.\n";

            return 0;
        }

        std::cout
            <<"\n"
            <<"============================================================\n"
            <<"                CopyCat Crusher Results\n"
            <<"============================================================\n\n";

        std::cout
            <<"Comparisons Generated: "
            <<results.size()
            <<"\n\n";

        std::cout
            <<std::left
            <<std::setw(30)
            <<"Source"

            <<std::setw(30)
            <<"Target"

            <<std::setw(12)
            <<"Score (%)"

            <<"\n";

        std::cout
            <<"--------------------------------------------------------------------------\n";

        for(const auto& result:results){
            std::cout
                <<std::left

                <<std::setw(30)
                <<result.sourceFileName

                <<std::setw(30)
                <<result.targetFileName

                <<std::setw(12)
                <<std::fixed
                <<std::setprecision(2)
                <<(
                    result
                        .similarityResult
                        .finalScore*100.0
                )

                <<"\n";
        }

        std::cout
            <<"\nHTML reports generated successfully.\n";

        return 0;
    }
    catch(const std::exception& exception){
        std::cerr
            <<"Fatal Error: "
            <<exception.what()
            <<"\n";

        return 1;
    }
    catch(...){
        std::cerr
            <<"Fatal Error: Unknown exception\n";

        return 1;
    }
}
