#include <format>
#include <iostream>

#include <oneapi/tbb.h>
#include <valgrind/callgrind.h>

#include "Utils.hpp"
#include "CosineGenerator.hpp"


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

    std::cout << std::format("Generating Cosine fractal image with size {}×{} using {} iterations on {} threads...\n", imageWidth, imageHeight, maxIterations, numThreads);

    CosineGenerator cosineGenerator{imageSize, maxIterations};
    TestSpeed(
        [&cosineGenerator]() -> void
        {
            CALLGRIND_ZERO_STATS;
            CALLGRIND_START_INSTRUMENTATION;

            cosineGenerator.render();

            CALLGRIND_STOP_INSTRUMENTATION;
        }, "Cosine set"
    );
    cosineGenerator.save("Cosine.png");

    return EXIT_SUCCESS;
}
