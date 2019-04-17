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

epochs = 1 # Set low by default for tests, set higher when you actually run this code.
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

target_wd = 40
target_ht = 40
img_list = []
sizeoftype = 6400
#def transform(data, target_wd, target_ht):
    # resize to target_wd * target_ht
#    data = mx.image.imresize(data, target_wd, target_ht)
    # transpose from (target_wd, target_ht, 3)
    # to (3, target_wd, target_ht)
#    data = nd.transpose(data, (2,0,1))
    # normalize to [-1, 1]
#    data = data.astype(np.float32)/127.5 - 1
    # if image is greyscale, repeat 3 times to get RGB image.
#    if data.shape[0] == 1:
#        data = nd.tile(data, (3, 1, 1))
#    return data.reshape((1,) + data.shape)

#for path, _, fnames in os.walk(data_path):
#    for fname in fnames:
#        if not fname.endswith('.jpg'):
#            continue
#        img = os.path.join(path, fname)
#        img_arr = mx.image.imread(img)
#        img_arr = transform(img_arr, target_wd, target_ht)
#        img_list.append(img_arr)

IMAGEFILE = "./image"
statinfo = os.stat(IMAGEFILE)
leadingshape = int(statinfo.st_size/sizeoftype)
print(leadingshape)
leadingshape=100000
X = np.memmap(IMAGEFILE, dtype='float32', mode='r', shape=(leadingshape,1,40,40))
Xnd = nd.array(X)
dataset = mx.gluon.data.dataset.ArrayDataset(Xnd)
train_data = mx.gluon.data.DataLoader(dataset, batch_size=batch_size, shuffle=True, num_workers=1)

#train_data = mx.io.NDArrayIter(data=nd.concatenate(img_list), batch_size=batch_size)
#
#def visualize(img_arr):
#    plt.imshow(((img_arr.asnumpy().transpose(1, 2, 0) + 1.0) * 127.5).astype(np.uint8))
#    plt.axis('off')

#for i in range(4):
#    plt.subplot(1,4,i+1)
#    visualize(img_list[i + 10][0])
#plt.show()
#
# build the generator
nc = 1
ngf = 40
netG = gluon.nn.HybridSequential()
with netG.name_scope():
    # input is Z, going into a convolution
    #channels, kernel_size, strides=(1, 1), padding=(0, 0)
    netG.add(nn.Conv2DTranspose(50,kernel_size=5, strides=(1, 1), padding=(0, 0), use_bias=True))
    #netG.add(nn.BatchNorm())
    netG.add(nn.Activation('relu'))
    # state size. (ngf*8) x 4 x 4
    #channels, kernel_size, strides=(1, 1), padding=(0, 0)
    netG.add(nn.Conv2DTranspose(30,kernel_size=4, strides=(1, 1), padding=(0, 0), use_bias=True))
    #netG.add(nn.BatchNorm())
    netG.add(nn.Activation('relu'))
    #channels, kernel_size, strides=(1, 1), padding=(0, 0)
    netG.add(nn.Conv2DTranspose(10,kernel_size=6, strides=(2, 2), padding=(0, 0), use_bias=True))
    #netG.add(nn.BatchNorm())
    netG.add(nn.Activation('relu'))
    #channels, kernel_size, strides=(1, 1), padding=(0, 0)
    netG.add(nn.Conv2DTranspose(1,kernel_size=2, strides=(2, 2), padding=(0, 0), use_bias=True))
    #netG.add(nn.BatchNorm())
    netG.add(nn.Activation('relu'))
    #netG.add(nn.Conv2DTranspose(ngf * 4, 4, strides=(1, 1), padding=(0, 0), use_bias=True))
    #netG.add(nn.Conv2DTranspose(ngf * 4, 4, 2, 1, use_bias=False))
    #netG.add(nn.BatchNorm())
    #netG.add(nn.Activation('relu'))
    netG.add(CenteredLayer())
    # state size. (ngf*8) x 8 x 8
    #netG.add(nn.Conv2DTranspose(ngf * 2, 4, 2, 1, use_bias=False))
    #netG.add(nn.BatchNorm())
    #netG.add(nn.Activation('relu'))
    # state size. (ngf*8) x 16 x 16
    #netG.add(nn.Conv2DTranspose(ngf, 4, 2, 1, use_bias=False))
    #netG.add(nn.BatchNorm())
    #netG.add(nn.Activation('relu'))
    # state size. (ngf*8) x 32 x 32
    #netG.add(nn.Conv2DTranspose(nc, 4, 2, 1, use_bias=False))
    #netG.add(nn.Activation('tanh'))
    # state size. (nc) x 64 x 64
