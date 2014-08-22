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
  void operator ()(const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));

private:
  Module *m_trace_module;
  std::string m_function_name;
  bool m_logged;
};

} // namespace

#endif
