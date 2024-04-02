# rodos-middleware: This file is a part of the Master Thesis of Sebastian Kind 2023

import socket
import threading
import queue
import select

import serial

from .networkmessage import *
from .linkinterface import *




class LinkinterfaceUART(LinkinterfaceI):


    messageQueue = queue.Queue()

    def __init__(self, path="/dev/rfcomm0"):
        self.ser = serial.Serial(path)
        print(self.ser.name)
        self.BIGENDIAN_P = False

        target = threading.Thread(target=self.__receiveLoop, args=[])
        target.daemon = True
        target.start()

    def sendNetworkMsg(self, msg: NetworkMessage):
        """
        Send Data over serial device

        msg: NetworkMessage to be sent

        """

        # msg = NetworkMessage()
        data = msg.getBinaryMsg()

        s3pframe = self.toS3p(data)

        self.ser.write(s3pframe)

    def getNetworkMsg(self):
        """returns received message from queue to the gateway"""
        return self.messageQueue.get()

    def __receiveLoop(self):
        """constantly reads from UART and puts data in Queue. Runs in own thread"""

        # RODOS Uart messages are framed within S3P (look the at the documentation)
        inputbuffer = []

        MARK = b"\xfe"
        BOM = b"\x02"
        EOM = b'\x03'
        STUFFING = b'\x7e'
        lastWasMark = False

        while True:
            inputbuffer = []
            lastWasMark = False
            while True:
                # check every byte coming from UART and parse S3P-Framing
                b = b""
                try:
                    b = self.ser.read(1)
                except:
                    print("linkinterface read exeption")
                if b == MARK:
                    lastWasMark = True
                    continue
                elif b == BOM and lastWasMark:
                    lastWasMark = False
                    continue
                elif b == STUFFING and lastWasMark:
                    lastWasMark = False
                    inputbuffer.append(0xFE)
                elif b == EOM and lastWasMark:
                    lastWasMark = False
                    self.messageQueue.put(bytes(inputbuffer))
                    break
                else:
                    inputbuffer.append(b[0])

            if False:
                print("")
                print("")
                print("GOT THIS FROM uart:", bytes(inputbuffer))
                print("")
                print("")
            # print ("select loop")
            #
            # ready = select.select([self.sock], [], [], 60)
            # if ready[0]:
            #     data = self.sock.recv(1300)
            #     self.messageQueue.put(data)


    def toS3p(self,data):
        """return s3p framed bytes of message"""
        BOM = [0xFE, 0x02]
        EOM = [0xFE, 0x03]
        escapedData =[]
        for b in data:
            if b == 0xFE:
                escapedData += [0xFE, 0x7E]
            else:
                escapedData += [b]

        frame = bytes(BOM + escapedData + EOM)
        return frame


