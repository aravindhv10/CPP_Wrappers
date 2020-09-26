#ifndef _HEADER_GUARD_Headers_
#define _HEADER_GUARD_Headers_

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
#include <map>
#include <set>
#include <mutex>

#ifdef USECBLAS
	#include "cblas.h"
#endif

#endif
