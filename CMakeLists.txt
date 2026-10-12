cmake_minimum_required(VERSION 3.20)
project(synergy_practice_gtest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ON  -> тесты гоняются на эталонной реализации (src/reference) -> зелёные
# OFF -> тесты гоняются на заглушках (src/stubs)                -> красные (TDD)
option(DS_USE_REFERENCE "Run tests against the reference implementation" ON)

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

# Интерфейсы (header-only)
add_library(ds_interfaces INTERFACE)
target_include_directories(ds_interfaces INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Реализация, на которой гоняются тесты: эталон или заглушки
add_library(ds_impl INTERFACE)
target_include_directories(ds_impl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ds_impl INTERFACE ds_interfaces)
if(DS_USE_REFERENCE)
    target_compile_definitions(ds_impl INTERFACE DS_USE_REFERENCE=1)
    message(STATUS "ds: tests run against REFERENCE implementation")
else()
    target_compile_definitions(ds_impl INTERFACE DS_USE_REFERENCE=0)
    message(STATUS "ds: tests run against STUBS (expected to fail)")
endif()

enable_testing()
include(GoogleTest)

add_executable(ds_tests
    tests/smoke_test.cpp
    tests/queue_test.cpp
    tests/heap_test.cpp
)
target_include_directories(ds_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(ds_tests PRIVATE ds_impl GTest::gtest_main)

if(MSVC)
    target_compile_options(ds_tests PRIVATE /W4 /utf-8)
else()
    target_compile_options(ds_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

gtest_discover_tests(ds_tests DISCOVERY_TIMEOUT 60)
