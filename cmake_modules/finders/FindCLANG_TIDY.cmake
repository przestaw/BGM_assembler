include(FeatureSummary)

find_program(
        CLANG_TIDY_EXE
        NAMES clang-tidy clang-tidy-11 clang-tidy-10
        DOC "Path to clang-tidy executable"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CLANG_TIDY
        DEFAULT_MSG
        CLANG_TIDY_EXE)

SET_PACKAGE_PROPERTIES(CLANG_TIDY PROPERTIES
        URL https://clang.llvm.org/extra/clang-tidy/
        DESCRIPTION "tool for diagnosing and fixing typical programming errors"
        )