# rodos-middleware: This file is a part of the Master Thesis of Sebastian Kind 2023

import struct

"""
this file should contain routines to calculate rodos checksums of
the NetworkMessage type

"""


def rotateRight(checksum):
    if checksum & 0x01:
        checksum = (((checksum & 0xFFFF) >> 1) | 0x8000)
    else:
        checksum = (checksum & 0xFFFF) >> 1
    return checksum & 0xFFFF



def calcCheckSum(data: bytes, length=0) -> int:
    if length == 0:
        length = len(data)
    checksum = 0
    cnt = 0
    for i in range(length):
        checksum = rotateRight((checksum & 0xFFFF))
        checksum = (checksum & 0xFFFF) + (data[cnt] & 0xFF)
        cnt += 1


    result = (checksum & 0xFFFF)
    return result

