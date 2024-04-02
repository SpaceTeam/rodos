# rodos-middleware: This file is a part of the Master Thesis of Sebastian Kind 2023


import socket
import threading
import queue
import select

from . import networkmessage
from . linkinterface import *

DEBUG_RAW_UDP_FLAG = False

class LinkinterfaceUDP(LinkinterfaceI):


    UDP_PORT = 50000
    UDP_IP =  "255.255.255.255"

    messageQueue = queue.Queue()

    def __init__(self, UDP_PORT=50000):
        self.UDP_PORT=UDP_PORT
        self.BIGENDIAN_P = True
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR , 1)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        self.sock.bind((self.UDP_IP, self.UDP_PORT))

        target = threading.Thread(target=self.__receiveLoop, args=[])
        target.daemon = True
        target.start()


    def sendNetworkMsg(self, msg: networkmessage.NetworkMessage):
        """
        Send Data over UDP Socket

        msg: NetworkMessage to be sent

        """
        data = msg.getBinaryMsg()
        #print("raaawww", data)
        self.sock.sendto(data, (self.UDP_IP, self.UDP_PORT))
        #print("sock has sent")

    def getNetworkMsg(self):
        """returns received message from queue to the gateway"""
        return self.messageQueue.get()

    def __receiveLoop(self):
        """constantly reads from UDP Socket and put packets in Queue. Runs in own thread"""

        # loop over socket and read data if it becomes available
        while True:
            # print ("select loop")
            ready = select.select([self.sock], [], [], 60)
            if ready[0]:
                data = self.sock.recv(1300)
                if DEBUG_RAW_UDP_FLAG:
                    print("")
                    print("")
                    print("GOT THIS FROM UDP:", data)
                    print("")
                    print("")
                self.messageQueue.put(data)


        pass


