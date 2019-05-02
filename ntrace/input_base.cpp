#include "input_base.h"

using namespace NTrace;

InputBase::InputBase (IManager *mgr, const std::string &name)
  : IInput (mgr), m_name (name)
{
  // nop
}

InputBase::~InputBase ()
{
  // nop
}


IManager *InputBase::getManager () const
{
  return m_manager;
}

std::string InputBase::getName () const
{
  return m_name;
}

