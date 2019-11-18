#!/bin/bash
#rodos-lib.sh linuxMC


\rm -rf tmp

echo "__________________________________________________________ core fast test programs"
execute-all.sh   linux        core-fast       tmp

echo "__________________________________________________________ core slow test programs"
execute-all.sh   linux        core-slow       tmp

echo "__________________________________________________________ middleware test programs"
execute-all.sh   linux        middleware-tests tmp


echo "__________________________________________________________ Diffs "
echo " ++++ preemptiontest-and-ceiler ist not deterministric compare by hand"

diff -rq                    expected-outputs  tmp


