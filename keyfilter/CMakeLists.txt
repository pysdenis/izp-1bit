cmake_minimum_required(VERSION 4.0)
project(keyfilter C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

add_executable(keyfilter keyfilter.c)
# zapneme warnings a error při warningu
