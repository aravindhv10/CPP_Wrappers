#!/usr/bin/python3
import pandas as pd
import h5py
import tables
import numpy as np

def EvalTrain () :
    f = h5py.File('./ORIGINAL/train.h5', 'r')
    file_TOP = open("./S1/TOP/TRAIN/out.bin","w")
    file_QCD = open("./S1/QCD/TRAIN/out.bin","w")
    dset = f['/table/table']
    for i in range(dset.shape[0]) :
        if (dset[i][2][1]==1) :
            dset[i][1].tofile(file_TOP)
        else:
            dset[i][1].tofile(file_QCD)
    #
    file_QCD.close()
    file_TOP.close()
#
def EvalTest () :
    f = h5py.File('./ORIGINAL/test.h5', 'r')
    file_TOP = open("./S1/TOP/TEST/out.bin","w")
    file_QCD = open("./S1/QCD/TEST/out.bin","w")
    dset = f['/table/table']
    for i in range(dset.shape[0]) :
        if (dset[i][2][1]==1) :
            dset[i][1].tofile(file_TOP)
        else:
            dset[i][1].tofile(file_QCD)
    #
    file_QCD.close()
    file_TOP.close()
#
def EvalVal () :
    f = h5py.File('./ORIGINAL/val.h5', 'r')
    file_TOP = open("./S1/TOP/VAL/out.bin","w")
    file_QCD = open("./S1/QCD/VAL/out.bin","w")
    dset = f['/table/table']
    for i in range(dset.shape[0]) :
        if (dset[i][2][1]==1) :
            dset[i][1].tofile(file_TOP)
        else:
            dset[i][1].tofile(file_QCD)
    #
    file_QCD.close()
    file_TOP.close()
#
EvalTest ()
EvalVal ()
