/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 5 June 2009                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Error handling and debugging.                                                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "include/common.h"
#include "base/utils/alloc.h"
#include "base/utils/math.h"
#include "base/utils/time.h"
#include "base/log/event/event.h"
#include "base/errdbg/errdbg_impl.h"
#include "base/errdbg/errdbg_macros.h"
#include "base/errdbg/errdbg_utils.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/*
 * lock mutex if errdbg module has been initialized
 * and set file name and line number
 */
#define _S_LOCK_AND_SET							\
	do {										\
		if (initialized == TRUE)				\
			s_mutex_lock(&errdbg_mutex);		\
												\
		error_file_name = file_name;			\
		error_line = line;						\
	} while (0)


#ifdef SPCT_USE_THREADS
#  define _S_LOG_WITHOUT_INITIALIZED_ERROR_PRINT(ERROR_TYPE)			\
	fprintf(stderr, "[%s (%s) %lu] ", ERROR_TYPE, error_string, s_thread_id())
#else /*! SPCT_USE_THREADS  */
#  define _S_LOG_WITHOUT_INITIALIZED_ERROR_PRINT(ERROR_TYPE)	\
	fprintf(stderr, "[%s (%s)] ", ERROR_TYPE, error_string)
#endif /* SPCT_USE_THREADS */



#define _S_LOG_WITH_INITIALIZED(ERROR_EVENT, FUNC, RETURN_ABORT)		\
	do {																\
		/* format message with layout */								\
		va_start(argp, fmt);											\
		msg = s_layout_vformat(my_layout, ERROR_EVENT, error_string,	\
							   function_name, error_file_name, error_line, \
							   fmt, argp);								\
		va_end(argp);													\
																		\
		if (msg == NULL)												\
		{																\
			/*															\
			 * if layout made an error it should still output message	\
			 * to stdout.												\
			 */															\
			S_ERR_PRINT(S_METHFAIL, FUNC,								\
						"Call to \"s_layout_vformat\" failed");			\
			S_FREE(error_string);										\
			/* unlock and return, nothing to do here */					\
			s_mutex_unlock(&errdbg_mutex);								\
			RETURN_ABORT;												\
		}																\
																		\
		/* send message to logger */									\
		local_err = s_logger_write(err_logger, msg);					\
		if (local_err != S_SUCCESS)										\
		{																\
			/*															\
			 * if logger made an error it should still output message	\
			 * to stdout.												\
			 */															\
			S_ERR_PRINT(local_err, FUNC,								\
						"Call to \"s_logger_write\" failed");			\
		}																\
																		\
		S_FREE(msg);													\
		S_FREE(error_string);											\
																		\
		/* unlock and return, nothing to do here */						\
		s_mutex_unlock(&errdbg_mutex);									\
		RETURN_ABORT;													\
	} while (0)


#define _S_LOG_WITHOUT_INITIALIZED(ERROR_TYPE, FUNC)					\
	do {																\
		/* not initialized, report error and print to stderr */			\
		S_ERR_PRINT(S_FAILURE, FUNC,									\
					"Error and debugging module has not been initialized.\n" \
					"This will lead to undefined behaviour. Call s_errdbg_init(...)\n" \
					"Printing unformatted error message to stderr.\n");	\
		_S_LOG_WITHOUT_INITIALIZED_ERROR_PRINT(ERROR_TYPE);				\
		va_start(argp, fmt);											\
		vfprintf(stderr, fmt, argp);									\
		va_end(argp);													\
		if (function_name != NULL)										\
			fprintf(stderr, " (in function '%s', %s, %d)\n",			\
					function_name, error_file_name, error_line);		\
		else															\
			fprintf(stderr, " (%s, %d)\n",								\
					error_file_name, error_line);						\
	} while (0)



/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static const char *error_file_name;

static int error_line;

static s_dbg_lvl dbg_level = S_DBG_INFO;

static s_logger *err_logger = NULL;

static s_logger *dbg_logger = NULL;

static s_layout *my_layout = NULL;

static s_bool initialized = FALSE;

/* declare mutex */
S_DECLARE_MUTEX_STATIC(errdbg_mutex);


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_log_event dbglvl_2_evntlvl(s_dbg_lvl level);

