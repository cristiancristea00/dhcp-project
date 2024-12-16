#include <iostream>
#include <format>
#include <chrono>
#include <opencv2/opencv.hpp>

#include <pthread.h>
#define NUM_THREADS 5
#define IMG_WIDTH      1000
#define IMG_HEIGHT     1000
#define MAX_ITERATIONS 1000

// Julia fractal parameters
#define TOP_LEFT_X     -2.0
#define TOP_LEFT_Y      1.6
#define BOTTOM_RIGHT_X  2.0
#define BOTTOM_RIGHT_Y -1.6
#define MAX_NORM  4
#define MAX_COLOR 255

uint8_t img[IMG_WIDTH*IMG_HEIGHT];
int startPoint[] = {0, 380000, 485000, 515000, 620000};
int stopPoint[] = {379999, 484999, 514999, 619999, 999999};

uint8_t tricornGenerate(int x, int y)
{
    // Pixel to point mapped as complex number
    // same as starting point
    float sr = TOP_LEFT_X + (BOTTOM_RIGHT_X - TOP_LEFT_X) * x / IMG_WIDTH;
    float si = TOP_LEFT_Y - (TOP_LEFT_Y - BOTTOM_RIGHT_Y) * y / IMG_HEIGHT;
    float zr = 0.0f;
    float zi = 0.0f;
    float zr2, zi2;

    // Iterative process for Julia set: z = conj(z)^2 + s
    for(int iteration = 0; iteration < MAX_ITERATIONS; iteration++)
    {
        if((zr*zr + zi*zi) > MAX_NORM)
        {
            return static_cast<uint8_t>(MAX_COLOR * logf(iteration + 1) / logf(MAX_ITERATIONS));
        }

        zr2 = zr*zr - zi*zi + sr;
        zi2 = -2.0f*zr*zi + si;

        // Update point
        zr = zr2;
        zi = zi2; 
    }
    return 0;
}

void *thread_func(void *arg)
{
    int id = *(int*) arg;
    int col, row;

    for(int i = startPoint[id]; i <= stopPoint[id]; i++)
    {
        col = i % IMG_WIDTH;
        row = i / IMG_WIDTH;
        // Generate pixel value
        img[i] = tricornGenerate(col, row);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t work_thread[NUM_THREADS];
    int arguments[NUM_THREADS];
    int id;
    void *status;

    auto const start = std::chrono::high_resolution_clock::now();

    for(id = 0; id < NUM_THREADS; id++)
    {
        arguments[id] = id;
        // Create thread to generate fractal
        pthread_create(&work_thread[id], NULL, thread_func, (void*) &arguments[id]);
    }
    for(id = 0; id < NUM_THREADS; id++)
    {
        pthread_join(work_thread[id], &status);
    }

    // Save image
    cv::Mat greyImage(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, img);
    cv::Mat coloredImage;
    cv::applyColorMap(greyImage, coloredImage, cv::COLORMAP_MAGMA);
    cv::imwrite("Tricorn_fractal.png", coloredImage);

    auto const stop = std::chrono::high_resolution_clock::now();
    auto const differenceMs = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    auto const timeMs = differenceMs.count();
    std::cout << std::format("Time taken: {} ms\n", timeMs);

    return EXIT_SUCCESS;
}