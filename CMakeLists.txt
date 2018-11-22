cmake_minimum_required(VERSION 3.12)
project(HOGSMaker)

set(CMAKE_CXX_STANDARD 11)


#if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release)
#endif()

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(HOGSMaker main.cpp CCExactSolver.cpp CCExactSolver.h Graph.cpp Graph.h
                TreeSource/trees/node.cpp TreeSource/trees/node.h
                TreeSource/trees/treeiterator.cpp TreeSource/trees/treeiterator.h
                TreeSource/trees/newicklex.cpp TreeSource/trees/newicklex.h
                TreeSource/trees/treeinfo.cpp TreeSource/trees/treeinfo.h
                TreeSource/div/define.h TreeSource/div/util.h HOGSMaker.cpp HOGSMaker.h TreeSource/div/Timer.cpp TreeSource/div/Timer.h)