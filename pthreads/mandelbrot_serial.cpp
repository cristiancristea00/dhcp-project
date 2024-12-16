#include <iostream>
#include <format>
#include <chrono>
#include <opencv2/opencv.hpp>

#define IMG_WIDTH      1000
#define IMG_HEIGHT     1000
#define MAX_ITERATIONS 1000

// Mandelbrot fractal parameters
#define TOP_LEFT_X     -2.0
#define TOP_LEFT_Y      1.2
#define BOTTOM_RIGHT_X  1.0
#define BOTTOM_RIGHT_Y -1.2
#define MAX_NORM  4
#define MAX_COLOR 255

uint8_t img[IMG_WIDTH*IMG_HEIGHT];

uint8_t mandelbrotGenerate(int x, int y)
{
    // Pixel to point mapped as complex number
    // same as starting point
    float sr = TOP_LEFT_X + (BOTTOM_RIGHT_X - TOP_LEFT_X) * x / IMG_WIDTH;
    float si = TOP_LEFT_Y - (TOP_LEFT_Y - BOTTOM_RIGHT_Y) * y / IMG_HEIGHT;
    float zr = 0.0f;
    float zi = 0.0f;
    float zr2, zi2;

    // Iterative process for Julia set: z = z^2 + s
    for(int iteration = 0; iteration < MAX_ITERATIONS; iteration++)
    {
        if((zr*zr + zi*zi) > MAX_NORM)
        {
            return static_cast<uint8_t>(MAX_COLOR * logf(iteration + 1) / logf(MAX_ITERATIONS));
        }

        zr2 = zr*zr - zi*zi + sr;
        zi2 = 2.0f*zr*zi + si;

        // Update point
        zr = zr2;
        zi = zi2; 
    }
    return 0;
}

int main()
{
    auto const start = std::chrono::high_resolution_clock::now();

    for(int row = 0; row < IMG_HEIGHT; row++)
    {
        for(int col = 0; col < IMG_WIDTH; col++)
        {
            // Generate pixel value
            img[row*IMG_WIDTH+col] = mandelbrotGenerate(col, row);
        }
    }

    // Save image
    cv::Mat greyImage(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, img);
    cv::Mat coloredImage;
    cv::applyColorMap(greyImage, coloredImage, cv::COLORMAP_MAGMA);
    cv::imwrite("Mandelbrot_fractal.png", coloredImage);

    auto const stop = std::chrono::high_resolution_clock::now();
    auto const differenceMs = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    auto const timeMs = differenceMs.count();
    std::cout << std::format("Time taken: {} ms\n", timeMs);

    return EXIT_SUCCESS;
}