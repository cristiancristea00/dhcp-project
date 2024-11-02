#pragma once

#include <cstddef>
#include <functional>
#include <iterator>
#include <ranges>
#include <utility>

#include <oneapi/tbb/parallel_invoke.h>


template <std::random_access_iterator Begin, std::sentinel_for<Begin> End, typename Comparator> requires std::sortable<Begin, Comparator>
static constexpr auto QuickSortHelper(Begin const begin, End const end, Comparator const comparator) -> std::pair<End, Begin>
{
    auto const size = std::distance(begin, end);
    auto const pivot = *std::next(begin, size / 2);

    auto const middle1 = std::partition(
        begin, end, [&](auto const & element)
        {
            return std::invoke(comparator, element, pivot);
        }
    );

    auto const middle2 = std::partition(
        middle1, end, [&](auto const & element)
        {
            return not std::invoke(comparator, pivot, element);
        }
    );

    return {middle1, middle2};
}

template <std::random_access_iterator Begin, std::sentinel_for<Begin> End, typename Comparator = std::ranges::less> requires std::sortable<Begin, Comparator>
constexpr auto QuickSort(Begin const begin, End const end, Comparator const comparator = Comparator{}) -> void
{
    if (begin == end)
    {
        return;
    }

    auto const [middle1, middle2] = QuickSortHelper(begin, end, comparator);

    QuickSort(begin, middle1, comparator);
    QuickSort(middle2, end, comparator);
}

template <std::ranges::random_access_range Range, typename Comparator = std::ranges::less> requires std::sortable<std::ranges::iterator_t<Range>, Comparator>
constexpr auto QuickSort(Range && range, Comparator const comparator = Comparator{}) -> void
{
    QuickSort(std::ranges::begin(range), std::ranges::end(range), comparator);
}

template <std::random_access_iterator Begin, std::sentinel_for<Begin> End, typename Comparator = std::ranges::less> requires std::sortable<Begin, Comparator>
constexpr auto ParallelQuickSort(Begin const begin, End const end, Comparator const comparator = Comparator{}, std::size_t const serialThreshold = 500) -> void
{
    if (begin == end)
    {
        return;
    }

    auto const size = std::distance(begin, end);

    if (size <= serialThreshold)
    {
        QuickSort(begin, end, comparator);
    }
    else
    {
        auto const [middle1, middle2] = QuickSortHelper(begin, end, comparator);

        oneapi::tbb::parallel_invoke(
            [&]()-> void
            {
                ParallelQuickSort(begin, middle1, comparator, serialThreshold);
            },
            [&]() -> void
            {
                ParallelQuickSort(middle2, end, comparator, serialThreshold);
            }
        );
    }
}

template <std::ranges::random_access_range Range, typename Comparator = std::ranges::less> requires std::sortable<std::ranges::iterator_t<Range>, Comparator>
constexpr auto ParallelQuickSort(Range && range, Comparator const comparator = Comparator{}, std::size_t const serialThreshold = 500) -> void
{
    ParallelQuickSort(std::ranges::begin(range), std::ranges::end(range), comparator, serialThreshold);
}
