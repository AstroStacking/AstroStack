#
# CMake utilities for ATK
#
include(CMakeParseArguments)

function(stagedebug target)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(APPLE)
      add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND dsymutil $<CONFIG>/$<TARGET_FILE_NAME:${target}>
        MAIN_DEPENDENCY ${target}
        COMMENT "Staging dSYM for ${target}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM)
    endif(APPLE)
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endfunction(stagedebug)

function(SOURCE_GROUP_BY_FOLDER target)
  SET(SOURCE_GROUP_DELIMITER "/")

  SET(last_dir "")
  SET(files "")
  FOREACH(file ${${target}_SRC} ${${target}_HEADERS}  ${${target}_DOC})
    file(RELATIVE_PATH relative_file "${CMAKE_CURRENT_SOURCE_DIR}" ${file})
    GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
    IF (NOT "${dir}" STREQUAL "${last_dir}")
      IF (files)
        SOURCE_GROUP("${last_dir}" FILES ${files})
      ENDIF (files)
      SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
  ENDFOREACH(file)
  IF (files)
    SOURCE_GROUP("${last_dir}" FILES ${files})
  ENDIF (files)
endfunction(SOURCE_GROUP_BY_FOLDER)

function(ASTRO_add_library PREFIX)

set(FLAGS )
set(SINGLEVALUES NAME FOLDER)
set(MULTIVALUES SRC HEADERS DOC DEFINITIONS INCLUDE LIBRARIES)

cmake_parse_arguments(${PREFIX}
                 "${FLAGS}"
                 "${SINGLEVALUES}"
                 "${MULTIVALUES}"
                ${ARGN})

SOURCE_GROUP_BY_FOLDER(${PREFIX})

if(NOT ${PREFIX}_NAME)
  message(ERROR "No name set for ${PREFIX}")
endif(NOT ${PREFIX}_NAME)

qt_add_library(${${PREFIX}_NAME}
    SHARED
        ${${PREFIX}_SRC} ${${PREFIX}_HEADERS} ${${PREFIX}_DOC}
)

