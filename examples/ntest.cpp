/**
 \brief A simple program to test various NTrace functions.

 Call with these command line options:

  -d : show debug output
  -f : use file logging (optional filename, defaults to 'ntest.log')

 */

#include <iostream>
#include <string>
#include <unistd.h>

#include "../ntrace.h"

TR_MODULE ("ntest");

static void help (const char *msg)
{
  std::cout << "ntest: a program to test NTrace functionality" << std::endl;
  if (msg)
  {
    std::cout << msg << std::endl;
  }
  std::cout << "  Options:" << std::endl;
  std::cout << "  -d            Use stdout and stderr for output" << std::endl;
  std::cout << "  -f[filename]  Use a file for logging; the filename is optional" << std::endl;
  std::cout << "                The file rotates after 1 kilobyte (1024 bytes) and keeps 5" << std::endl;
  std::cout << "                versions of the log files; older ones are removed." << std::endl;
}


// Do some math work (fibonacci sequence)

static int func3 (int fib)
{
  int f1 = 0;
  int f2 = 1;
  int f3 = 0;
  while (fib > 0)
  {
    f3 = f1 + f2;
    f1 = f2;
    f2 = f3;
    fib--;
  }
  TR(1, "fib = %d", f3);
  sleep(1);
  return f3;
}

// Print a little nursery rhyme

static void func2 (int number_of_kids)
{
  TR_FUNC ("number of kids: %d", number_of_kids);

  if (number_of_kids == 0)
  {
    TR (NTrace::Warning, "Removing last kid");
    std::cout << "And then there were none." << std::endl;
  }
  else if(number_of_kids == 1)
  {
    TR (NTrace::Info, "Removing one kid");
    std::cout << number_of_kids << " child was sitting on a fence, then it fell down." << std::endl;
  }
  else
  {
    TR (NTrace::Info, "Removing one kid");
    std::cout << number_of_kids << " children were sitting on a fence, then one fell down." << std::endl;
  }

  func3 (number_of_kids * number_of_kids);
}

// The main function

static void func1 ()
{
  TR_FUNC ();

  for (int i = 10; i >= 0; i--)
  {
    func2 (i);
  }
}

int main (int argc, char *argv[])
{
  bool enable_debug = false;
  bool enable_file = false;
  std::string filename = "ntest";
  int opt = 0;

  while ((opt = getopt (argc, argv, "df::")) != -1)
  {
    switch (opt)
    {
      case 'd':
        enable_debug = true;
        break;
      case 'f':
        enable_file = true;
        if (optarg != 0)
        {
          filename = optarg;
        }
        break;
      default:
        help ("Unknown argument");
        exit (1);
        break;
    }
  }

  if (enable_debug == false && enable_file == false)
  {
    help ("Error: no argument given");
    exit (1);
  }

  NTrace::IManager *ntrace_mgr = NTrace::IManager::instance ();
  if (enable_debug)
  {
    ntrace_mgr->enableDebugOutput ();
  }
  if (enable_file)
  {
    NTrace::FileOutput *fo = new NTrace::FileOutput (filename, ".log", 1024, 5);
    ntrace_mgr->addOutput (fo);
  }

  func1 ();

  ntrace_mgr->shutdown ();
  return 0;
}