netG.hybridize()

#netG.initialize(mx.init.Normal(0.02), ctx=ctx)
#latent_z = mx.nd.random_normal(0, 1, shape=(batch_size, latent_z_size, 1, 1), ctx=ctx)
#out = nd.ones((batch_size,latent_z_size))
#out = netG(latent_z)
#print(out.shape)
#exit ()
# build the discriminator
ndf = 64
netD = gluon.nn.HybridSequential()
with netD.name_scope():
    netD.add(gluon.nn.Conv2D(channels=50, kernel_size=10, activation='relu'))
    netD.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    netD.add(gluon.nn.Conv2D(channels=30, kernel_size=5, activation='relu'))
    netD.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    netD.add(gluon.nn.Conv2D(channels=10, kernel_size=2, activation='relu'))
    netD.add(gluon.nn.MaxPool2D(pool_size=2, strides=2))
    # The Flatten layer collapses all axis, except the first one, into one axis.
    netD.add(gluon.nn.Flatten())
    netD.add(gluon.nn.Dense(num_fc, activation="relu"))
    netD.add(gluon.nn.Dense(num_outputs))
    # input is (nc) x 64 x 64
    #netD.add(nn.Conv2D(ndf, 4, 2, 1, use_bias=False))
    #netD.add(nn.LeakyReLU(0.2))
    # state size. (ndf) x 32 x 32
    #netD.add(nn.Conv2D(ndf * 2, 4, 2, 1, use_bias=False))
    #netD.add(nn.BatchNorm())
    #netD.add(nn.LeakyReLU(0.2))
    # state size. (ndf) x 16 x 16
    #netD.add(nn.Conv2D(ndf * 4, 4, 2, 1, use_bias=False))
    #netD.add(nn.BatchNorm())
    #netD.add(nn.LeakyReLU(0.2))
    # state size. (ndf) x 8 x 8
    #netD.add(nn.Conv2D(ndf * 8, 4, 2, 1, use_bias=False))
    #netD.add(nn.BatchNorm())
    #netD.add(nn.LeakyReLU(0.2))
    # state size. (ndf) x 4 x 4
    #netD.add(nn.Conv2D(1, 4, 1, 0, use_bias=False))
#
netD.hybridize()
# loss
#loss = gluon.loss.SigmoidBinaryCrossEntropyLoss()
#loss = gluon.loss.SoftmaxCrossEntropyLoss(sparse_label=False)
loss = gluon.loss.SoftmaxCrossEntropyLoss()
#exit()
# initialize the generator and the discriminator
netG.initialize(mx.init.Normal(0.02), ctx=ctx)
netD.initialize(mx.init.Normal(0.02), ctx=ctx)

# trainer for the generator and the discriminator
trainerG = gluon.Trainer(netG.collect_params(), 'adam', {'learning_rate': lr, 'beta1': beta1})
trainerD = gluon.Trainer(netD.collect_params(), 'adam', {'learning_rate': lr, 'beta1': beta1})
from datetime import datetime
import time
import logging


#real_label = nd.zeros((batch_size,2),ctx=ctx)
#tmp_real = nd.zeros((2),ctx=ctx)
#tmp_real[0] = 1
#fake_label = nd.zeros((batch_size,2),ctx=ctx)
#tmp_fake = nd.zeros((2),ctx=ctx)
#tmp_fake[1] = 1
#for e in range(batch_size):
#    real_label[e]=tmp_real
#    fake_label[e]=tmp_fake
#print(real_label)
#print(fake_label)
#exit()

real_label = nd.zeros((batch_size),ctx=ctx)
fake_label = nd.ones((batch_size),ctx=ctx)
#print(real_label)
#print(fake_label)
#exit()
#print(real_label.shape)
#exit()
#def facc(label, pred):
#    ret = label[0] - pred[0]
#    if ret < 0 :
#        ret = -ret
#    if ret > 0.5:
#        ret = 1
#    else :
#        ret = 0
#    return ret
#return ((pred > 0.5) == label).mean()
#metric = mx.metric.CustomMetric(facc)

