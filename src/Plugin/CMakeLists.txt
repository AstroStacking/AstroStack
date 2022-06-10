FILE(GLOB ASTRO_PLUGIN_SRC
    Plugin/pluginfactory.cpp
)

FILE(GLOB ASTRO_PLUGIN_HEADERS
    Plugin/config.h
    Plugin/pluginfactory.h
)

ASTRO_add_library(
    AstroPlugin
    NAME AstroPlugin
    FOLDER Plugin
    SRC ${ASTRO_PLUGIN_SRC}
    HEADERS ${ASTRO_PLUGIN_HEADERS}
    LIBRARIES Qt6::Core Qt6::Widgets ${ITK_LIBRARIES}
)
