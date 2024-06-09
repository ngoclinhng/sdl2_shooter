# FindSDL2_mixer.cmake

# Attemp to find the SDL2_mixer library
# Define the folloeing variables:
#   SDL2_MIXER_FOUND - system has SDL2_mixer
#   SDL2_MIXER_INCLUDE_DIRS - the SDL2_mixer include directories
#   SDL2_MIXER_LIBRARIES - the libraries needed to use SDL2_mixer

set(HOMEBREW_INCLUDE_SEARCH_PATHS "")
set(HOMEBREW_LIB_SEARCH_PATHS "")

if (APPLE)
  if(EXISTS /opt/homebrew/include AND EXISTS /opt/homebrew/lib)
    list(APPEND HOMEBREW_INCLUDE_SEARCH_PATHS /opt/homebrew/include)
    list(APPEND HOMEBREW_LIB_SEARCH_PATHS /opt/homebrew/lib)
  endif()

  if(EXISTS /usr/local/include AND EXISTS /usr/local/lib)
    list(APPEND HOMEBREW_INCLUDE_SEARCH_PATHS /usr/local/include)
    list(APPEND HOMEBREW_LIB_SEARCH_PATHS /usr/local/lib)
  endif()
endif()

find_path(SDL2_MIXER_INCLUDE_DIR SDL2/SDL_mixer.h HINTS
  ${HOMEBREW_INCLUDE_SEARCH_PATHS}
  /usr/include
  /usr/local/include
  /opt/local/include)

find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer HINTS
  ${HOMEBREW_LIB_SEARCH_PATHS}
  /usr/lib
  /usr/local/lib
  /opt/local/lib)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SDL2_mixer
  DEFAULT_MSG
  SDL2_MIXER_LIBRARY
  SDL2_MIXER_INCLUDE_DIR)

if(SDL2_MIXER_FOUND)
  set(SDL2_MIXER_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIR})
  set(SDL2_MIXER_LIBRARIES ${SDL2_MIXER_LIBRARY})
endif()

mark_as_advanced(SDL2_MIXER_INCLUDE_DIR SDL2_MIXER_LIBRARY)

