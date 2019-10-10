#!/bin/bash
#rodos-lib.sh linuxMC

execute-all    linux        core-tests       tmp
diff -rq                    core-expected    tmp


