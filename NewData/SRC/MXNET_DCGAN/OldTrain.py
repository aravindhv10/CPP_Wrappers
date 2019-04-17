#!/usr/bin/python3

from __future__ import print_function
import os
import matplotlib as mpl
import tarfile
import matplotlib.image as mpimg
from matplotlib import pyplot as plt

import mxnet as mx
from mxnet import gluon
from mxnet import ndarray as nd
from mxnet.gluon import nn, utils
from mxnet import autograd
import numpy as np

from datetime import datetime
import time
import logging

epochs = 2 # Set low by default for tests, set higher when you actually run this code.
batch_size = 200
latent_z_size = 100
num_outputs = 2
num_fc = 32
use_gpu = False
ctx = mx.gpu() if use_gpu else mx.cpu()

PARAM_NAME_D = "./OUTS/PARS/GAN_PARS_D"
PARAM_NAME_G = "./OUTS/PARS/GAN_PARS_G"

lr = 0.0002
beta1 = 0.5
#
#lfw_url = 'http://vis-www.cs.umass.edu/lfw/lfw-deepfunneled.tgz'
#data_path = 'lfw_dataset'
#if not os.path.exists(data_path):
#    os.makedirs(data_path)
#    data_file = utils.download(lfw_url)
#    with tarfile.open(data_file) as tar:
#        tar.extractall(path=data_path)
#

def visualize(img_arr):
    plt.imshow(((img_arr.asnumpy().transpose(1, 2, 0) + 1.0) * 127.5).astype(np.uint8))
    plt.axis('off')

class CenteredLayer(mx.gluon.nn.HybridSequential):
    def __init__(self, **kwargs):
        super(CenteredLayer, self).__init__(**kwargs)

    def forward(self, x):
        return x.reshape((batch_size,40*40)).softmax(axis=1).reshape((batch_size,1,40,40))

def EvalTrain(filename):
    target_wd = 40
    target_ht = 40
    img_list = []
    sizeoftype = 6400
    IMAGEFILE = filename
    statinfo = os.stat(IMAGEFILE)
    leadingshape = int(statinfo.st_size/sizeoftype)
    print(leadingshape)
    X = np.memmap(IMAGEFILE, dtype='float32', mode='r', shape=(leadingshape,1,40,40))
    Xnd = nd.array(X)
    dataset = mx.gluon.data.dataset.ArrayDataset(Xnd)
    train_data = mx.gluon.data.DataLoader(dataset, batch_size=batch_size, shuffle=True, num_workers=1)
    nc = 1
    ngf = 40
    netG = gluon.nn.HybridSequential()
    with netG.name_scope():
        netG.add(nn.Conv2DTranspose(50,kernel_size=5, strides=(1, 1), padding=(0, 0), use_bias=True))
        netG.add(nn.Activation('relu'))
        netG.add(nn.Conv2DTranspose(30,kernel_size=4, strides=(1, 1), padding=(0, 0), use_bias=True))
        netG.add(nn.Activation('relu'))
        netG.add(nn.Conv2DTranspose(10,kernel_size=6, strides=(2, 2), padding=(0, 0), use_bias=True))
        netG.add(nn.Activation('relu'))
        netG.add(nn.Conv2DTranspose(1,kernel_size=2, strides=(2, 2), padding=(0, 0), use_bias=True))
        netG.add(nn.Activation('relu'))
        netG.add(CenteredLayer())
    netG.hybridize()
    ndf = 64
    netD = gluon.nn.HybridSequential()
    with netD.name_scope():
        netD.add(gluon.nn.Conv2D(channels=50, kernel_size=10, activation='relu'))
        netD.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
        netD.add(gluon.nn.Conv2D(channels=30, kernel_size=5, activation='relu'))
        netD.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
        netD.add(gluon.nn.Conv2D(channels=10, kernel_size=2, activation='relu'))
        netD.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
        netD.add(gluon.nn.Flatten())
        netD.add(gluon.nn.Dense(num_fc, activation="relu"))
        netD.add(gluon.nn.Dense(num_outputs))
    netD.hybridize()
    loss = gluon.loss.SoftmaxCrossEntropyLoss()
    netG.initialize(mx.init.Normal(0.02), ctx=ctx)
    netD.initialize(mx.init.Normal(0.02), ctx=ctx)
    trainerG = gluon.Trainer(netG.collect_params(), 'adam', {'learning_rate': lr, 'beta1': beta1})
    trainerD = gluon.Trainer(netD.collect_params(), 'adam', {'learning_rate': lr, 'beta1': beta1})
    real_label = nd.zeros((batch_size),ctx=ctx)
    fake_label = nd.ones((batch_size),ctx=ctx)
    stamp =  datetime.now().strftime('%Y_%m_%d-%H_%M')
    logging.basicConfig(level=logging.DEBUG)
    netD.load_parameters(PARAM_NAME_D, ctx=ctx)
    netG.load_parameters(PARAM_NAME_G, ctx=ctx)
    for epoch in range(epochs):
        tic = time.time()
        btic = time.time()
        iter = 0
        for i, data in enumerate(train_data):
            data = data.as_in_context(ctx)
            latent_z = mx.nd.random_normal(0, 1, shape=(batch_size, latent_z_size, 1, 1), ctx=ctx)
            with autograd.record():
                output = netD(data)
                errD_real = loss(output, real_label)
                fake = netG(latent_z)
                output = netD(fake)
                errD_fake = loss(output, fake_label)
                errD = errD_real + errD_fake
                errD.backward()
                print("dis: ",nd.sum(errD).asscalar())
            trainerD.step(data.shape[0])
            with autograd.record():
                fake = netG(latent_z)
                output = netD(fake)
                errG = loss(output, real_label)
                errG.backward()
                print("gen: ",nd.sum(errG).asscalar())
            trainerG.step(data.shape[0])
            if((i%50)==0):
                print("saving...")
                netD.save_parameters(PARAM_NAME_D)
                netG.save_parameters(PARAM_NAME_G)
    latent_z = mx.nd.random_normal(0, 1, shape=(batch_size, latent_z_size, 1, 1), ctx=ctx)
    img = netG(latent_z).asnumpy()
    print(img[0])


EvalTrain("./OUTS/QCD/TRAIN/4/image")
EvalTrain("./OUTS/QCD/TRAIN/5/image")
EvalTrain("./OUTS/QCD/TRAIN/4/image")
EvalTrain("./OUTS/QCD/TRAIN/5/image")

EvalTrain("./OUTS/QCD/TRAIN/6/image")
EvalTrain("./OUTS/QCD/TRAIN/7/image")
EvalTrain("./OUTS/QCD/TRAIN/6/image")
EvalTrain("./OUTS/QCD/TRAIN/7/image")

EvalTrain("./OUTS/QCD/TRAIN/0/image")
EvalTrain("./OUTS/QCD/TRAIN/1/image")
EvalTrain("./OUTS/QCD/TRAIN/0/image")
EvalTrain("./OUTS/QCD/TRAIN/1/image")

EvalTrain("./OUTS/QCD/TRAIN/2/image")
EvalTrain("./OUTS/QCD/TRAIN/3/image")
EvalTrain("./OUTS/QCD/TRAIN/2/image")
EvalTrain("./OUTS/QCD/TRAIN/3/image")
