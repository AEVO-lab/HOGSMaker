cmake_minimum_required(VERSION 3.12)
project(PACClustering)

set(CMAKE_CXX_STANDARD 11)

add_executable(PACC main.cpp CCExactSolver.cpp CCExactSolver.h Graph.cpp Graph.h util.h)