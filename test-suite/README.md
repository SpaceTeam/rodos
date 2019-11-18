
Test Suite
==========

This directory is a test suite for almost all functions of RODOS.

Each function is tested in a .cpp file in core-test, middleware-test etc.
The test programs were executed once and their results were written in the
directories expected-outputs. We check by hand the correct execution of each program.
We consider the results in expected-outputs are right.

To test again, for example after a modification execute again all programs
and store the outputs in a directory `tmp`, then compare these output
with the expected outputs, eg. 

```
  diff -rq tmp expected-outpus
or  better
  meld tmp expected-outputs
```
See the shell scripts

* doit-compare.sh
* execute-all

Some results are not deterministic and will differ, for example  

* Preemption tests
* Asynchronous reading and writing in comm buffers and fifos
* Time reports (resolution of nanoseconds), eg. errorLog
* The end of some executions, because each execution is just terminated after 20 seconds

These cases have to be checked by hand. But this is simple. (use the program meld)

