rm test-report.txt &>/dev/null || true
for diff in ${@//;/ }
do
    if [ -s "$diff" ]
    then
        echo "%%" >> test-report.txt
        echo "%% $diff" >> test-report.txt
        echo "%%" >> test-report.txt
        cat "$diff" >> test-report.txt
        echo >> test-report.txt
    fi
done
