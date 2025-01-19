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
