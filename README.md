# universal-ppm-image-filter
A program that reads an image, applies a filter kernel, and outputs a new image.

## Usage
The program shall provide the following command-line interface:
```sh
imfilter.exe INPUT_FILE OUTPUT_FILE KERNEL
```
* `INPUT_FILE` is the filename for the input .ppm image.
* `OUTPUT_FILE` is the filename for the output .ppm image.
* `KERNEL` is the filter to be applied to the input image. This is a 3-by-3 matrix of floating-point coefficients. It is written from top left to bottom right as a series of 9 numbers separated by ',' (e.g., "-1,-1,-1,-1,8,-1,-1,-1,-1").

## Examples
### Box Blur
```sh
imfilter input.ppm output.ppm 0.111,0.111,0.111,0.111,0.111,0.111,0.111,0.111,0.111
```
### Edge Detection
```sh
imfilter input.ppm output.ppm -1,-1,-1,-1,8,-1,-1,-1,-1
```
### Sharpen
```sh
imfilter input.ppm output.ppm 0,-1,0,-1,5,-1,0,-1,0
```
