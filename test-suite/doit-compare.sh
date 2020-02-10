#!/bin/bash
#rodos-lib.sh linuxMC


\rm -rf tmp

echo "__________________________________________________________ core fast test programs"
execute-all.sh   linux        core-fast       tmp

echo "__________________________________________________________ core slow test programs"
execute-all.sh   linux        core-slow       tmp

echo "__________________________________________________________ middleware test programs"
execute-all.sh   linux        middleware-tests tmp

echo "__________________________________________________________ middleware gateway"
cd middleware-gateway
./executeit.sh   linux                         tmp
cd ..

echo "__________________________________________________________ Embedded test programs"
execute-all.sh   linux        embedded-test     tmp-embedded



cat << EOT
__________________________________________________________ Diffs
 ++++ preemptiontest-and-ceiler and combuffer-test are not deterministric 
      compare by hand
 please compare (diff -rq or meld) :
 diff -rq   expected-outputs           tmp
 diff -rq   expected-embedded-outputs  tmp-embedded
_________________________________________________________________

