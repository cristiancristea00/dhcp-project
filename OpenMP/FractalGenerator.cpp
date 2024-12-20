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
    auto const width{bottomRight.real() - topLeft.real()};
    auto const height{topLeft.imag() - bottomRight.imag()};

    auto const row{topLeft.real() + (static_cast<decltype(width)>(pixel.first) * width / static_cast<decltype(width)>(imageSize.second))};
    auto const col{topLeft.imag() - (static_cast<decltype(height)>(pixel.second) * height / static_cast<decltype(height)>(imageSize.first))};

    return {row, col};
}

auto FractalGenerator::render() -> void
{
    #pragma omp parallel for num_threads(208) schedule(dynamic, 1)
    for (std::size_t row = 0; row < imageSize.first; ++row)
    {
        for (std::size_t col = 0; col < imageSize.second; ++col)
        {
            image[(row * imageSize.second) + col] = generate(pixelToPoint({col, row}), maxIterations);
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

auto FractalGenerator::getMaxNorm() const -> double
{
    auto const radius{getRadius()};
    return radius * radius;
}
