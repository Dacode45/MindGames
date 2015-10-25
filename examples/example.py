"""
It is important to understand how a neural network is actually implemented
with the packages used. This code contains prettymuch everything that you need to code
for an actual neural network. This code is taken from
https://www.youtube.com/watch?v=S75EdAcXHKk but heavily commented.
Run it to check if you have your workspace set up.


It trains the neural network to determine the m in a function y = mx + b
"""

#AVI WAS HERE!!!!!
import theano
from theano import tensor as T
import numpy as np

#This line creates an aray of 101 numbers between -1 and 1. Each number is
#equally spaced.
trX = np.linspace(-1, 1, 101)
#This is the output that we want for every input x. y = 2x + randomNumber*.33
#since trx is a vector of 101 numbers, we need to make sure y is a vector of 101 inputs
#2*x is a scalar times a vector so that is fine
#we canot simply add the scalar .33 to the vector 2x so we generate a vector of
#101 random numbers using np.random.randn trX.shape is just the number of elements in the vector
trY = 2 * trX + np.random.randn(*trX.shape) * .33

#Theano is a sort of algebraic library. it uses variables like x and y
#We have to define those variables.
X = T.scalar() #integer number
Y = T.scalar()

#this is the function that converts x to Y. w will fluctuate to minimize the error
def model(X, w):
    return X*w
#This is like T.scalar(). It allows w to work on both scalars and floats
w = theano.shared(np.asarray(0., dtype=theano.config.floatX))
y = model(X, w) #set y to always be the output to X*w

cost = T.mean(T.sqr(y-Y)) #Error = cost = mean squared
gradient = T.grad(cost=cost, wrt=w) #great thing about Theano is that we dont have to program derivatives
#it calculates them itself. cost=function wrt = with respect to w
updates = [[w, w-gradient *.01]] #.01 is a scaling function determing the speed as to which w changes

#Theano functions are black boxes. Take the inputs. transform then with outputs.
#it works because cost depends on y and y depends on X and w.
#allow_input_downcast means we will use 64 bit floats on cpu and 32 bit floats on gpu
train = theano.function(inputs=[X, Y], outputs = cost, updates = updates, allow_input_downcast=True)

for i in range(100):
    for x, y in zip(trX, trY):
        train(x, y) #for every x y pair train.

print w
