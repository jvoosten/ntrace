#include <iomanip>
#include <iostream>
#include <sstream>

#if defined(_WIN32)
#include <Windows.h>
#endif

#include "debug_output.h"


using namespace NTrace;

DebugOutput::DebugOutput (const Timestamp &start_time)
  :IOutput (start_time)
{
  m_indent = 0;
}

DebugOutput::~DebugOutput ()
{

}

std::string DebugOutput::getName () const 
{
  return "ntrace.debugoutput";
}


void DebugOutput::saveMessage (const Message &msg)
{
  std::ostringstream buf;

  // Log processs ID
  buf << "(" << std::setw (5) << msg.pid << ") ";
  // Timestamp (relative)
  buf << "[" << std::setw (10) << std::fixed << std::setprecision (3) << msg.timestamp.getDifference (m_startTime) << "] ";

  switch (msg.type)
  {
    case Message::Type::Normal:
      buf << m_indentString;
      break;

    case Message::Type::Out:
    case Message::Type::Error:
      buf.clear (); // no prefix
      break;

    case Message::Type::Entry:
      buf << m_indentString << ">> ";
      m_indent++;
      m_indentString.resize (2 * m_indent, ' ');
      break;
     case Message::Type::Exit:
      if (m_indent > 0)
      {
        m_indent--;
      }
      m_indentString.resize (2 * m_indent, ' ');
      buf << m_indentString << "<< ";
      break;
  }

  // Finally add message
  buf << msg.message;

  // Distinguish between error message and regular messages
  if (Message::Type::Error == msg.type)
  {
    std::cerr << buf.str () << std::endl;
  }
  else
  {
    std::cout << buf.str () << std::endl;
  }
#if defined(_WIN32)
  buf << std::endl; // add newline
  OutputDebugString (buf.str ().c_str ());
#endif
}
