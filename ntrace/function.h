#pragma once

#include <string>

#include "ntrace_exports.h"

namespace NTrace
{

class IModule;

/**
  \brief Helper class to automatically log enter and leave events

  By creating a Function object inside your function, entry and exit messages are
  automatically generated (due to the wonders of OOP programming and destructors).
*/

class Function
{
public:
  NTRACE_EXPORT Function (IModule *trace_module, const char *funcname);
  NTRACE_EXPORT ~Function ();

  NTRACE_EXPORT void NTRACE_CALL operator ()();
#if defined(__GCC__) && (__GCC__ >= 4)
  NTRACE_EXPORT void NTRACE_CALL operator ()(const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
#else
  NTRACE_EXPORT void NTRACE_CALL operator ()(const char *fmt, ...);
#endif

private:
  IModule *m_trace_module; ///< Pointer to the module object
  std::string m_function_name;  ///< Name that was determined upon function invocation
  bool m_logged;  ///< Whether an enter() was logged
};

} // namespace

