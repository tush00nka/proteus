project(Proteus)

set (CMAKE_CXX_STANDARD 17)

set(SOURCE_LIST sources/main.cpp
				sources/utils.cpp
				sources/options.cpp)

add_executable(${PROJECT_NAME}
			   ${SOURCE_LIST})