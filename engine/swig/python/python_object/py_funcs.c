/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* DATE    : April 2010                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Auxiliary Python object function.                                                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "pyobject.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/* enum for SObject types */
typedef enum
{
	S_TYPE_SINT = 0,     /*!< SInt, signed integer value.                */
	S_TYPE_SFLOAT,       /*!< SFloat, floating point value.              */
	S_TYPE_SSTRING,      /*!< SString, string value (char*).             */
	S_TYPE_SPYOBJECT,    /*!< SPyObject, Python object.                  */
	S_TYPE_UNKNOWN,      /*!< Unknown type (not primitive), use SObject. */
	S_TYPE_ERROR         /*!< error, should not get here.                */
} s_sobject_type;


/* helper structure for defining a list of object types */
typedef struct
{
	const char     *class;
	s_sobject_type  type;
} s_sobject_defs;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* a list of object types with there enum types */
static const s_sobject_defs object_type_list[] =
{
	{ "Sint", S_TYPE_SINT },
	{ "SFloat", S_TYPE_SFLOAT },
	{ "SString", S_TYPE_SSTRING },
	{ "SPyObject", S_TYPE_SPYOBJECT },
	{ NULL, S_TYPE_UNKNOWN }
};


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static PyObject *sint_2_pyobject(const SObject *object, s_erc *error);

static PyObject *sfloat_2_pyobject(const SObject *object, s_erc *error);

static PyObject *sstring_2_pyobject(const SObject *object, s_erc *error);

static s_sobject_type get_sobject_type(const SObject *object, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API PyObject *sobject_2_pyobject(const SObject *object, s_erc *error, s_bool own)
{
	s_sobject_type type;
	PyObject *pobject;


	S_CLR_ERR(error);

	type = get_sobject_type(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sobject_2_pyobject",
				  "Call to \"get_sobject_type\" failed"))
		return NULL;

	switch(type)
	{
	case S_TYPE_SINT:
	{
		pobject = sint_2_pyobject(object, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "sobject_2_pyobject",
					  "Call to \"sint_2_pyobject\" failed"))
			return NULL;

		goto check_owner;
	}
	case S_TYPE_SFLOAT:
	{
		pobject = sfloat_2_pyobject(object, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "sobject_2_pyobject",
					  "Call to \"sfloat_2_pyobject\" failed"))
			return NULL;

		goto check_owner;
	}
	case S_TYPE_SSTRING:
	{
		pobject = sstring_2_pyobject(object, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "sobject_2_pyobject",
					  "Call to \"sstring_2_pyobject\" failed"))
			return NULL;

		goto check_owner;
	}

	default:
		return;
	}

check_owner:
	if (own == TRUE)
		S_DELETE(object, "sobject_2_pyobject", error);

	return pobject;
}



S_API SObject *pyobject_2_sobject(PyObject *pobject, s_erc *error);



S_API char *s_get_pyobject_str(PyObject *pobject, s_erc *error)
{
	PyObject *strObject;
	PyObject *utf8Object;
	char *cstr;
	char *new_cstr;


	S_CLR_ERR(&error);

	if (pobject == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_get_pyobject_str",
				  "Argument \"pobject\" is NULL");
		return NULL;
	}

#if PY_VERSION_HEX >= 0x03000000
	strObject = PyObject_Str(pobject);
	if (strObject == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyObject_Str\" failed");
		return NULL;
	}

	utf8Object = PyUnicode_AsUTF8String(strObject);
	Py_XDECREF(strObject);
	if (utf8Object == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyUnicode_AsUTF8String\" failed");
		return NULL;
	}

	cstr = PyBytes_AsString(utf8Object);
	if (cstr == NULL)
	{
		Py_XDECREF(utf8Object);
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyBytes_AsString\" failed");
		return NULL;
	}

	new_cstr = s_strdup(cstr, error);
	Py_XDECREF(utf8Object);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_pyobject_str",
				  "Call to \"s_strdup\" failed"))
		return NULL;
#else /* ! PY_VERSION_HEX >= 0x03000000 */
	strObject = PyObject_Unicode(pobject);
	Py_XDECREF(strObject);
	if (strObject == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyObject_Unicode\" failed");
		return NULL;
	}

	utf8Object = PyUnicode_AsUTF8String(strObject);
	Py_XDECREF(strObject);
	if (utf8Object == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyUnicode_AsUTF8String\" failed");
		return NULL;
	}

	cstr = PyString_AsString(utf8Object);
	if (cstr == NULL)
	{
		Py_XDECREF(utf8Object);
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyString_AsString\" failed");
		return NULL;
	}

	new_cstr = s_strdup(cstr, error);
	Py_XDECREF(utf8Object);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_pyobject_str",
				  "Call to \"s_strdup\" failed"))
		return NULL;
