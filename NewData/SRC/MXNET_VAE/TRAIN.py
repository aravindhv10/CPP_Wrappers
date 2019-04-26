#!/usr/bin/python3

from __future__ import division, print_function, absolute_import
import numpy as np
import mxnet as mx
from mxnet import nd, autograd, gluon
from mxnet.gluon import nn
import matplotlib.pyplot as plt
import os

ctx = mx.cpu()
data_ctx = ctx
model_ctx = ctx
batch_size = 200
width=40
num_inputs = width*width
sizeoftype = num_inputs*4
SizeOfBatch = sizeoftype*batch_size
PARAM_NAME_E = "./OUTS/PARS/VAE_net_params_E"
PARAM_NAME_D = "./OUTS/PARS/VAE_net_params_D"
mx.random.seed(1)
os.system("taskset -a -p 0xFFFFFFFF %d" % os.getpid())
num_outputs = num_inputs
#
class CenteredLayer(mx.gluon.nn.HybridSequential):
    def __init__(self, **kwargs):
        super(CenteredLayer, self).__init__(**kwargs)

    def forward(self, x):
        return x.reshape((batch_size,width*width)).softmax(axis=1).reshape((batch_size,1,width,width))
#
class Reshaper(mx.gluon.nn.HybridSequential):
    def __init__(self, **kwargs):
        super(Reshaper, self).__init__(**kwargs)

    def forward(self, x):
        return x.reshape((batch_size,50,3,3))
#
softmax_cross_entropy = gluon.loss.L2Loss()
#
netE = gluon.nn.HybridSequential()
with netE.name_scope():
    netE.add(gluon.nn.Conv2D(channels=50, kernel_size=5))
    netE.add(nn.BatchNorm())
    netE.add(nn.Activation('relu'))
    netE.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    netE.add(gluon.nn.Conv2D(channels=50, kernel_size=5))
    netE.add(nn.BatchNorm())
    netE.add(nn.Activation('relu'))
    netE.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    netE.add(gluon.nn.Conv2D(channels=50, kernel_size=5))
    netE.add(nn.BatchNorm())
    netE.add(nn.Activation('relu'))
    netE.add(gluon.nn.Flatten())
    netE.add(gluon.nn.Dense(10*10, activation="relu"))
#
netE.collect_params().initialize(mx.init.Normal(sigma=.1), ctx=model_ctx)
netE.hybridize()
#
netE.load_parameters(PARAM_NAME_E, ctx=ctx)
trainerE = gluon.Trainer(netE.collect_params(), 'adam', {'learning_rate': 0.001})
#
netD = gluon.nn.HybridSequential()
with netD.name_scope():
    netD.add(gluon.nn.Dense(50*3*3, activation="relu"))
    netD.add(Reshaper())
    netD.add(gluon.nn.Conv2DTranspose(channels=50, kernel_size=5, strides=(1, 1), use_bias=True))
    netD.add(nn.BatchNorm())
    netD.add(nn.Activation('relu'))
    netD.add(gluon.nn.Conv2DTranspose(channels=50, kernel_size=5, strides=(2, 2), use_bias=True))
    netD.add(nn.BatchNorm())
    netD.add(nn.Activation('relu'))
    netD.add(gluon.nn.Conv2DTranspose(channels=50, kernel_size=5, strides=(2, 2), use_bias=True))
    netD.add(nn.BatchNorm())
    netD.add(nn.Activation('relu'))
    netD.add(gluon.nn.Conv2DTranspose(channels=1, kernel_size=4, strides=(1, 1), use_bias=True))
    netD.add(CenteredLayer())
#
netD.collect_params().initialize(mx.init.Normal(sigma=.1), ctx=model_ctx)
netD.hybridize()
#
netD.load_parameters(PARAM_NAME_D, ctx=ctx)
trainerD = gluon.Trainer(netD.collect_params(), 'adam', {'learning_rate': 0.001})
#
def TrainOnFile(filename):
    QCDFILE = filename
    statinfo = os.stat(QCDFILE)
    leadingshape = int(statinfo.st_size/SizeOfBatch)
    print(leadingshape)
    X = np.memmap(QCDFILE, dtype='float32', mode='r', shape=(leadingshape,batch_size,1,width,width))
    for e in range(2):
        for i in range(leadingshape):
            A = nd.array(X[i])
            #out = netE(A)
            with autograd.record():
                out = netD(netE(A))
                loss = softmax_cross_entropy(out, A)
            loss.backward()
            trainerD.step(batch_size)
            trainerE.step(batch_size)
            inst_loss = nd.sum(loss).asscalar()
            #print(X[i].shape)
            print("    ",inst_loss)
            if i % 10 == 0:
                print("Saving...")
                netE.save_parameters(PARAM_NAME_E)
                netD.save_parameters(PARAM_NAME_D)
                print("Saved...")
            # if i % 10 == 0
        # for i in range(leadingshape)
    # for e in range(e)
# TrainOnFile(filename)
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
