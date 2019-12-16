
#mask-generator < mask.txt


mkdir -p  ../$2


rodos-executable.sh $1  gateway.cpp demo_topics.cpp    receiver-int64top.cpp
tst | grep -v "Node Number"  > ../$2/gateway-rec64 &
sleep 1
rodos-executable.sh $1  gateway.cpp demo_topics.cpp    receiver-int32top.cpp
tst | grep -v "Node Number"  > ../$2/gateway-rec32 &
sleep 1
rodos-executable.sh $1  gateway.cpp demo_topics.cpp    receiver-doubletop.cpp
tst | grep -v "Node Number"  > ../$2/gateway-recdoubl &
sleep 1
rodos-executable.sh $1  gateway.cpp demo_topics.cpp    sender.cpp
tst | grep -v "Node Number"  > ../$2/gateway-sender




