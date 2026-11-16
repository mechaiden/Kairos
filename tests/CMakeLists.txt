include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(kairos_tests
    distributions.cpp
    root_finding.cpp
    matrix.cpp
)
target_link_libraries(kairos_tests PRIVATE kairos_core kairos_warnings GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(kairos_tests)
