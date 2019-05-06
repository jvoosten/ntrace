#include "pch.h"

#include "ntrace.h"


TR_MODULE ("OtherFile");

void other_function (int x)
{
  TR_FUNC ("%d", x);


  int y = x * x;
  TR (3, "Result = %d", y);
}
