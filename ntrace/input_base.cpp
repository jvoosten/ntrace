#include "input_base.h"

using namespace NTrace;

InputBase::InputBase (IManager *mgr, const std::string &name)
  : IInput (mgr), m_name (name)
{
  // nop
}

std::string InputBase::getName () const
{
  return m_name;
}

