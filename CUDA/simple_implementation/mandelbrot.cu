#include <iostream>
#include <opencv2/opencv.hpp>
#include <cuda_runtime.h>

#define MAX_COLOR 255 // Maximum color intensity for grayscale (used for coloring the fractal)


// Complex plane boundaries for Mandelbrot set
#define X_MIN -2.0f   // Minimum real      part (left   boundary of the complex plane)
#define X_MAX  1.0f   // Maximum real      part (right  boundary of the complex plane)
#define Y_MIN -1.2f   // Minimum imaginary part (bottom boundary of the complex plane)
#define Y_MAX  1.2f   // Maximum imaginary part (top    boundary of the complex plane)


// Struct to hold image size (width and height)
struct ImageSize
{
   int width;  // Image width  in pixels
   int height; // Image height in pixels
};


// CUDA device function: calculates the Mandelbrot value for a given pixel
__device__ uint8_t mandelbrot(uint32_t x, uint32_t y, ImageSize size, float x_min, float x_max, float y_min, float y_max, uint32_t max_iterations)
{
   // Map pixel coordinates (x, y) to complex plane coordinates (real and imaginary parts)
   // The resolution of the image is mapped to the range defined by X_MIN, X_MAX, Y_MIN, Y_MAX
   float real = x_min + (x_max - x_min) * x / size.width;  // x-coordinate mapped to the real      part of the complex number
   float imag = y_min + (y_max - y_min) * y / size.height; // y-coordinate mapped to the imaginary part of the complex number
   
   // Initialize z = 0 + 0i (starting point for the iteration process)
   float zr = 0.0f, zi = 0.0f;
   uint32_t iterations = 0; // Counter for the number of iterations before escaping
   
   // Iterative process for Mandelbrot set: z = z^2 + c
   // If the magnitude of z exceeds 2 (i.e., zr^2 + zi^2 > 4), the point escapes to infinity
   // and is considered outside the Mandelbrot set
   
   while(iterations < max_iterations)
   {
      float zr2 = zr * zr - zi * zi + real; // Real      part of the new z
      float zi2 = 2.0f * zr * zi + imag;    // Imaginary part of the new z
      
      zr = zr2; // Update the real      part of z
      zi = zi2; // Update the imaginary part of z
      
      // Escape condition: if the magnitude of z exceeds 2, the point escapes to infinity
      if(zr * zr + zi * zi > 4.0f)
      {
         break; // Exit the loop if the point escapes (it’s not part of the Mandelbrot set)
      }
      ++iterations; // Increment the iteration counter
   }
   
   // Color mapping:
   // If the point didn't escape (iterations == max_iterations), it’s part of the Mandelbrot set and is colored black (0)
   // If the point escaped, the color depends on how quickly it escaped (this is determined by the iteration count)
   if(iterations == max_iterations)
   {
      return 0; // Points inside the Mandelbrot set are black (color 0)
   }
   
   // Use a logarithmic scale for coloring based on the escape time (iteration count)
   // The longer the point takes to escape, the more intense its color will be
   return static_cast<uint8_t>(MAX_COLOR * logf(static_cast<float>(iterations + 1)) / logf(static_cast<float>(max_iterations)));
}


// CUDA kernel: This function runs on the GPU and calculates Mandelbrot fractal for each pixel
__global__ void generateMandelbrotKernel(uint8_t *image, ImageSize size, float x_min, float x_max, float y_min, float y_max, uint32_t max_iterations)
{
   // Calculate the pixel coordinates (x, y) for this thread in the image
   uint32_t x = blockIdx.x * blockDim.x + threadIdx.x; // x-coordinate of the pixel (thread index in x)
   uint32_t y = blockIdx.y * blockDim.y + threadIdx.y; // y-coordinate of the pixel (thread index in y)
   
   // Check if the current thread is within the image bounds (to avoid out-of-bounds memory access)
   if(x < size.width && y < size.height)
   {
      // Call the Mandelbrot device function to calculate the color for the pixel at (x, y)
      uint8_t color = mandelbrot(x, y, size, x_min, x_max, y_min, y_max, max_iterations);
      
      // Store the calculated color in the image buffer (image[y * width + x] represents the pixel location)
      image[y * size.width + x] = color;
   }
}


