#!/bin/bash


HELP="Usage: $0 {Serial|TBB|CUDA|OpenMP|pthreads|all} {width} {height} {iterations}"

TYPE=$1
WIDTH=$2
HEIGHT=$3
ITERATIONS=$4

TYPES=(Serial TBB CUDA OpenMP pthreads)

FRACTALS=(Mandelbrot Julia Tricorn Cosine)


function build
{
    local type=$1

    echo "Building $type..."

    cd $type
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -B build
    cd build && ninja
    cd ../..
}

function run
{
    local type=$1

    echo "Running $type..."

    cd $type/build

    for fractal in ${FRACTALS[@]}; do
        ./$fractal $WIDTH $HEIGHT $ITERATIONS
    done

    cd ../..
}

function move
{
    local type=$1

    echo "Moving $type..."

    for fractal in ${FRACTALS[@]}; do
        mv $type/build/$fractal.png output/$type-$fractal-$WIDTH×$HEIGHT-$ITERATIONS.png
    done
}

# Create output directory
mkdir output 2> /dev/null

# Check if the number of arguments is correct
if [ $# -ne 4 ]; then
    echo $HELP
    exit 1
fi

# Process the build and run commands
case $TYPE in
    Serial)
        build Serial
        run Serial
        move Serial
        ;;
    TBB)
        build TBB
        run TBB
        move TBB
        ;;
    CUDA)
        build CUDA
        run CUDA
        move CUDA
        ;;
    OpenMP)
        build OpenMP
        run OpenMP
        move OpenMP
        ;;
    pthreads)
        build pthreads
        run pthreads
        move pthreads
        ;;
    all)
        for type in ${TYPES[@]}; do
            build $type
            run $type
            move $type
        done
        ;;
    *)
        echo $HELP
        exit 1
        ;;
esac