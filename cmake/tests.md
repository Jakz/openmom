### Test Coverage 
Coverage can be done through llvm coverage [tools](https://clang.llvm.org/docs/SourceBasedCodeCoverage.html), for example:

     xcrun llvm-profdata merge -sparse default.profraw -o default.profdata
     xcrun llvm-cov show openmom-tests -instr-profile=default.profdata