static void log_error(s_erc *error_code, s_erc new_error,
					  const char *function_name, const char *fmt, ...);

static void log_fatal_error(s_erc *error_code, s_erc new_error,
							const char *function_name, const char *fmt, ...);

static void log_warn(s_erc new_error, const char *function_name,
					 const char *fmt, ...);

static s_bool check_error(s_erc *error_code, s_erc this_error,
						  const char *function_name, const char *fmt, ...);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_errdbg_init(s_logger *e_logger, s_logger *d_logger,
							s_layout *layout, s_dbg_lvl level, s_erc *error)
{
	S_CLR_ERR(error);

	if (initialized == TRUE)
		return;

	if (e_logger == NULL)
	{
		S_NEW_ERR(error, S_ARGERROR);
		S_ERR_PRINT(S_ARGERROR, "_s_errdbg_init",
					"Argument \"e_logger\" is NULL");
		return;
	}

	if (layout == NULL)
	{
		S_NEW_ERR(error, S_ARGERROR);
		S_ERR_PRINT(S_ARGERROR, "_s_errdbg_init",
					"Argument \"layout\" is NULL");
		return;
	}

	/* initialize time module */
	_s_time_init(error);
	if ((error != NULL) && (*error != S_SUCCESS))
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "_s_errdbg_init",
					"Call to \"_s_time_init\" failed");
		return;
	}

	/* init and lock mutex */
	s_mutex_init(&errdbg_mutex);
	s_mutex_lock(&errdbg_mutex);

	dbg_level = level;
	err_logger = e_logger;
	dbg_logger = d_logger;
	my_layout = layout;

	initialized = TRUE;

	/* unlock mutex before return */
	s_mutex_unlock(&errdbg_mutex);
}


S_LOCAL void _s_errdbg_quit(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(error);

	if (initialized == FALSE)
		return;

	/* lock mutex */
	s_mutex_lock(&errdbg_mutex);

	if (err_logger == dbg_logger) /* loggers are the same */
		dbg_logger = NULL;

	local_err = s_logger_destroy(err_logger);
	if (local_err != S_SUCCESS)
	{
		S_ERR_PRINT(local_err, "_s_errdbg_quit",
					"Failed to destroy error logger");
		S_NEW_ERR(error, local_err);
	}
	S_FREE(err_logger);

	if (dbg_logger != NULL)
	{
		local_err = s_logger_destroy(dbg_logger);
		if (local_err != S_SUCCESS)
		{
			S_ERR_PRINT(local_err, "_s_errdbg_quit",
						"Failed to destroy debug logger");
			if (*error == S_SUCCESS)
				S_NEW_ERR(error, local_err);
		}
		S_FREE(dbg_logger);
	}

	local_err = s_layout_destroy(my_layout);
	if (local_err != S_SUCCESS)
	{
		S_ERR_PRINT(local_err, "_s_errdbg_quit",
					"Failed to destroy layout");
		S_NEW_ERR(error, local_err);
	}
	S_FREE(my_layout);

	error_file_name = NULL;
	error_line = 0;
	dbg_level = S_DBG_INFO;
	initialized = FALSE;

	/* unlock and destroy mutex */
	s_mutex_unlock(&errdbg_mutex);
	s_mutex_destroy(&errdbg_mutex);

	/* quit time module */
	_s_time_quit(error);
	if ((error != NULL) && (*error != S_SUCCESS))
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "_s_errdbg_quit",
					"Call to \"_s_time_quit\" failed");
	}
}


S_API void s_errdbg_level(s_dbg_lvl level, s_erc *error)
{
	S_CLR_ERR(error);

	/* lock mutex */
	s_mutex_lock(&errdbg_mutex);

	dbg_level = level;

	/* unlock mutex */
	s_mutex_unlock(&errdbg_mutex);
}


/************************************************************************************/
/*                                                                                  */
/* Private function prototypes                                                      */
/*                                                                                  */
/************************************************************************************/


