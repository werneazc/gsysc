# - Find SystemC
# Find the SystemC libraries (systemc)
#
#  This module defines the following variables:
#     SYSTEMC_FOUND       - True if SYSTEMC_INCLUDE_DIR & SYSTEMC_LIBRARY are found
#     SYSTEMC_LIBRARIES   - Set when SYSTEMC_LIBRARY is found
#     SYSTEMC_INCLUDE_DIRS - Set when SYSTEMC_INCLUDE_DIR is found
#
#     SYSTEMC_INCLUDE_DIR - where to find systemc.h, etc.
#     SYSTEMC_LIBRARY     - the systemc library
#     SYSTEMC_VERSION_STRING - the version of SystemC found (since CMake 2.8.8)
#

#=============================================================================
# Copyright 2012 Javier Moreno
# Copyright 2012 Javier Moreno <moreno@cs.uni-kl.de>
#
# Distributed under the OSI-approved BSD License (the "License");
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

find_path(SYSTEMC_INCLUDE_DIR NAMES systemc systemc.h HINTS $ENV{SYSTEMC_BASE} $ENV{SYSTEMC} $ENV{SYSTEMC_ROOT}
          PATH_SUFFIXES include
          DOC "The SystemC include directory"
)

find_library(SYSTEMC_LIBRARY NAMES systemc HINTS $ENV{SYSTEMC_BASE} $ENV{SYSTEMC} $ENV{SYSTEMC_ROOT}
          PATH_SUFFIXES lib lib-linux lib-linux64 lib-macosx lib-macosx64 lib-darwin lib64
          DOC "The SystemC library"
)

if(SYSTEMC_INCLUDE_DIR AND EXISTS "${SYSTEMC_INCLUDE_DIR}/sysc/kernel/sc_ver.h")
  file(STRINGS "${SYSTEMC_INCLUDE_DIR}/sysc/kernel/sc_ver.h" systemc_version_str REGEX "^#define[\t ]+SYSTEMC_[0-9]_[0-9]_[0-9]")
  string(REGEX REPLACE "^.*SYSTEMC_([0-9])_([0-9])_([0-9]).*$" "\\1.\\2.\\3" SYSTEMC_VERSION_STRING "${systemc_version_str}")
  unset(systemc_version_str)
endif()

# handle the QUIETLY and REQUIRED arguments and set SYSTEMC_FOUND to TRUE if 
# all listed variables are TRUE
include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SYSTEMC
                                  REQUIRED_VARS SYSTEMC_LIBRARY SYSTEMC_INCLUDE_DIR
                                  VERSION_VAR SYSTEMC_VERSION_STRING)

if(SYSTEMC_FOUND)
  set( SYSTEMC_LIBRARIES ${SYSTEMC_LIBRARY} )
  set( SYSTEMC_INCLUDE_DIRS ${SYSTEMC_INCLUDE_DIR} )
endif()