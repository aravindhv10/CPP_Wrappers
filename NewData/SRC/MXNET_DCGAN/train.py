#!/usr/bin/python3
from __future__ import print_function
import numpy as np
import os
import matplotlib as mpl
import tarfile
import mxnet as mx
import matplotlib.image as mpimg
from matplotlib import pyplot as plt
from mxnet import gluon
from mxnet import ndarray as nd
from mxnet.gluon import nn, utils
from mxnet import autograd
#exit()

PARAM_NAME_D = "./OUTS/PARS/GAN_PARS_D"
PARAM_NAME_G = "./OUTS/PARS/GAN_PARS_G"

PARAM_NAME_D = "./GAN_PARS_D"
PARAM_NAME_G = "./GAN_PARS_G"

WIDTH = 64
RESOLUTION = WIDTH*WIDTH
IMAGESIZE = RESOLUTION*4
batch_size = 100

class CenteredLayer(mx.gluon.nn.HybridSequential):
    def __init__(self, **kwargs):
        super(CenteredLayer, self).__init__(**kwargs)

    def forward(self, x):
        return x.reshape((batch_size,WIDTH*WIDTH)).softmax(axis=1).reshape((batch_size,1,WIDTH,WIDTH))


def EvalFile (filename) :
    epochs = 2 # Set low by default for tests, set higher when you actually run this code.
    batch_size = 100
    latent_z_size = 100
    use_gpu = False
    ctx = mx.gpu() if use_gpu else mx.cpu()
    lr = 0.0002
    beta1 = 0.5
    #
    lfw_url = 'http://vis-www.cs.umass.edu/lfw/lfw-deepfunneled.tgz'
    data_path = 'lfw_dataset'
#    if not os.path.exists(data_path):
#        os.makedirs(data_path)
#        data_file = utils.download(lfw_url)
#        with tarfile.open(data_file) as tar:
#            tar.extractall(path=data_path)
#    #
    target_wd = 64
    target_ht = 64
    img_list = []
    #
    IMAGEFILE = filename
    statinfo = os.stat(IMAGEFILE)
    leadingshape = int(statinfo.st_size/IMAGESIZE)
    #leadingshape = 1000
    print(leadingshape)
    X = np.memmap(IMAGEFILE, dtype='float32', mode='r', shape=(leadingshape,1,WIDTH,WIDTH))
    Xnd = nd.array(X)
    #
    train_data = mx.io.NDArrayIter(data=Xnd, batch_size=batch_size)
    #print(X[0][0][0][0])
    #
    #exit()
    #
    def visualize(img_arr):
        plt.imshow(img_arr[0])
        plt.axis('off')

    for i in range(4):
        plt.subplot(1,4,i+1)
        visualize(X[i])
    plt.show()
    #