/*
 * A dummy error function, does nothing. Replaces #_s_err, #_s_fatal_err,
 * and #_s_check_err when error handling is turned off.
 * @private
 *
 * @note Even though it replaces above mentioned, it never gets called
 * because the macros become <c> if (0 && _s_err_dummy) </c>
 * which @e always evaluates to @c FALSE @e before calling @c _s_err_dummy
 */
S_API int _s_err_dummy(s_erc *error_code, s_erc this_error,
					   const char *function_name, const char *fmt, ...)
{
	S_CLR_ERR(error_code);
	this_error = S_SUCCESS;
	function_name = NULL;
	fmt = NULL;

	return 0;
}


/*
 * A dummy warning function, does nothing. Replaces #_s_warn when
 * error handling is turned off.
 * @private
 *
 * @note Even though it replaces above mentioned, it never gets called
 * because the macros become <c> if (0 && _s_warn_dummy) </c>
 * which @e always evaluates to @c FALSE @e before calling @c _s_warn_dummy
 */
S_API int _s_warn_dummy(s_erc this_error, const char *function_name,
						const char *fmt, ...)
{
	this_error = S_SUCCESS;
	function_name = NULL;
	fmt = NULL;

	return 0;
}


S_API void (*_s_err(const char *file_name, int line))(s_erc *error_code,
													  s_erc this_error,
													  const char *function_name,
													  const char *fmt, ...)
{
	/*
	 * lock mutex if errdbg module has been initialized
	 * and set file name and line number
	 */
	_S_LOCK_AND_SET;

	return log_error;
}


/* we can probably make this function a lot smaller since it is fatal */
S_API void (*_s_fatal_err(const char *file_name, int line))(s_erc *error_code,
															s_erc this_error,
															const char *function_name,
															const char *fmt, ...)
{
	/*
	 * lock mutex if errdbg module has been initialized
	 * and set file name and line number
	 */
	_S_LOCK_AND_SET;

	return log_fatal_error;
}


S_API s_bool (*_s_check_err(const char *file_name, int line))(s_erc *error_code,
															  s_erc this_error,
															  const char *function_name,
															  const char *fmt, ...)
{
	/*
	 * lock mutex if errdbg module has been initialized
	 * and set file name and line number
	 */
	_S_LOCK_AND_SET;

	return check_error;
}


S_API void (*_s_warn(const char *file_name, int line))(s_erc this_error,
													   const char *function_name,
													   const char *fmt, ...)
{
	/*
	 * lock mutex if errdbg module has been initialized
	 * and set file name and line number
	 */
	_S_LOCK_AND_SET;

	return log_warn;
}


