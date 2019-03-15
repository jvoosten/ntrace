
#include "output_base.h"

using namespace NTrace;



OutputBase::OutputBase (const std::string &name)
  :m_name (name), m_startTime (IManager::instance ()->getStartTimestamp ())
{
}

OutputBase::OutputBase (const std::string &name, const Timestamp &start_time)
  :m_name (name), m_startTime (start_time)
{
}

OutputBase::~OutputBase ()
{
}


std::string OutputBase::getName () const
{
  return m_name;
}
