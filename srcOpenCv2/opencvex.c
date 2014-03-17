/*
 * Final Project
 * The final project OpenCV portion is taken and edited from homework assignment 4
 *  Due on: Mar 17, 2013
 *      Author: Chris Khedoo & Juan Munoz
 */

#include "opencv.h"

#define VIS_SIZE 10

int main(int argc, char ** argv) {
    int numColumns, // the number of horizontal images in the result
        numRows, // the number of vertical images in the result
        i, j, k; // just an iterator
    IplImage *src, // the source image
             *res, // the resulting mosaic image
             *smallRes; // the resulting mosaic image that is only 2x the size of the original
    IplImage **subImages; // small images created from the source image
    CvScalar *scolors; // the average color of the sub images of the source image
    // Load the source image
    src = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);

    // make sure we have an even separation
    assert(src->width%5==0);
    assert(src->height%5==0);
    numColumns = src->width / 5;
    numRows = src->height / 5;

    // compute the sub images
    printf("Computing sub images\n");
    // create the sub images
    subImages = getSubImages(src, numColumns, numRows);
    // compute the average colors of the sub images
    printf("Computing sub image colors\n");
    scolors = getAvgColors(subImages, numColumns * numRows);

    printf("Closest Images found\n");
    // stitch the result
    res = stitchImages(subImages, numRows, numColumns);
    //POSSIBLY ROTATE IMAGE?
    /*IplImage *rotatedImg = rotateImage90(res);
    rotatedImg = rotateImage90(rotatedImg);
    rotatedImg = rotateImage90(rotatedImg);*/
    printf("Images stitched\n");

    IplImage *grayImg = cvCreateImage(cvGetSize(res), IPL_DEPTH_8U, 1);
    cvCvtColor(res, grayImg, CV_RGB2GRAY);
    // create a smaller version of the result for visualization
    smallRes = cvCreateImage(cvSize((src->width)*VIS_SIZE, (src->height)*VIS_SIZE), IPL_DEPTH_8U, 1);
    cvResize(grayImg, smallRes, CV_INTER_CUBIC);
    printf("saving scaled image to out.jpg\n");
    cvSaveImage("out2.jpg", smallRes, NULL);

    return EXIT_SUCCESS;
}
