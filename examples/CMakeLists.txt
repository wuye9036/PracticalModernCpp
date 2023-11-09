cmake_minimum_required(VERSION 3.22)

project(PMCppSamples)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)

include(cmake/unifex/Findunifex.cmake)

function( sample_link_libraries target )
  target_link_libraries(${target} PRIVATE fmt::fmt-header-only)
  target_link_libraries(${target} PRIVATE range-v3 range-v3-meta range-v3::meta range-v3-concepts)
  target_link_libraries(${target} PRIVATE Boost::boost)
  target_link_libraries(${target} PRIVATE GTest::gtest GTest::gtest_main)
  target_link_libraries(${target} PRIVATE absl::strings)
endfunction()

file(GLOB sources11 CONFIGURE_DEPENDS samples/*_Cpp11.cpp)
add_library(samples11 SHARED ${sources11})
# target_compile_features(samples11 PUBLIC cxx_std_11) doesn't work
target_compile_options(samples11 PRIVATE "-std=c++11")
sample_link_libraries(samples11)

file(GLOB sources14 CONFIGURE_DEPENDS *_Cpp14.cpp)
add_library(samples14 SHARED ${sources14})
target_compile_features(samples14 PUBLIC cxx_std_14)
sample_link_libraries(samples14)

# file(GLOB sources17 CONFIGURE_DEPENDS samples/*_Cpp17.cpp)
# add_library(samples17 SHARED ${sources17})
# target_compile_features(samples17 PUBLIC cxx_std_17)
# target_link_libraries(samples17 PRIVATE unifex::unifex)
# sample_link_libraries(samples17)

file(GLOB sources20 CONFIGURE_DEPENDS *_Cpp20.cpp)
add_library(samples20 SHARED ${sources20})
target_compile_features(samples20 PUBLIC cxx_std_20)
target_link_libraries(samples20 PRIVATE unifex::unifex)
sample_link_libraries(samples20)

file(GLOB sources23 CONFIGURE_DEPENDS *_Cpp23.cpp)
add_library(samples23 SHARED ${sources23})
target_compile_features(samples23 PUBLIC cxx_std_23)
sample_link_libraries(samples23)

file(GLOB sources CONFIGURE_DEPENDS main.cpp)
add_executable(samples ${sources})
target_compile_features(samples PUBLIC cxx_std_20)
target_link_libraries(samples PRIVATE samples11 samples14 samples20 samples23)
target_link_libraries(samples PRIVATE GTest::gtest GTest::gtest_main)