#    exit()
    # build the generator
    nc = 1
    ngf = 64
    netG = gluon.nn.HybridSequential()

    with netG.name_scope():
        # input is Z, going into a convolution
        netG.add(nn.Conv2DTranspose(ngf * 8, 4, 1, 0, use_bias=False))
        netG.add(nn.BatchNorm())
        netG.add(nn.Activation('relu'))
        # state size. (ngf*8) x 4 x 4
        netG.add(nn.Conv2DTranspose(ngf * 4, 4, 2, 1, use_bias=False))
        netG.add(nn.BatchNorm())
        netG.add(nn.Activation('relu'))
        # state size. (ngf*8) x 8 x 8
        netG.add(nn.Conv2DTranspose(ngf * 2, 4, 2, 1, use_bias=False))
        netG.add(nn.BatchNorm())
        netG.add(nn.Activation('relu'))
        # state size. (ngf*8) x 16 x 16
        netG.add(nn.Conv2DTranspose(ngf, 4, 2, 1, use_bias=False))
        netG.add(nn.BatchNorm())
        netG.add(nn.Activation('relu'))
        # state size. (ngf*8) x 32 x 32
        netG.add(nn.Conv2DTranspose(nc, 4, 2, 1, use_bias=False))
        netG.add(CenteredLayer())
        #netG.add(nn.Activation('tanh'))
        # state size. (nc) x 64 x 64
    netG.hybridize()
    # build the discriminator
    ndf = 64
    netD = gluon.nn.HybridSequential()
    with netD.name_scope():
        # input is (nc) x 64 x 64
        netD.add(nn.Conv2D(ndf, 4, 2, 1, use_bias=False))
        netD.add(nn.LeakyReLU(0.2))
        # state size. (ndf) x 32 x 32
        netD.add(nn.Conv2D(ndf * 2, 4, 2, 1, use_bias=False))
        netD.add(nn.BatchNorm())
        netD.add(nn.LeakyReLU(0.2))
        # state size. (ndf) x 16 x 16
        netD.add(nn.Conv2D(ndf * 4, 4, 2, 1, use_bias=False))
        netD.add(nn.BatchNorm())
        netD.add(nn.LeakyReLU(0.2))
        # state size. (ndf) x 8 x 8
        netD.add(nn.Conv2D(ndf * 8, 4, 2, 1, use_bias=False))
        netD.add(nn.BatchNorm())
        netD.add(nn.LeakyReLU(0.2))
        # state size. (ndf) x 4 x 4
        netD.add(nn.Conv2D(1, 4, 1, 0, use_bias=False))
    #
    # loss
    netD.hybridize()
    loss = gluon.loss.SigmoidBinaryCrossEntropyLoss()

    # initialize the generator and the discriminator
    netG.initialize(mx.init.Normal(0.02), ctx=ctx)
    netD.initialize(mx.init.Normal(0.02), ctx=ctx)
    netG.load_parameters(PARAM_NAME_G, ctx=ctx)
    netD.load_parameters(PARAM_NAME_D, ctx=ctx)

    # trainer for the generator and the discriminator
    trainerG = gluon.Trainer(netG.collect_params(), 'adam', {'learning_rate': lr, 'beta1': beta1})
    trainerD = gluon.Trainer(netD.collect_params(), 'adam', {'learning_rate': lr, 'beta1': beta1})
    #exit()
    from datetime import datetime
    import time
    import logging

    real_label = nd.ones((batch_size,), ctx=ctx)
    fake_label = nd.zeros((batch_size,),ctx=ctx)

    def facc(label, pred):
        pred = pred.ravel()
        label = label.ravel()
        return ((pred > 0.5) == label).mean()
    metric = mx.metric.CustomMetric(facc)

    stamp =  datetime.now().strftime('%Y_%m_%d-%H_%M')
    logging.basicConfig(level=logging.DEBUG)

    for epoch in range(epochs):
        tic = time.time()
        btic = time.time()
        train_data.reset()
        iter = 0
        for batch in train_data:
            ############################
            # (1) Update D network: maximize log(D(x)) + log(1 - D(G(z)))
            ###########################
            data = batch.data[0].as_in_context(ctx)
            latent_z = mx.nd.random_normal(0, 1, shape=(batch_size, latent_z_size, 1, 1), ctx=ctx)

            with autograd.record():
                # train with real image
                tmpout = netD(data)
                #print(tmpout.shape)
                output = tmpout.reshape((-1, 1))
                errD_real = loss(output, real_label)
                metric.update([real_label,], [output,])

                # train with fake image
                fake = netG(latent_z)
                output = netD(fake.detach()).reshape((-1, 1))
                errD_fake = loss(output, fake_label)
                errD = errD_real + errD_fake
                errD.backward()
                metric.update([fake_label,], [output,])

            trainerD.step(batch.data[0].shape[0])

            ############################
            # (2) Update G network: maximize log(D(G(z)))
            ###########################
            with autograd.record():
                fake = netG(latent_z)
                output = netD(fake).reshape((-1, 1))
                errG = loss(output, real_label)
                errG.backward()

            trainerG.step(batch.data[0].shape[0])

            # Print log infomation every ten batches
            if iter % 10 == 0:
                name, acc = metric.get()
                logging.info('speed: {} samples/s'.format(batch_size / (time.time() - btic)))
                logging.info('discriminator loss = %f, generator loss = %f, binary training acc = %f at iter %d epoch %d'
                         %(nd.mean(errD).asscalar(),
                           nd.mean(errG).asscalar(), acc, iter, epoch))
            iter = iter + 1
            btic = time.time()
            netD.save_parameters(PARAM_NAME_D)
            netG.save_parameters(PARAM_NAME_G)

        name, acc = metric.get()
        metric.reset()

        # logging.info('\nbinary training acc at epoch %d: %s=%f' % (epoch, name, acc))
        # logging.info('time: %f' % (time.time() - tic))

        # Visualize one generated image for each epoch
        # fake_img = fake[0]
        # visualize(fake_img)
        # plt.show()
    #
    num_image = 8

    for i in range(num_image):
        print("### CAME HERE 1 ###")
        latent_z = mx.nd.random_normal(0, 1, shape=(batch_size, latent_z_size, 1, 1), ctx=ctx)
        print("### CAME HERE 2 ###")
        img = netG(latent_z).asnumpy()
        print("### CAME HERE 3 ###")
        plt.subplot(2,4,i+1)
        visualize(img[0])
    plt.show()
    #
    num_image = 12
    latent_z = mx.nd.random_normal(0, 1, shape=(1, latent_z_size, 1, 1), ctx=ctx)
    step = 0.05
    for i in range(num_image):
        img = netG(latent_z)
        plt.subplot(3,4,i+1)
        visualize(img[0])
        latent_z += 0.05
    plt.show()
#
EvalFile("image")
