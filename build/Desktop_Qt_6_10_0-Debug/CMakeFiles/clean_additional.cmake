# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Stickman_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Stickman_autogen.dir\\ParseCache.txt"
  "Stickman_autogen"
  )
endif()
