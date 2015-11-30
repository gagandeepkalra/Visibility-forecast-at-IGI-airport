#normalize the csv which will be provied as the stdin, and print the normalized output to stdout. also print min, max values of last column to minmaxFile

import csv
import sys

filename=input()
max_array=[]
min_array = []
with open(filename, 'r') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    for row in reader:
        for i in range(0,len(row)):
            if i>=len(max_array):
                max_array.append(float(row[i]))
                min_array.append(float(row[i]))
            else:
                if min_array[i] > float(row[i]):
                    min_array[i] = float(row[i])
                if max_array[i] < float(row[i]):
                    max_array[i] = float(row[i])
    #normalize
with open(filename, 'r') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    l=list(reader)
    result=[]
    for i in range(0,len(l)):
        if i>=len(result):
            result.append([])
        for j in range(0,len(l[i])):
            result[i].append((float(l[i][j])-min_array[j])/(max_array[j]-min_array[j]))

t=csv.writer(sys.stdout,delimiter=",")
for i in result:
    t.writerow(i)

#normalize min max values

with open('minmaxFile', 'w') as minmaxFile:
    t=csv.writer(minmaxFile, delimiter=",")
    t.writerow(min_array)    
    t.writerow(max_array)
