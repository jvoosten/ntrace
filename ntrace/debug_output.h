#pragma once


#include "interfaces.h"

namespace NTrace
{


/**
\brief Write message to default debug output



*/
class DebugOutput: public IOutput
{
public:
  DebugOutput (const Timestamp &start_time);
  ~DebugOutput ();

  virtual std::string NTRACE_CALL getName () const;
  virtual void NTRACE_CALL saveMessage (const Message &msg);

private:
  int m_indent;
  std::string m_indentString;
  std::string m_prefix;
};

}