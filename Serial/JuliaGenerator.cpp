#include "JuliaGenerator.hpp"


JuliaGenerator::JuliaGenerator(Size const & imageSize, std::size_t const maxIterations) : FractalGenerator{imageSize, topLeft, bottomRight, maxIterations} { }

auto JuliaGenerator::generate(Point const & startPoint, std::size_t const maxIterations) const -> std::uint8_t
{
    auto point{startPoint};

    for (std::size_t iteration = 0; iteration < maxIterations; ++iteration)
    {
        if (std::norm(point) > getMaxNorm())
        {
            return static_cast<std::uint8_t>(MAX_COLOR * std::log(iteration + 1) / std::log(maxIterations));
        }

        point = point * point + cPoint;
    }

    return 0;
}

constexpr auto JuliaGenerator::getRadius() const -> double
{
    return 2.0;
}
