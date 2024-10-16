#pragma once

#include "FractalGenerator.hpp"


class CosineGenerator final : public FractalGenerator
{
public:
    CosineGenerator(Size const & imageSize, std::size_t const maxIterations);

private:
    [[nodiscard]] auto generate(Point const & startPoint, std::size_t const maxIterations) const -> std::uint8_t override;

    [[nodiscard]] constexpr auto getRadius() const -> double override;

    static constexpr Point topLeft{-2.0, 2.0};
    static constexpr Point bottomRight{5.0, -2.0};
};
