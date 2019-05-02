# Example program

This directory contains a small test program that highlights some of the
possibilities of NTrace.

## Invocation

The program has 3 options:

* -d Use the standard (debug) output for the log messages
* -f Use a file for logging; the filename can be supplied as an optional parameter
* -l For the initial debug level.


Note that the initial debug level (without the '-l' option) is Notice (5); therefor
you will not see all messages from inside the func2() function. However, if
you specify '-l6' or '-l7' you will see all of them.

## Threading

Also if you run this program you may see that the debug output is mixed with
the regular output; this is normal since the NTrace output runs in a separate
thread. For an example, see output.txt
