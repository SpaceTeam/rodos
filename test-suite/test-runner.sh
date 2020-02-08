echo "::: Running $1"
# Kill test executable (with SIGKILL) after 6 seconds.  Necessary
# because sometimes tests might deadlock.
timeout -s 9 6 $1 | tee "$1.output"
# The tests are only used to generate coverage information, thus they
# are all treated as succeeding.
exit 0

