/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SWIG common C convenience functions for SIterator.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%inline
{
	typedef struct
	{
		SIterator *itr;
	} PMapIterator;


	PMapIterator *make_PMapIterator(SIterator *itr, s_erc *error)
	{
		PMapIterator *pitr;


		S_CLR_ERR(error);
		pitr = S_MALLOC(PMapIterator, 1);
		if (pitr == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "make_PMapIterator",
					  "Failed to allocate memory for 'PMapIterator' object");
			return NULL;
		}

		pitr->itr = itr;
		return pitr;
	}

	typedef struct
	{
		SIterator *itr;
	} PListIterator;


	PListIterator *make_PListIterator(SIterator *itr, s_erc *error)
	{
		PListIterator *pitr;


		S_CLR_ERR(error);
		pitr = S_MALLOC(PListIterator, 1);
		if (pitr == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "make_PListIterator",
					  "Failed to allocate memory for 'PListIterator' object");
			return NULL;
		}

		pitr->itr = itr;
		return pitr;
	}
}

%nodefaultctor PListIterator;
%nodefaultctor PMapIterator;
