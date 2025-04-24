cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_ITERATOR_DEBUG_LEVEL=0) 

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

project(redis_lite)

add_executable(${PROJECT_NAME} main.cpp "test_step1.cpp"  "simple_string.cpp" "error.cpp" "RedisInteger.cpp" "BulkString.cpp" "RedisArray.cpp")

target_link_libraries(${PROJECT_NAME} GTest::gtest_main)

include(GoogleTest)
#gtest_discover_tests(${PROJECT_NAME})