#!/usr/bin/python
from __future__ import division, print_function, absolute_import
import numpy as np
import mxnet as mx
from mxnet import nd, autograd, gluon
import os
import lzma

ctx = mx.cpu()
num_outputs = 2
batch_size = 200
width = 40
image_size = width*width

def READ_XZ (filename):
    file = lzma.LZMAFile(filename)
    type_bytes = file.read(-1)
    type_array = np.frombuffer(type_bytes,dtype='float32')
    return type_array


num_fc = 32
net = gluon.nn.HybridSequential()
smoothing_constant = 0.01
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

epochs = 10

def evaluate_accuracy(data_iterator):
    acc = mx.metric.Accuracy()
    for i, (data, label) in enumerate(data_iterator):
        data = data.as_in_context(ctx)
        label = label.as_in_context(ctx)
        output = net(data)
        predictions = nd.argmax(output, axis=1)
        prednew = nd.reshape(predictions,label.shape)
        acc.update(preds=prednew, labels=label)
    return acc.get()[1]

def TrainOnFile(name_image,name_label,name_val_image,name_val_label):
    input_data  = nd.array(READ_XZ(name_image).reshape(-1,1,width,width))
    input_label = nd.array(READ_XZ(name_label).reshape(-1))
    dataset = mx.gluon.data.dataset.ArrayDataset(input_data, input_label)
    train_data = mx.gluon.data.DataLoader(dataset, batch_size=batch_size, shuffle=True, num_workers=1)
    print(input_data.shape)
    print(input_label.shape)
    for e in range(epochs):
        cumulative_loss = 0
        for i, (data, label) in enumerate(train_data):
            data = data.as_in_context(ctx)
            label = label.as_in_context(ctx)
            with autograd.record():
                output = net(data)
                loss = softmax_cross_entropy(output, label)
            loss.backward()
            trainer.step(data.shape[0])
            inst_loss = nd.sum(loss).asscalar()
            cumulative_loss += inst_loss
            curr_loss = inst_loss
            moving_loss = (curr_loss if ((i == 0) and (e == 0))
                else (1 - smoothing_constant) * moving_loss + smoothing_constant * curr_loss)
        print(cumulative_loss)
        train_accuracy = evaluate_accuracy(train_data)
        print("Epoch %s. Loss: %s, Train_acc %s" % (e, moving_loss, train_accuracy))
