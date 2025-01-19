# Subset View

## Usage

### Testing

To run all tests:

```shell
mkdir build
cd build
cmake ..
make
ctest
```

You can use test/gen.py for generating new tests

To run a single test use:

```shell
./build/subset test/input1.txt test/answer1.txt
```

### Simple usage

Example of getting subsets of a custom set use:

```shell
./build/subset 3
1 2 3
```

### Old standart buildng

This subset_view requires C++20 std or C++23 std, but it uses different interfaces.

For building with C++20 without errors use:

```shell
cmake -DOLD_STD ..
```