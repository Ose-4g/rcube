cmake_minimum_required(VERSION 3.26)
project(rcube)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Recursively Get all .cpp
file(GLOB ALL_CPP_FILES ./*.cpp)

# Remove test files (*.t.cpp)
foreach(FILE ${ALL_CPP_FILES})
    if(NOT FILE MATCHES "\\.t\\.cpp$")
        list(APPEND CPP_FILES ${FILE})
    endif()
endforeach()

FetchContent_Declare(
  commandprocessor
  GIT_REPOSITORY https://github.com/Ose-4g/command-processor.git
  GIT_TAG v1.3.1
)

FetchContent_MakeAvailable(commandprocessor)
add_executable(rcube ${CPP_FILES})
target_link_libraries(rcube commandprocessor)


FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(gtest_capture no)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Remove main files (*.m.cpp)
foreach(FILE ${ALL_CPP_FILES})
    if(NOT FILE MATCHES "\\.m\\.cpp$")
        list(APPEND TEST_FILES ${FILE})
    endif()
endforeach()

add_executable(
  rcubetest
  ${TEST_FILES}
)

target_link_libraries(
  rcubetest
  gtest
  gmock
)

include(GoogleTest)
gtest_discover_tests(rcubetest)