# Star Register

Registers images on a middle one.
Internally does the following:

* Creates grey versions of the images
* Detects stars on the grey image
* Matches graphs between an image and the middle one
* Estimates a transform between the images
* Stores the tranform image in hdf5

## Parameters

* *minStars* Minimum stars to detect
* *maxStars* Maximum stars to detect
* *fullGraph* Number of stars used for a full dense graph matching at the start of the process
