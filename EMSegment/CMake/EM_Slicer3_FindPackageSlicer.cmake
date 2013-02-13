#
# Slicer3 specific FindPackage and Include call if outside of Slicer3's source-tree
#

# Since 'project()' shouldn't be used with Slicer4 extension and since 'project()' statement
# can't be conditionnally invoked, let's set the following variables manually.
set(EMSegment_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(EMSegment_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

if(NOT Slicer3_SOURCE_DIR)
  find_package(Slicer3 REQUIRED)
  include(${Slicer3_USE_FILE})
  slicer3_set_default_install_prefix_for_external_projects()
endif()

set(Slicer_VERSION_MAJOR ${Slicer3_VERSION_MAJOR})
set(Slicer_VERSION_MINOR ${Slicer3_VERSION_MINOR})
set(Slicer_VERSION_PATCH ${Slicer3_VERSION_PATCH})
set(Slicer_VERSION "${Slicer_VERSION_MAJOR}.${Slicer_VERSION_MINOR}.${Slicer_VERSION_PATCH}")

SET(EM_SHARE_DIR ${Slicer3_INSTALL_MODULES_SHARE_DIR})
SET(EM_INSTALL_SHARE_DIR ${Slicer3_INSTALL_MODULES_SHARE_DIR})
