#!/usr/bin/python3

from __future__ import division, print_function, absolute_import
import numpy as np
import mxnet as mx
from mxnet import nd, autograd, gluon
import os

ctx = mx.cpu()
data_ctx = ctx
model_ctx = ctx
batch_size = 128
width=40
num_inputs = width*width
sizeoftype = 6400
midlayer = 5
PARAM_NAME = "./OUTS/PARS/net_params"
mx.random.seed(1)
os.system("taskset -a -p 0xFFFFFFFF %d" % os.getpid())
sizes = [ 40*40 , 33*33 , 26*26 , 19*19 , 12*12 , 5*5 ]
num_outputs = num_inputs

class CenteredLayer(mx.gluon.nn.HybridSequential):
    def __init__(self, **kwargs):
        super(CenteredLayer, self).__init__(**kwargs)

    def forward(self, x):
        return x.softmax(axis=1)


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

#net.collect_params().initialize(mx.init.Xavier(), ctx=model_ctx)
net.collect_params().initialize(mx.init.Normal(sigma=.1), ctx=model_ctx)
net.hybridize()
QCDFILE = "./OUTS/TMP/QCD_TRAIN.image"
statinfo = os.stat(QCDFILE)
leadingshape = int(statinfo.st_size/sizeoftype)
print(leadingshape)
#leadingshape = 2000000
X = np.memmap(QCDFILE, dtype='float32', mode='r', shape=(leadingshape,num_inputs))
A = nd.array(X)
train_data = mx.gluon.data.DataLoader(A, batch_size, shuffle=True)

softmax_cross_entropy = gluon.loss.L2Loss()
trainer = gluon.Trainer(net.collect_params(), 'adam', {'learning_rate': 0.0001})

epochs = 10
smoothing_constant = 0.001
net.load_parameters(PARAM_NAME, ctx=ctx)
for e in range(epochs):
    cumulative_loss = 0
    for i, data in enumerate(train_data):
        data = data.as_in_context(model_ctx)
        label = data.as_in_context(model_ctx)
        with autograd.record():
            output = net(data)
            loss = softmax_cross_entropy(output, label)
        loss.backward()
        trainer.step(data.shape[0])
        inst_loss = nd.sum(loss).asscalar()
        cumulative_loss += inst_loss
        print("    ",inst_loss)
        #net.save_parameters(PARAM_NAME)
    print(cumulative_loss);
    net.save_parameters(PARAM_NAME)
