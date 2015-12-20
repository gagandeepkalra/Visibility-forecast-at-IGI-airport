# normalize the csv which will be provided as the stdin, and print the normalized output to stdout. it also prints
# scaleRangeMin, scaleRangeMax, min_array, max_array, sum_array, mean_array. sd_array, totalRows to a file: minmaxFile

import csv
import sys
import optparse

parser = optparse.OptionParser()
parser.add_option("-e", "--enableHeader", action="store", dest="enableHeader",
                  default=False, help="enableHeader enables the csv header")
parser.add_option("-n", "--denormalize", action="store", dest="denormalizeFileName",
                  help="file to denorm")
parser.add_option("-m", "--minmaxFile", action="store", dest="minmaxFileName",
                  help="file to which min, max, sum, mean, sd etc are written")
options, args = parser.parse_args()

enableHeader = options.enableHeader
denormalizeFileName = options.denormalizeFileName
minmaxFileName = options.minmaxFileName
digPlaces = 1

with open(minmaxFileName, 'r') as minMaxFile:
    reader = csv.reader(minMaxFile, delimiter=',')
    buffer = next(reader)
    scaleRangeMin = float(buffer[0])
    scaleRangeMax = float(buffer[1])
    min_array = [float(x) for x in next(reader)]
    max_array = [float(x) for x in next(reader)]
    sum_array = [float(x) for x in next(reader)]
    mean_array = [float(x) for x in next(reader)]
    sd_array = [float(x) for x in next(reader)]
    totalRows = float(next(reader)[0])


def denormalize(val, mean, sd, minval, maxval):
    """
    :param val: float
    :param mean:float
    :param sd:float
    :param minval:float
    :param maxval:float
    :rtype: float
    """

    val = (((val - scaleRangeMin)*(maxval - minval)) / (scaleRangeMax - scaleRangeMin)) + minval
    val *= sd
    val += mean
    val = round(val, digPlaces)
    if int(val) == val:
        return int(val)
    return val

output = csv.writer(sys.stdout, delimiter=",")
# output.writerow(["currOutput", "desiredOutput", "error"])
with open(denormalizeFileName, 'r') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    if enableHeader:
        output.writerow(next(reader) + ["error"])
    for row in reader:
        result = []
        for i in range(0, len(row)-2):
            result.append(denormalize(float(row[i]), mean_array[i], sd_array[i], min_array[i], max_array[i]))
        actualOutput = denormalize(float(row[-2]), mean_array[-1], sd_array[-1], min_array[-1], max_array[-1])
        desiredOutput = denormalize(float(row[-1]), mean_array[-1], sd_array[-1], min_array[-1], max_array[-1])
        error = round(actualOutput - desiredOutput, digPlaces)

        result.append(actualOutput)
        result.append(desiredOutput)
        result.append(error)
        output.writerow(result)
