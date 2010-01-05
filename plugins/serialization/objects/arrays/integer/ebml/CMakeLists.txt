######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Template CMakeList for Speect Plug-ins                                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"Integer Array Ebml\"")
project(speect_array_int_ebml_plugin C)

# Plug-in shared object name
set(PLUGIN_SONAME array-int-ebml)

set(${PLUGIN_SONAME}_VERSION_MAJOR 0)
set(${PLUGIN_SONAME}_VERSION_MINOR 9)
set(${PLUGIN_SONAME}_VERSION_PATCH 0)
set(${PLUGIN_SONAME}_VERSION "${${PLUGIN_SONAME}_VERSION_MAJOR}.${${PLUGIN_SONAME}_VERSION_MINOR}.${${PLUGIN_SONAME}_VERSION_PATCH}")


#------------------------------------------------------------------------------------#
#                             Get all CMake modules                                  #
#------------------------------------------------------------------------------------#

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                     Include array-float and ebml plug-in                           #
#------------------------------------------------------------------------------------#

include(array-int)
include(ebml)


#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

# Library build configuration
include(plugin)


#------------------------------------------------------------------------------------#
#                        Create a plug-in cmake config file                          #
#------------------------------------------------------------------------------------#

include(plugin_conf)

# add dependencies
add_dependencies(${PLUGIN_SONAME}_plugin array-int_plugin)
add_dependencies(${PLUGIN_SONAME}_plugin ebml_plugin)
  

#------------------------------------------------------------------------------------#
#                                   examples                                         #
#------------------------------------------------------------------------------------#

if(WANT_EXAMPLES)
  add_subdirectory(examples)
endif(WANT_EXAMPLES)