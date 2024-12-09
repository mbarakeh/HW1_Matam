cmake_minimum_required(VERSION 3.25)
project(HW1_Matam)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(HW1_Matam
        BlockChain.cpp
        mtm_blockchain.cpp
        Transaction.cpp
        Utilities.cpp
        )


include_directories(${CMAKE_SOURCE_DIR})

# Enable testing if required
enable_testing()