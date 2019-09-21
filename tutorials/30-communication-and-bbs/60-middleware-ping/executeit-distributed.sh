
set -e

rodos-executable.sh  linux mytopics.cpp pong.cpp
xterm -bg white  -fg black -title pongReceivers -e tst &

sleep 2

rodos-executable.sh linux  mytopics.cpp ping.cpp
xterm -bg white  -fg black -title pingSender -e tst &

echo "CR to close all"
read JA

kill $(jobs -p)


