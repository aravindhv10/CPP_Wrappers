#!/usr/bin/python
from __future__ import division, print_function, absolute_import
import numpy as np
from numpy import linalg as LA
import mxnet as mx
from mxnet import nd, autograd, gluon
import os
import lzma
#
ctx = mx.gpu()
#ctx = mx.cpu()
batch_size = 200
width = 40
image_size = width*width
epochs = 10
PARAM_NAME = "./net_ae_pars_huge"
sizes = [ 40*40 , 33*33 , 26*26 , 19*19 , 12*12 , 5*5 ]
smoothing_constant = 0.01
#
def READ_XZ (filename):
    file = lzma.LZMAFile(filename)
    type_bytes = file.read(-1)
    type_array = np.frombuffer(type_bytes,dtype='float32')
    return type_array
#
############################
### SOFTMAX CLASS BEGIN: ###
############################
class CenteredLayer(mx.gluon.nn.HybridSequential):
#
    def __init__(self, **kwargs):
        super(CenteredLayer, self).__init__(**kwargs)
#
    def forward(self, x):
        return x.softmax(axis=1)
#
##########################
### SOFTMAX CLASS END. ###
##########################
#
##############################
#### NET STRUCTURE BEGIN: ####
##############################
#
net = gluon.nn.HybridSequential()
with net.name_scope():
    net.add ( gluon.nn.Dense ( sizes[1] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[2] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[3] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[4] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[5] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[4] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[3] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[2] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[1] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[0] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[0]                     ) )
    net.add ( CenteredLayer()                                 )
#
net.hybridize()
net.collect_params().initialize(mx.init.Xavier(magnitude=2.24), ctx=ctx)
#net.load_parameters(PARAM_NAME, ctx=ctx)
#
############################
#### NET STRUCTURE END. ####
############################
#
softmax_cross_entropy = gluon.loss.L2Loss()
trainer = gluon.Trainer(net.collect_params(), 'adam', {'learning_rate': 0.001})
#
#########################
### EvalOnFile BEGIN: ###
#########################
#
def EvalOnFile(name_image):
#
    #print("DEBUG: EvalOnFile\n")
    input_data = nd.array(READ_XZ(name_image).reshape(-1,batch_size,image_size))
    input_data_ctx = input_data.as_in_context(ctx)
    batches = input_data.shape[0]
    #batches = 5
    print(input_data.shape[0])
    ret = 0.0
#
    ### LOOP BEGIN: ###
    for i in range(batches) :
        #print("DEBUG: EvalOnFile ",i,"\n")
        output = net(input_data_ctx[i])
        diff = (output - input_data_ctx[i]).asnumpy()
        ret += LA.norm(diff) / batch_size
#        print(ret)
    ### LOOP END. ###
#
    return ret / batches
#######################
### EvalOnFile END. ###
#######################
#
##########################
### TrainOnFile BEGIN: ###
##########################
def TrainOnFile(name_image,name_image_test):
#
    input_data = nd.array(READ_XZ(name_image).reshape(-1,batch_size,1,width,width))
    input_data_ctx = input_data.as_in_context(ctx)
    batches = input_data.shape[0]
    #batches = 5
    #epochs = 10
    print(input_data.shape[0])
#
    for e in range(epochs) :
        cumulative_loss = 0
        for i in range(batches) :
            with autograd.record() :
                output = net(input_data_ctx[i])
                loss = softmax_cross_entropy(output, input_data_ctx[i])
            loss.backward()
            trainer.step(batch_size)
            inst_loss = nd.sum(loss).asscalar()
            if((i%500)==0):
                print(inst_loss)
            cumulative_loss += inst_loss
            curr_loss = inst_loss
            moving_loss = (
                curr_loss if ((i == 0) and (e == 0))
                else (1 - smoothing_constant) * moving_loss + smoothing_constant * curr_loss
            )
        print(cumulative_loss)
        net.save_parameters(PARAM_NAME)
        train_accuracy = EvalOnFile(name_image)
        test_accuracy = EvalOnFile(name_image_test)
        print("Epoch %s. Loss: %s, Train_acc %s, Test_acc %s" % (e, moving_loss, train_accuracy, train_accuracy))
#
########################
### TrainOnFile END. ###
########################
#
TrainOnFile("./TRAIN/0/image.xz","./TEST/0/image.xz")
TrainOnFile("./TRAIN/1/image.xz","./TEST/1/image.xz")
TrainOnFile("./TRAIN/2/image.xz","./TEST/2/image.xz")
TrainOnFile("./TRAIN/3/image.xz","./TEST/3/image.xz")
TrainOnFile("./TRAIN/4/image.xz","./TEST/4/image.xz")
TrainOnFile("./TRAIN/5/image.xz","./TEST/5/image.xz")
TrainOnFile("./TRAIN/6/image.xz","./TEST/6/image.xz")
TrainOnFile("./TRAIN/7/image.xz","./TEST/7/image.xz")
