#normalizes only one input line from std out using min max from minmaxFile

import csv
import sys

temp = raw_input()

input = temp.split()

if len(input) != 7:
    sys.stderr.write("7 inputs not provided\nExiting...\n")
    sys.exit(1)
    
filename = 'minmaxFile'
with open(filename, 'r') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    min = reader.next()
    max = reader.next()

final = []
for i in range(0,len(min)):
    ans = (float(input[i]) * (float(max[i]) - float(min[i]))) + float(min[i])
    #final = final + str(ans)
    final.append(str(ans))

delim = " "
print delim.join(final)
