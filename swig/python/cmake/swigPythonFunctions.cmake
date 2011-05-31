######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake custom functions for Speect plug-ins SWIG Python interfaces                ##
##                                                                                  ##
##   speect_plugin_swig_python_interface    (Generate SWIG interface file)          ##
##   speect_plugin_swig_python_loader       (Generate SWIG interface file that      ##
##                                           only loads plug-in when imported)      ##
##   speect_plugin_swig_python_wrapper      (Generate SWIG Python wrapper)          ##
##   speect_include_python_directories      (Include Python include directories)    ##
##                                                                                  ##
## See the documentation at each function/macro below.                              ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                       Create plug-in SWIG Python interface                         #
#------------------------------------------------------------------------------------#
#
# macro(speect_plugin_swig_python_interface)
#
# Create a plug-in interface file plugin_lowercase_name.i 
# The file will be created in the CMAKE_CURRENT_BINARY_DIR
# 
# The following variables need to be set before calling this macro:
#
# :param swig_classname: The name of the class for which this SWIG Python interface is
#                        defined.
# :type swig_classname: string
# :param swig_includes: SWIG includes (e.g. typemaps.i, exception.i, ...)
# :type swig_includes: CMake list
# :param swig_c_includes: C header files (speect.h and py_native.h are automatically included)
# :type swig_c_includes: CMake list
# :param swig_load_plugin: Whether to load the plug-in on initialization.
# :type swig_load_plugin: bool (0 or 1)
# :param swig_python_docstrings: SWIG Python interfaces files, containing docstrings for
#                                the plug-in class functions.
# :type swig_python_docstrings: CMake list
# :param swig_python_typemaps: SWIG Python typemap files, containing typemaps for
#                               the plug-in class functions.
# :type swig_python_typemaps: CMake list
# :param swig_common: SWIG common interface files, containing SWIG language independant 
#                     interface definitions for the plug-in class functions.
# :type swig_common: CMake list
# :param swig_python: SWIG Python interface files, containing SWIG Python specific
#                     interface definitions for the plug-in class functions.
# :type swig_python: CMake list
#
# Note that only swig_classname is required.
#

