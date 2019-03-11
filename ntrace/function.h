#pragma once

#include <string>

#include "ntrace_exports.h"

namespace NTrace
{

class IInput;

/**
  \brief Helper class to automatically log enter and leave events

  By creating a Function object inside your function, entry and exit messages are
  automatically generated (due to the wonders of OOP programming and destructors);
*/

class Function
{
public:
  NTRACE_EXPORT Function (IInput *trace_module, const char *funcname);
  NTRACE_EXPORT ~Function ();

  NTRACE_EXPORT void NTRACE_CALL operator ()();
#if defined(__GCC__) && (__GCC__ >= 4)
  NTRACE_EXPORT void NTRACE_CALL operator ()(const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
#else
  NTRACE_EXPORT void NTRACE_CALL operator ()(const char *fmt, ...);
#endif

private:
  IInput *m_trace_module;
  std::string m_function_name;
  bool m_logged;
};

} // namespace

