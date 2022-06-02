cmake_minimum_required(VERSION 3.22)
project(AstroStack VERSION 1.0.0 LANGUAGES C CXX)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ITK_NO_IO_FACTORY_REGISTER_MANAGER ON)

option(ENABLE_TEST_DISCOVERY "Activate test discovery in ctest" ON)

find_package(Git REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts Linguist)
find_package(ITK REQUIRED COMPONENTS ITKIOImageBase ITKIOJPEG ITKIOPNG ITKIOTIFF ITKStatistics)
find_package(GTest REQUIRED)

set(ITK_FACTORY_LIST ImageIO)
include(${ITK_USE_FILE})

qt_standard_project_setup()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "src/config.h.in"
  "src/config.h" @ONLY
  )

include(Utilities)

add_subdirectory(src)

FILE(GLOB CMAKE_TS_SRC *.ts)
SOURCE_GROUP_BY_FOLDER(CMAKE_TS_SRC)
add_custom_target(Translations SOURCES ${CMAKE_TS_SRC})

FILE(GLOB TRANSLATION_SRC *.ts)
qt_add_translations(AstroStack
    TS_FILES ${TRANSLATION_SRC}
    QM_FILES_OUTPUT_VARIABLE TRANSLATION_QM
    )
set_source_files_properties(${TRANSLATION_SRC}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/translations")

FILE(GLOB CMAKE_OTHER_SRC .clang-format CMake/*.cmake *.in src/*.in src/*.txt *.yml *.md scripts/*.sh scripts/*.bat CMakeLists.txt)
SOURCE_GROUP_BY_FOLDER(CMAKE_OTHER)
add_custom_target(CMake SOURCES ${CMAKE_OTHER_SRC})

# The following script must only be executed at install time
set(deploy_script "${CMAKE_CURRENT_BINARY_DIR}/deploy_AstroStack.cmake")

#file(GENERATE OUTPUT ${deploy_script} CONTENT "
# Including the file pointed to by QT_DEPLOY_SUPPORT ensures the generated
# deployment script has access to qt_deploy_runtime_dependencies()
#include(\"${QT_DEPLOY_SUPPORT}\")
#qt_deploy_runtime_dependencies(
#    EXECUTABLE \"${executable_path}\"
#    ADDITIONAL_EXECUTABLES \"${helper_app_path}\"
#    GENERATE_QT_CONF
#    VERBOSE
#)")
#install(SCRIPT ${deploy_script})    # Add its runtime dependencies
