#pragma once

#include "FractalGenerator.hpp"


class CosineGenerator final : public FractalGenerator
{
public:
    CosineGenerator(Size const & imageSize, std::size_t const maxIterations);

private:
    [[nodiscard]] auto generate(Point const & startPoint, std::size_t const maxIterations) const -> std::uint8_t override;

    static constexpr Point TOP_LEFT{-2.0, 2.0};
    static constexpr Point BOTTOM_RIGHT{5.0, -2.0};

    static constexpr double RADIUS{10.0 * std::numbers::pi};
};
