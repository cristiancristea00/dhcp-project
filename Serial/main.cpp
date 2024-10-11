#include <format>
#include <iostream>

#include "JuliaGenerator.hpp"
#include "MandelbrotGenerator.hpp"


#define ARGS_COUNT    ( 4 )


enum Arguments : std::uint8_t
{
    NAME           = 0x00,
    WIDTH          = 0x01,
    HEIGHT         = 0x02,
    MAX_ITERATIONS = 0x03,
};


auto main(int const argc, char * argv[]) -> int
{
    if (argc != ARGS_COUNT)
    {
        std::cerr << std::format("Usage: {} <width> <height> <max_iterations>\n", argv[NAME]);

        return EXIT_FAILURE;
    }

    std::size_t const imageWidth{std::stoul(argv[WIDTH])};
    std::size_t const imageHeight{std::stoul(argv[HEIGHT])};
    Size const imageSize{imageHeight, imageWidth};

    std::size_t const maxIterations{std::stoul(argv[MAX_ITERATIONS])};

    std::cout << std::format("Generating fractals images with size {}x{} using {} iterations\n", imageWidth, imageHeight, maxIterations);

    MandelbrotGenerator mandelbrotGenerator{imageSize, maxIterations};
    std::cout << "Rendering Mandelbrot set...\n";
    TestSpeed([&mandelbrotGenerator]() -> void { mandelbrotGenerator.render(); }, "Mandelbrot set");
    mandelbrotGenerator.save("mandelbrot.png");

    std::cout << "Rendering Julia set...\n";
    JuliaGenerator juliaGenerator{imageSize, maxIterations};
    TestSpeed([&juliaGenerator]() -> void { juliaGenerator.render(); }, "Julia set");
    juliaGenerator.save("julia.png");

    return EXIT_SUCCESS;
}
