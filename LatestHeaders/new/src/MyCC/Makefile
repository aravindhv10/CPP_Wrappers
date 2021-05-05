.PHONY all: ./bin/clang-cc ./bin/clang-cxx ./bin/gcc-cc ./bin/gcc-cxx ./bin/clang-omp-cc ./bin/clang-omp-cxx ./bin/gcc-omp-cc ./bin/gcc-omp-cxx

./bin/clang-cc: ./compile ./src/clang-cc.cc
	./compile ./src/clang-cc.cc

./bin/clang-omp-cc: ./compile ./src/clang-omp-cc.cc
	./compile ./src/clang-omp-cc.cc

./bin/clang-cxx: ./compile ./src/clang-cxx.cc
	./compile ./src/clang-cxx.cc

./bin/clang-omp-cxx: ./compile ./src/clang-omp-cxx.cc
	./compile ./src/clang-omp-cxx.cc

./bin/gcc-cc: ./compile ./src/gcc-cc.cc
	./compile ./src/gcc-cc.cc

./bin/gcc-omp-cc: ./compile ./src/gcc-omp-cc.cc
	./compile ./src/gcc-omp-cc.cc

./bin/gcc-cxx: ./compile ./src/gcc-cxx.cc
	./compile ./src/gcc-cxx.cc

./bin/gcc-omp-cxx: ./compile ./src/gcc-omp-cxx.cc
	./compile ./src/gcc-omp-cxx.cc
