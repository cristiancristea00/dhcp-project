# Fractal Image Generation

## Team Members

- [Cristian Cristea](@cristian.cristea) (TBB)
- [Antonio Lăzărescu](@antonio.lazarescu) (CUDA)
- [Franciso Câmara](@francisco.bessa) (OpenMP)
- [Mihaela Țigănoiu](@maria.tiganoiu) (pthreads)

## Implementations

- [TBB](TBB/README.md)
- [CUDA](CUDA/README.md)
- [OpenMP](OpenMP/README.md)
- [pthreads](pthreads/README.md)

## Dependencies

We use [OpenCV](https://github.com/opencv/opencv) for writing the images to disk. To install it on Ubuntu, run:

```bash
sudo apt-get install libopencv-dev
```

## Building

1. Clone the repository:

    ```bash
    git clone git@gitlab.cs.pub.ro:pp-2024/fractal-image-generation.git
    ```

    You will not be able to use the HTTPS protocol for cloning the repository, as logging into UNSTPB CS GitLab is provided by the SSO service. To use the SSH protocol, you need to [add an SSH key to your GitLab account](https://docs.gitlab.com/ee/user/ssh.html).

2. Change the working directory to the repository root:

    ```bash
    cd fractal-image-generation
    ```

3. Run the script to build and run one or all implementations:

    ```bash
    bash run.sh
    ```

## Examples

### Mandelbrot

![Mandelbrot](img/TBB-Mandelbrot-8000×6000-5000.png)

### Julia

![Julia](img/TBB-Julia-8000×6000-5000.png)

### Tricorn

![Tricorn](img/TBB-Tricorn-8000×6000-5000.png)

### Cosine

![Cosine](img/TBB-Cosine-8000×6000-5000.png)