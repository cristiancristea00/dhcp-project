#!/bin/bash

# Check the number of arguments
if [ $# -lt 4 ]; then
   echo "Usage: $0 <Mandelbrot/Tricorn/Julia/Cosine> <Width> <Height> <Max Iterations>"
   exit 1
fi

# Fractal type
FRACTAL=$1
WIDTH=$2
HEIGHT=$3
MAX_ITER=$4

# Check which fractal is selected and set the appropriate parameters
case "$FRACTAL" in
   Mandelbrot)
      nvcc -o mandelbrot mandelbrot.cu `pkg-config --cflags --libs opencv4` -lcudart
      ./mandelbrot $WIDTH $HEIGHT $MAX_ITER
      ;;
   Tricorn)
      nvcc -o tricorn tricorn.cu `pkg-config --cflags --libs opencv4` -lcudart
      ./tricorn $WIDTH $HEIGHT $MAX_ITER
      ;;
   Julia)
      if [ $# -lt 6 ]; then
         echo "Usage for Julia: $0 <Julia> <Width> <Height> <Max Iterations> <C_real> <C_imag>"
         exit 1
      fi
      C_REAL=$5
      C_IMAG=$6
      nvcc -o julia julia.cu `pkg-config --cflags --libs opencv4` -lcudart
      ./julia $WIDTH $HEIGHT $MAX_ITER $C_REAL $C_IMAG
      ;;
   Cosine)
      nvcc -o cosine cosine.cu `pkg-config --cflags --libs opencv4` -lcudart
      ./cosine $WIDTH $HEIGHT $MAX_ITER
      ;;
   *)
      echo "Unknown fractal type. Choose from: Mandelbrot, Tricorn, Julia, Cosine."
      exit 1
      ;;
esac
