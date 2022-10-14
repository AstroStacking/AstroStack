cmake_minimum_required(VERSION 3.22)
set(ASTROSTACK_VERSION 0.2.0)
project(AstroStack VERSION ${ASTROSTACK_VERSION} LANGUAGES C CXX)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ITK_NO_IO_FACTORY_REGISTER_MANAGER ON)

option(ENABLE_TEST_DISCOVERY "Activate test discovery in ctest" ON)
option(ENABLE_EXIV2 "Enable exiv2 support" OFF)

find_package(Git REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts Linguist)
find_package(ITK REQUIRED COMPONENTS ITKIOImageBase ITKIOJPEG ITKIOPNG ITKIOTIFF ITKStatistics ITKFFT)
find_package(HDF5 COMPONENTS CXX)
find_package(Eigen3 REQUIRED)
find_package(GTest REQUIRED)

if(ENABLE_EXIV2)
    find_package(Exiv2)
endif()
if(TARGET exiv2lib)
    SET(EXIV2_FOUND 1)
else()
    SET(EXIV2_FOUND 0)
endif()

configure_file (
  "src/config.h.in"
  "src/config.h" @ONLY
  )

include_directories(${CMAKE_BINARY_DIR}/src)

set(ITK_FACTORY_LIST ImageIO)
include(${ITK_USE_FILE})

qt_standard_project_setup()

include(Utilities)

add_subdirectory(src)
add_subdirectory(translations)
add_subdirectory(workflows)

FILE(GLOB CMAKE_OTHER_SRC .clang-format *.py CMake/*.cmake src/*.in src/*.in src/*.txt *.yml *.md scripts/*.sh scripts/*.bat CMakeLists.txt)
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
