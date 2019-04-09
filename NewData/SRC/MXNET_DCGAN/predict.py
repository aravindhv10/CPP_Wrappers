#!/usr/bin/python3

from __future__ import division, print_function, absolute_import
import numpy as np
import mxnet as mx
from mxnet import nd, autograd, gluon
import os

os.system("taskset -a -p 0xFFFFFFFF %d" % os.getpid())
mx.random.seed(1)

ctx = mx.cpu()
data_ctx = ctx
model_ctx = ctx
batch_size = 200
width=40
num_inputs = width*width
num_outputs = 2
sizeoftype = 6400
PARAM_NAME = "./OUTS/PARS/CNN_PARS"

def PredictFile (infilename, outfilename):
    statinfo = os.stat(infilename)
    leadingshape = int(statinfo.st_size/sizeoftype)
    print(leadingshape)
    X = np.memmap(infilename, dtype='float32', mode='r', shape=(leadingshape,1,40,40))
    Xnd = nd.array(X)
    Fout = open(outfilename,"w")
    dataset = mx.gluon.data.dataset.ArrayDataset(Xnd)
    predict_data = mx.gluon.data.DataLoader(dataset, batch_size=batch_size, num_workers=1)
    num_fc = 32
    net = gluon.nn.HybridSequential()
    with net.name_scope():
        net.add(gluon.nn.Conv2D(channels=50, kernel_size=10, activation='relu'))
        net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
        net.add(gluon.nn.Conv2D(channels=30, kernel_size=5, activation='relu'))
        net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
        net.add(gluon.nn.Conv2D(channels=10, kernel_size=2, activation='relu'))
        net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
        # The Flatten layer collapses all axis, except the first one, into one axis.
        net.add(gluon.nn.Flatten())
        net.add(gluon.nn.Dense(num_fc, activation="relu"))
        net.add(gluon.nn.Dense(num_outputs))
    net.hybridize()
    net.collect_params().initialize(mx.init.Xavier(magnitude=2.24), ctx=ctx)
    softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss()
    trainer = gluon.Trainer(net.collect_params(), 'adam', {'learning_rate': 0.001})
    epochs = 30
    smoothing_constant = .01
    net.load_parameters(PARAM_NAME, ctx=ctx)
    for i, data in enumerate(predict_data):
        data = data.as_in_context(ctx)
        output = net(data).softmax(axis=1).asnumpy()
        output.tofile(Fout)
        #print(output.shape)
        #print("output ",output[0],output[1])
        #predictions = nd.argmax(output, axis=1)
        #prednew = nd.reshape(predictions,label.shape)
    Fout.close()
#
PredictFile("./OUTS/QCD/TEST/0/image","./OUTS/QCD/TEST/0/CNNPredict")
PredictFile("./OUTS/QCD/TEST/1/image","./OUTS/QCD/TEST/1/CNNPredict")
PredictFile("./OUTS/QCD/TEST/2/image","./OUTS/QCD/TEST/2/CNNPredict")
PredictFile("./OUTS/QCD/TEST/3/image","./OUTS/QCD/TEST/3/CNNPredict")
PredictFile("./OUTS/QCD/TEST/4/image","./OUTS/QCD/TEST/4/CNNPredict")
PredictFile("./OUTS/QCD/TEST/5/image","./OUTS/QCD/TEST/5/CNNPredict")
PredictFile("./OUTS/QCD/TEST/6/image","./OUTS/QCD/TEST/6/CNNPredict")
PredictFile("./OUTS/QCD/TEST/7/image","./OUTS/QCD/TEST/7/CNNPredict")
#
PredictFile("./OUTS/TOP/TEST/0/image","./OUTS/TOP/TEST/0/CNNPredict")
PredictFile("./OUTS/TOP/TEST/1/image","./OUTS/TOP/TEST/1/CNNPredict")
PredictFile("./OUTS/TOP/TEST/2/image","./OUTS/TOP/TEST/2/CNNPredict")
PredictFile("./OUTS/TOP/TEST/3/image","./OUTS/TOP/TEST/3/CNNPredict")
PredictFile("./OUTS/TOP/TEST/4/image","./OUTS/TOP/TEST/4/CNNPredict")
PredictFile("./OUTS/TOP/TEST/5/image","./OUTS/TOP/TEST/5/CNNPredict")
PredictFile("./OUTS/TOP/TEST/6/image","./OUTS/TOP/TEST/6/CNNPredict")
PredictFile("./OUTS/TOP/TEST/7/image","./OUTS/TOP/TEST/7/CNNPredict")
#
PredictFile("./OUTS/QCD/TRAIN/0/image","./OUTS/QCD/TRAIN/0/CNNPredict")
PredictFile("./OUTS/QCD/TRAIN/1/image","./OUTS/QCD/TRAIN/1/CNNPredict")
PredictFile("./OUTS/QCD/TRAIN/2/image","./OUTS/QCD/TRAIN/2/CNNPredict")
PredictFile("./OUTS/QCD/TRAIN/3/image","./OUTS/QCD/TRAIN/3/CNNPredict")
PredictFile("./OUTS/QCD/TRAIN/4/image","./OUTS/QCD/TRAIN/4/CNNPredict")
PredictFile("./OUTS/QCD/TRAIN/5/image","./OUTS/QCD/TRAIN/5/CNNPredict")
PredictFile("./OUTS/QCD/TRAIN/6/image","./OUTS/QCD/TRAIN/6/CNNPredict")
PredictFile("./OUTS/QCD/TRAIN/7/image","./OUTS/QCD/TRAIN/7/CNNPredict")
#
PredictFile("./OUTS/TOP/TRAIN/0/image","./OUTS/TOP/TRAIN/0/CNNPredict")
PredictFile("./OUTS/TOP/TRAIN/1/image","./OUTS/TOP/TRAIN/1/CNNPredict")
PredictFile("./OUTS/TOP/TRAIN/2/image","./OUTS/TOP/TRAIN/2/CNNPredict")
PredictFile("./OUTS/TOP/TRAIN/3/image","./OUTS/TOP/TRAIN/3/CNNPredict")
PredictFile("./OUTS/TOP/TRAIN/4/image","./OUTS/TOP/TRAIN/4/CNNPredict")
PredictFile("./OUTS/TOP/TRAIN/5/image","./OUTS/TOP/TRAIN/5/CNNPredict")
PredictFile("./OUTS/TOP/TRAIN/6/image","./OUTS/TOP/TRAIN/6/CNNPredict")
PredictFile("./OUTS/TOP/TRAIN/7/image","./OUTS/TOP/TRAIN/7/CNNPredict")
#
