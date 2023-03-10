cmake_minimum_required(VERSION 3.19)
project(archiver)

set(CMAKE_CXX_STANDARD 20)

add_executable(archiver main.cpp BufferedReader.cpp BufferedReader.h BufferedWriter.cpp BufferedWriter.h BinHeap.cpp BinHeap.h Huffman.cpp Huffman.h)

