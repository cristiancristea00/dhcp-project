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
    using namespace oneapi::tbb;

    static affinity_partitioner affinityPartitioner;

    auto const range2d{blocked_range2d<std::size_t>{0U, imageSize.first, 0U, imageSize.second}};

    auto const process{
        [this](auto const & range) -> void
        {
            for (auto row{range.rows().begin()}; row < range.rows().end(); ++row)
            {
                for (auto col{range.cols().begin()}; col < range.cols().end(); ++col)
                {
                    image[(row * imageSize.second) + col] = generate(pixelToPoint({col, row}));
                }
            }
        }
    };

    parallel_for(range2d, process, affinityPartitioner);

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
