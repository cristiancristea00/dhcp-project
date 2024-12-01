#include <format>
#include <iostream>

#include <oneapi/tbb.h>
#include <valgrind/callgrind.h>

#include "Utils.hpp"
#include "CosineGenerator.hpp"


auto main(int const argc, char * argv[]) -> int
{
    if (argc != ARGS_COUNT && argc != ARGS_COUNT_WITH_OPT)
    {
        std::cerr << std::format("Usage: {} <width> <height> <max_iterations> [grainsize_row] [grainsize_col]\n", argv[NAME]);

        return EXIT_FAILURE;
    }

    std::size_t const imageWidth{std::stoul(argv[WIDTH])};
    std::size_t const imageHeight{std::stoul(argv[HEIGHT])};
    Size const imageSize{imageHeight, imageWidth};

    std::size_t const maxIterations{std::stoul(argv[MAX_ITERATIONS])};

    std::size_t const grainsizeRow{argc == ARGS_COUNT_WITH_OPT ? std::stoul(argv[GRAINSIZE_ROW]) : 1U};
    std::size_t const grainsizeCol{argc == ARGS_COUNT_WITH_OPT ? std::stoul(argv[GRAINSIZE_COL]) : 1U};

    Size const grainsize{grainsizeRow, grainsizeCol};

    auto const numThreads{oneapi::tbb::info::default_concurrency()};

    std::cout << std::format("Generating Cosine fractal image with size {}×{} using {} iterations and grainsize {}×{} on {} threads\n", imageWidth, imageHeight, maxIterations, grainsizeRow, grainsizeCol, numThreads);

    CosineGenerator cosineGenerator{imageSize, maxIterations};
    TestSpeed(
        [&cosineGenerator, &grainsize]() -> void
        {
            CALLGRIND_ZERO_STATS;
            CALLGRIND_START_INSTRUMENTATION;

            cosineGenerator.render(grainsize);

            CALLGRIND_STOP_INSTRUMENTATION;
        }, "Cosine set"
    );
    cosineGenerator.save("Cosine.png");

    return EXIT_SUCCESS;
}
