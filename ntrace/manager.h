#pragma once

#include <condition_variable>
#include <deque>
#include <list>
#include <map>
#include <string>

#include <atomic>
#include <mutex>
#include <thread>

#include "interfaces.h"
#include "timestamp.h"

namespace NTrace
{

class Module;

/**
\brief Instance of central logging manager

Central logging manager; there is only ever one instance of this object (it's a singleton).
It will accept messages and distribute them to the output objects.

\note Important note: the manager is thread-safe, and uses threads to handle writing
messages to the output(s). This is to minimize impact on execution speed of the main
program while some output may take a while to process.

*/


class Manager: public IManager
{
public:
  Manager ();
  ~Manager ();

  virtual Timestamp NTRACE_CALL getStartTimestamp () const;

  virtual IModule *NTRACE_CALL registerModule (const std::string &module_name, int initial_log_level);

  virtual std::list<IModule *> NTRACE_CALL getModules ();

  virtual std::list<std::weak_ptr<IOutput>> NTRACE_CALL getOutputs ();
  virtual void NTRACE_CALL addOutput (IOutput *out);
  virtual void NTRACE_CALL removeOutput (IOutput *out);

  virtual void NTRACE_CALL pushMessage (const Message &msg);

  virtual void NTRACE_CALL enableDebugOutput ();

//  void log (const std::string &log_string);
//  void error (const std::string &err_string);
//  void out (const std::string &out_string);

  void readConfiguration (std::istream &str);
  void readConfiguration (const std::string &filename);
  void writeConfiguration (std::ostream &str);
  void writeConfiguration (const std::string &filename);
  //void setLogStream (std::ostream *str);
  //void setLogStream (const std::string &filename);

//  void clearText ();
//  void mark ();
//  void setMute (bool);

protected:

  void start ();
  void stop ();

  void outputLoop ();

private:
  // Our modules
  typedef std::map<std::string, Module *> modules_list;
  modules_list m_modules;
  std::mutex m_modulesMutex;

  // Output objects
  typedef std::shared_ptr<IOutput> output_ptr;
  std::list<output_ptr> m_outputs;
  std::mutex m_outputsMutex;

  // The messages
  std::deque<Message> m_messages;
  std::mutex m_messagesMutex;
  std::condition_variable m_messagesAvailable;

  std::thread m_outputThread;
  std::atomic<bool> m_endLoop;
  //std::ostream *m_logStream;
  //bool m_ownLogStream;

  //bool m_mute;

  //std::string m_pidString;
  //std::deque<std::string> m_loggedText;
  //unsigned int m_maxLines;

  //bool m_logPid;
  //bool m_logTime;
  //bool m_logToStdout;

  Timestamp m_startTime;
};

} // namespace


