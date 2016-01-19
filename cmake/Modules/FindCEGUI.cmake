#
# Try to find CEGUI library and include path.
# Once done this will define
#
# CEGUI_INCLUDE_DIR
# CEGUI_LIBRARIES_DEBUG
# CEGUI_LIBRARIES_RELEASE
# CEGUI_DLLS_DEBUG
# CEGUI_DLLS_RELEASE
# 

if(MSVC14)
    set(CEGUI_DIR "${CMAKE_SOURCE_DIR}/libs/cegui/vs2015")
elseif(MSVC12)
    set(CEGUI_DIR "${CMAKE_SOURCE_DIR}/libs/cegui/vs2013")
endif(MSVC14)

MESSAGE("CEGUI_DIR is ${CEGUI_DIR}")

find_path(CEGUI_INCLUDE_DIR CEGUI/CEGUI.h
    PATHS
    "${CEGUI_DIR}/include"
)

MESSAGE("CEGUI_INCLUDE_DIR is ${CEGUI_INCLUDE_DIR}")

find_library(CEGUI_LIBRARY_BASE_DEBUG
  NAMES CEGUIBase-0_d
  PATHS
   "${CEGUI_DIR}/lib"
)

find_library(CEGUI_LIBRARY_BASE_RELEASE
  NAMES CEGUIBase-0
  PATHS
   "${CEGUI_DIR}/lib"
)

find_library(CEGUI_LIBRARY_GL_DEBUG
  NAMES CEGUIOpenGLRenderer-0_d
  PATHS
   "${CEGUI_DIR}/lib"
)

find_library(CEGUI_LIBRARY_GL_RELEASE
  NAMES CEGUIOpenGLRenderer-0
  PATHS
   "${CEGUI_DIR}/lib"
)

set(CEGUI_LIBRARIES_DEBUG "${CEGUI_LIBRARY_BASE_DEBUG};${CEGUI_LIBRARY_GL_DEBUG}")
list(REMOVE_DUPLICATES CEGUI_LIBRARIES_DEBUG)

set(CEGUI_LIBRARIES_RELEASE "${CEGUI_LIBRARY_BASE_RELEASE};${CEGUI_LIBRARY_GL_RELEASE}")
list(REMOVE_DUPLICATES CEGUI_LIBRARIES_RELEASE)

file(GLOB CEGUI_DLLS_DEBUG
  "${CEGUI_DIR}/bin/*_d.dll"
  "${CEGUI_DIR}/dependencies/bin/*_d.dll"
)

file(GLOB CEGUI_DLLS_RELEASE
  "${CEGUI_DIR}/bin/*[^_d].dll"
  "${CEGUI_DIR}/dependencies/bin/*[^_d].dll"
)
MESSAGE("CEGUI_DLLS_RELEASE is ${CEGUI_DLLS_RELEASE}")

MESSAGE("CEGUI_LIBRARIES_DEBUG is ${CEGUI_LIBRARIES_DEBUG}")
MESSAGE("CEGUI_LIBRARIES_RELEASE is ${CEGUI_LIBRARIES_RELEASE}")

