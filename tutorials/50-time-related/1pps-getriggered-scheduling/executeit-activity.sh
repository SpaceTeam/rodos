
set -e

rodos-executable.sh linux scheduler-1pps-getriggert.cpp  example-activities1pps.cpp

cat << EOT

________________________________________________
       to test please
       in one term start tst
       in other term type killall tst
       or execute the script generate-1pps.sh
________________________________________________

EOT

tst
