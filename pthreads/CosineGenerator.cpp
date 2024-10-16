#include "CosineGenerator.hpp"


CosineGenerator::CosineGenerator(Size const & imageSize, std::size_t const maxIterations) : FractalGenerator{imageSize, topLeft, bottomRight, maxIterations} { }

auto CosineGenerator::generate(Point const & startPoint, std::size_t const maxIterations) const -> std::uint8_t
{
    Point point{0.0, 0.0};

    for (std::size_t iteration = 0; iteration < maxIterations; ++iteration)
    {
        if (std::norm(point) > getMaxNorm())
        {
            return static_cast<std::uint8_t>(MAX_COLOR * std::log(iteration + 1) / std::log(maxIterations));
        }

        point = std::cos(point) + startPoint;
    }

    return 0;
}

constexpr auto CosineGenerator::getRadius() const -> double
{
    return 10.0 * std::numbers::pi;
}