// Function to generate the Mandelbrot fractal on the GPU and save the result as an image
void generateFractal(ImageSize size, uint32_t max_iterations)
{
   uint8_t *d_image;                                         // Pointer to image data in device memory (GPU)
   uint8_t *h_image = new uint8_t[size.width * size.height]; // Pointer to image data in host   memory (CPU)
   
   // Step 1: Allocate memory on the GPU
   // The GPU will store the image as a 1D array of uint8_t values. Each value represents a pixel's grayscale intensity
   // The size of the memory allocated is proportional to the image's dimensions: width * height * size of each pixel (1 byte)
   cudaMalloc((void**)&d_image, size.width * size.height * sizeof(uint8_t));
   
   // Step 2: Define the CUDA thread block size
   // A block contains a 2D grid of threads. Here, we use 8x8 threads per block, for a total of 64 pixels processed in parallel per block
   // This block size is chosen to balance GPU memory usage and computational efficiency, though it can be adjusted for optimization
   dim3 blockSize(8, 8); // A block processes an 8x8 section of the image
   
   // Step 3: Calculate the grid size
   // The grid determines how many blocks are needed to cover the entire image
   // Each dimension of the grid is calculated by dividing the image dimensions by the block size, with rounding up to handle edge cases
   // The "+ blockSize.x - 1" ensures that the grid size is rounded up if the image dimensions aren't divisible by the block size
   dim3 gridSize(
      (size.width  + blockSize.x - 1) / blockSize.x, // Total blocks needed along the image width
      (size.height + blockSize.y - 1) / blockSize.y  // Total blocks needed along the image height
   );
   
   // Step 4: Launch the CUDA kernel
   // The generateMandelbrotKernel function is executed in parallel across all threads in the grid
   // Each thread computes the Mandelbrot set for one pixel in the image
   generateMandelbrotKernel<<<gridSize, blockSize>>>(d_image, size, X_MIN, X_MAX, Y_MIN, Y_MAX, max_iterations);
   
   // Step 5: Synchronize the device
   // Ensures that all threads finish their computation before moving to the next step
   // Without this, the program may proceed prematurely, potentially causing incorrect or incomplete results
   cudaDeviceSynchronize();
   
   // Check for any errors that occurred during the kernel execution
   cudaError_t err = cudaGetLastError();
   if(err != cudaSuccess)
   {
      std::cerr << "CUDA error: " << cudaGetErrorString(err) << std::endl;
   }
   
   // Copy the resulting image from GPU memory back to CPU memory (host)
   cudaMemcpy(h_image, d_image, size.width * size.height * sizeof(uint8_t), cudaMemcpyDeviceToHost);
   
   // Create an OpenCV matrix (image) from the raw pixel data (grayscale image)
   cv::Mat img(size.height, size.width, CV_8UC1, h_image);
   
   // Apply a colormap (e.g., MAGMA) to enhance the visualization of the fractal
   cv::Mat coloredImage;
   cv::applyColorMap(img, coloredImage, cv::COLORMAP_MAGMA);
   
   // Save the generated fractal image as a PNG file
   cv::imwrite("Mandelbrot_CUDA.png", coloredImage);
   
   // Free the allocated memory on both GPU and CPU
   cudaFree(d_image);
   delete[] h_image;
}


int main(int argc, char *argv[])
{
   // Check if the correct number of arguments is passed to the program
   if(argc != 4)
   {
      std::cerr << "Usage: " << argv[0] << " <width> <height> <max_iterations> " << std::endl;
      return EXIT_FAILURE;
   }
   
   // Parse command-line arguments for image width, height, and maximum iterations
   int width  = std::stoi(argv[1]); // Image width  in pixels
   int height = std::stoi(argv[2]); // Image height in pixels
   uint32_t max_iterations = std::stoi(argv[3]); // Maximum iterations for Mandelbrot calculation
   
   // Store image size in a structure
   ImageSize size = {width, height};
   
   // Call the function to generate the Mandelbrot fractal on the GPU and save the result as an image
   generateFractal(size, max_iterations);
   
   return EXIT_SUCCESS;
}