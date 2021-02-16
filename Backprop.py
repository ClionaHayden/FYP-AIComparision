import csv
from random import seed
from random import randrange
import array as arr
import tensorflow.compat.v1 as tf
tf.disable_v2_behavior() 

seed(100)
TrainingData = {}
inputs = arr.array('d',[0, 0, 0, 0, 0])
outputs = arr.array('d',[0, 0, 0, 0, 0])
weights = arr.array('d')
biases = arr.array('d')

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

def setupWeightsBias():
    i = 0
    while i < 8:
        i += 1
        weights.append(randrange(-1,1))
    i = 0
    while i < 4:
        i += 1
        biases.append(randrange(-1,1))

tf.placeholder(tf.float32, [None, 784])
middle = 30
w_1 = tf.Variable(tf.truncated_normal([784, middle]))
loadTrainingData()
setupWeightsBias()
writeData()
