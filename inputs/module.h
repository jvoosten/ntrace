#pragma once

#include "ntrace/interfaces.h"

namespace NTrace
{

/**
\brief Central input class for a module

In the context of NTrace, a 'module' is a set of related source files;
it could be a library, a submodule in your program, or even a single large
object. Log messages that belong to the same module are treated the same.

A module has convenience logging functions to actually generate log messages with
automatic timestamp, process ID and printf() style formatting.

NTrace::Module objects are created by the central Manager, since you can have
multiple source files that belong to the same module and it would be a waste to
create multiple objects that essentially contain the same information. Your source
file should contain a statically generated poiner to a Module object.


Modules also contain a basic log-level; any log message with a priority
above the log level is discarded (e.g. use log(0, ...) for very important messages,
log(2) for less so, and log(7) for debug. See also \ref IInput::Level.
 
*/
class Module: public IInput
{
public:
  Module (IManager *mgr, const std::string &name, int level);

  IManager * NTRACE_CALL getManager () const;
  std::string NTRACE_CALL getName () const;

/*  void enter (const std::string &function_name);
  void enter (const std::string &function_name, const std::string &args);
  void enter (const std::string &file_name, int line_number);
  void leave (const std::string &function_name);
  void leave (const std::string &file_name, int line_number);
  */
#if defined(__GCC__) && (__GCC__ >= 4)
  void NTRACE_CALL log (int level, const char *fmt, ...) __attribute__ ((format (printf, 3, 4)));
  void NTRACE_CALL error (const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
  void NTRACE_CALL out (const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
#else
  void NTRACE_CALL log (int level, const char *fmt, ...);
  void NTRACE_CALL error (const char *fmt, ...);
  void NTRACE_CALL out (const char *fmt, ...);
#endif
  void NTRACE_CALL log (int level, const std::string &msg);
  void NTRACE_CALL error (const std::string &msg);
  void NTRACE_CALL out (const std::string &msg);

  int getLevel () const;
  void setLevel (int);
  void incLevel ();
  void decLevel ();

private:
	IManager *m_manager;
	std::string m_moduleName;
	int m_level;
  static const int s_buffersize = 2048;
	char m_buffer[s_buffersize]; ///< Local buffer for all formatting
};

} // namespace

