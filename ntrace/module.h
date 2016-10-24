#ifndef TRACEMODULE_H
#define TRACEMODULE_H


namespace NTrace
{

class Manager; // forward

class Module
{
private:
  Manager *m_manager;
  std::string m_moduleName;
  int m_level;
  char m_buffer[2048];

public:
  Module (Manager *mgr, const std::string &name, int level);

  std::string getName () const;
  int getLevel () const;

  void enter (const std::string &function_name);
  void enter (const std::string &function_name, const std::string &args);
  void enter (const std::string &file_name, int line_number);
  void leave (const std::string &function_name);
  void leave (const std::string &file_name, int line_number);
#if defined(__GCC__) && (__GCC__ >= 4)
  void log (int level, const char *fmt, ...) __attribute__ ((format (printf, 3, 4)));
  void error (const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
  void out (const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
#else
  void log (int level, const char *fmt, ...);
  void error (const char *fmt, ...);
  void out (const char *fmt, ...);
#endif
  void log (int level, const std::string &msg);
  void error (const std::string &msg);
  void out (const std::string &msg);

  void incLevel ();
  void decLevel ();
  void setLevel (int);
};

} // namespace

#endif
