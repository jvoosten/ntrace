// NTraceTest.cpp : Windows version of a test program for NTrace
//

#include "pch.h"
#include <iostream>

#include "ntrace.h"


#include "other_file.h"

TR_MODULE ("main");


int main()
{
    std::cout << "NTrace Windows Test Program!" << std::endl;

    NTrace::IManager *mgr = NTrace::IManager::instance ();
    mgr->enableDebugOutput ();

    // get list of modules, print names and debug levels
    std::list<NTrace::IModule *> modules = mgr->getModules ();
    for (std::list<NTrace::IModule *>::const_iterator it = modules.begin (); it != modules.end (); ++it)
    {
      std::cout << (*it)->getName () << ": " << (*it)->getLevel () << ", " << ((*it)->getFunctionTracking () ? "enabled" : "disabled") << std::endl;
    }

    other_function (42);

    NTrace::IManager::shutdown ();
    return 0;
}

