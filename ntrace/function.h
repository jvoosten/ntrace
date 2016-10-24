#ifndef TRACEFUNCTION_HPP
#define TRACEFUNCTION_HPP

#include <string>

/**
  \brief Helper class to automatically log enter and leave events

*/

namespace NTrace
{

class Module;

class Function
{
public:
  Function(Module *trace_module, const char *funcname);
  ~Function();

  void operator ()();
#if defined(__GCC__) && (__GCC__ >= 4)
  void operator ()(const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
#else
  void operator ()(const char *fmt, ...);
#endif

private:
  Module *m_trace_module;
  std::string m_function_name;
  bool m_logged;
};

} // namespace

#endif
