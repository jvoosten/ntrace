#pragma once

#include "interfaces.h"

namespace NTrace
{

/**
  \brief Some common/handy functions for output objects.

  */
class OutputBase : public IOutput
{
public:
  virtual std::string NTRACE_CALL getName () const;

protected:
  OutputBase (const std::string &name);
  OutputBase (const std::string &name, const Timestamp &start_time);
  ~OutputBase ();

  const Timestamp m_startTime;

private:
  std::string m_name;
};


}