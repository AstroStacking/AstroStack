FILE(GLOB
    ASTRO_MAIN_SRC
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

FILE(GLOB
    ASTRO_MAIN_HEADERS
    mainwindow.h
)

ASTRO_add_executable(AstroStack
  NAME AstroStack
  FOLDER Main
  SRC ${ASTRO_MAIN_SRC}
  HEADERS ${ASTRO_MAIN_HEADERS}
  LIBRARIES Qt6::Widgets
)

set_target_properties(AstroStack PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
