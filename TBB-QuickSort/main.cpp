#include <algorithm>
#include <cstddef>
#include <execution>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

#include "QuickSort.hpp"
#include "Utils.hpp"


#define SIZE    ( 20'000'000U )
#define SEED    ( 42U )


auto main() -> int
{
    using number_type = std::size_t;

    std::mt19937 randomEngine;
    std::uniform_int_distribution distribution{std::numeric_limits<number_type>::min(), std::numeric_limits<number_type>::max()};
    auto const generator{
        [&]() -> number_type
        {
            return distribution(randomEngine);
        }
    };

    randomEngine.seed(SEED);
    std::vector<number_type> numbers1(SIZE);
    std::ranges::generate(numbers1, generator);

    randomEngine.seed(SEED);
    std::vector<number_type> numbers2(SIZE);
    std::ranges::generate(numbers2, generator);

    TestSpeed(
        [&]() -> void
        {
            std::ranges::sort(numbers1);
        },
        "Standard Template Library Sort"
    );

    TestSpeed(
        [&]() -> void
        {
            QuickSort(numbers2);
        },
        "QuickSort"
    );

    if (not CompareElements(numbers1, numbers2))
    {
        std::cerr << "The two ranges are NOT equal.\n";
        return EXIT_FAILURE;
    }

    std::cout << "The two ranges are equal.\n";

    randomEngine.seed(SEED);
    std::ranges::generate(numbers1, generator);

    randomEngine.seed(SEED);
    std::ranges::generate(numbers2, generator);

    TestSpeed(
        [&]() -> void
        {
            std::sort(std::execution::par_unseq, std::begin(numbers1), std::end(numbers1));
        },
        "Parallel Standard Template Library Sort"
    );

    TestSpeed(
        [&]() -> void
        {
            ParallelQuickSort(numbers2);
        },
        "Parallel QuickSort"
    );

    if (not CompareElements(numbers1, numbers2))
    {
        std::cerr << "The two ranges are NOT equal.\n";
        return EXIT_FAILURE;
    }

    std::cout << "The two ranges are equal.\n";
    return EXIT_SUCCESS;
}
