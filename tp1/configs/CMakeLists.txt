cmake_minimum_required (VERSION 3.0)
# Name our project
project (sat)
# Add all the *.c files in our source directory to our executable output
FILE(GLOB SRC_FILES *.c)
add_executable(sat.o ${SRC_FILES})
