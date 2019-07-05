#!/usr/bin/python3
from __future__ import division, print_function, absolute_import
import numpy as np
from numpy import linalg as LA
import mxnet as mx
from mxnet import nd, autograd, gluon
import os
#
ctx = mx.cpu()
data_ctx = ctx
model_ctx = ctx
batch_size = 2000
width=40
num_inputs = width*width
sizeoftype = num_inputs*4
PARAM_NAME = "net_ae_pars_10"
mx.random.seed(1)
os.system("taskset -a -p 0xFFFFFFFF %d" % os.getpid())
sizes = [ 40*40 , 24*24 , 10*10 ]
num_outputs = num_inputs
#
class CenteredLayer(mx.gluon.nn.HybridSequential):
    def __init__(self, **kwargs):
        super(CenteredLayer, self).__init__(**kwargs)

    def forward(self, x):
        return x.softmax(axis=1)
#
net = gluon.nn.HybridSequential()
with net.name_scope():
    net.add ( gluon.nn.Dense ( sizes[1] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[2] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[1] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[0] , activation="relu" ) )
    net.add ( gluon.nn.Dense ( sizes[0]                     ) )
    net.add ( CenteredLayer()                                 )
#net.collect_params().initialize(mx.init.Xavier(), ctx=model_ctx)
net.collect_params().initialize(mx.init.Normal(sigma=.1), ctx=model_ctx)
net.hybridize()
net.load_parameters(PARAM_NAME, ctx=ctx)
#
def PredictFile(infilename,outfilename):
    statinfo = os.stat(infilename)
    leadingshape = int(statinfo.st_size/(sizeoftype*batch_size))
    print(leadingshape)
    X = np.memmap(infilename, dtype='float32', mode='r', shape=(leadingshape,batch_size,num_inputs))
    Fout = open(outfilename,"w")
    res=np.zeros(batch_size,dtype='float32')
    for i in range(leadingshape):
        A = nd.array(X[i])
        out=(net(A)-A).asnumpy()
        #print(out.shape)
        for j in range(batch_size):
            res[j]=LA.norm(out[j])
            #print("DEBUG: ",res[j])
        #print(res)
        #print(i,"/",leadingshape)
        res.tofile(Fout)
    Fout.close()
#
PredictFile("./OUTS/QCD_HPT/TEST/0/image","./OUTS/QCD_HPT/TEST/0/predict");
PredictFile("./OUTS/QCD_HPT/TEST/1/image","./OUTS/QCD_HPT/TEST/1/predict");
PredictFile("./OUTS/QCD_HPT/TEST/2/image","./OUTS/QCD_HPT/TEST/2/predict");
PredictFile("./OUTS/QCD_HPT/TEST/3/image","./OUTS/QCD_HPT/TEST/3/predict");
PredictFile("./OUTS/QCD_HPT/TEST/4/image","./OUTS/QCD_HPT/TEST/4/predict");
PredictFile("./OUTS/QCD_HPT/TEST/5/image","./OUTS/QCD_HPT/TEST/5/predict");
PredictFile("./OUTS/QCD_HPT/TEST/6/image","./OUTS/QCD_HPT/TEST/6/predict");
PredictFile("./OUTS/QCD_HPT/TEST/7/image","./OUTS/QCD_HPT/TEST/7/predict");
PredictFile("./OUTS/QCD_HPT/TRAIN/0/image","./OUTS/QCD_HPT/TRAIN/0/predict");
PredictFile("./OUTS/QCD_HPT/TRAIN/1/image","./OUTS/QCD_HPT/TRAIN/1/predict");
PredictFile("./OUTS/QCD_HPT/TRAIN/2/image","./OUTS/QCD_HPT/TRAIN/2/predict");
PredictFile("./OUTS/QCD_HPT/TRAIN/3/image","./OUTS/QCD_HPT/TRAIN/3/predict");
PredictFile("./OUTS/QCD_HPT/TRAIN/4/image","./OUTS/QCD_HPT/TRAIN/4/predict");
PredictFile("./OUTS/QCD_HPT/TRAIN/5/image","./OUTS/QCD_HPT/TRAIN/5/predict");
PredictFile("./OUTS/QCD_HPT/TRAIN/6/image","./OUTS/QCD_HPT/TRAIN/6/predict");
PredictFile("./OUTS/QCD_HPT/TRAIN/7/image","./OUTS/QCD_HPT/TRAIN/7/predict");
exit()
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
