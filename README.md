# AstroStack
A Tool to Create Astrophotographies from Several Photos

## Install

You need the following development packages and tools:

* ITK >=5
* Eigen (provided by ITK)
* Qt >=6.3
* CMake
* a C++17 compiler

On Windows, you can also use the package that are packaged in a release.

Otherwise, launch the CMake GUI and set ITK_DIR, Eigen_DIR and Qt6_DIR. Then build the project and run it.
Installation process should install all AstroStack libraries and plugins at the proper locations.

## How to use AstroStack and what's available

Currently only processing one photo at a time is possible. The future stacking process will be based on the Python code prototype in [the Stack project](https://github.com/AstroStacking/Stack).

### Processing one photo

Several workflows for single photos are possible.
A right click on an image will open a contextual menu with possible workflows.
The current shipped ones are:

* Deconvolve:
  * Deconvolves and image based on the ITK Richardson Lucy deconvolution algorithm

* History Stretch
  * Applies a point transform by taking the power of each pixel values (smaller than 1)
  * Stretches the histogram, selecting the maximum of the histogram of each channel and removing smaller values below xx% of the max.

## Support

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=VYJ38NPHF4MKL)