macro(speect_plugin_swig_python_interface)
  set(filename "${CMAKE_CURRENT_BINARY_DIR}/${plugin_lowercase_name}.i")
  set(swig_deps)  # dependencies of interface file.
  
  # write header info 
  file(WRITE ${filename} "/* The ${swig_classname} plug-in SWIG/Python interface definition. */\n\n")
  file(APPEND ${filename} "/*\n * ${plugin_lowercase_name}.i is auto generated by the \"speect_plugin_swig_python_interface\"\n")
  file(APPEND ${filename} " * macro in speect/swig/python/cmake/swigPythonFunctions.cmake, do not edit ${plugin_lowercase_name}.i\n */\n\n")

  # docstring
  file(APPEND ${filename} "%module(docstring=\"Speect Plug-in for ${swig_classname} class\") @plugin_lowercase_name@\n\n")

  # version and license info
  file(APPEND ${filename} "%pythoncode\n")
  file(APPEND ${filename} "%{\n\t__version__ = \"${${plugin_lowercase_name}_VERSION}\"\n\t__license__= \"mit\"\n%}\n\n")

  # SWIG includes
  if(swig_includes)
    file(APPEND ${filename} "/* SWIG includes. */\n")
    foreach(include_file ${swig_includes})
      file(APPEND ${filename} "%include \"${include_file}\"\n")
    endforeach(include_file ${swig_includes})
    file(APPEND ${filename} "\n")
  endif(swig_includes)
  
  # C header files (speect.h and py_native.h are always included) 
  file(APPEND ${filename} "/* Speect Engine header & plug-in headers. */\n")
  file(APPEND ${filename} "%header\n%{\n")
  file(APPEND ${filename} "#include \"speect.h\"\n") # speect.h
  file(APPEND ${filename} "#include \"py_native.h\"\n") # py_native.h
  foreach(include_file ${swig_c_includes})
    file(APPEND ${filename} "#include \"${include_file}\"\n")
 #   list(APPEND swig_deps ${include_file})
  endforeach(include_file ${swig_c_includes})
  file(APPEND ${filename} "%}\n\n")

  # enable autodoc SWIG directive
  file(APPEND ${filename} "/* enable autodoc */\n%feature(\"autodoc\", \"1\");\n\n")

  # Load the plug-in on initialization ? 
  if(swig_load_plugin)
    file(APPEND ${filename} "/* Load the ${swig_classname} class plug-in */\n")
    file(APPEND ${filename} "%init\n%{\n\t{\n")
    file(APPEND ${filename} "\t\ts_erc rv = S_SUCCESS;\n")
    file(APPEND ${filename} "\t\tSPlugin *plugin;\n\n\n")
    file(APPEND ${filename} "\t\tplugin = s_pm_load_plugin(\"@plugin_lowercase_name@.spi\", &rv);\n")
    file(APPEND ${filename} "\t\tif (rv != S_SUCCESS)\n")
    file(APPEND ${filename} "\t\t\tSWIG_Error(SWIG_RuntimeError, \"Failed to load ${swig_classname} plug-in\");\n")
    file(APPEND ${filename} "\t}\n%}\n\n")
  endif(swig_load_plugin)

  # import the Speect Engine Python interface
  file(APPEND ${filename} "/* import the Speect Engine Python interface */\n")
  file(APPEND ${filename} "%import \"${CMAKE_BINARY_DIR}/engine/swig/python/speect.i\"\n\n")
  list(APPEND swig_deps ${CMAKE_BINARY_DIR}/engine/swig/python/speect.i)

  # include the Python docstrings
  if(swig_python_docstrings)
    file(APPEND ${filename} "/* ${swig_classname} Python documentation strings. */\n")
    foreach(include_file ${swig_python_docstrings})
      file(APPEND ${filename} "%include \"${CMAKE_CURRENT_SOURCE_DIR}/${include_file}\"\n")
      list(APPEND swig_deps ${CMAKE_CURRENT_SOURCE_DIR}/${include_file})
    endforeach(include_file ${swig_python_docstrings})
    file(APPEND ${filename} "\n")
  endif(swig_python_docstrings)

  # include the Python typemaps
  if(swig_python_typemaps)
    file(APPEND ${filename} "/* ${swig_classname} Python typemaps. */\n")
    foreach(include_file ${swig_python_typemaps})
      file(APPEND ${filename} "%include \"${CMAKE_CURRENT_SOURCE_DIR}/${include_file}\"\n")
      list(APPEND swig_deps ${CMAKE_CURRENT_SOURCE_DIR}/${include_file})
    endforeach(include_file ${swig_python_typemaps})
    file(APPEND ${filename} "\n")
  endif(swig_python_typemaps)

  # SWIG common interface definition
  if(swig_common)
    file(APPEND ${filename} "/* ${swig_classname} SWIG common interface definition. */\n")
    foreach(include_file ${swig_common})
      file(APPEND ${filename} "%include \"${CMAKE_CURRENT_SOURCE_DIR}/../common/${include_file}\"\n")
      list(APPEND swig_deps ${CMAKE_CURRENT_SOURCE_DIR}/../common/${include_file})
    endforeach(include_file ${swig_common})
    file(APPEND ${filename} "\n")
  endif(swig_common)

  # SWIG Python interface definition
  if(swig_python)
    file(APPEND ${filename} "/* ${swig_classname} SWIG Python interface definition. */\n")
    foreach(include_file ${swig_python})
      file(APPEND ${filename} "%include \"${CMAKE_CURRENT_SOURCE_DIR}/${include_file}\"\n")
      list(APPEND swig_deps ${CMAKE_CURRENT_SOURCE_DIR}/${include_file})
    endforeach(include_file ${swig_python})
    file(APPEND ${filename} "\n")
  endif(swig_python)

  # flags for the SWIG generation
  list(APPEND SPCT_SWIG_FLAGS -Wall -Werror)

  # add the generated interface file dependencies
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${plugin_lowercase_name}.i
    PROPERTIES 
    SWIG_FLAGS "${SPCT_SWIG_FLAGS}"
    GENERATED TRUE
    )
endmacro(speect_plugin_swig_python_interface)

  
#------------------------------------------------------------------------------------#
#                       Create plug-in SWIG Python loader                            #
#------------------------------------------------------------------------------------#
#
# macro(speect_plugin_swig_python_loader)
#
# Create a plug-in loader file plugin_lowercase_name.i 
# This is just a simple wrapper that loads the plug-in in Python when imported.
#