#endif /* PY_VERSION_HEX >= 0x03000000 */

	return new_cstr;
}




S_API char *s_get_python_error_str(void)
{
	PyObject *pyErr;
	PyObject *ptype;
	PyObject *pvalue;
	PyObject *ptraceback;
	char *rerror;
	s_erc error;


	S_CLR_ERR(&error);

	pyErr = PyErr_Occurred();

	/* return NULL if error is not set */
	if (pyErr == NULL)
		return NULL;

	PyErr_Fetch(&ptype, &pvalue, &ptraceback);
	if (ptype == NULL)
	{
		rerror = s_strdup("Unknown error", &error);
		return rerror;
	}

	PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);
	if (ptype == NULL)
	{
		rerror = s_strdup("Unknown error", &error);
		return rerror;
	}

	rerror = s_get_pyobject_str(ptype, &error);
	/* clear */
	Py_XDECREF(ptype);
	Py_XDECREF(pvalue);
	Py_XDECREF(ptraceback);

	if (error != S_SUCCESS)
	{
		rerror = s_strdup("Unknown error", &error);
		return rerror;
	}

	return rerror;
}


/* S_API s_erc SPyObjectModuleInit(void) */
/* { */
/* 	s_erc error = S_SUCCESS; */



/* 	pyobject_class_reg(&error); */

/* 	S_CHK_ERR(&error, S_CONTERR, */
/* 			  "SPyObjectModuleInit", */
/* 			  "Failed to register SPyObjectModule classes"); */

/* 	return error; */
/* } */

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_sobject_type get_sobject_type(const SObject *object, s_erc *error)
{
	const char *class;
	const s_sobject_defs *itr;
	int s_comp;

	S_CLR_ERR(error);

	class = SObjectType(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_sobject_type",
				  "Call to \"SObjectType\" failed"))
		return S_TYPE_ERROR;

	/* iterate through object type list to get the
	 * right s_sobject_type enum
	 */
	for (itr = object_type_list; itr->class != NULL; itr++)
	{
		s_comp = s_strcmp(class, itr->class, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_sobject_type",
					  "Call to \"s_strcmp\" failed"))
			return S_TYPE_ERROR;

		if (s_comp == 0)
			return itr->type;
	}

	return S_TYPE_UNKNOWN;
}



static PyObject *sint_2_pyobject(const SObject *object, s_erc *error)
{
	sint32 val;
	PyObject *pobject;


	S_CLR_ERR(error);

	val = SObjectGetInt(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sint_2_pyobject",
				  "Call to \"SObjectGetInt\" failed"))
		return NULL;

	pobject = PyLong_FromLong((long)val);
	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sint_2_pyobject",
					  "Call to \"PyLong_FromLong\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sint_2_pyobject",
					  "Call to \"PyLong_FromLong\" failed");
		}
	}

	return pobject;
}


static PyObject *sfloat_2_pyobject(const SObject *object, s_erc *error)
{
	float val;
	PyObject *pobject;


	S_CLR_ERR(error);

	val = SObjectGetFloat(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sfloat_2_pyobject",
				  "Call to \"SObjectGetFloat\" failed"))
		return NULL;

	pobject = PyFloat_FromDouble((double)val);
	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sfloat_2_pyobject",
					  "Call to \"PyFloat_FromDouble\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sfloat_2_pyobject",
					  "Call to \"PyFloat_FromDouble\" failed");
		}
	}

	return pobject;
}


static PyObject *sstring_2_pyobject(const SObject *object, s_erc *error)
{
	const char *val;
	size_t slen;
	PyObject *pobject;


	S_CLR_ERR(error);

	val = SObjectGetString(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sstring_2_pyobject",
				  "Call to \"SObjectGetString\" failed"))
		return NULL;

	slen = s_strsize(val, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sstring_2_pyobject",
				  "Call to \"s_strsize\" failed"))
		return NULL;

#if PY_VERSION_HEX >= 0x03000000
	pobject = PyUnicode_FromStringAndSize(val, slen);
    #define SPCT_SSTRING_2_POBJECT_FUNC "PyUnicode_FromStringAndSize"
#else /* ! PY_VERSION_HEX >= 0x03000000 */
	pobject = PyString_FromStringAndSize(val, slen);
    #define SPCT_SSTRING_2_POBJECT_FUNC "PyString_FromStringAndSize"
#endif /* PY_VERSION_HEX >= 0x03000000 */

	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sstring_2_pyobject",
					  "Call to \"%s\" failed. Reported error: %s",
					  SPCT_SSTRING_2_POBJECT_FUNC, py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sstring_2_pyobject",
					  "Call to \"%s\" failed", SPCT_SSTRING_2_POBJECT_FUNC);
		}
	}

	return pobject;
}
