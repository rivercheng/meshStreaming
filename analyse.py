#!/usr/bin/env python
import string
import math
import datetime
import sys
import os

input = 'DEFAULT.packet.2009.02.17.21.29.49.txt'
inputFile = open(input)
datarate = {}
for line in inputFile.readlines():
    fields = line.split(' ')
    if fields[2] == 'R':
       sec = int(fields[0])
       if datarate.get(sec) == None:
          datarate[sec] = int(fields[4])
       else:
          datarate[sec] += int(fields[4])
        
inputFile.close()

print "time","datarate"
count = 0
total = 0
for key in datarate.keys():
        print key,datarate[key]
	count = count +1
	total = total + datarate[key]

print "Avarage data rate " , total/count

