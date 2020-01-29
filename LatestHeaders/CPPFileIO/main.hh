﻿#ifndef CPPFileIO_HH
#define CPPFileIO_HH

	#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
	#endif


	#include <sched.h>
	#include <stdio.h>
	#include <time.h>
	#include <stdlib.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <sys/stat.h>
	#include <sys/mman.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <math.h>

	#include <cmath>
	#include <string.h>
	#include <iostream>
	#include <vector>
	#include <algorithm>
	#include <random>
	#include <complex>

	#include "pcg-cpp/main.hh"
	#include "Basic.hh"

	#ifdef USECBLAS
		#include "cblas.h"
	#endif

	#include "Tensors3.hh"

	#include "Histograms.hh"

	#include "./Array/DYNAMIC/D1.hh"

	#include "MXNET_WRAPPERS.hh"
//	#include "NeuralNetwork1.hh"

#endif
