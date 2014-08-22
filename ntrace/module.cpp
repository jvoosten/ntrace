#include <stdarg.h>
#include <stdio.h>

#include "manager.h"
#include "module.h"


/**
 \class Module
  The tracemodule is the object that gets instantiated in your source file.
  It forwards log events to the TraceManager
 */

using namespace NTrace;

/**
  \brief Constructor; there is no default constructor.

  \param mgr Pointer to the manager object
  \param name The name of the module
  \param level Initial level
 */
Module::Module (Manager *mgr, const std::string &name, int level)
{
  m_manager = mgr;
  m_moduleName = name;
  m_level = level;
}

std::string Module::getName () const
{
  return m_moduleName;
}

int Module::getLevel () const
{
  return m_level;
}

/**
  \brief Log function enter point

  This function marks the entry of a function. It prefixes the function name with
  >> and increments the indent level.

 */
void Module::enter (const std::string &function_name)
{
  if (0 == m_level)
    return;
  m_manager->log (">> " + function_name);
  m_manager->incIndent ();
}

/**
  \brief Log function enter with the arguments

  This function allows you to create log entries of the form:
  \code
  >> MyFunction(a = 45, b = 3, string = 'hello')
  \endcode

  Of course you don't have to specify just the arguments supplied to the function,
  but it's good practice.

  This function is normally called by using the TR_FUNC(arg, arg...) macro
 */
void Module::enter (const std::string &function_name, const std::string &args)
{
  if (0 == m_level)
    return;
  m_manager->log (">> " + function_name + " (" + args + ")");
  m_manager->incIndent ();
}

/**
 \brief Log function enter with filename and line number
 
 For compilers that do not have a PRETTY_FUNCTION or similar macro.
 */
void Module::enter (const std::string &file_name, int line_number)
{
  int sep;
  char bf[20];

  if (0 == m_level)
    return;
  snprintf (bf, 20, "%d", line_number);
  sep = file_name.rfind ('/');
  enter (file_name.substr (sep + 1) + ":" + bf);
}

/**
  \brief Mark function exit point

  Calling this function marks the end of the current function. It decrements the logging
  level and logs a line of the form:

  \code
  << MyFunction()
  \endcode

 */
void Module::leave (const std::string &function_name)
{
  if (0 == m_level)
    return;
  m_manager->decIndent ();
  m_manager->log ("<< " + function_name);
}

/**
 \brief Mark function exit point
 
 Version for compilers that do not support PRETTY_FUNCTION or similar macro.
 */
void Module::leave (const std::string &file_name, int line_number)
{
  int sep;
  char bf[20];

  if (0 == m_level)
    return;
  snprintf (bf, 20, "%d", line_number);
  sep = file_name.rfind ('/');
  leave (file_name.substr (sep + 1) + ":" + bf);
}

/**
 \brief Log message with printf() style formatting
 \param level Desired log level
 \param fmt Formatting string
 
 
 
 */
void Module::log (int level, const char *fmt, ...)
{
  va_list args;

  if (level > m_level)
    return;

  /* Unfortunately we cannot propagate the ellipses (...), so we must build
     the string here */
  va_start (args, fmt);
#if defined(_WIN32)
  _vsnprintf (m_buffer, 2048, fmt, args);
#else
  vsnprintf (m_buffer, 2048, fmt, args);
#endif
  va_end (args);
  m_manager->log (m_buffer);
}

/**
 \brief Log message 
 \param level The level of the message
 \param msg Preformatted string 
 
 This is an overloaded version of the function above, that takes an std::string as argument; this
 can be useful for messages that are preformatted/concatenated using strings.
 */
void Module::log (int level, const std::string &msg)
{
  if (level > m_level)
    return;

  m_manager->log (msg);
}

/**
 \brief Log error with printf style formatting
 \param fmt Formatting style
 
 This function logs an error; it is written to both the regular log file and stderr or equivalent.
 It is intended for serious errors; therefor it does not have a debuglevel and is always
 written to the log and standard error output.
 
 */
void Module::error (const char *fmt, ...)
{
  va_list args;

  va_start (args, fmt);
#if defined(_WIN32)
  _vsnprintf (m_buffer, 2048, fmt, args);
#else
  vsnprintf (m_buffer, 2048, fmt, args);
#endif
  va_end (args);
  m_manager->error (m_buffer);
}

/**
 \overload
 */
void Module::error (const std::string &msg)
{
  m_manager->error (msg);
}

/**
 \brief Output content directly to stdout
 \param fmt Formatting string

 Outputs content directly on the standard output but does not log. This is
 mainly useful as a quick debug hook.

 */
void Module::out (const char *fmt, ...)
{
  va_list args;

  va_start (args, fmt);
#if defined(_WIN32)
  _vsnprintf (m_buffer, 2048, fmt, args);
#else
  vsnprintf (m_buffer, 2048, fmt, args);
#endif
  va_end (args);
  m_manager->out (m_buffer);

}

void Module::out (const std::string &msg)
{
  m_manager->out (msg);
}

void Module::incLevel ()
{
  m_level++;
}

void Module::decLevel ()
{
  if (m_level > 0)
    m_level--;
}

void Module::setLevel (int level)
{
  if (level >= 0)
    m_level = level;
}
