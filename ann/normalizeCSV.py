# normalizes the csv and prints the normalized output to stdout.
# it also prints scaleRangeMin, scaleRangeMax, min_array, max_array, sum_array, mean_array. sd_array,
# totalRows to a file path given by minmaxFile option

import csv
import sys
import optparse

parser = optparse.OptionParser()
parser.add_option("-e", "--enableHeader", action="store", dest="enableHeader",
                  default=False, help="enableHeader enables the csv header")
parser.add_option("-n", "--normalize", action="store", dest="normalizeFileName",
                  help="file to normalize")
parser.add_option("-m", "--minmaxFile", action="store", dest="minmaxFileName",
                  help="file to which min, max, sum, mean, sd etc are written")
options, args = parser.parse_args()

enableHeader = options.enableHeader
normalizeFileName = options.normalizeFileName
minmaxFileName = options.minmaxFileName

digPlaces = 6

scaleRangeMin = -1
scaleRangeMax = +1

max_array = []  # max value of the column
min_array = []  # min value of the column
sum_array = []  # sum of all the values in the column
mean_array = []  # mean of the values in the column
sd_array = []  # standard deviation of the column
totalRows = 0  # totalRows in the file

# calc max_array, min_array, sum_array
with open(normalizeFileName, 'r') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    if enableHeader:
        header = next(reader)
    for row in reader:
        totalRows += 1
        for i in range(0, len(row)):
            if i >= len(max_array):
                max_array.append(float(row[i]))
                min_array.append(float(row[i]))
                sum_array.append(float(row[i]))
            else:
                if min_array[i] > float(row[i]):
                    min_array[i] = float(row[i])
                if max_array[i] < float(row[i]):
                    max_array[i] = float(row[i])
                sum_array[i] += float(row[i])

# calculate mean
for i in range(0, len(sum_array)):
    mean_array.append(round(sum_array[i] / totalRows, digPlaces))
    sum_array[i] = round(sum_array[i], digPlaces)

# calculate standard deviation
with open(normalizeFileName, 'r') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    if enableHeader:
        header = next(reader)
    for row in reader:
        for i in range(0, len(row)):
            if i >= len(sd_array):
                sd_array.append((float(row[i]) - mean_array[i]) ** 2)
            else:
                sd_array[i] += (float(row[i]) - mean_array[i]) ** 2
for i in range(0, len(sd_array)):
    sd_array[i] = pow(sd_array[i] / totalRows, 0.5)
    sd_array[i] = round(sd_array[i], digPlaces)

# normalize min_array, max_array so that they can be used in the range scaling formula
for i in range(0, len(min_array)):
    min_array[i] = (min_array[i] - mean_array[i]) / sd_array[i]
    max_array[i] = (max_array[i] - mean_array[i]) / sd_array[i]

# normalize
with open(normalizeFileName, 'r') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    if enableHeader:
        header = next(reader)
    l = list(reader)
    result = []
    for i in range(0, len(l)):
        if i >= len(result):
            result.append([])
        for j in range(0, len(l[i])):
            val = float(l[i][j]) - mean_array[j]  # make zero-mean by removing average
            val = val / sd_array[j]  # make variance 1 by dividing by standard deviation

            val = (((scaleRangeMax - scaleRangeMin) * (val - min_array[j])) /
                   (max_array[j] - min_array[j])) + scaleRangeMin  # scale the value in the required range
            val = round(val, digPlaces)
            result[i].append(val)

t = csv.writer(sys.stdout, delimiter=",")
if enableHeader:
    t.writerow(header)
for i in result:
    t.writerow(i)

# normalize min max values

with open(minmaxFileName, 'w') as minmaxFile:
    t = csv.writer(minmaxFile, delimiter=",")
    t.writerow([scaleRangeMin, scaleRangeMax])
    t.writerow(min_array)
    t.writerow(max_array)
    t.writerow(sum_array)
    t.writerow(mean_array)
    t.writerow(sd_array)
    t.writerow([totalRows])
