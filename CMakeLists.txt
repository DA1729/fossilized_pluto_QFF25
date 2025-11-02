cmake_minimum_required(VERSION 3.10)
project(quantum_fhe_sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_executable(quantum_fhe_sim
    src/main.cpp
    src/quantum/qubit_register.cpp
    src/quantum/gates.cpp
    src/quantum/qft.cpp
    src/fhe/fhe_engine.cpp
    src/quantum_accelerated_ops.cpp
)

target_include_directories(quantum_fhe_sim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/quantum
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fhe
)
