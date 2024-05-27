# FindCMocka.cmake

# Attemp to find the CMocka library
# Define the following variables:
#   CMOCKA_FOUND - system has CMocka
#   CMOCKA_INCLUDE_DIRS - the CMocka include directories
#   CMOCKA_LIBRARIES - the libraries needed to use CMocka

find_path(CMOCKA_INCLUDE_DIR
  NAMES cmocka.h
  HINTS
  /usr/include
  /usr/local/include
  /opt/local/include
  /opt/homebrew/include)

find_library(CMOCKA_LIBRARY
  NAMES cmocka
  HINTS
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  /opt/homebrew/lib)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(CMocka
  DEFAULT_MSG
  CMOCKA_LIBRARY
  CMOCKA_INCLUDE_DIR)

if (CMOCKA_FOUND)
  set(CMOCKA_INCLUDE_DIRS ${CMOCKA_INCLUDE_DIR})
  set(CMOCKA_LIBRARIES ${CMOCKA_LIBRARY})
endif()

mark_as_advanced(CMOCKA_INCLUDE_DIR CMOCKA_LIBRARY)
