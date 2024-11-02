#pragma once

#include <execution>
#include <functional>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string_view>


auto TestSpeed(std::function<void()> const & function, std::string_view const message) -> void;

template <std::random_access_iterator Begin, std::sentinel_for<Begin> End>
constexpr auto CompareElements(Begin const begin1, End const end1, Begin const begin2, End const end2) -> bool
{
    auto const size1 = std::distance(begin1, end1);
    auto const size2 = std::distance(begin2, end2);

    if (size1 != size2)
    {
        return false;
    }

    return std::equal(std::execution::par_unseq, begin1, end1, begin2, end2);
}

template <std::ranges::random_access_range Range>
constexpr auto CompareElements(Range const range1, Range const range2) -> bool
{
    return CompareElements(std::begin(range1), std::end(range1), std::begin(range2), std::end(range2));
}

template <std::random_access_iterator Begin, std::sentinel_for<Begin> End>
constexpr auto PrintElements(Begin const begin, End const end) -> void
{
    std::cout << "| ";

    for (auto current{begin}; current != end; ++current)
    {
        std::cout << *current << " | ";
    }

    std::cout << '\n';
}

template <std::ranges::random_access_range Range>
constexpr auto PrintElements(Range const range) -> void
{
    PrintElements(std::begin(range), std::end(range));
}
