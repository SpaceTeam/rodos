
echo "no gateway, no distribution, all 3 tasks in the same node"

set -e

rodos-executable.sh  linux   demo_topics.cpp  sender.cpp receiver1.cpp  receiver2.cpp 
tst

