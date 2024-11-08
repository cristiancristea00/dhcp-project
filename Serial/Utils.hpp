#pragma once

#include <complex>
#include <cstdint>
#include <functional>


#define ARGS_COUNT    ( 4U )


enum Arguments : std::uint8_t
{
    NAME           = 0x00,
    WIDTH          = 0x01,
    HEIGHT         = 0x02,
    MAX_ITERATIONS = 0x03,
};


using Pixel = std::pair<std::size_t, std::size_t>;
using Size = std::pair<std::size_t, std::size_t>;
using Point = std::complex<float>;


auto TestSpeed(std::function<void()> const & function, std::string_view const message) -> void;
