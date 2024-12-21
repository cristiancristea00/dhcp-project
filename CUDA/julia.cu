#include <iostream>
#include <opencv2/opencv.hpp>
#include <cuda_runtime.h>

#define MAX_COLOR 255 // Maximum color intensity for grayscale (used for coloring the fractal)


// Struct to hold image size (width and height)
struct ImageSize
{
   int width;  // Image width  in pixels
   int height; // Image height in pixels
};


// CUDA device function: calculates the Julia value for a given pixel
__device__ uint8_t julia(uint32_t x, uint32_t y, ImageSize size, float c_real, float c_imag, uint32_t max_iterations)
{
   // Map pixel coordinates (x, y) to complex plane coordinates (real and imaginary parts)
   // The resolution of the image is mapped to the range defined by X_MIN, X_MAX, Y_MIN, Y_MAX
   float real = (x - size.width  / 2.0f) * 4.0f / size.width;  // x-coordinate mapped to the real      part of the complex number
   float imag = (y - size.height / 2.0f) * 4.0f / size.height; // y-coordinate mapped to the imaginary part of the complex number
   
   // Initialize z = real + imag * i (starting point for the iteration process)
   float zr = real, zi = imag;
   uint32_t iterations = 0; // Counter for the number of iterations before escaping
   
   // Iterative process for Julia set: z = z^2 + c
   // If the magnitude of z exceeds 2 (i.e., zr^2 + zi^2 > 4), the point escapes to infinity
   // and is considered outside the Julia set
   
   while(iterations < max_iterations)
   {
      float zr2 = zr * zr - zi * zi + c_real; // Real      part of the new z
      float zi2 = 2.0f * zr * zi + c_imag;    // Imaginary part of the new z
      
      zr = zr2; // Update the real      part of z
      zi = zi2; // Update the imaginary part of z
      
      // Escape condition: if the magnitude of z exceeds 2, the point escapes to infinity
      if(zr * zr + zi * zi > 4.0f)
      {
         break; // Exit the loop if the point escapes (it’s not part of the Julia set)
      }
      ++iterations; // Increment the iteration counter
   }
   
   // Color mapping:
   // If the point didn't escape (iterations == max_iterations), it’s part of the Julia set and is colored black (0)
   // If the point escaped, the color depends on how quickly it escaped (this is determined by the iteration count)
   if(iterations == max_iterations)
   {
      return 0; // Points inside the Julia set are black (color 0)
   }
   
   // Use a logarithmic scale for coloring based on the escape time (iteration count)
   // The longer the point takes to escape, the more intense its color will be
   return static_cast<uint8_t>(MAX_COLOR * logf(static_cast<float>(iterations + 1)) / logf(static_cast<float>(max_iterations)));
}


// CUDA kernel: This function runs on the GPU and calculates Julia fractal for each pixel
__global__ void generateJuliaKernel(uint8_t *image, ImageSize size, float c_real, float c_imag, uint32_t max_iterations)
{
   // Calculate the pixel coordinates (x, y) for this thread in the image
   uint32_t x = blockIdx.x * blockDim.x + threadIdx.x; // x-coordinate of the pixel (thread index in x)
   uint32_t y = blockIdx.y * blockDim.y + threadIdx.y; // y-coordinate of the pixel (thread index in y)
   
   // Check if the current thread is within the image bounds (to avoid out-of-bounds memory access)
   if(x < size.width && y < size.height)
   {
      // Call the Julia device function to calculate the color for the pixel at (x, y)
      uint8_t color = julia(x, y, size, c_real, c_imag, max_iterations);
      
      // Store the calculated color in the image buffer (image[y * width + x] represents the pixel location)
      image[y * size.width + x] = color;
   }
}


