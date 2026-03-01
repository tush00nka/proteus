cmake_minimum_required(VERSION 3.10)
project(proteus)

set(CMAKE_CXX_STANDARD 23)

add_executable(proteus 
    sources/main.cpp 
	sources/application.cpp
    sources/options.cpp 
    sources/vector4.cpp
	sources/utils.cpp
)

enable_testing()
find_package(GTest REQUIRED)

add_executable(proteus_tests 
    sources/test/test_app.cpp
	sources/application.cpp
    sources/options.cpp 
    sources/vector4.cpp
	sources/utils.cpp
)

target_link_libraries(proteus_tests GTest::GTest GTest::Main)
target_include_directories(proteus_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME ProteusTests COMMAND proteus_tests)