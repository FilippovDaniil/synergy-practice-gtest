cmake_minimum_required(VERSION 3.20)
project(synergy_practice_gtest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# googletest через FetchContent (ручная установка не нужна)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
)
# MSVC: использовать тот же CRT, что и проект
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_executable(ds_tests
    tests/smoke_test.cpp
)
target_link_libraries(ds_tests PRIVATE GTest::gtest_main)

gtest_discover_tests(ds_tests DISCOVERY_TIMEOUT 60)