macro(speect_plugin_swig_python_loader)
  set(filename "${CMAKE_CURRENT_BINARY_DIR}/${plugin_lowercase_name}.i")
  set(swig_deps)  # dependencies of interface file.
  
  # write header info 
  file(WRITE ${filename} "/* Definition of a simple Python importer for the ${plugin_lowercase_name}.spi plug-in. */\n\n")
  file(APPEND ${filename} "/*\n * ${plugin_lowercase_name}.i is auto generated by the \"speect_plugin_swig_python_loader\"\n")
  file(APPEND ${filename} " * macro in speect/swig/python/cmake/swigPythonFunctions.cmake, do not edit ${plugin_lowercase_name}.i\n */\n\n")

  # docstring
  file(APPEND ${filename} "%module(docstring=\"A simple wrapper to load the ${plugin_lowercase_name}.spi plug-in.\") @plugin_lowercase_name@\n\n")

  # C header files (speect.h is always included) 
  file(APPEND ${filename} "/* Speect Engine header. */\n")
  file(APPEND ${filename} "%header\n%{\n")
  file(APPEND ${filename} "#include \"speect.h\"\n") # speect.h
  file(APPEND ${filename} "%}\n\n")

  # Load the plug-in on initialization 
  file(APPEND ${filename} "/* Load the ${plugin_lowercase_name}.spi plug-in */\n")
  file(APPEND ${filename} "%init\n%{\n\t{\n")
  file(APPEND ${filename} "\t\ts_erc rv = S_SUCCESS;\n")
  file(APPEND ${filename} "\t\tSPlugin *plugin;\n\n\n")
  file(APPEND ${filename} "\t\tplugin = s_pm_load_plugin(\"@plugin_lowercase_name@.spi\", &rv);\n")
  file(APPEND ${filename} "\t\tif (rv != S_SUCCESS)\n")
  file(APPEND ${filename} "\t\t\tSWIG_Error(SWIG_RuntimeError, \"Failed to load '${plugin_lowercase_name}.spi' plug-in\");\n")
  file(APPEND ${filename} "\t}\n%}\n\n")

  # flags for the SWIG generation
  list(APPEND SPCT_SWIG_FLAGS -Wall -Werror)

  # add the generated interface file dependencies
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${plugin_lowercase_name}.i
    PROPERTIES 
    SWIG_FLAGS "${SPCT_SWIG_FLAGS}"
    GENERATED TRUE
    )  
endmacro(speect_plugin_swig_python_loader)


#------------------------------------------------------------------------------------#
#                       Include Python include directories                           #
#------------------------------------------------------------------------------------#
#
# macro(speect_include_python_directories)
#
# PYTHON_INCLUDE_PATH is deprecated in older CMAKE versions, use this macro 
#
macro(speect_include_python_directories)
  if(${PYTHON_INCLUDE_PATH} STREQUAL "")
    include_directories(${PYTHON_INCLUDE_DIRS})  
  else(${PYTHON_INCLUDE_PATH} STREQUAL "")
    include_directories(${PYTHON_INCLUDE_PATH})  
  endif(${PYTHON_INCLUDE_PATH} STREQUAL "")
endmacro(speect_include_python_directories)


#------------------------------------------------------------------------------------#
#                       Create plug-in SWIG Python wrapper                           #
#------------------------------------------------------------------------------------#
#
# macro(speect_plugin_swig_python_wrapper)
#
# Create a plug-in wrapper library and 
# Python loader (_@plugin_lowercase_name@.so and @pplugin_lowercase_name@.py)
#

macro(speect_plugin_swig_python_wrapper)
  # find swig
  find_package(SWIG REQUIRED)

  # include swig
  include(${SWIG_USE_FILE})

  # find Python 
  find_package(PythonLibs REQUIRED) # Note, currently PythonLibs does not look for Python 3
  mark_as_advanced(FORCE PYVERSIONS_EXE)

  # include Python
  speect_include_python_directories()

  # include Speect 
  include_directories(${SPCT_INCLUDE_DIRS})

  # include Speect Python native 
  include_directories(${SPCT_PYTHON_NATIVE_INCLUDE_DIRS})

  # clear C flags, we don't want the same stuff as Speect
  set(CMAKE_C_FLAGS "")

  # wrapper file properties
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${plugin_lowercase_name}PYTHON_wrap.c 
    PROPERTIES 
    COMPILE_FLAGS ${SWIG_C_COMPILE_FLAGS}
    GENERATED TRUE
    )

  # swig dependencies
  set(SWIG_MODULE_${plugin_lowercase_name}_EXTRA_DEPS "${swig_deps}")

  # SWIG Python wrapper output directory
  set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR}/swig/python/speect)

  # create the wrapper with SWIG
  swig_add_module(${plugin_lowercase_name} python ${CMAKE_CURRENT_BINARY_DIR}/${plugin_lowercase_name}.i)

  # link with Python and Speect Engine
  swig_link_libraries(${plugin_lowercase_name} 
    ${PYTHON_LIBRARIES} 
    ${SPCT_LIBRARIES_TARGET}
    ${SPCT_PYTHON_NATIVE_LIBRARY_TARGET})

  # set the output directory
  set_target_properties(${SWIG_MODULE_${plugin_lowercase_name}_REAL_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/swig/python/speect/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/swig/python/speect/lib
    )
endmacro(speect_plugin_swig_python_wrapper)

