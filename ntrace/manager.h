#ifndef TRACEMANAGER_H
#define TRACEMANAGER_H

#include <deque>
#include <map>
#include <string>

#include <sys/time.h>

namespace NTrace
{

class Module;

class Manager
{
  friend class Module;
private:
  typedef std::map<std::string, Module *> modules_list;

  modules_list m_modules;
  std::ostream *m_logStream;

  bool m_mute;

  int m_indent;
  std::string m_indentString;
  std::string m_pidString;
  std::deque<std::string> m_loggedText;
  int m_maxLines;

  bool m_logPid;
  bool m_logTime;

  struct timeval m_startTime;

  std::string makePrefix ();

protected:
  Manager ();

  void incIndent ();
  void decIndent ();

public:
  ~Manager ();

  static Manager *instance ();
  Module *registerModule (const std::string &module_name, int initial_value = 1);

  void setLogStream (std::ostream *str);
  void setLogPid (bool b);
  void setLogTime (bool b);

  void log (const std::string &log_string);
  void error (const std::string &err_string);
  void out (const std::string &out_string);

  void readConfiguration (std::istream &str);
  void readConfiguration (const std::string &filename);
  void writeConfiguration (std::ostream &str);
  void writeConfiguration (const std::string &filename);

  void clearText ();
  void mark ();
  void setMute (bool);
};

} // namespace


#endif
