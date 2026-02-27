project(Proteus)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 23)

set(SOURCE_LIST sources/main.cpp
				sources/utils.cpp
				sources/options.cpp)

add_executable(${PROJECT_NAME}
			   ${SOURCE_LIST})