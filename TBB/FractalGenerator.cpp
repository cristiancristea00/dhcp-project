#include "FractalGenerator.hpp"

#include <ranges>

#include <oneapi/tbb.h>
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
    using namespace oneapi::tbb;

    auto const range2d{blocked_range2d<std::size_t>{0U, imageSize.first, 0U, imageSize.second}};

    auto const process{
        [this](auto const & range) -> void
        {
            for (auto row{range.rows().begin()}; row < range.rows().end(); ++row)
            {
                for (auto col{range.cols().begin()}; col < range.cols().end(); ++col)
                {
                    image[(row * imageSize.second) + col] = generate(pixelToPoint({col, row}), maxIterations);
                }
            }
        }
    };

    parallel_for(range2d, process);

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
