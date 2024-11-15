#!/bin/sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
echo "Started running"
perf stat -a -e power/energy-pkg/ ./build/imtool-soa/imtool-soa input/deer-small.ppm output/time-eval.ppm resize 8000 8000
echo "Finished running"