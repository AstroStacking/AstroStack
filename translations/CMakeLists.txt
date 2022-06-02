
FILE(GLOB CMAKE_TS_SRC *.ts)
SOURCE_GROUP_BY_FOLDER(CMAKE_TS_SRC)
add_custom_target(Translations SOURCES ${CMAKE_TS_SRC} CMakeLists.txt)

qt_add_translations(AstroStack
    TS_FILES ${CMAKE_TS_SRC}
    QM_FILES_OUTPUT_VARIABLE TRANSLATION_QM
    )
set_source_files_properties(${CMAKE_TS_SRC}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/translations")
