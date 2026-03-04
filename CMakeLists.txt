cmake_minimum_required(VERSION 3.10)
project(proteus)

set(CMAKE_CXX_STANDARD 23)

set(SOURCES
	sources/application.cpp
    sources/options.cpp 
    sources/vector4.cpp
	sources/data_pool.cpp
	sources/utils.cpp
	sources/accessibility.cpp
)

add_executable(proteus 
    sources/main.cpp 
	${SOURCES}
)

enable_testing()
find_package(GTest REQUIRED)

add_executable(proteus_tests 
    sources/test/test_app.cpp
	${SOURCES}
)

target_link_libraries(proteus_tests GTest::GTest GTest::Main)
target_include_directories(proteus_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME ProteusTests COMMAND proteus_tests)

add_executable(proteus_asan     
	sources/main.cpp 
	${SOURCES}
)

target_compile_options(proteus_asan PRIVATE -O1 -g -fsanitize=address -fno-omit-frame-pointer)
target_link_options(proteus_asan PRIVATE -fsanitize=address)