import theano
from theano import tensor as T #a tensor is an n dimensional matrix

"""Creating Variables and Functions"""
x = T.vector('x') #define vector named x. Always name your vectors even though you dont
#have to
W = T.matrix('W')
b = T.vector('b')

#Symbolic expression doesn't actually do anything
dot = T.dot(x, W)
out = T.nnet.sigmoid(dot + b) #out = f(Wx + bias)

"""Recipe for Theano applications
Define symbolic expressions
Compile a function that can compute numeric values using those expressions
Execute that function on data"""

#example
#y = a x b

a = T.scalar() #symbolic variables
b = T.scalar() #symboic variables
y = a * b #symbolic expression
multiply = theano.function(inputs=[a, b], outputs=y) #compile function

print multiply(3, 2)
print multiply(5, 6)

#leaky rectifier linear activation function
#neuron activation function
def leaky(x):
    return T.switch(x<0, .01 *x, x) #switch is element wise.

#cross-entropy cost
def ce_loss(x, y):
    return -T.sum(x * T.log(x) + (1-x)*T.log(1-z), axis=1)

"""Symbolic Differentiation"""
gradient = T.grad(cost=L, wrt=W) #L is layer W is weight get partial derivative
updates = [[W, W-gradient * .01]] #update wight w with the gradient times a speed

"""Training a linear model
Fit a linear model to scatter plot. find w such that y = wx"""

X = T.scalar()
Y = T.scalar()

def moddel(X, W):
    return X * W
W = theano.shared(np.asarray(0., dtype=theano.config.floatX)) #has data associated with it.
y = model(X, W)

cost = T.mean(T.sqr(y-Y)) #define symbolic lost
gradient = T.grad(cost=cost, wrt=W)
updates = [[W, W-gradient * .01]] #all of backprop

train = theano.function(inputs=[X, Y], outputs=cost, updates = updates, allow_input_downcast=True)

for i in range(100):
    for x, y in zip(train_x, train_y):
        train(x, y)

"""Making layers"""
class ConvLayer(object):
    def __init__(self, rng, input, filter_shape, image_shape, poolsize=(2,2)):
        self.input = input
        self.W = theano.shared(numpy.asarray(rng.uniform(size=filter_shape), dtype=theano.config.floatX), borrow=True)

        conv_out = conv.conv2d(
            input = input,
            filters=self.W,
            filter_shape=filter_shape,
            image_shape = image_shape
        )

        self.output = T.tanh(conv_out)

        self.params = [self.W]

layer0 = ConvLayer(rng, input=layer0_input, image_shape=(batch_size, 1, 28, 28), filter_shape=(n_filter0, 1, 5, 5))
layer1 = ConvLayer(rng, input=layer0.output, image_shape=(batch_size, n_filter0, 12, 12), filter_shape=(n_filter1, n_filter0, 5, 5))