#def facc(label, pred):
#    predictions = nd.argmax(pred, axis=1)
    #pred = pred.ravel()
    #label = label.ravel()
#    return ((pred > 0.5) == label).mean()
#metric = mx.metric.CustomMetric(facc)

stamp =  datetime.now().strftime('%Y_%m_%d-%H_%M')
logging.basicConfig(level=logging.DEBUG)
#exit()
netD.load_parameters(PARAM_NAME_D, ctx=ctx)
netG.load_parameters(PARAM_NAME_G, ctx=ctx)
for epoch in range(epochs):
    tic = time.time()
    btic = time.time()
#    train_data.reset()
    iter = 0
    for i, data in enumerate(train_data):
        ############################
        # (1) Update D network: maximize log(D(x)) + log(1 - D(G(z)))
        ###########################
        #data = batch.data[0].as_in_context(ctx)
        data = data.as_in_context(ctx)
        latent_z = mx.nd.random_normal(0, 1, shape=(batch_size, latent_z_size, 1, 1), ctx=ctx)
        #exit()
        with autograd.record():
            # train with real image
            output = netD(data)
            #.reshape((-1, 1))
            errD_real = loss(output, real_label)
            #metric.update(real_label,output)
            #exit()
            # train with fake image
            fake = netG(latent_z)
            #output = netD(fake.detach()).reshape((-1, 1))
            output = netD(fake)
            #exit()
            errD_fake = loss(output, fake_label)
            errD = errD_real + errD_fake
            errD.backward()
            print("dis: ",nd.sum(errD).asscalar())
            #exit()
            #metric.update([fake_label,], [output,])

        trainerD.step(data.shape[0])

        ############################
        # (2) Update G network: maximize log(D(G(z)))
        ###########################
        with autograd.record():
            fake = netG(latent_z)
            #output = netD(fake).reshape((-1, 1))
            output = netD(fake)
            errG = loss(output, real_label)
            errG.backward()
            print("gen: ",nd.sum(errG).asscalar())

        trainerG.step(data.shape[0])
        #exit()

        # Print log infomation every ten batches
        #if iter % 10 == 0:
        #    name, acc = metric.get()
        #    logging.info('speed: {} samples/s'.format(batch_size / (time.time() - btic)))
        #    logging.info('discriminator loss = %f, generator loss = %f, binary training acc = %f at iter %d epoch %d'
        #             %(nd.mean(errD).asscalar(),
        #               nd.mean(errG).asscalar(), acc, iter, epoch))
        #iter = iter + 1
        #btic = time.time()
    if((i%50)==0):
        print("saving...")
        netD.save_parameters(PARAM_NAME_D)
        netG.save_parameters(PARAM_NAME_G)
    #name, acc = metric.get()
    #metric.reset()
    # logging.info('\nbinary training acc at epoch %d: %s=%f' % (epoch, name, acc))
    # logging.info('time: %f' % (time.time() - tic))

    # Visualize one generated image for each epoch
    # fake_img = fake[0]
    # visualize(fake_img)
    # plt.show()
#
#exit()
#
latent_z = mx.nd.random_normal(0, 1, shape=(batch_size, latent_z_size, 1, 1), ctx=ctx)
img = netG(latent_z).asnumpy()
#plt.subplot(2,4,i+1)
print(img[0])
#plt.imshow(img[0])
#visualize(img[0])
#exit()
#num_image = 8
#for i in range(num_image):
#    #latent_z = mx.nd.random_normal(0, 1, shape=(1, latent_z_size, 1, 1), ctx=ctx)
#    #img = netG(latent_z)
#    #exit()
#
#plt.show()
exit()
#
num_image = 12
latent_z = mx.nd.random_normal(0, 1, shape=(batch_size, latent_z_size, 1, 1), ctx=ctx)
img = netG(latent_z)
step = 0.05
for i in range(num_image):
    plt.subplot(3,4,i+1)
    visualize(img[0])
    latent_z += 0.05
plt.show()
