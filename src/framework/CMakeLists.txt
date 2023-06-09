# Define a CMake variable


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




