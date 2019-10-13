#!/bin/bash
#rodos-lib.sh linuxMC

execute-all.sh linux        core-tests       tmp
diff -rq                    core-expected    tmp


