#include <format>
#include <iostream>

#include <oneapi/tbb.h>

#include "Utils.hpp"
#include "JuliaGenerator.hpp"


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

    auto const numThreads{oneapi::tbb::info::default_concurrency()};

    std::cout << std::format("Generating Julia fractal image with size {}×{} using {} iterations on {} threads...\n", imageWidth, imageHeight, maxIterations, numThreads);

    JuliaGenerator juliaGenerator{imageSize, maxIterations};
    TestSpeed(
        [&juliaGenerator]() -> void
        {
            juliaGenerator.render();
        }, "Julia set"
    );
    juliaGenerator.save("Julia.png");

    return EXIT_SUCCESS;
}
