add_library(mod_lang_compile_II SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_lang_compile_II.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/mod_lang_compile_II_bundle.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/stub/stub_shell_mod_lang_compile_II.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/ast_code.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/ast_compile.c
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/lang_ast.c
        )

target_include_directories(mod_lang_compile_II PRIVATE
        ${SUB_PUBLIC_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/private/include)

target_link_libraries(mod_lang_compile_II PRIVATE pthread ${LINK_PROFILER})

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/private/script/mod_lang_compile_II_test.nl
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/script/ )

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/public/desc.json
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_lang_compile_II/ )

INSTALL(TARGETS mod_lang_compile_II
        DESTINATION ${${PROJECT_CUSTOM_NAME}_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE}/bundle/mod_lang_compile_II/ )
