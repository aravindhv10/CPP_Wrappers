#!/usr/bin/python3

from __future__ import division, print_function, absolute_import
import numpy as np
import mxnet as mx
from mxnet import nd, autograd, gluon
import os

ctx = mx.cpu()
data_ctx = ctx
model_ctx = ctx
batch_size = 200
width=40
num_inputs = width*width
sizeoftype = num_inputs*4
PARAM_NAME = "./OUTS/PARS/net_params_cnn_ae_bigslow"
mx.random.seed(1)
os.system("taskset -a -p 0xFFFFFFFF %d" % os.getpid())
sizes = [ 40*40 , 24*24 , 10*10 ]
num_outputs = num_inputs
#
class CenteredLayer(mx.gluon.nn.HybridSequential):
    def __init__(self, **kwargs):
        super(CenteredLayer, self).__init__(**kwargs)
#
    def forward(self, x):
        return x.reshape((batch_size,width*width)).softmax(axis=1).reshape((batch_size,1,width,width))
#
class Reshaper(mx.gluon.nn.HybridSequential):
    def __init__(self, **kwargs):
        super(Reshaper, self).__init__(**kwargs)
#
    def forward(self, x):
        return x.reshape((batch_size,50,3,3))
#
net = gluon.nn.HybridSequential()
#
with net.name_scope():
    net.add(gluon.nn.Conv2D(channels=50, kernel_size=5, activation='relu'))
    net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    net.add(gluon.nn.Conv2D(channels=50, kernel_size=5, activation='relu'))
    net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    net.add(gluon.nn.Conv2D(channels=50, kernel_size=5, activation='relu'))
    net.add(gluon.nn.Flatten())
    net.add(gluon.nn.Dense(100, activation="relu"))
    net.add(gluon.nn.Dense(450, activation="relu"))
    net.add(Reshaper())
    net.add(gluon.nn.Conv2DTranspose(50, 9, 2, 0,activation="relu"))
    net.add(gluon.nn.Conv2DTranspose(50, 9, 2, 0,activation="relu"))
    net.add(gluon.nn.Conv2DTranspose(1, 8, 1, 0,activation="relu"))
    net.add(CenteredLayer())
#
#net.collect_params().initialize(mx.init.Xavier(), ctx=model_ctx)
net.collect_params().initialize(mx.init.Normal(sigma=.1), ctx=model_ctx)
net.hybridize()
net.load_parameters(PARAM_NAME, ctx=ctx)
softmax_cross_entropy = gluon.loss.L2Loss()
trainer = gluon.Trainer(net.collect_params(), 'adam', {'learning_rate': 0.001})

def TrainOnFile(filename):
    QCDFILE = filename
    statinfo = os.stat(QCDFILE)
    leadingshape = int(statinfo.st_size/sizeoftype)
    print(leadingshape)
    X = np.memmap(QCDFILE, dtype='float32', mode='r', shape=(leadingshape,1,40,40))
    A = nd.array(X)
    train_data = mx.gluon.data.DataLoader(A, batch_size, shuffle=True)
    epochs = 2
    smoothing_constant = 0.001
    for e in range(epochs):
        cumulative_loss = 0
        for i, data in enumerate(train_data):
            output = net(data)
            #net.save_parameters(PARAM_NAME)
            #print(output.shape)
            #exit()
            data = data.as_in_context(model_ctx)
            label = data.as_in_context(model_ctx)
            with autograd.record():
                output = net(data)
                #print(output.shape)
                loss = softmax_cross_entropy(output, label)
            loss.backward()
            trainer.step(data.shape[0])
            inst_loss = nd.sum(loss).asscalar()
            cumulative_loss += inst_loss
            print("    ",inst_loss)
            #net.save_parameters(PARAM_NAME)
        print(cumulative_loss);
        net.save_parameters(PARAM_NAME)


for e in range(4):
  TrainOnFile("./OUTS/QCD/TRAIN/0/image")
  TrainOnFile("./OUTS/QCD/TRAIN/1/image")
  TrainOnFile("./OUTS/QCD/TRAIN/0/image")
  TrainOnFile("./OUTS/QCD/TRAIN/1/image")

  TrainOnFile("./OUTS/QCD/TRAIN/2/image")
  TrainOnFile("./OUTS/QCD/TRAIN/3/image")
  TrainOnFile("./OUTS/QCD/TRAIN/2/image")
  TrainOnFile("./OUTS/QCD/TRAIN/3/image")

  TrainOnFile("./OUTS/QCD/TRAIN/4/image")
  TrainOnFile("./OUTS/QCD/TRAIN/5/image")
  TrainOnFile("./OUTS/QCD/TRAIN/4/image")
  TrainOnFile("./OUTS/QCD/TRAIN/5/image")

  TrainOnFile("./OUTS/QCD/TRAIN/6/image")
  TrainOnFile("./OUTS/QCD/TRAIN/7/image")
  TrainOnFile("./OUTS/QCD/TRAIN/6/image")
  TrainOnFile("./OUTS/QCD/TRAIN/7/image")
