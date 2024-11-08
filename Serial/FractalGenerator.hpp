#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "Utils.hpp"


class FractalGenerator
{
public:
    FractalGenerator(Size const & imageSize, Point const & topLeft, Point const & bottomRight, std::size_t const maxIterations);

    virtual ~FractalGenerator() = default;

    auto render() -> void;

    auto save(std::string_view const & filename) -> void;

protected:
    virtual auto generate(Point const & startPoint) const -> std::uint8_t = 0;

    [[gnu::always_inline]] inline auto pixelToPoint(Pixel const & pixel) const -> Point;

    bool isRendered{false};

    Size const imageSize;

    Point const topLeft;
    Point const bottomRight;

    std::size_t const maxIterations;

    std::vector<std::uint8_t> image;

    static constexpr std::size_t MAX_COLOR{255};
};
