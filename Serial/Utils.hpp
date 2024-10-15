#pragma once

#include <complex>
#include <functional>


using Pixel = std::pair<std::size_t, std::size_t>;
using Size = std::pair<std::size_t, std::size_t>;
using Point = std::complex<double>;


auto TestSpeed(std::function<void()> const & function, std::string_view const message) -> void;
