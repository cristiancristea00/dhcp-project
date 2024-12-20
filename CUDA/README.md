# Fractals: Mandelbrot, Tricorn, Julia & Cosine

This project explores four popular fractals: **Mandelbrot**, **Tricorn**, **Julia** & **Cosine**.

## Hardware Configuration

- **Processor:** Intel(R) Core(TM) i7-10870H
- **Graphics Card:** NVIDIA GeForce GTX 1650 (for GPU execution)

## Running on GPU with CUDA

### 1. Mandelbrot

**Compilation:**
nvcc -o mandelbrot mandelbrot.cu `pkg-config --cflags --libs opencv4` -lcudart

**Execution:**
./mandelbrot 5000 3000 1000

![Mandelbrot](img_fractals/CUDA_mandelbrot.png)

![BlockSize Mandelbrot](results/BlockSize_Mandelbrot.png)

![SpeedUp Mandelbrot](results/SpeedUp_Mandelbrot.png)

![Profiling Mandelbrot](results/Profiling_Mandelbrot.png)

### 2. Tricorn

**Compilation:**
nvcc -o tricorn tricorn.cu `pkg-config --cflags --libs opencv4` -lcudart

**Execution:**
./tricorn 5000 3000 1000

![Tricorn](img_fractals/CUDA_tricorn.png)

![BlockSize Tricorn](results/BlockSize_Tricorn.png)

![SpeedUp Tricorn](results/SpeedUp_Tricorn.png)

![Profiling Tricorn](results/Profiling_Tricorn.png)

### 3. Julia

**Compilation:**
nvcc -o julia julia.cu `pkg-config --cflags --libs opencv4` -lcudart

**Execution:**
./julia 5000 3000 1000 -0.7 0.27015

![Julia](img_fractals/CUDA_julia.png)

![BlockSize Julia](results/BlockSize_Mandelbrot.png)

![SpeedUp Julia](results/SpeedUp_Julia.png)

![Profiling Julia](results/Profiling_Julia.png)

### 4. Cosine

**Compilation:**
nvcc -o cosine cosine.cu `pkg-config --cflags --libs opencv4` -lcudart

**Execution:**
./cosine 5000 3000 1000

![Cosine](img_fractals/CUDA_cosine.png)

![BlockSize Cosine](results/BlockSize_Cosine.png)

![SpeedUp Cosine](results/SpeedUp_Cosine.png)

![Profiling Cosine](results/Profiling_Cosine.png)