target_compile_definitions(${${PREFIX}_NAME} PRIVATE ${${PREFIX}_DEFINITIONS} -DBUILD_${PREFIX})
target_include_directories(${${PREFIX}_NAME} PRIVATE ${${PREFIX}_INCLUDE})
target_include_directories(${${PREFIX}_NAME} BEFORE PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${${PREFIX}_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${${PREFIX}_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${${PREFIX}_NAME} PUBLIC ${${PREFIX}_LIBRARIES})

set_target_properties (${${PREFIX}_NAME} PROPERTIES
    FOLDER ${${PREFIX}_FOLDER}
)

stagedebug(${${PREFIX}_NAME})

if(APPLE)
INSTALL(TARGETS ${${PREFIX}_NAME}
    EXPORT ASTRO
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.app/Contents/Frameworks COMPONENT libraries
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.app/Contents/Frameworks COMPONENT libraries
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.app/Contents/Frameworks COMPONENT libraries
)
else()
INSTALL(TARGETS ${${PREFIX}_NAME}
    EXPORT ASTRO
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/ COMPONENT libraries
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/ COMPONENT libraries
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/ COMPONENT libraries
)
endif()
endfunction()

function(ASTRO_add_plugin PREFIX)

set(FLAGS )
set(SINGLEVALUES NAME FOLDER)
set(MULTIVALUES SRC HEADERS DOC DEFINITIONS INCLUDE LIBRARIES)

cmake_parse_arguments(${PREFIX}
                 "${FLAGS}"
                 "${SINGLEVALUES}"
                 "${MULTIVALUES}"
                ${ARGN})

SOURCE_GROUP_BY_FOLDER(${PREFIX})

if(NOT ${PREFIX}_NAME)
  message(ERROR "No name set for ${PREFIX}")
endif(NOT ${PREFIX}_NAME)

qt_add_plugin(${${PREFIX}_NAME}
    STATIC
)
target_sources(${${PREFIX}_NAME} PRIVATE ${${PREFIX}_SRC} ${${PREFIX}_HEADERS} ${${PREFIX}_DOC})

target_compile_definitions(${${PREFIX}_NAME} PRIVATE ${${PREFIX}_DEFINITIONS})
target_include_directories(${${PREFIX}_NAME} PRIVATE ${${PREFIX}_INCLUDE})
target_include_directories(${${PREFIX}_NAME} BEFORE PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${${PREFIX}_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${${PREFIX}_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${${PREFIX}_NAME} PUBLIC ${${PREFIX}_LIBRARIES})

set_target_properties (${${PREFIX}_NAME} PROPERTIES
    FOLDER ${${PREFIX}_FOLDER}
)

stagedebug(${${PREFIX}_NAME})

if(APPLE)
INSTALL(TARGETS ${${PREFIX}_NAME}
    EXPORT ASTRO
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.app/plugins COMPONENT libraries
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.app/plugins COMPONENT libraries
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.app/plugins COMPONENT libraries
)
else()
INSTALL(TARGETS ${${PREFIX}_NAME}
    EXPORT ASTRO
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins COMPONENT libraries
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins COMPONENT libraries
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins COMPONENT libraries
)
endif()
endfunction()

function(ASTRO_add_executable PREFIX)

set(FLAGS INSTALL BUNDLE)
set(SINGLEVALUES NAME FOLDER)
set(MULTIVALUES SRC HEADERS DOC DEFINITIONS INCLUDE LIBRARIES)

cmake_parse_arguments(${PREFIX}
                 "${FLAGS}"
                 "${SINGLEVALUES}"
                 "${MULTIVALUES}"
                ${ARGN})

SOURCE_GROUP_BY_FOLDER(${PREFIX})

if(NOT ${PREFIX}_NAME)
  message(ERROR "No name set for ${PREFIX}")
endif(NOT ${PREFIX}_NAME)

add_definitions(${${PREFIX}_DEFINITIONS})

include_directories(${PROJECT_SOURCE_DIR} ${${PREFIX}_INCLUDE})

qt_add_executable(${${PREFIX}_NAME}
  ${${PREFIX}_SRC} ${${PREFIX}_HEADERS} ${${PREFIX}_DOC}
)

if(${PREFIX}_FOLDER)
  set_target_properties (${${PREFIX}_NAME} PROPERTIES
    FOLDER ${${PREFIX}_FOLDER}
  )
endif(${PREFIX}_FOLDER)

target_include_directories(${${PREFIX}_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${${PREFIX}_NAME} PUBLIC ${${PREFIX}_LIBRARIES})

set_target_properties(${${PREFIX}_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE ${${PREFIX}_BUNDLE}
)

INSTALL(TARGETS ${${PREFIX}_NAME}
    EXPORT ASTRO
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin COMPONENT app
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib COMPONENT app
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib COMPONENT app
    BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT app
)

endfunction()

function(ASTRO_add_test PREFIX)

set(FLAGS)
set(SINGLEVALUES NAME TESTNAME WORKING_DIRECTORY FOLDER)
set(MULTIVALUES SRC HEADERS DEFINITIONS INCLUDE LIBRARIES)

cmake_parse_arguments(${PREFIX}
                 "${FLAGS}"
                 "${SINGLEVALUES}"
                 "${MULTIVALUES}"
                ${ARGN})

if(NOT ${PREFIX}_TESTNAME)
  message(ERROR "No test name set for ${PREFIX}")
endif(NOT ${PREFIX}_TESTNAME)
if(NOT ${PREFIX}_WORKING_DIRECTORY)
  set(${PREFIX}_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

ASTRO_add_executable(${PREFIX}
  NAME ${${PREFIX}_NAME}
  FOLDER ${${PREFIX}_FOLDER}
  SRC ${${PREFIX}_SRC}
  HEADERS ${${PREFIX}_HEADERS}
  DEFINITIONS ${${PREFIX}_DEFINITIONS}
  INCLUDE ${${PREFIX}_INCLUDE}
  LIBRARIES ${${PREFIX}_LIBRARIES} GTest::gtest_main GTest::gtest
)

if(ENABLE_TEST_DISCOVERY)
  gtest_discover_tests(${${PREFIX}_NAME}
    TEST_PREFIX ${${PREFIX}_TESTNAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DISCOVERY_MODE PRE_TEST
  )
else()
  add_test(NAME ${${PREFIX}_TESTNAME}
     COMMAND ${${PREFIX}_NAME}
     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()

set_target_properties(${${PREFIX}_NAME} PROPERTIES
    MACOSX_BUNDLE FALSE
)

endfunction()

include(CheckCXXCompilerFlag)

if(CMAKE_CXX_COMPILER_ID MATCHES "Apple")
  include(apple-clang)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  include(clang)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  include(gcc)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
  if(MSVC)
    include(intel-msvc)
  else()
    include(intel-gcc)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  include(msvc)
else()
  message(FATAL_ERROR "Compiler '${CMAKE_CXX_COMPILER_ID}' not recognized")
endif()

