#pragma once

#include <numbers>

#include "FractalGenerator.hpp"


class CosineGenerator final : public FractalGenerator
{
public:
    CosineGenerator(Size const & imageSize, std::size_t const maxIterations);

private:
    [[nodiscard]] auto generate(Point const & startPoint) const -> std::uint8_t override;

    template <std::floating_point T>
    static constexpr auto cosine(std::complex<T> const & value) -> std::complex<T>;

    static constexpr Point TOP_LEFT{-2.0, 2.0};
    static constexpr Point BOTTOM_RIGHT{5.0, -2.0};

    static constexpr float RADIUS{10.0 * std::numbers::pi};
};
