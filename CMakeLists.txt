cmake_minimum_required(VERSION 3.21.3)

include(CheckFunctionExists)

project(kat VERSION 0.8.1 LANGUAGES C)

if(MSVC)
  message(FATAL_ERROR "Doesn't work in MSVC yet")
else()
  add_compile_options(-Wall -Wextra -O3)
endif()

# function checks
function(check_func func_name var)
  check_function_exists(${func_name} ${var})
  if(NOT ${var})
    set(${var} 0 PARENT_SCOPE)
  else()
    set(${var} 1 PARENT_SCOPE)
  endif()
endfunction()

# checks
check_func(getopt_long GETOPT_LONG_EXISTS)
check_func(fprintf FPRINTF_EXISTS)

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


# CPack
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${kat_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${kat_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${kat_VERSION_PATCH}")
include(CPack)


