#include "benchmarkSingularAnalyzer.h"

namespace benchmark {
namespace singularAnalysis {

Results<std::size_t> run( const Settings& settings ) {
    Timer totalRunningTime;
    Results<std::size_t> ress;

    
    std::cout << "Benchmarking singular time elapse." << std::endl;
    
    auto tmp = singularTimeElapse( settings );
    ress.insert( ress.end(), tmp.begin(), tmp.end() );

    std::cout << "Benchmarking singular with jumps." << std::endl;
    tmp = singularJumps( settings );
    ress.insert( ress.end(), tmp.begin(), tmp.end() );

    std::cout << "Singular analyzer benchmarking took " << totalRunningTime.elapsedMs() << " sec in total." << std::endl;

    return ress;
}
}  // namespace singularAnalysis
}  // namespace benchmark
