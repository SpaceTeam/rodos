#!/bin/bash
#rodos-lib.sh linuxMC

\rm -rf expected-outputs

echo "__________________________________________________________ core fast test programs"
execute-all.sh   linux        core-fast       expected-outputs

echo "__________________________________________________________ core slow test programs"
execute-all.sh   linux        core-slow       expected-outputs

echo "__________________________________________________________ middleware test programs"
execute-all.sh   linux        middleware-tests expected-outputs

echo "__________________________________________________________ Embedded test programs"
execute-all.sh   linux        embedded-test     expected-embedded-outputs


