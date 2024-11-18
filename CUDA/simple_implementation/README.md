Mandelbrot Fractal Generator (CUDA)
This project generates a Mandelbrot fractal using GPU acceleration with CUDA and saves it as Mandelbrot_CUDA.png. OpenCV is used for coloring and saving the image.

Compilation:
nvcc -o mandelbrot mandelbrot.cu `pkg-config --cflags --libs opencv4` -lcudart

Usage:
./mandelbrot <width> <height> <max_iterations>

Example:
./mandelbrot 1920 1920 1000

Output:
The fractal will be saved as Mandelbrot_CUDA.png in the current directory.
