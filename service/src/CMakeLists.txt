file (GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
cmake_print_variables(SOURCE_FILES)

add_executable(Service ${SOURCE_FILES})
target_include_directories(Service PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
