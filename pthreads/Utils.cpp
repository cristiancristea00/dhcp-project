#include "Utils.hpp"

#include <chrono>
#include <format>
#include <iostream>


auto TestSpeed(std::function<void()> const & function, std::string_view const message) -> void
{
    using namespace std::chrono;

    auto const start = high_resolution_clock::now();
    function();
    auto const stop = high_resolution_clock::now();

    auto const differenceMs = duration_cast<milliseconds>(stop - start);
    auto const timeMs = differenceMs.count();

    std::cout << std::format("Time taken for {} : {} ms\n", message, timeMs);
}