S_API void _s_dbg(s_dbg_lvl level, const char *fmt, ...)
{
	va_list argp;
	s_erc local_err;
	char *msg;


	if (dbg_logger == NULL)
		return;

	S_CLR_ERR(&local_err);

	/* lock mutex if errdbg module has been initialized */
	if (initialized == TRUE)
	{
		s_mutex_lock(&errdbg_mutex);

		if (dbg_level < level)
		{
			/* unlock and return, nothing to do here */
			s_mutex_unlock(&errdbg_mutex);
			return;
		}

		/* format message with layout */
		va_start(argp, fmt);
		msg = s_layout_vformat(my_layout, dbglvl_2_evntlvl(level),
							   NULL, NULL, NULL, 0, fmt, argp);
		va_end(argp);

		if (msg == NULL)
		{
			/*
			 * if layout made an error it should still output message
			 * to stdout.
			 */
			S_ERR_PRINT(S_METHFAIL, "_s_dbg",
						"Call to \"s_layout_vformat\" failed");
			/* unlock and return, nothing to do here */
			s_mutex_unlock(&errdbg_mutex);
			return;
		}

		/* send message to logger */
		local_err = s_logger_write(dbg_logger, msg);
		if (local_err != S_SUCCESS)
		{
			/*
			 * if logger made an error it should still output message
			 * to stdout.
			 */
			S_ERR_PRINT(local_err, "_s_dbg",
						"Call to \"s_logger_write\" failed");
		}

		S_FREE(msg);

		/* unlock and return, nothing to do here */
		s_mutex_unlock(&errdbg_mutex);
		return;
	}
	else
	{
		char *event_string;

		/* not initialized, report error and print to stderr */
		event_string = s_log_event_str(level);
		S_ERR_PRINT(S_FAILURE, "_s_dbg",
					"Error and debugging module has not been initialized.\n"
					"This will lead to undefined behaviour. Call s_errdbg_init(...)\n"
					"Printing unformatted debug message to stdout.\n");
#ifdef SPCT_USE_THREADS
		fprintf(stdout, "[%s %lu] ", event_string, s_thread_id());
#else /*! SPCT_USE_THREADS  */
		fprintf(stdout, "[%s] ", event_string);
#endif /* SPCT_USE_THREADS */
		va_start(argp, fmt);
		vfprintf(stdout, fmt, argp);
		va_end(argp);
		fprintf(stdout, "\n");

		S_FREE(event_string);
	}
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_log_event dbglvl_2_evntlvl(s_dbg_lvl level)
{
	if (S_NUM_IN_RANGE(level, S_DBG_TRACE, S_DBG_INFO))
		return level + S_WARN_EVENT;
	else
		return S_TRACE_EVENT;  /* we dont know as level is invalid, so make it trace */
}


static void log_error(s_erc *error_code, s_erc new_error,
					  const char *function_name, const char *fmt, ...)
{
	s_erc local_err;
	va_list argp;
	char *msg;
	char *error_string;


	S_CLR_ERR(&local_err);

	/* used in _S_LOG_WITH_INITIALIZED macro */
	error_string = s_error_str(new_error);

	/* set the error */
	if (error_code != NULL)
		(*error_code) = new_error;

	if (initialized == TRUE)
		_S_LOG_WITH_INITIALIZED(S_ERROR_EVENT, "log_error", return);
	else
		_S_LOG_WITHOUT_INITIALIZED("ERROR", "log_error");

	S_FREE(error_string);
}


static void log_warn(s_erc new_error, const char *function_name,
					 const char *fmt, ...)
{
	s_erc local_err;
	va_list argp;
	char *msg;
	char *error_string;


	S_CLR_ERR(&local_err);

	/* used in _S_LOG_WITH_INITIALIZED macro */
	error_string = s_error_str(new_error);

	if (initialized == TRUE)
		_S_LOG_WITH_INITIALIZED(S_WARN_EVENT, "log_warn_error", return);
	else
		_S_LOG_WITHOUT_INITIALIZED("WARN", "log_warn_error");

	S_FREE(error_string);
}


static void log_fatal_error(s_erc *error_code, s_erc new_error,
							const char *function_name, const char *fmt, ...)
{
	s_erc local_err;
	va_list argp;
	char *msg;
	char *error_string;


	S_CLR_ERR(&local_err);

	/* used in _S_LOG_WITH_INITIALIZED macro */
	error_string = s_error_str(new_error);

	/* set the error */
	if (error_code != NULL)
		(*error_code) = new_error;

	if (initialized == TRUE)
		_S_LOG_WITH_INITIALIZED(S_FATAL_EVENT, "log_fatal_error", abort());
	else
	{
		_S_LOG_WITHOUT_INITIALIZED("FATAL", "log_fatal_error");
		abort();
	}

	S_FREE(error_string);
}


static s_bool check_error(s_erc *error_code, s_erc this_error,
						  const char *function_name, const char *fmt, ...)
{
	s_erc local_err;
	va_list argp;
	char *msg;
	char *error_string;


	S_CLR_ERR(&local_err);

	if ((error_code == NULL) || ((*error_code) == S_SUCCESS))
	{
		/* unlock and return, nothing to do here */
		s_mutex_unlock(&errdbg_mutex);
		return FALSE;
	}

	/* used in _S_LOG_WITH_INITIALIZED macro */
	error_string = s_error_str(this_error);

	if (this_error != S_CONTERR)
		(*error_code) = this_error;
	else
		this_error = (*error_code);

	if (initialized == TRUE)
		_S_LOG_WITH_INITIALIZED(S_ERROR_EVENT, "check_error", return TRUE);

	_S_LOG_WITHOUT_INITIALIZED("ERROR", "check_error");
	S_FREE(error_string);

	return TRUE;
}
