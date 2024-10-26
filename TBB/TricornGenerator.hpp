#pragma once

#include "FractalGenerator.hpp"


class TricornGenerator final : public FractalGenerator
{
public:
    TricornGenerator(Size const & imageSize, std::size_t const maxIterations);

private:
    [[nodiscard]] auto generate(Point const & startPoint) const -> std::uint8_t override;

    static constexpr Point TOP_LEFT{-2.0, 1.6};
    static constexpr Point BOTTOM_RIGHT{2.0, -1.6};

    static constexpr float RADIUS{2.0};
};
