#!/usr/bin/env python
# _*_ coding: utf-8 _*_

## @package serial_control.py
# Author: WeiZhang
# Date: May 29th, 2017
# Update: May 29th, 2017
# Address: CCNU

import sys
import os
import time
from serial import * 
# connet to serial port
ser = Serial('/dev/ttyUSB0', 9600)      #
#================================================================#
## main function
# @para no parameter
def main():
    string = sys.argv[1];
    time.sleep(1)
    ser.write(chr(int(string)))
    print "OK!"
#================================================================#
if __name__ == "__main__":
    sys.exit(main())