// Function to generate the Julia fractal on the GPU and save the result as an image
void generateFractal(ImageSize size, float c_real, float c_imag, uint32_t max_iterations)
{
   // Declare and create CUDA events to measure GPU execution time
   cudaEvent_t start, stop;
   cudaEventCreate(&start);
   cudaEventCreate(&stop);
   
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
   
   std::cout << "Total threads per block = 8x8 = 64 threads\n";
   std::cout << "Each thread is responsible for computing one pixel of the image, meaning each block processes a 64 pixel region in parallel.\n\n";
   std::cout << "The grid is a 2D arrangement of blocks, and its size is calculated to cover the entire image.\n";
   std::cout << "The number of blocks required for the width and height of the image is computed as follows:\n";
   std::cout << "Total blocks needed along the image width:  " << (size.width  + blockSize.x - 1) / blockSize.x << std::endl;
   std::cout << "Total blocks needed along the image height: " << (size.height + blockSize.y - 1) / blockSize.y << std::endl << std::endl;
   
   // Record the start time for GPU execution using cudaEvent
   cudaEventRecord(start);
   
   // Step 4: Launch the CUDA kernel
   // The generateJuliaKernel function is executed in parallel across all threads in the grid
   // Each thread computes the Julia set for one pixel in the image
   generateJuliaKernel<<<gridSize, blockSize>>>(d_image, size, c_real, c_imag, max_iterations);
   
   // Step 5: Synchronize the device
   // Ensures that all threads finish their computation before moving to the next step
   // Without this, the program may proceed prematurely, potentially causing incorrect or incomplete results
   cudaDeviceSynchronize();
   
   // Record the stop time for GPU execution using cudaEvent
   cudaEventRecord(stop);
   // Synchronize to ensure all GPU operations are complete before measuring time
   cudaEventSynchronize(stop);
   
   float elapsed_cuda = 0;
   cudaEventElapsedTime(&elapsed_cuda, start, stop);
   std::cout << "Execution time using cudaEvent:   " << elapsed_cuda << " ms" << std::endl;
   
   cudaEventDestroy(start);
   cudaEventDestroy(stop);
   
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
   cv::imwrite("img_fractals/CUDA_julia.png", coloredImage);
   
   // Free the allocated memory on both GPU and CPU
   cudaFree(d_image);
   delete[] h_image;
}


//*****************************************************************************
// a) cudaEvent
//    Measures the actual execution time of the kernel on the GPU.
//    It excludes data transfer times between the CPU (host) and GPU (device)
//    as well as other initialization operations.
//    It is precise because it directly synchronizes with the GPU.
//
// b) std::chrono
//    Measures the total execution time, including:
//    - Kernel launch (which is typically asynchronous).
//    - Data transfer times between the CPU and GPU.
//    - Any other operations running on the CPU, such as CUDA API calls.
//    - Possible delays caused by synchronization between the CPU and GPU
//    (e.g., cudaDeviceSynchronize() waits for all GPU operations to complete).
//*****************************************************************************


int main(int argc, char *argv[])
{
   auto start_chrono = std::chrono::high_resolution_clock::now();
   
   // Check if the correct number of arguments is passed to the program
   if(argc != 6)
   {
      std::cerr << "Usage: " << argv[0] << " <width> <height> <max_iterations> <c_real> <c_imag>" << std::endl;
      return EXIT_FAILURE;
   }
   
   // Parse command-line arguments for image width, height, and maximum iterations
   int width  = std::stoi(argv[1]); // Image width  in pixels
   int height = std::stoi(argv[2]); // Image height in pixels
   uint32_t max_iterations = std::stoi(argv[3]); // Maximum iterations for Julia calculation
   float c_real = std::stof(argv[4]); // Real      part of constant C for Julia
   float c_imag = std::stof(argv[5]); // Imaginary part of constant C for Julia
   
   // Store image size in a structure
   ImageSize size = {width, height};
   
   // Call the function to generate the Julia fractal on the GPU and save the result as an image
   generateFractal(size, c_real, c_imag, max_iterations);
   
   auto stop_chrono = std::chrono::high_resolution_clock::now();
   
   std::chrono::duration<double, std::milli> elapsed_chrono = stop_chrono - start_chrono;
   std::cout << "Execution time using std::chrono: " << elapsed_chrono.count() << " ms" << std::endl;
   
   return EXIT_SUCCESS;
}