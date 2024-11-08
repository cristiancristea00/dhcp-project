#include "CosineGenerator.hpp"


CosineGenerator::CosineGenerator(Size const & imageSize, std::size_t const maxIterations) : FractalGenerator{imageSize, TOP_LEFT, BOTTOM_RIGHT, maxIterations} { }

auto CosineGenerator::generate(Point const & startPoint) const -> std::uint8_t
{
    static const auto LOG_MAX_ITERATIONS{std::log(maxIterations)};

    Point point{0.0, 0.0};

    for (std::size_t iteration = 0; iteration < maxIterations; ++iteration)
    {
        if (std::abs(point) > RADIUS)
        {
            return static_cast<std::uint8_t>(MAX_COLOR * std::log(iteration + 1) / LOG_MAX_ITERATIONS);
        }

        point = cosine(point) + startPoint;
    }

    return 0;
}

template <std::floating_point T>
constexpr auto CosineGenerator::cosine(std::complex<T> const & value) -> std::complex<T>
{
    return {std::cos(value.real()) * std::cosh(value.imag()), -std::sin(value.real()) * std::sinh(value.imag())};
}
