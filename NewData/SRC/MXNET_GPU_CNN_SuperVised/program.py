#!/usr/bin/python
from __future__ import division, print_function, absolute_import
import numpy as np
import mxnet as mx
from mxnet import nd, autograd, gluon
import os
import lzma

ctx = mx.gpu()
num_outputs = 2
batch_size = 200
width = 40
image_size = width*width
epochs = 10
num_fc = 32
smoothing_constant = 0.01
PARAM_NAME = "./cnn_top_tagger_pars"

def READ_XZ (filename):
    file = lzma.LZMAFile(filename)
    type_bytes = file.read(-1)
    type_array = np.frombuffer(type_bytes,dtype='float32')
    return type_array

net = gluon.nn.HybridSequential()
with net.name_scope():
    net.add(gluon.nn.Conv2D(channels=50, kernel_size=10, activation='relu'))
    net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    net.add(gluon.nn.Conv2D(channels=30, kernel_size=5, activation='relu'))
    net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    net.add(gluon.nn.Conv2D(channels=10, kernel_size=2, activation='relu'))
    net.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    net.add(gluon.nn.Flatten())
    net.add(gluon.nn.Dense(num_fc, activation="relu"))
    net.add(gluon.nn.Dense(num_outputs))
net.hybridize()
net.collect_params().initialize(mx.init.Xavier(magnitude=2.24), ctx=ctx)
softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss()
trainer = gluon.Trainer(net.collect_params(), 'adam', {'learning_rate': 0.001})

def EvalOnFile(name_image,name_label):

    input_data = nd.array(READ_XZ(name_image).reshape(-1,batch_size,1,width,width))
    input_data_ctx = input_data.as_in_context(ctx)

    input_label = nd.array(READ_XZ(name_label).reshape(-1,batch_size))
    input_label_ctx = input_label.as_in_context(ctx)

    batches = input_data.shape[0]
#    batches = 5

    print(input_data.shape[0])
    print(input_label.shape[0])

    acc = mx.metric.Accuracy()
    for i in range(batches):
        output = net(input_data_ctx[i])
        predictions = nd.argmax(output, axis=1)
        prednew = nd.reshape(predictions,input_label_ctx[i].shape)
        acc.update(preds=prednew, labels=input_label_ctx[i])
    return acc.get()[1]

def TrainOnFile(name_image,name_label,name_image_test,name_label_test):

    input_data = nd.array(READ_XZ(name_image).reshape(-1,batch_size,1,width,width))
    input_data_ctx = input_data.as_in_context(ctx)

    input_label = nd.array(READ_XZ(name_label).reshape(-1,batch_size))
    input_label_ctx = input_label.as_in_context(ctx)

    batches = input_data.shape[0]
#    batches = 5

    print(input_data.shape[0])
    print(input_label.shape[0])

    for e in range(epochs):
        cumulative_loss = 0
        for i in range(batches):
            with autograd.record():
                output = net(input_data_ctx[i])
                loss = softmax_cross_entropy(output, input_label_ctx[i])
            loss.backward()
            trainer.step(batch_size)
            inst_loss = nd.sum(loss).asscalar()
            if((i%100)==0):
                print(inst_loss)
            cumulative_loss += inst_loss
            curr_loss = inst_loss
            moving_loss = (
                curr_loss if ((i == 0) and (e == 0))
                else (1 - smoothing_constant) * moving_loss + smoothing_constant * curr_loss
            )
        print(cumulative_loss)
        net.save_parameters(PARAM_NAME)
        train_accuracy = EvalOnFile(name_image,name_label)
        test_accuracy = EvalOnFile(name_image_test,name_label_test)
        print("Epoch %s. Loss: %s, Train_acc %s, Test_acc %s" % (e, moving_loss, train_accuracy, train_accuracy))

TrainOnFile("./TRAIN/0/image.xz","./TRAIN/0/label.xz","./TEST/0/image.xz","./TEST/0/label.xz")
TrainOnFile("./TRAIN/1/image.xz","./TRAIN/1/label.xz","./TEST/1/image.xz","./TEST/1/label.xz")
TrainOnFile("./TRAIN/2/image.xz","./TRAIN/2/label.xz","./TEST/2/image.xz","./TEST/2/label.xz")
TrainOnFile("./TRAIN/3/image.xz","./TRAIN/3/label.xz","./TEST/3/image.xz","./TEST/3/label.xz")
TrainOnFile("./TRAIN/4/image.xz","./TRAIN/4/label.xz","./TEST/4/image.xz","./TEST/4/label.xz")
TrainOnFile("./TRAIN/5/image.xz","./TRAIN/5/label.xz","./TEST/5/image.xz","./TEST/5/label.xz")
TrainOnFile("./TRAIN/6/image.xz","./TRAIN/6/label.xz","./TEST/6/image.xz","./TEST/6/label.xz")
TrainOnFile("./TRAIN/7/image.xz","./TRAIN/7/label.xz","./TEST/7/image.xz","./TEST/7/label.xz")
#test_accuracy = evaluate_accuracy(test_data, net)
#train_accuracy = evaluate_accuracy(train_data, net)
#print("Epoch %s. Loss: %s, Train_acc %s, Test_acc %s" % (e, moving_loss, train_accuracy, test_accuracy))
