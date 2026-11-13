#!/bin/bash
./samc \
	--nstep 10000000 \
	--nchain 1 \
	--nprint 10000 \
	--nwrite 0 \
	--ncorr 100000 \
	--out out \
	-x 100 \
	-y 100 \
	-z 100 \
	-v true

