#ifndef CPPFileIO_HH
#define CPPFileIO_HH

	#ifndef _GNU_SOURCE
		#define _GNU_SOURCE
	#endif


	#ifndef _XOPEN_SOURCE
		#define _XOPEN_SOURCE
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
	#include <string.h>

	#include <cmath>
	#include <iostream>
	#include <vector>
	#include <algorithm>
	#include <random>
	#include <complex>
	#include <sstream>
	#include <locale>
	#include <iomanip>

	#include "./ParseTime.hh"
	#include "./pcg-cpp/main.hh"
	#include "./Basic.hh"
	#include "./ExternalPrograms.hh"
	#include "./FileIO.hh"
	#include "./FileSplitter.hh"
	#include "./ExternalMergeSorter.hh"
	#include "./ExternalMergeSorterClean.hh"
	#include "./LineReader.hh"
	#include "./FastLineReader.hh"
	#include "./FasterLineReader.hh"
	#include "./Shuffel.hh"
	#include "./BinarySearch.hh"
	#include "./Inner_Join.hh"

	#ifdef USECBLAS
		#include "cblas.h"
	#endif

	#include "./Tensors3.hh"

	#include "./Histograms.hh"

	#include "./Array/DYNAMIC/main.hh"
	#include "./DynamicStore.hh"

	#include "./MXNET_WRAPPERS.hh"
	#include "./Polynomial/main.hh"
	#include "./Read_Show_Functions.hh"
	#include "./AnalyzeLines.hh"

#endif
