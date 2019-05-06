
#include "output_base.h"

using namespace NTrace;



OutputBase::OutputBase (const std::string &name)
  :m_name (name), m_startTime (0, 0)
{
}

OutputBase::OutputBase (const std::string &name, const Timestamp &start_time)
  :m_name (name), m_startTime (start_time)
{
}

std::string OutputBase::getName () const
{
  return m_name;
}
