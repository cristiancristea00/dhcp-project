#include "JuliaGenerator.hpp"


JuliaGenerator::JuliaGenerator(Size const & imageSize, std::size_t const maxIterations) : FractalGenerator{imageSize, TOP_LEFT, BOTTOM_RIGHT, maxIterations} { }

auto JuliaGenerator::generate(Point const & startPoint) const -> std::uint8_t
{
    static const auto LOG_MAX_ITERATIONS{std::log(maxIterations)};

    auto point{startPoint};

    for (std::size_t iteration = 0; iteration < maxIterations; ++iteration)
    {
        if (std::abs(point) > RADIUS)
        {
            return static_cast<std::uint8_t>(MAX_COLOR * std::log(iteration + 1) / LOG_MAX_ITERATIONS);
        }

        point = point * point + C_POINT;
    }

    return 0;
}
