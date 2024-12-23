#pragma once

#include "FractalGenerator.hpp"


class TricornGenerator final : public FractalGenerator
{
public:
    TricornGenerator(Size const & imageSize, std::size_t const maxIterations);

private:
    [[nodiscard]] auto generate(Point const & startPoint, std::size_t const maxIterations) const -> std::uint8_t override;

    [[nodiscard]] constexpr auto getRadius() const -> double override;

    static constexpr Point topLeft{-2.0, 1.6};
    static constexpr Point bottomRight{2.0, -1.6};
};
