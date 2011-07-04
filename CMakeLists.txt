#***************************************************************************
#* (c) 2008-2011 Aleksandar Topuzović                                      *
#* <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
#***************************************************************************


cmake_minimum_required (VERSION 2.6)
project (gp-market-trading)

include_directories("${PROJECT_BINARY_DIR}")

# Set where to look for modules
set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake/Modules
)

# PACC
find_package(PACC REQUIRED)
include_directories(${PACC_INCLUDE_DIRS})
set(LIBS ${LIBS} ${PACC_LIBRARIES})

# OpenBeagle
find_package(OpenBeagle REQUIRED)
include_directories(${OPENBEAGLE_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OPENBEAGLE_LIBRARIES})


if(UNIX)
	add_definitions(-O2 -pipe -fomit-frame-pointer -Wall -Wno-uninitialized)
endif(UNIX)

# Function set
set(OPERATOR_SOURCES
	Abs.cpp
	Avg.cpp
	ConstantDouble.cpp
	EphemeralDay.cpp
	Eq.cpp
	GreaterThan.cpp
	IfThenElse.cpp
	LessThan.cpp
	Max.cpp
	Min.cpp
	ROC.cpp
	RSI.cpp
)

set(PROG_SOURCES
	Primitives.cpp
	Context.cpp
	eval.cpp
	main.cpp
)

add_executable(prog ${PROG_SOURCES} ${OPERATOR_SOURCES})
#target_link_libraries(prog ${OPENBEAGLE_LIBRARIES})

