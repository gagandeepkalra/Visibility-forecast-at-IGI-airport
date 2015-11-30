#denormalize a single input using the min max

from __future__ import division
import csv

value=input("Enter input: ")

filename = "minmaxFile"
with open(filename, 'r') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    min = reader.next()
    max = reader.next()
    
#print (min)
#print (max)

min = float(min[len(min)-1])
max = float(max[len(max)-1])

print (value * (max - min) + min)