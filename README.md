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

On MacOS, set CMAKE_XCODE_CODESIGN_IDENTITY and CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM.

## How to use AstroStack and what's available

Currently only processing one photo at a time is possible. The future stacking process will be based on the Python code prototype in [the Stack project](https://github.com/AstroStacking/Stack).

### Processing plugins

* Mono image plugins:
    * [ChromaSmoothing](src/QtProcessing/QtProcessing/Mono2Mono/chromasmoothing.md)
    * [Exponential](src/QtProcessing/QtProcessing/Mono2Mono/exponential.md)
    * [Histogram Stretch](src/QtProcessing/QtProcessing/Mono2Mono/histostretch.md)
    * [Light Pollution](src/QtProcessing/QtProcessing/Mono2Mono/lightpollution.md)
    * [Richardson Lucy Deconvolve](src/QtProcessing/QtProcessing/Mono2Mono/rldeconvolution.md)
* Multi images plugins:
    * [MaxStacking](src/QtProcessing/QtProcessing/Multi2Multi/maxstacking.md)
    * [MedianStacking](src/QtProcessing/QtProcessing/Multi2Multi/medianstacking.md)
    * [Reader](src/QtProcessing/QtProcessing/Multi2Multi/reader.md)
    * [Register](src/QtProcessing/QtProcessing/Multi2Multi/register.md)
    * [RobustMeanStacking](src/QtProcessing/QtProcessing/Multi2Multi/robustmeanstacking.md)

### Processing workflows

Several workflows for single photos are possible.
A right click on an image will open a contextual menu with possible workflows.
The current shipped ones are:

* [Cleanup](workflows/Cleanup.md)
* [Deconvolve](workflows/Deconvolve.md)
* [Histogram Stretch](workflows/HistoStretch.md)

For multiple photos, they are available from the main menu:

* [Deep Sky Stacking](workflows/DeepSkyStacking.md)
* [Max Stacking](workflows/MaxStacking.md)

## Changelog
### 0.4.0
* Starting the Multi processes
  * Max Stacking
  * Deep Sky Stacking

### 0.3.0

* Fix documentation with pandoc 3.1
* More command line tools:
  * Stacking
  * Registration by star detection

### 0.2.0

* Expose the processes as command line tools
* Mono processing:
  * Light pollution
  * Chromatic smoothing

### 0.1.0

* Display an image and its histogram
* Explorer UI
* Support for mono photo processing
* Mono processing:
  * Stretch histogram
  * Exponential transform of an image
  * Richardson Lucy deconvolution

## Support

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=VYJ38NPHF4MKL)
