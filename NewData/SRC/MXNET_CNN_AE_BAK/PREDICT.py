#!/usr/bin/python3
from __future__ import division, print_function, absolute_import
import numpy as np
import mxnet as mx
from mxnet import nd, autograd, gluon
import os
#
ctx = mx.cpu()
data_ctx = ctx
model_ctx = ctx
batch_size = 200
width=40
num_inputs = width*width
sizeoftype = num_inputs*4
PARAM_NAME = "./OUTS/PARS/net_params"
mx.random.seed(1)
os.system("taskset -a -p 0xFFFFFFFF %d" % os.getpid())
sizes = [ 40*40 , 24*24 , 10*10 ]
num_outputs = num_inputs
#
class Reshaper(mx.gluon.nn.HybridSequential):
    def __init__(self, **kwargs):
        super(Reshaper, self).__init__(**kwargs)

    def forward(self, x):
        return x.reshape((batch_size,50,5,5))

net = gluon.nn.HybridSequential()
with net.name_scope():
    net.add(gluon.nn.Conv2D(channels=50, kernel_size=10, activation='relu'))
    net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    net.add(gluon.nn.Conv2D(channels=50, kernel_size=5, activation='relu'))
    net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    net.add(gluon.nn.Flatten())
    net.add(gluon.nn.Dense(100, activation="relu"))
    net.add(gluon.nn.Dense(1250, activation="relu"))
    net.add(Reshaper())
    net.add(gluon.nn.Conv2DTranspose(50, 5, 2, 0,activation="relu"))
    net.add(gluon.nn.Conv2DTranspose(50, 10, 2, 0,activation="relu"))
    net.add(gluon.nn.Conv2DTranspose(1, 7, 1, 0,activation="relu"))
    net.add(CenteredLayer())
#net.collect_params().initialize(mx.init.Xavier(), ctx=model_ctx)
net.collect_params().initialize(mx.init.Normal(sigma=.1), ctx=model_ctx)
net.hybridize()
net.load_parameters(PARAM_NAME, ctx=ctx)
#
def PredictFile(infilename,outfilename):
    statinfo = os.stat(infilename)
    leadingshape = int(statinfo.st_size/sizeoftype)
    leadingshape = min(leadingshape,100000)
    print(leadingshape)
    X = np.memmap(infilename, dtype='float32', mode='r', shape=(leadingshape,num_inputs))
    A = nd.array(X)
    train_data = mx.gluon.data.DataLoader(A, batch_size, shuffle=True)
    AOUT = (A-net(A)).asnumpy()
    AOUTNP = np.memmap(outfilename, dtype='float32', mode='write', shape=(leadingshape,num_inputs))
    np.copyto(AOUTNP,AOUT)
    print("done...")
#
PredictFile("./OUTS/QCD/TEST/0/image","./OUTS/QCD/TEST/0/predict");
PredictFile("./OUTS/QCD/TEST/1/image","./OUTS/QCD/TEST/1/predict");
PredictFile("./OUTS/QCD/TEST/2/image","./OUTS/QCD/TEST/2/predict");
PredictFile("./OUTS/QCD/TEST/3/image","./OUTS/QCD/TEST/3/predict");
PredictFile("./OUTS/QCD/TEST/4/image","./OUTS/QCD/TEST/4/predict");
PredictFile("./OUTS/QCD/TEST/5/image","./OUTS/QCD/TEST/5/predict");
PredictFile("./OUTS/QCD/TEST/6/image","./OUTS/QCD/TEST/6/predict");
PredictFile("./OUTS/QCD/TEST/7/image","./OUTS/QCD/TEST/7/predict");
PredictFile("./OUTS/QCD/TRAIN/0/image","./OUTS/QCD/TRAIN/0/predict");
PredictFile("./OUTS/QCD/TRAIN/1/image","./OUTS/QCD/TRAIN/1/predict");
PredictFile("./OUTS/QCD/TRAIN/2/image","./OUTS/QCD/TRAIN/2/predict");
PredictFile("./OUTS/QCD/TRAIN/3/image","./OUTS/QCD/TRAIN/3/predict");
PredictFile("./OUTS/QCD/TRAIN/4/image","./OUTS/QCD/TRAIN/4/predict");
PredictFile("./OUTS/QCD/TRAIN/5/image","./OUTS/QCD/TRAIN/5/predict");
PredictFile("./OUTS/QCD/TRAIN/6/image","./OUTS/QCD/TRAIN/6/predict");
PredictFile("./OUTS/QCD/TRAIN/7/image","./OUTS/QCD/TRAIN/7/predict");
PredictFile("./OUTS/TOP/TEST/0/image","./OUTS/TOP/TEST/0/predict");
PredictFile("./OUTS/TOP/TEST/1/image","./OUTS/TOP/TEST/1/predict");
PredictFile("./OUTS/TOP/TEST/2/image","./OUTS/TOP/TEST/2/predict");
PredictFile("./OUTS/TOP/TEST/3/image","./OUTS/TOP/TEST/3/predict");
PredictFile("./OUTS/TOP/TEST/4/image","./OUTS/TOP/TEST/4/predict");
PredictFile("./OUTS/TOP/TEST/5/image","./OUTS/TOP/TEST/5/predict");
PredictFile("./OUTS/TOP/TEST/6/image","./OUTS/TOP/TEST/6/predict");
PredictFile("./OUTS/TOP/TEST/7/image","./OUTS/TOP/TEST/7/predict");
PredictFile("./OUTS/TOP/TRAIN/0/image","./OUTS/TOP/TRAIN/0/predict");
PredictFile("./OUTS/TOP/TRAIN/1/image","./OUTS/TOP/TRAIN/1/predict");
PredictFile("./OUTS/TOP/TRAIN/2/image","./OUTS/TOP/TRAIN/2/predict");
PredictFile("./OUTS/TOP/TRAIN/3/image","./OUTS/TOP/TRAIN/3/predict");
PredictFile("./OUTS/TOP/TRAIN/4/image","./OUTS/TOP/TRAIN/4/predict");
PredictFile("./OUTS/TOP/TRAIN/5/image","./OUTS/TOP/TRAIN/5/predict");
PredictFile("./OUTS/TOP/TRAIN/6/image","./OUTS/TOP/TRAIN/6/predict");
PredictFile("./OUTS/TOP/TRAIN/7/image","./OUTS/TOP/TRAIN/7/predict");
