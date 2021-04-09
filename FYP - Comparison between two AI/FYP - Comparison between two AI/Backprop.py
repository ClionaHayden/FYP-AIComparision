from numpy import loadtxt
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense

from tensorflow.keras.models import load_model
import numpy
import os
import csv

dataset = loadtxt('DATA/TrainingData.csv', delimiter=',')
X = dataset[:,0:10]
y = dataset[:,10:15]

model = Sequential()
model.add(Dense(10, input_dim=10, activation='relu'))
model.add(Dense(5, activation='sigmoid'))

model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

model.fit(X, y, epochs=10, batch_size=500)

_, accuracy = model.evaluate(X, y)
print('Accuracy: %.2f' % (accuracy*100))

predictions = model.predict_classes(X)

#for i in range(15):
	#print('%s => %d (expected %d)' % (X[i].tolist(), predictions[i], y[i]))

model.save("carModel.h5")
print("Saved model to disk")

# load model
loaded_model = load_model('carModel.h5')
# summarize model.
loaded_model.summary()

print("Loaded model from disk")

index=0
with open('DATA/CalculatedWeights.csv', mode='w') as weights_file:
    weights_writer = csv.writer(weights_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    for layer in loaded_model.layers:
        weights = layer.get_weights() # list of numpy arrays
        print("Layer %d" %index)
        print("Layer name %s" %layer.name)
        print(weights)
        for item in weights:   
            for item2 in item:   
                weights_writer.writerow(item2.flatten())
        index = index+1