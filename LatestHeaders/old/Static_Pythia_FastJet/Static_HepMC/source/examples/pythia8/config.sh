#!/bin/sh
if [ ! $?LD_LIBRARY_PATH ]; then
  export LD_LIBRARY_PATH=/usr/local/lib
fi
if [ $?LD_LIBRARY_PATH ]; then
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib
fi
export PYTHIA8DATA=${PYTHIA8_HOME}/xmldoc
