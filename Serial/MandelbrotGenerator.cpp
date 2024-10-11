#include "MandelbrotGenerator.hpp"


MandelbrotGenerator::MandelbrotGenerator(Size const & imageSize, std::size_t const maxIterations) : FractalGenerator{imageSize, topLeft, bottomRight, maxIterations} { }

auto MandelbrotGenerator::generate(Point const & startPoint, std::size_t const maxIterations) const -> std::uint8_t
{
    static constexpr double MAX_NORM = 4.0;

    Point point{0.0, 0.0};

    for (std::size_t iteration = 0; iteration < maxIterations; ++iteration)
    {
        if (std::norm(point) > MAX_NORM)
        {
            return static_cast<std::uint8_t>(MAX_COLOR * std::log(iteration + 1) / std::log(maxIterations));
        }

        point = point * point + startPoint;
    }

    return 0;
}
