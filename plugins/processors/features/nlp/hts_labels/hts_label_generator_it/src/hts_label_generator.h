/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                                                                                  */
/* Permission is hereby granted, free of charge, to any person obtaining a copy     */
/* of this software and associated documentation files (the "Software"), to deal    */
/* in the Software without restriction, including without limitation the rights     */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        */
/* copies of the Software, and to permit persons to whom the Software is            */
/* furnished to do so, subject to the following conditions:                         */
/* The above copyright notice and this permission notice shall be included in       */
/* all copies or substantial portions of the Software.                              */
/*                                                                                  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        */
/* THE SOFTWARE.                                                                    */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  : Simone Daminato                                                        */
/* DATE    : January 2016                                                           */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Generate HTS Labels for italian voices.                                          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_FEATPROCESSOR_HTS_DATA_GENERATOR_H__
#define _SPCT_PLUGIN_FEATPROCESSOR_HTS_DATA_GENERATOR_H__


/**
 * @file hts_data_generator.h
 * Generate HTS Labels for italian voices.
 */


/**
 * @ingroup SFeatureProc
 * @defgroup SHTSLabelsGeneratorSimple2FeatProc Generate data for HTS Labels for italian voices.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* SHTSLabelsConfigurableFeatProc definition                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The HTS label generator feature processor structure. Does not add any
 * new members, therefore exactly the same as #SFeatProcessor.
 * Typedef of the HTS label generator (cconfigurable) feature processor.
 * @extends SFeatProcessor
 */
typedef struct
{
	/**
	 * @protected Inherit from #SFeatProcessor.
	 */
	SFeatProcessor 	obj;

	/**
	 * @protected SHTSLabelsGeneratorItFeatProc special phones list.
	 */
	SMap            *specialPhones;
	
	/**
	 * @protected SHTSLabelsGeneratorItFeatProc null phoneme string.
	 */
	const char      *nullPhoneme;

} SHTSLabelsGeneratorItFeatProc;


/************************************************************************************/
/*                                                                                  */
/* SHTSLabelsSimple1FeatProcClass definition                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the HTS label data generator feature processor
 * class. Does not add any new methods, therefore exactly the same as
 * #SFeatProcessorClass.
 */
typedef SFeatProcessorClass SHTSLabelsGeneratorItFeatProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SHTSLabelsDataCollectorFeatProc plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hts_labels_generator_class_reg(s_erc *error);


/**
 * Free the #SHTSLabelsDataCollectorFeatProc plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hts_labels_generator_class_free(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_PLUGIN_FEATPROCESSOR_HTS_DATA_GENERATOR_H__ */
