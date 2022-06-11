
FILE(GLOB CMAKE_WORKFLOWS_SRC *.json)
SOURCE_GROUP_BY_FOLDER(CMAKE_WORKFLOWS_SRC)
add_custom_target(Workflows SOURCES ${CMAKE_WORKFLOWS_SRC} CMakeLists.txt)

set_source_files_properties(${CMAKE_WORKFLOWS_SRC}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/workflows")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src/Main/workflows/)
add_custom_target(
    CopyWorkflows
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_WORKFLOWS_SRC}
            ${CMAKE_BINARY_DIR}/src/Main/workflows/)
add_dependencies(AstroStack CopyWorkflows)

if(APPLE)
INSTALL(FILES ${CMAKE_WORKFLOWS_SRC}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.app/workflows/
)
else()
INSTALL(FILES ${CMAKE_WORKFLOWS_SRC}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/workflows/
)
endif()
