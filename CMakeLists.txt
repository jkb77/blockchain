cmake_minimum_required(VERSION 3.8.2)
project(BC)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(sodium)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-fsanitize=address -g3")

set(SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/miner.cpp"
    "${PROJECT_SOURCE_DIR}/src/blockchain.cpp"
    "${PROJECT_SOURCE_DIR}/src/wallet.cpp"
    "${PROJECT_SOURCE_DIR}/src/utxo.cpp"
    "${PROJECT_SOURCE_DIR}/src/transaction.cpp")
set(TESTS "${PROJECT_SOURCE_DIR}/tests/main.cpp"
    "${PROJECT_SOURCE_DIR}/tests/wallet.cpp")
set(SOURCE_TESTS "${PROJECT_SOURCE_DIR}/src/miner.cpp"
    "${PROJECT_SOURCE_DIR}/src/blockchain.cpp"
    "${PROJECT_SOURCE_DIR}/src/wallet.cpp"
    "${PROJECT_SOURCE_DIR}/src/utxo.cpp"
    "${PROJECT_SOURCE_DIR}/src/transaction.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${sodium_LIBRARY_RELEASE})
add_subdirectory(${PROJECT_SOURCE_DIR}/googletest EXCLUDE_FROM_ALL)
add_executable(tests EXCLUDE_FROM_ALL ${TESTS} ${SOURCE_TESTS})
target_compile_options(tests PRIVATE ${WARN_FLAGS})
target_include_directories(tests
                           SYSTEM
                           PRIVATE "${PROJECT_SOURCE_DIR}/googletest/googletest/include"
                           PRIVATE "${PROJECT_SOURCE_DIR}/googletest/googlemock/include"
)
target_link_libraries(tests gtest gmock ${sodium_LIBRARY_RELEASE})
