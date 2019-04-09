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

def evaluate_accuracy(data_iterator, net):
    acc = mx.metric.Accuracy()
    for i, (data, label) in enumerate(data_iterator):
        data = data.as_in_context(ctx)
        label = label.as_in_context(ctx)
        output = net(data)
        predictions = nd.argmax(output, axis=1)
        prednew = nd.reshape(predictions,label.shape)
        #predictions = nd.argmax(output)
        #print(predictions[1])
        #print(data.shape,label.shape,output.shape,predictions.shape,prednew.shape)
        acc.update(preds=prednew, labels=label)
    return acc.get()[1]

def trainonfiles (dataname,labelname,testdata,testlabel,NEpochs):
    IMAGEFILE = dataname
    statinfo = os.stat(IMAGEFILE)
    leadingshape = int(statinfo.st_size/sizeoftype)
    print(leadingshape)
    X = np.memmap(IMAGEFILE, dtype='float32', mode='r', shape=(leadingshape,1,40,40))
    Xnd = nd.array(X)
    LABELFILE = labelname
    Y = np.memmap(LABELFILE, dtype='float32', mode='r', shape=(leadingshape,1))
    Ynd = nd.array(Y)
    dataset = mx.gluon.data.dataset.ArrayDataset(Xnd, Ynd)
    train_data = mx.gluon.data.DataLoader(dataset, batch_size=batch_size, shuffle=True, num_workers=1)
    IMAGEFILE = testdata
    statinfo = os.stat(IMAGEFILE)
    leadingshape = int(statinfo.st_size/sizeoftype)
    print(leadingshape)
    tX = np.memmap(IMAGEFILE, dtype='float32', mode='r', shape=(leadingshape,1,40,40))
    tXnd = nd.array(tX)
    LABELFILE = testlabel
    tY = np.memmap(LABELFILE, dtype='float32', mode='r', shape=(leadingshape,1))
    tYnd = nd.array(tY)
    tdataset = mx.gluon.data.dataset.ArrayDataset(tXnd, tYnd)
    test_data = mx.gluon.data.DataLoader(tdataset, batch_size=batch_size, shuffle=True, num_workers=1)
    num_fc = 32
    net = gluon.nn.HybridSequential()
    with net.name_scope():
        net.add(gluon.nn.Conv2D(channels=20, kernel_size=5, activation='relu'))
        net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
        net.add(gluon.nn.Conv2D(channels=50, kernel_size=5, activation='relu'))
        net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
        # The Flatten layer collapses all axis, except the first one, into one axis.
        net.add(gluon.nn.Flatten())
        net.add(gluon.nn.Dense(num_fc, activation="relu"))
        net.add(gluon.nn.Dense(num_outputs))
    net.hybridize()
    net.collect_params().initialize(mx.init.Xavier(magnitude=2.24), ctx=ctx)
    softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss()
    #softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss(sparse_label=False)
    trainer = gluon.Trainer(net.collect_params(), 'adam', {'learning_rate': 0.001})
    epochs = NEpochs
    smoothing_constant = .01
    #net.load_parameters(PARAM_NAME, ctx=ctx)
    for e in range(epochs):
        for i, (data, label) in enumerate(train_data):
            data = data.as_in_context(ctx)
            label = label.as_in_context(ctx)
            with autograd.record():
                output = net(data)
                loss = softmax_cross_entropy(output, label)
            loss.backward()
            trainer.step(data.shape[0])
            inst_loss = nd.sum(loss).asscalar()
            print("    ",inst_loss)
            if((i%50)==0):
                print("saving...")
                net.save_parameters(PARAM_NAME)
                exit()
            curr_loss = nd.mean(loss).asscalar()
            moving_loss = (curr_loss if ((i == 0) and (e == 0))
                           else (1 - smoothing_constant) * moving_loss + smoothing_constant * curr_loss)

        test_accuracy = evaluate_accuracy(test_data, net)
        train_accuracy = evaluate_accuracy(train_data, net)
        print("Epoch %s. Loss: %s, Train_acc %s, Test_acc %s" % (e, moving_loss, train_accuracy, test_accuracy))

trainonfiles ("./OUTS/TMP/QCD_TOP/TRAIN/0/image","./OUTS/TMP/QCD_TOP/TRAIN/0/label","./OUTS/TMP/QCD_TOP/TEST/0/image","./OUTS/TMP/QCD_TOP/TEST/0/label",5)
trainonfiles ("./OUTS/TMP/QCD_TOP/TRAIN/1/image","./OUTS/TMP/QCD_TOP/TRAIN/1/label","./OUTS/TMP/QCD_TOP/TEST/1/image","./OUTS/TMP/QCD_TOP/TEST/1/label",5)
trainonfiles ("./OUTS/TMP/QCD_TOP/TRAIN/2/image","./OUTS/TMP/QCD_TOP/TRAIN/2/label","./OUTS/TMP/QCD_TOP/TEST/2/image","./OUTS/TMP/QCD_TOP/TEST/2/label",5)
trainonfiles ("./OUTS/TMP/QCD_TOP/TRAIN/3/image","./OUTS/TMP/QCD_TOP/TRAIN/3/label","./OUTS/TMP/QCD_TOP/TEST/3/image","./OUTS/TMP/QCD_TOP/TEST/3/label",5)
trainonfiles ("./OUTS/TMP/QCD_TOP/TRAIN/4/image","./OUTS/TMP/QCD_TOP/TRAIN/4/label","./OUTS/TMP/QCD_TOP/TEST/4/image","./OUTS/TMP/QCD_TOP/TEST/4/label",5)
trainonfiles ("./OUTS/TMP/QCD_TOP/TRAIN/5/image","./OUTS/TMP/QCD_TOP/TRAIN/5/label","./OUTS/TMP/QCD_TOP/TEST/5/image","./OUTS/TMP/QCD_TOP/TEST/5/label",5)
trainonfiles ("./OUTS/TMP/QCD_TOP/TRAIN/6/image","./OUTS/TMP/QCD_TOP/TRAIN/6/label","./OUTS/TMP/QCD_TOP/TEST/6/image","./OUTS/TMP/QCD_TOP/TEST/6/label",5)
trainonfiles ("./OUTS/TMP/QCD_TOP/TRAIN/7/image","./OUTS/TMP/QCD_TOP/TRAIN/7/label","./OUTS/TMP/QCD_TOP/TEST/7/image","./OUTS/TMP/QCD_TOP/TEST/7/label",5)
