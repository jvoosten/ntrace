/**
  \brief Function logger object

  This is the implementation of a helper object to make it easier to log
  function enter and exit points. Using TR_FUNC macro an object is created
  on the stack within your function. This automatically
  logs entry and, more importantly, exit points of your function since the
  object is automatically destroyed when it gets out of scope.

  \par Usage is simple. Include the following in your code to log a function,
  without arguments:

  \code
  void MyFunction(int a, const char *string)
  {
    TR_FUNC();

    // your code here
  }
  \endcode

  \par You can also use TR_FUNC with arguments: this will append the arguments to the
  function name. If you use the function arguments, it immediately makes clear with
  which the function was called.

  \code
  void MyFunction(int a, const char *string)
  {
    TR_FUNC("a = %d, string = '%s'", a, string);

    // your code here
  }
  \endcode

 */

// stop complaining about _vsnprintf. The other ones aren't any more secure than this.
#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>

#include "function.h"
#include "interfaces.h"

using namespace NTrace;

/**
  \brief Constructor; this just creates the object, but does not create a log entry yet

  This function just creates the object, but does not generate logging yet. So, how is
  the logging actually done then? For that, see the () operator.

  The clue is a subtle interaction between the preprocessor and C++ operator overloading.
  If you write your macro as in the examples above, you end up with this in your code:
  <b>Function TracerObject(s_trace_module, FUNCNAME)(); TraceObject();</b>.
  This is actually 2 statements. The first declares the object with 2 arguments to the
  constructor (which is exactly this one). The second statement calls the () operator
  on the object we just created. Actually, the () are replaced by anything you place after
  TR_FUNC, including the ';'. By using an overloaded () operator, it is thus possible
  to supply arguments.

 */
Function::Function (IModule *trace_module, const char *funcname)
{
  m_trace_module = trace_module;
  m_function_name = funcname;
  m_logged = false; // precaution: in case a typo is made and the () operator is not called do not log a spurious Leave event.
}


/**
  \brief Log without arguments.
 */
void Function::operator ()()
{
  if (m_trace_module)
  {
    m_trace_module->enter (m_function_name);
    m_logged = true;
  }
}

/**
  \brief Log with arguments.
 */
void Function::operator ()(const char *fmt, ...)
{
  if (m_trace_module)
  {
    va_list args;
    char buffer[2050];

    va_start (args, fmt);
#if defined(_WIN32)
    _vsnprintf (buffer, 2048, fmt, args);
#else
    vsnprintf (buffer, 2048, fmt, args);
#endif
    va_end (args);

    m_trace_module->enter (m_function_name, buffer);
  }
  m_logged = true;
}

Function::~Function ()
{
  if (m_trace_module)
  {
    if (m_logged)
    {
      m_trace_module->leave (m_function_name);
    }
    else
    {
      m_trace_module->error (m_function_name + " has malformed TR_FUNC macro!");
    }
  }
}

