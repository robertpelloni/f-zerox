cmake_minimum_required(VERSION 3.10)
project(fzerox_pc C)

set(CMAKE_C_STANDARD 99)
add_definitions(-D_LANGUAGE_C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

# Attempt to find SDL2 and OpenGL
find_package(SDL2 QUIET)
find_package(OpenGL QUIET)

include_directories(include/pc include)

# Source files from Makefile.pc
file(GLOB PC_SRCS 
    "src/pc/*.c" 
    "src/pc/sdl2/*.c" 
    "src/pc/ui/*.c" 
    "src/pc/ui/tabs/*.c"
)

set(GAME_SRCS 
    src/math_utils.c 
    src/debug_text.c 
    src/audio_state.c 
    src/audio_util.c
)

if(PC_SRCS OR GAME_SRCS)
    add_executable(fzerox_pc ${PC_SRCS} ${GAME_SRCS})
    
    if(SDL2_FOUND)
        target_link_libraries(fzerox_pc ${SDL2_LIBRARIES})
        target_include_directories(fzerox_pc PRIVATE ${SDL2_INCLUDE_DIRS})
    endif()
    if(OPENGL_FOUND)
        target_link_libraries(fzerox_pc OpenGL::GL)
    endif()
endif()

# Test runner
if(EXISTS "tests/test_math_utils.c" AND EXISTS "tests/stubs.c")
    add_executable(test_runner tests/test_math_utils.c tests/stubs.c src/math_utils.c src/pc/ultra_impl.c)
    target_include_directories(test_runner PRIVATE include/pc include)
endif()
