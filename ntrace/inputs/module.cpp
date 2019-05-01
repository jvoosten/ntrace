#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>

#include "ntrace/manager.h"
#include "module.h"

#ifdef _WIN32
  #define snprintf _snprintf
#endif
  
using namespace NTrace;

/**
  \brief Constructor; there is no default constructor.

  \param mgr Pointer to the manager object
  \param name The name of the module
  \param level Initial level
 */
Module::Module (IManager *mgr, const std::string &name, int level, bool track_enter_leave)
  : IInput(mgr), InputBase(mgr, name),
  m_level (level), m_functionTracking (track_enter_leave)
{
  // nothing to do here
}

int Module::getLevel () const
{
  return m_level;
}

void Module::setLevel (int level)
{
  if (level >= 0)
    m_level = level;
}

bool Module::getFunctionTracking () const
{
  return m_functionTracking;
}

void Module::setFunctionTracking (bool enable)
{
  m_functionTracking = enable;
}

#if 0
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

#endif

/**
 \brief Log message with printf() style formatting
 \param level Desired log level
 \param fmt Formatting string
 
 
 
 */
void Module::log (int level, const char *fmt, ...)
{
  Message message;
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

  message.level = level;
  message.type = Message::Normal;
  message.message = m_buffer;
  m_manager->pushMessage (message);
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
  Message message;

  if (level > m_level)
    return;

  message.level = level;
  message.type = Message::Normal;
  message.message = msg;
  m_manager->pushMessage (message);
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
  Message message;
  va_list args;

  va_start (args, fmt);
#if defined(_WIN32)
  _vsnprintf (m_buffer, 2048, fmt, args);
#else
  vsnprintf (m_buffer, 2048, fmt, args);
#endif
  va_end (args);

  message.type = Message::Error;
  message.message = m_buffer;
  m_manager->pushMessage (message);
}

/**
 \overload
 */
void Module::error (const std::string &msg)
{
  Message message;

  message.type = Message::Error;
  message.message = msg;
  m_manager->pushMessage (message);
}

/**
 \brief Output content directly to stdout
 \param fmt Formatting string

 Outputs content directly on the standard output but does not log. This is
 mainly useful as a quick debug hook.

 */
void Module::out (const char *fmt, ...)
{
  Message message;
  va_list args;

  va_start (args, fmt);
#if defined(_WIN32)
  _vsnprintf (m_buffer, 2048, fmt, args);
#else
  vsnprintf (m_buffer, 2048, fmt, args);
#endif
  va_end (args);

  message.type = Message::Out;
  message.message = m_buffer;
  m_manager->pushMessage (message);
}

void Module::out (const std::string &msg)
{
  Message message;

  message.type = Message::Out;
  message.message = msg;
  m_manager->pushMessage (message);
}



