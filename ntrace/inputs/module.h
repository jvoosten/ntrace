#pragma once

#include "../input_base.h"


#pragma warning (push)
// Surpress 'inherits X via dominance warning'.
#pragma warning (disable: 4250)

namespace NTrace
{

/**
  \brief Implemenation of IModule interface.

  The tracemodule is the object that gets instantiated in your source file.
  It forwards log events to the TraceManager

  Note: IModule is inherited for the function definitions, InputBase for the implementation
*/
class Module: public virtual IModule, public virtual InputBase
{
public:
  Module (IManager *mgr, const std::string &name, int level = Level::Notice, bool track_enter_leave = true);

  virtual int NTRACE_CALL getLevel () const;
  virtual void NTRACE_CALL setLevel (int level);
  virtual bool NTRACE_CALL getFunctionTracking () const;
  virtual void NTRACE_CALL setFunctionTracking (bool enable);

#if defined(__GCC__) && (__GCC__ >= 4)
  virtual void NTRACE_CALL log (int level, const char *fmt, ...) __attribute__ ((format (printf, 3, 4)));
  virtual void NTRACE_CALL error (const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
  virtual void NTRACE_CALL out (const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
#else
  virtual void NTRACE_CALL log (int level, const char *fmt, ...);
  virtual void NTRACE_CALL error (const char *fmt, ...);
  virtual void NTRACE_CALL out (const char *fmt, ...);
#endif
  virtual void NTRACE_CALL log (int level, const std::string &msg);
  virtual void NTRACE_CALL error (const std::string &msg);
  virtual void NTRACE_CALL out (const std::string &msg);
  virtual void NTRACE_CALL enter (const std::string &function);
  virtual void NTRACE_CALL enter (const std::string &function, const std::string &args);
  virtual void NTRACE_CALL leave (const std::string &function);

private:
  int m_level; ///< Our current log level
  bool m_functionTracking; ///< if true, function tracking is enabled
  static const int s_buffersize = 2048;
	char m_buffer[s_buffersize]; ///< Local buffer for all formatting
};

} // namespace

#pragma warning (pop)
