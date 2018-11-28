cmake_minimum_required(VERSION 3.12)
project(HOGSMaker)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/zlib)


set(CMAKE_BUILD_TYPE Release)


#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


set(ZLIB_PRIVATE_HDRS
        zlib/crc32.h
        zlib/deflate.h
        zlib/gzguts.h
        zlib/inffast.h
        zlib/inffixed.h
        zlib/inflate.h
        zlib/inftrees.h
        zlib/trees.h
        zlib/zutil.h
        )
set(ZLIB_SRCS
        zlib/adler32.c
        zlib/compress.c
        zlib/crc32.c
        zlib/deflate.c
        zlib/gzclose.c
        zlib/gzlib.c
        zlib/gzread.c
        zlib/gzwrite.c
        zlib/inflate.c
        zlib/infback.c
        zlib/inftrees.c
        zlib/inffast.c
        zlib/trees.c
        zlib/uncompr.c
        zlib/zutil.c
        )


add_executable(HOGSMaker main.cpp CCExactSolver.cpp CCExactSolver.h Graph.cpp Graph.h
                TreeSource/trees/node.cpp TreeSource/trees/node.h
                TreeSource/trees/treeiterator.cpp TreeSource/trees/treeiterator.h
                TreeSource/trees/newicklex.cpp TreeSource/trees/newicklex.h
                TreeSource/trees/treeinfo.cpp TreeSource/trees/treeinfo.h
                TreeSource/trees/genespeciestreeutil.cpp TreeSource/trees/genespeciestreeutil.h
                TreeSource/div/define.h TreeSource/div/util.h HOGSMaker.cpp HOGSMaker.h TreeSource/div/Timer.cpp TreeSource/div/Timer.h OMAParser.cpp OMAParser.h
                zlib/gzstream.h zlib/gzstream.C
        ${ZLIB_PRIVATE_HDRS} ${ZLIB_SRCS})