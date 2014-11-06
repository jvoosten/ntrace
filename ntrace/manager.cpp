#include <fstream>
#include <iostream>
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#include <sys/types.h>
#include <unistd.h>

#include "manager.h"
#include "module.h"

using namespace NTrace;

static Manager *s_pTraceManager = 0;

/***************************************************************************/

Manager::Manager ()
{
  m_indent = 0;
  m_mute = false;
  m_logStream = 0;
  m_maxLines = 0;

  m_logPid = true;
  m_logTime = false;

  char pidbuf[20];
  pidbuf[19] = '\0';
  snprintf (pidbuf, 20, "(%5d)", getpid ());
  m_pidString = pidbuf;
  gettimeofday (&m_startTime, 0);
}

Manager::~Manager ()
{
  s_pTraceManager = 0;
}

// private




// protected

void Manager::incIndent ()
{
  m_indent++;
  m_indentString.clear ();
  m_indentString.resize (m_indent * 2, ' ');
}

void Manager::decIndent ()
{
  if (m_indent > 0)
  {
    m_indent--;
    m_indentString.clear ();
    m_indentString.resize (m_indent * 2, ' ');
  }
}




// public

Manager *Manager::instance ()
{
  if (s_pTraceManager == 0)
    s_pTraceManager = new Manager ();
  return s_pTraceManager;
}

Module *Manager::registerModule (const std::string &module_name, int initial_value)
{
  Module *mod = 0;
  modules_list::iterator mit;

  mit = m_modules.find (module_name);
  if (mit == m_modules.end ())
  {
    mod = new Module (this, module_name, initial_value);

    m_modules[module_name] = mod;
  }
  else
  {
    mod = (*mit).second;
  }
  return mod;
}

void Manager::setLogStream (std::ostream *str)
{
  m_logStream = str;
}

/// Enable/disable logging of process ID

void Manager::setLogPid (bool b)
{
  m_logPid = b;
}

/// Enable/disable logging of elapsed time

void Manager::setLogTime (bool b)
{
  m_logTime = b;
}

void Manager::log (const std::string &log_string)
{
  std::string ns;

  if (m_mute)
    return;

  ns = makePrefix () + m_indentString + log_string;
  m_loggedText.push_back (ns);
  if (m_loggedText.size () >= m_maxLines)
  {
    m_loggedText.pop_front ();
  }

  if (0 != m_logStream)
  {
    *m_logStream << ns << std::endl;
  }
}

void Manager::error (const std::string &err_string)
{
  std::string ns;

  ns = "!" + makePrefix () + m_indentString + err_string;
  m_loggedText.push_back (ns);
  if (m_loggedText.size () >= m_maxLines)
  {
    m_loggedText.pop_front ();
  }
  if (0 != m_logStream)
  {
    *m_logStream << ns << std::endl;
  }
  std::cerr << ns << std::endl;
}

/**
 \brief Output string to standard output
 \param out_string String to display

 Prints a message directly to stdout, including a prefix. A new line ending
 is automatically appended.
 */
void Manager::out (const std::string &out_string)
{
  std::cout << makePrefix () << out_string << std::endl;
}

/**
  \brief Read configuration from stream
  \param str Input stream

 */
void Manager::readConfiguration (std::istream &str)
{
  std::string modname;
  int level;
  modules_list::iterator mit;

  if (!str.good ())
  {
    return;
  }
  while (!str.eof ())
  {
    str >> modname >> level;

    if ("_logPid" == modname)
    {
      m_logPid = (level > 0);
    }
    else if ("_logTime" == modname)
    {
      m_logTime = (level > 0);
    }
    else
    {
      mit = m_modules.find (modname);
      if (mit != m_modules.end ())
      {
        (*mit).second->setLevel (level);
      }
    }
  }
}

/**
 \brief Read configuration from filename
 */

void Manager::readConfiguration (const std::string &filename)
{
  std::fstream tracer;
  tracer.open (filename.c_str (), std::fstream::in);
  readConfiguration (tracer);
  tracer.close ();
}

/**
  \brief Write configuration to stream
  \param str output stream
  \sa readConfiguration


 */
void Manager::writeConfiguration (std::ostream &str)
{
  modules_list::const_iterator mit;
  const Module *mod = 0;

  if (!str.good ())
  {
    return;
  }
  for (mit = m_modules.begin (); mit != m_modules.end (); ++mit)
  {
    mod = (*mit).second;
    str << mod->getName () << " " << mod->getLevel () << std::endl;
  }
  str << "_logPid" << " " << (int) (m_logPid ? 1 : 0) << std::endl;
  str << "_logTime" << " " << (int) (m_logTime ? 1 : 0) << std::endl;
}

void Manager::writeConfiguration (const std::string &filename)
{
  std::fstream tracer;
  tracer.open (filename.c_str (), std::fstream::out | std::fstream::trunc);
  writeConfiguration (tracer);
  tracer.close ();
}

void Manager::clearText ()
{
  m_loggedText.clear ();
}

void Manager::mark ()
{
  m_loggedText.push_back ("-------------- MARK ---------------");
  if (m_loggedText.size () >= m_maxLines)
  {
    m_loggedText.pop_front ();
  }
}

void Manager::setMute (bool mute)
{
  m_mute = mute;
}

/**
 \brief Create prefix for log message
 
 */
std::string Manager::makePrefix ()
{
  std::string ret;

  if (m_logPid || m_logTime)
  {
    if (m_logPid)
    {
      ret += m_pidString;
    }
    if (m_logTime)
    {
      char timebuf[20];
      struct timeval now;
      gettimeofday (&now, 0);

      int sec = now.tv_sec - m_startTime.tv_sec;
      int usec = now.tv_usec - m_startTime.tv_usec;
      if (usec < 0)
      {
        usec += 1000000;
        sec--;
      }
      snprintf (timebuf, 20, "[%4d.%03d]", sec, usec / 1000);
      ret += timebuf;
    }
    ret += " ";
  }
  return ret;
}
