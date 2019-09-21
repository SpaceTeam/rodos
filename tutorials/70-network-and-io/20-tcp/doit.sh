
# Waning: For Linux we do not need the ipstack.cpp, but
# for embedded you have to compile and link to client and server


rodos-executable.sh linux tcp-server.cpp
xterm -e tst &

rodos-executable.sh linux tcp-client.cpp
tst
