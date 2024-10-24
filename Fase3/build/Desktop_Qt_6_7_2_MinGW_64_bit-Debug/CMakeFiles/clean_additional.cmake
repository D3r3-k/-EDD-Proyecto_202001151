# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Fase2_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Fase2_autogen.dir\\ParseCache.txt"
  "Fase2_autogen"
  )
endif()
