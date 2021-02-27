import csv
from random import seed
from random import randrange
import array as arr
import tensorflow.compat.v1 as tf
import numpy as np
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
            #TrainingData[line_count] = {'left' : row[0], 'right' : row[1], 'up' : row[2], 'down' : row[3]}
            TrainingData[line_count] ={row[0], row[1],row[2], row[3]}
            line_count += 1
        print(f'Processed {line_count} lines.')

def writeData():
    with open('FYP - Comparison between two AI/FYP - Comparison between two AI/DATA/temp.csv', mode='w') as csv_file:
        csv_writer = csv.writer(csv_file, delimiter=',', quotechar='"', lineterminator = '\n')
       # for row in TrainingData:
         # csv_writer.writerow([TrainingData[row]['left'], TrainingData[row]['right'], TrainingData[row]['up'],TrainingData[row]['down']])

def setupWeightsBias():
    i = 0
    while i < 8:
        i += 1
        weights.append(randrange(-1,1))
    i = 0
    while i < 4:
        i += 1
        biases.append(randrange(-1,1))

def sigma(x):
    return tf.div(tf.constant(1.0),
                  tf.add(tf.constant(1.0), tf.exp(tf.negative(x))))

def sigmaprime(x):
    return tf.multiply(sigma(x), tf.subtract(tf.constant(1.0), sigma(x)))

loadTrainingData()
setupWeightsBias()
writeData()

#setup
a_0 = tf.placeholder(tf.float32, [1, 4]) #inputs
y = tf.placeholder(tf.float32, [1, 4]) # outputs
middle = 2
w_1 = tf.Variable(tf.truncated_normal([4, middle]))
b_1 = tf.Variable(tf.truncated_normal([1, middle]))
w_2 = tf.Variable(tf.truncated_normal([middle, 4]))
b_2 = tf.Variable(tf.truncated_normal([1, 4]))

#forward prop
z_1 = tf.add(tf.matmul(a_0, w_1), b_1)
a_1 = sigma(z_1)
z_2 = tf.add(tf.matmul(a_1, w_2), b_2)
a_2 = sigma(z_2)

#difference
diff = tf.subtract(a_2, y)

#backprop
d_z_2 = tf.multiply(diff, sigmaprime(z_2))
d_b_2 = d_z_2
d_w_2 = tf.matmul(tf.transpose(a_1), d_z_2)

d_a_1 = tf.matmul(d_z_2, tf.transpose(w_2))
d_z_1 = tf.multiply(d_a_1, sigmaprime(z_1))
d_b_1 = d_z_1
d_w_1 = tf.matmul(tf.transpose(a_0), d_z_1)

#update network
eta = tf.constant(0.5)
step = [
    tf.assign(w_1,
            tf.subtract(w_1, tf.multiply(eta, d_w_1)))
  , tf.assign(b_1,
            tf.subtract(b_1, tf.multiply(eta,
                               tf.reduce_mean(d_b_1, axis=[0]))))
  , tf.assign(w_2,
            tf.subtract(w_2, tf.multiply(eta, d_w_2)))
  , tf.assign(b_2,
            tf.subtract(b_2, tf.multiply(eta,
                               tf.reduce_mean(d_b_2, axis=[0]))))
]

# test
acct_mat = tf.equal(tf.argmax(a_2, 1), tf.argmax(y, 1))
acct_res = tf.reduce_sum(tf.cast(acct_mat, tf.float32))

sess = tf.InteractiveSession()
sess.run(tf.global_variables_initializer())

xrange = [1000]

for i in TrainingData:
   #batch_xs, batch_ys = mnist.train.next_batch(10)
   batch_xs = TrainingData[0]
   batch_ys = TrainingData[1]
   #batch_xs.shape: (513,4)
   #batch_ys.shape: (513,4)
   sess.run(step, feed_dict = {a_0: batch_xs,
                               y : batch_ys})
    #if i % 1000 == 0:
     #   res = sess.run(acct_res, feed_dict =
      #                 {a_0: mnist.test.images[:1000],
       #                 y : mnist.test.labels[:1000]})
   #print(res)

