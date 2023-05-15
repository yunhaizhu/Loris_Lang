cmake_minimum_required(VERSION 3.16)
set(PROJECT_CUSTOM_NAME "Loris_Lang")
project(${PROJECT_CUSTOM_NAME} C)

set(CMAKE_VERBOSE_MAKEFILE ON)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_COMPILER "gcc")
    message(" ")
    message("CMAKE IN DEBUG MODE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -O3 -Wall -Werror -fPIC -march=native ")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -DNDEBUG -O3 -Wall -Werror -fPIC -march=native")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-enum-compare -DDEBUG_JIT -fsanitize=address -fno-omit-frame-pointer")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(LINK_PROFILER "profiler")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_COMPILER "clang")
    message(" ")
    message("CMAKE IN RELEASE MODE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -Ofast -Wall -Werror -fPIC -march=native  -funroll-loops -mtune=native -flto -ftree-vectorize -flax-vector-conversions -fomit-frame-pointer -finline-functions")

#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -O3 -Wall -Werror -fPIC -march=native  -funroll-loops -mtune=native -flto  -ftree-vectorize -flax-vector-conversions -fomit-frame-pointer ")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -Ofast -Wall -Werror -fPIC -march=native  -funroll-loops -mtune=native -flto  -ftree-vectorize -flax-vector-conversions -fomit-frame-pointer -finline-functions -fprofile-instr-generate -fcoverage-mapping ")
#llvm-profdata merge -output=default.profdata default.profraw
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -Ofast -Wall -fPIC -march=native -Werror -Wno-profile-instr-unprofiled -Wno-profile-instr-out-of-date -funroll-loops -mtune=native -flto -ftree-vectorize -flax-vector-conversions -fomit-frame-pointer -finline-functions -fprofile-instr-use=/home/yunhai/CLionProject/Loris_Lang/deploy/Release/default.profdata ")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -O3 -Wall -Werror -fPIC -march=native -fno-stack-protector -funroll-loops -mtune=native -flto")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -D_USE_JIT_ -DSOFT_FPU -g -Wall -D_DEBUG -Werror -fPIC -fno-stack-protector -fno-common -march=native")
endif ()

set(CMAKE_ENABLE_EXPORTS ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

set(ENABLE_SELF_HEAL FALSE)
set(KEY_NAME_SIZE BUF_SIZE_128)
set(CMD_LINE_SIZE BUF_SIZE_256)
set(MAX_BODY_SIZE BUF_SIZE_8192*10)
set(LINE_BUF_SIZE BUF_SIZE_1024)
set(MAX_STRING_SIZE BUF_SIZE_4096)

#recursive may exceed this limit. BE CAREFUL.
set(RECURSIVE_LOOP_MAX 1024)
set(MAX_CODE 2000*10)
set(MAX_LABEL 200*5)
set(MAX_STACK 6000*2)
set(MAX_CODE_SIZE MAX_BODY_SIZE*10)

set(THREAD_MAX 128)

set(MAX_GEN_CODES 4000)
set(MAX_GEN_BUFFER 128)

set(EXEC_COMPILED_CODE 0)
set(DUMP_EXEC_CODE 0)

set(MAX_DEF_FUNC_COUNT 1024)
set(MAX_LOAD_LIB_COUNT 128)
set(MAX_REQUIRED_STATE_COUNT 128)

set(FAST_VAR_ENABLE 1)

# Configure a header file with preprocessor definitions
configure_file(${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/src/framework/config.h.in ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/src/framework/public/include/config.h)

add_subdirectory(src)
add_subdirectory(code_generator)

file(MAKE_DIRECTORY ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/dynamic_exec/src)
file(MAKE_DIRECTORY ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/dynamic_exec/obj)
file(MAKE_DIRECTORY ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/dynamic_exec/lib)
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/files/
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/)


