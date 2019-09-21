#!/bin/sh

GATEWAY=gateway-forward-all.cpp



rodos-executable.sh linux ${GATEWAY} demo_topics.cpp receiver.cpp
mv tst tst-recv

