#include "FractalGenerator.hpp"

#include <ranges>

#include <opencv2/opencv.hpp>


FractalGenerator::FractalGenerator(Size const & imageSize, Point const & topLeft, Point const & bottomRight, std::size_t const maxIterations) : imageSize{imageSize}, topLeft{topLeft},
    bottomRight{bottomRight}, maxIterations{maxIterations}
{
    image.resize(imageSize.first * imageSize.second);
}

auto FractalGenerator::pixelToPoint(Pixel const & pixel) const -> Point
{
    static auto const WIDTH{bottomRight.real() - topLeft.real()};
    static auto const HEIGHT{topLeft.imag() - bottomRight.imag()};

    auto const row{topLeft.real() + (static_cast<decltype(WIDTH)>(pixel.first) * WIDTH / static_cast<decltype(WIDTH)>(imageSize.second))};
    auto const col{topLeft.imag() - (static_cast<decltype(HEIGHT)>(pixel.second) * HEIGHT / static_cast<decltype(HEIGHT)>(imageSize.first))};

    return {row, col};
}

auto FractalGenerator::render() -> void
{
    for (auto const & row : std::views::iota(0U, imageSize.first))
    {
        for (auto const & col : std::views::iota(0U, imageSize.second))
        {
            image[(row * imageSize.second) + col] = generate(pixelToPoint({col, row}));
        }
    }

    isRendered = true;
}

auto FractalGenerator::save(std::string_view const & filename) -> void
{
    using namespace cv;

    if (!isRendered)
    {
        throw std::runtime_error("The fractal has not been rendered yet.");
    }

    Mat const greyImage(static_cast<int>(imageSize.first), static_cast<int>(imageSize.second), CV_8UC1, image.data());
    Mat coloredImage;
    applyColorMap(greyImage, coloredImage, COLORMAP_MAGMA);
    imwrite(filename.data(), coloredImage);
}
