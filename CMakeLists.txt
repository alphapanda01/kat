cmake_minimum_required(VERSION 3.21.3)

project(kat VERSION 0.8.1 LANGUAGES C)

if(MSVC)
  message(FATAL_ERROR "Doesn't work in MSVC yet")
else()
  add_compile_options(-Wall -Wextra -O3)
endif()

file(GLOB Kat_SRC "src/*.c")

add_executable(${PROJECT_NAME} ${Kat_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC src/)

include_directories(${PROJECT_BINARY_DIR})

configure_file("src/KatConfig.h.in" "${PROJECT_BINARY_DIR}/KatConfig.h")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/run_tests.sh.in" "${CMAKE_CURRENT_SOURCE_DIR}/tests/run_tests.sh")

install(TARGETS kat DESTINATION bin) # You sure?

# Taken from CMake doc

enable_testing()

# check if the application runs
add_test(NAME RunCheck COMMAND kat -v)

#verify the help message
add_test(NAME Help COMMAND kat -h)
#set_tests_properties(Help PROPERTIES PASS_REGULAR_EXPRESSION
#  "Usage: (.*)kat \[OPTION\]\.\.\. \[FILE\]\.\.\.\n[.]*") # I'm lazy

function(do_test target arg)
  add_test(NAME test${arg} COMMAND ${target} ${arg})
endfunction(do_test)

do_test(kat "${CMAKE_CURRENT_SOURCE_DIR}/tests/saitama.txt")

