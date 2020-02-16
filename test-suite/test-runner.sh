echo "::: Running $1 against $2"

# Kill test executable (with SIGKILL) after 8 seconds.  Necessary
# because sometimes tests might deadlock.
timeout -s 9 8 $1 > "$1.output"

# Remove header / intro from output and expected output
linecount=`wc -l < "$2"`
pattern="-----------------------------------------------------"
grep -h -B 0 -A $linecount -e $pattern $1.output > $1.output_trimmed
grep -h -B 0 -A $linecount -e $pattern $2 > $1.expected_trimmed

# Compare acutal with expected output
diff -ruh $1.output_trimmed $1.expected_trimmed
