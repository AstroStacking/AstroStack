
INSTALL(FILES ${PROJECT_BINARY_DIR}/AstroStack/config.h
   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/AstroStack
    )

file(GLOB entries *)
foreach(entry ${entries})
  if(IS_DIRECTORY ${entry} AND EXISTS ${entry}/CMakeLists.txt)
    add_subdirectory(${entry})
  endif()
endforeach(entry)
