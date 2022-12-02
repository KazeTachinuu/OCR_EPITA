# OneToNine

First defence for OCR Project at EPITA by four A2 students.

## How to use

## Solve a Sudoku Board

```shell
cd Solver/
make
./solver <boardFilename>
```

It will write the solution in the file `<boardFilename>.result`.

## The XOR Perceptron

```shell
cd Neural_Network/
make
./perceptron <nbEpochs> (or by default 10_000)
```

It will train the XOR perceptron and display the result of the training each 100 epochs and save the result in `xor.nn`.

## Image Processing

```shell
cd IMGProcess/
make
./rotate <imageFilename>
```

It will display the image in a window allowing you to rotate it with arrow keys and save the result with RETURN key in `<imageFilename>_rotated.bmp`.

## Display a Sudoku Board

```shell
cd Renderer/
make
./sudrend <boardFilename>
```

It will also create a file named `<boardFilename>.png` in the same directory.

## Generate a Sudoku Board

```shell
cd Solver/
make
./generate <boardFilename> <difficulty>
```

It will generate a pseudo-random board in the file `<boardFilename>.gen` with `<difficulty>` number of blank square.
