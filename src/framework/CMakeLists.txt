# Define a CMake variable
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

set(EXEC_COMPILED_CODE 1)
set(DUMP_EXEC_CODE 1)

set(MAX_DEF_FUNC_COUNT 1024)
set(MAX_LOAD_LIB_COUNT 128)
set(MAX_REQUIRED_STATE_COUNT 128)

# Configure a header file with preprocessor definitions
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/public/include/config.h)

add_executable(${PROJECT_CUSTOM_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_ownership.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_bundle.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/std_random.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/std_safe.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/std_lock_free_list.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/std_hazard_ptr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/std_lock_free_key_hash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/std_log.c
        ${CMAKE_CURRENT_SOURCE_DIR}/public/src/rsa.o
        ${CMAKE_CURRENT_SOURCE_DIR}/public/src/json-maker.o
        ${CMAKE_CURRENT_SOURCE_DIR}/public/src/tiny-json.o
        )

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/public/src/rsa.o
        ${CMAKE_CURRENT_SOURCE_DIR}/public/src/json-maker.o
        ${CMAKE_CURRENT_SOURCE_DIR}/public/src/tiny-json.o
        COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/public/src
)

target_include_directories(${PROJECT_CUSTOM_NAME} PRIVATE
        ${SUB_PUBLIC_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/private/include)

target_link_libraries(${PROJECT_CUSTOM_NAME} PUBLIC gsl gslcblas m dl rt pthread ${LINK_PROFILER})

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/private/script/
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/script/)

INSTALL(TARGETS ${PROJECT_CUSTOM_NAME} DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/)




