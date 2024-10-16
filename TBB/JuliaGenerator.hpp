#pragma once

#include "FractalGenerator.hpp"


class JuliaGenerator final : public FractalGenerator
{
public:
    JuliaGenerator(Size const & imageSize, std::size_t const maxIterations);

private:
    [[nodiscard]] auto generate(Point const & startPoint, std::size_t const maxIterations) const -> std::uint8_t override;

    [[nodiscard]] constexpr auto getRadius() const -> double override;

    static constexpr Point topLeft{-1.6, 1.2};
    static constexpr Point bottomRight{1.6, -1.2};

    static constexpr Point cPoint{-0.7, 0.27015};
};
