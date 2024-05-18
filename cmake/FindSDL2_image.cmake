# FindSDL2_image.cmake

# Attemp to find the SDL2_image library
# Define the following variables:
#   SDL2_IMAGE_FOUND - system has SDL2_image
#   SDL2_IMAGE_INCLUDE_DIRS - the SDL2_image include directories
#   SDL2_IMAGE_LIBRARIES - the libraries needed to use SDL2_image

# Homebrew specific path for Intel and Apple Silicon Macs
set(SDL2_IMAGE_SEARCH_PATHS "")
if (APPLE)
  # Check for Apple Silicon Homebrew path
  if(EXISTS /opt/homebrew/include AND EXISTS /opt/homebrew/lib)
    list(APPEND SDL2_IMAGE_SEARCH_PATHS
      /opt/homebrew/include
      /opt/homebrew/lib)
  endif()

  # Check for Intel Mac Homebrew path
  if (EXISTS /usr/local/include AND EXISTS /usr/local/lib)
    list(APPEND SDL2_IMAGE_SEARCH_PATHS
      /usr/local/include
      /usr/local/lib)
  endif()
endif()

find_path(SDL2_IMAGE_INCLUDE_DIR SDL2/SDL_image.h HINTS
  $ENV{SDL2_IMAGE_DIR}/include
  ${SDL2_PATH}/include
  ${SDL2_IMAGE_SEARCH_PATHS}
  /usr/include
  /usr/local/include
  /opt/local/include)

find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image HINTS
  $ENV{SDL2_IMAGE_DIR}/lib
  ${SDL2_PATH}/lib
  ${SDL2_IMAGE_SEARCH_PATHS}
  /usr/lib
  /usr/local/lib
  /opt/local/lib)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SDL2_image
  DEFAULT_MSG
  SDL2_IMAGE_LIBRARY
  SDL2_IMAGE_INCLUDE_DIR)

if(SDL2_IMAGE_FOUND)
  set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIR})
  set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_LIBRARY})
endif()

mark_as_advanced(SDL2_IMAGE_INCLUDE_DIR SDL2_IMAGE_LIBRARY)
  
  
  
