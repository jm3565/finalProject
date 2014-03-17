/*
 * FinalProjectEx.c
 *
 * Due on: Mar 17, 2013
 *      Author: Chris Khedoo
 */

#include "FinalProject.h"
#define VIS_SIZE 10

int main(int argc, char *argv[])
{
    // VARIABLES
    IplImage *src;        // the source image loaded from the disk
    IplImage **splitSrc;  // the smaller images that make up the source image
    int numColumns = 1;       // the number of columns the source image is split up into
    int numRows = 1;          // the number of rows the source image is split up into

    // load the image from the disk
    src = cvLoadImage(argv[1], 0);
    if (src == NULL)
    {
        fprintf(stderr, "The image cannot be opened.\n");
        exit(0);
    }

    printf("%d\n%d\n", src->width, src->height);

    // split the source image up into smaller images
    getSizeValue(src->width, &numColumns);
    getSizeValue(src->height, &numRows);
    splitSrc = getSubImages(src, numColumns, numRows);

    // get the color of each subimage
    CvScalar *splitColors = getAvgColors(splitSrc, numColumns * numRows);
    IplImage **dottedImages = dotPattern(splitSrc, splitColors, numColumns, numRows);
    IplImage *finalResults = stitchImages(dottedImages, numColumns, numRows);

    free(dottedImages);
    free(finalResults);
    free(splitColors);
    free(splitSrc);
    return 0;
}
