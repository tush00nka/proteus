project(Proteus)

set (CMAKE_CXX_STANDARD 17)

set(SOURCE_LIST sources/main.cpp)

add_executable(${PROJECT_NAME}
			   ${SOURCE_LIST})