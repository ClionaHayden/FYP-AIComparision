import csv

TrainingData = {}

def loadTrainingData():
    with open('FYP - Comparison between two AI/FYP - Comparison between two AI/DATA/TrainingData.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            TrainingData[line_count] = {'left' : row[0], 'right' : row[1], 'up' : row[2], 'down' : row[3]}
            line_count += 1
        print(f'Processed {line_count} lines.')

def writeData():
    with open('FYP - Comparison between two AI/FYP - Comparison between two AI/DATA/temp.csv', mode='w') as csv_file:
        csv_writer = csv.writer(csv_file, delimiter=',', quotechar='"', lineterminator = '\n')
        for row in TrainingData:
          csv_writer.writerow([TrainingData[row]['left'], TrainingData[row]['right'], TrainingData[row]['up'],TrainingData[row]['down']])

loadTrainingData()
writeData()