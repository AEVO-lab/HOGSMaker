cmake_minimum_required(VERSION 3.12)
project(HOGSMaker)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

add_executable(HOGSMaker main.cpp CCExactSolver.cpp CCExactSolver.h Graph.cpp Graph.h
                TreeSource/trees/node.cpp TreeSource/trees/node.h
                TreeSource/trees/treeiterator.cpp TreeSource/trees/treeiterator.h
                TreeSource/trees/newicklex.cpp TreeSource/trees/newicklex.h
                TreeSource/trees/treeinfo.cpp TreeSource/trees/treeinfo.h
                TreeSource/div/define.h TreeSource/div/util.h HOGSMaker.cpp HOGSMaker.h TreeSource/div/Timer.cpp TreeSource/div/Timer.h TreeSource/div/FixedObjectPool.cpp TreeSource/div/FixedObjectPool.h)