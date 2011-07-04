# - Try to find PACC
# Once done this will define
#
#  PACC_FOUND - system has PACC
#  PACC_INCLUDE_DIRS - the PACC include directory
#  PACC_LIBRARIES - Link these to use PACC
#  PACC_DEFINITIONS - Compiler switches required for using PACC
#
#  Copyright (c) 2009 Andreas Schneider <mail@cynapses.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (PACC_LIBRARIES AND PACC_INCLUDE_DIRS)
  # in cache already
  set(PACC_FOUND TRUE)
else (PACC_LIBRARIES AND PACC_INCLUDE_DIRS)
  find_path(PACC_INCLUDE_DIRS
    NAMES
      PACC.hpp
    PATHS
		~/local/include
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
  )

  find_library(PACC_LIBRARIES
    NAMES
        pacc
    PATHS
		~/local/lib
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
  )

  if (LIBPACC_LIBRARY)
    set(LIBPACC_FOUND TRUE)
  endif (LIBPACC_LIBRARY)

  set(PACC_INCLUDE_DIRS
    ${PACC_INCLUDE_DIR}
  )

  if (LIBPACC_FOUND)
    set(PACC_LIBRARIES
      ${PACC_LIBRARIES}
      ${LIBPACC_LIBRARY}
    )
  endif (LIBPACC_FOUND)

  if (PACC_INCLUDE_DIRS AND PACC_LIBRARIES)
     set(PACC_FOUND TRUE)
  endif (PACC_INCLUDE_DIRS AND PACC_LIBRARIES)

  if (PACC_FOUND)
    if (NOT PACC_FIND_QUIETLY)
      message(STATUS "Found PACC: ${PACC_LIBRARIES}")
    endif (NOT PACC_FIND_QUIETLY)
  else (PACC_FOUND)
    if (PACC_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find PACC")
    endif (PACC_FIND_REQUIRED)
  endif (PACC_FOUND)

endif (PACC_LIBRARIES AND PACC_INCLUDE_DIRS)

