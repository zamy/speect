######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : May 2011                                                               ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for SSyllabificationRewrites JSON Plug-in                              ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                             Define plug-in                                         #
#------------------------------------------------------------------------------------#

speect_plugin_definition(Syllab_Rewrites_JSON "SJSONSyllabificationRewritesFile" 1 0 0)


#------------------------------------------------------------------------------------#
#                        Configure plugin_info.h.in                                  #
#------------------------------------------------------------------------------------#

set(description "Load SSyllabificationRewrites type data in JSON format from files")

# Minimum required Speect Engine version 
set(major_min 1)
set(minor_min 0)

speect_plugin_configure_info(${description} ${major_min} ${minor_min})


#------------------------------------------------------------------------------------#
#                               Source files                                         #
#------------------------------------------------------------------------------------#

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                      Include syllabification rewrites class                        #
#------------------------------------------------------------------------------------#

include(syllab_rewrites)


#------------------------------------------------------------------------------------#
#                            Plug-in shared object                                   #
#------------------------------------------------------------------------------------#

speect_plugin_create()


#------------------------------------------------------------------------------------#
#                                 examples/tests                                     #
#------------------------------------------------------------------------------------#

if(WANT_EXAMPLES)
  add_subdirectory(examples)
endif(WANT_EXAMPLES)


#------------------------------------------------------------------------------------#
#                              SWIG wrappers                                         #
#------------------------------------------------------------------------------------#

add_subdirectory(swig)

