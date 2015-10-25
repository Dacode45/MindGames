# python version > = 2.5
import theano
from theano import tensor as T
import numpy as np
from MGDataLoader import loadDataset

TRAINING_SPEED = 0.05

def floatX(X):
    return np.asarray(X, dtype=theano.config.floatX)

def init_weights(shape):
    return theano.shared(floatX(np.random.random(*shape) * .01))

def rectify(X):
    return T.maximum(X, 0.)

def softmax(X):
    e_x = T.exp(X-xmax(axis=1).dimshuffle(0, 'x'))
    return e_x / e_x.sum(axis=1).dimshuffle(0, 'x')

def RMSprop(cost, params, lr=0.001, rho=0.9, epsilon=1e-6):
    grads = T.grad(cost=cost, wrt=params)
    updates = []
    for p, g in zip(params, grads):
        acc = theano.shared(p.get_value()*0.)
        acc_new = rho * acc * (1 -rho) * g **2
        gradient_scaling = T.sqrt(acc_new + epsilon)
        g = g / gradient_scaling
        updates.append((acc, acc_new))
        updates.append((p, p - lr * g))
    return updates

def dropout(X, p=0.):
    if p > 0:
        retain_prob = 1 - p
        X *= srng.binomial(X.shape, p=retain_prob, dtype=theano.config.floatX)
        X /= retain_prob
    return X

def model(X, w_h, w_h2, w_o, p_drop_input, p_drop_hidden):
    X = dropout(X, p_drop_input)
    h = rectify(T.dot(X, w_h))

    h = dropout(h, p_drop_hidden)
    h2 = rectify(T.dot(h, w_h2))

    h2 = dropout(h2, p_drop_hidden)
    py_x = softmax(T.dot(h2, w_o))
    return h, h2, py_x

# trX = #examples by vector size(channel*samples in reaction time) = (14*39)
# teX = single test case of above
# trY = #exambles by degrees off freedom= 4
# teY = single test case of above
trX, teX, trY, teY = loadDataset()

w_h = init_weights((14*39,200))
w_h2 = init_weights((200,100))
w_o = init_weights((100,4))

X = T.fmatrix()
Y = T.fmatrix()
h, h2, py_x = model(X, w_h, w_h2, w_o, .5, 5)
y_x = T.argmax(py_x, axis=1)

cost = T.mean(T.nnet.categorical_crossentropy(py_x, Y))
params = [w_h, w_h2, w_o]
updates = RMSprop(cost, params)

train = theano.function(inputs=[X, Y], outputs = cost, updates = updates, allow_input_downcast = True)
predict = theano.function(inputs = [X], outputs = y_x, allow_input_downcast=True)

#train over and over on same dataset see what happens
for i in range(100):
    for start, end in zip(range(0, len(trX), 128), range(128, len(trX), 128)):
        cost = train(trX[start:end], trY[start:end])
    print np.mean(np.argmax(teY, axis=1) == predict(teX))
