######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 11 June 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for nlp utterance processors plug-ins                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                        Plug-in directories                                         #
#------------------------------------------------------------------------------------#

add_subdirectory(tokenizer)
add_subdirectory(normalizer)
add_subdirectory(phrasify)
add_subdirectory(lexlookup)
add_subdirectory(pauses)
add_subdirectory(syltone)
add_subdirectory(morphdecomp)
add_subdirectory(uttbreak)

# Also see speect/plugins/cmake/pluginOptions.cmake
if(HAVE_HUNPOS)
  add_subdirectory(hunpos)
endif(HAVE_HUNPOS)
