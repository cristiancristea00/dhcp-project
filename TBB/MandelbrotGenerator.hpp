#pragma once

#include "FractalGenerator.hpp"


class MandelbrotGenerator final : public FractalGenerator
{
public:
    MandelbrotGenerator(Size const & imageSize, std::size_t const maxIterations);

private:
    [[nodiscard]] auto generate(Point const & startPoint, std::size_t const maxIterations) const -> std::uint8_t override;

    static constexpr Point TOP_LEFT{-2.0, 1.2};
    static constexpr Point BOTTOM_RIGHT{1.0, -1.2};

    static constexpr double RADIUS{2.0};
};
