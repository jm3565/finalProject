/*
 * opencv.c
 *
 *  Due on: Mar 17, 2013
 *      Author: Chris Khedoo & Juan Munoz
 */

#include "opencv.h"


/**
 * Splits up an image into numColumns by numRows sub-images and returns the results.
 *
 * @param src	The source image to split
 * @param numColumns	The number of columns to split into
 * @param numRows 		The number of rows to split into
 * @returns				A numColumns x numRows array of IplImages
 */
IplImage ** getSubImages(IplImage* src, int numColumns, int numRows)
{
    int cellWidth, cellHeight, y, x, i;
    IplImage ** rv;
    CvSize s = cvGetSize(src);

    // Compute the cell width and the cell height
    cellWidth = s.width / numColumns;
    cellHeight = s.height / numRows;

    // Allocate an array of IplImage* s
    rv = malloc(sizeof(IplImage*) * numColumns * numRows);
    if (rv == NULL)
    {
        fprintf(stderr, "Could not allocate rv array\n");
    }
    // Iterate over the cells
    i = 0;
    for (y = 0; y < s.height; y += cellHeight)
    {
        for (x = 0; x < s.width; x += cellWidth)
        {
            // Create a new image of size cellWidth x cellHeight and
            // store it in rv[i]. The depth and number of channels should
            // be the same as src.
            rv[i] = cvCreateImage(cvSize(cellWidth, cellHeight), src->depth,
                    src->nChannels);
            if (rv[i] == NULL)
            {
                fprintf(stderr, "Could not allocate image %d\n", i);
            }

            // set the ROI of the src image
            cvSetImageROI(src, cvRect(x, y, cellWidth, cellHeight));

            // copy src to rv[i] using cvCopy, which obeys ROI
            cvCopy(src, rv[i], NULL);

            // reset the src image roi
            cvResetImageROI(src);

            // increment i
            i++;
        }
    }

    // return the result
    return rv;
}

/**
 * For each image provided, computes the average color vector
 * (represented as a CvScalar object).
 *
 * @param images	The images
 * @param numImages	The length of images
 * @returns 		An numImages length array of CvScalars were rv[i] is the average color in images[i]
 */
CvScalar* getAvgColors(IplImage** images, int numImages)
{
    CvScalar* rv;
    int i;
    // create return vector
    rv = malloc(sizeof(CvScalar)*numImages);

    // iterate over images and compute average color
    for (i = 0; i < numImages; i++)
    {
        // for each image, compute the average color (hint: use cvAvg)
        rv[i] = cvAvg(images[i], NULL);
    }

    // return result
    return rv;
}

/**
 * Given an ordered list of images (iclosest), creates a
 * numColumns x numRows grid in a new image, copies each image in, and returns the result.
 *
 * Thus, if numColumns is 10, numRows is 5, and each iclosest image is 64x64, the resulting image
 * would be 640x320 pixels.
 *
 * @param iclostest		A numColumns x numRows list of images in row-major order to be put into the resulting image.
 * @param numColumns  	Number of horizontal cells
 * @param numRows		Number of vertical cells
 */
IplImage* stitchImages(IplImage **iclosest, int numColumns, int numRows)
{
    int j, // for iterating over the rows
        i, // for iterating over the columns
        k = 0;
    // using cvGetSize, get the size of the first image in iclosest.
    // remember all of the images should be the same size
    CvSize s = cvGetSize(iclosest[0]);

    // Compute the size of the final destination image.
    CvSize finalImageSize = cvSize(s.width*numColumns, s.height*numRows);	

    // allocate the return image. This can be potentially large, so
    // you should make sure the result is not null
    if (iclosest == NULL)
    {
        return NULL;
    }

    IplImage *result = cvCreateImage(finalImageSize, IPL_DEPTH_8U, 3);
    // iterate over each cell and copy the closest image into it
    for (i = 0; i < numColumns; i++)
    {
        for (j = 0; j < numRows; j++)
        {
            // set the ROI of the result
            CvRect rect;
            rect.x = s.width*i;
            rect.y = s.height*j;
            rect.width = s.width;
            rect.height = s.height;

            // set the ROI of the result
            cvSetImageROI(result, rect);						
            // copy the proper image into the result
            cvCopy(iclosest[k], result, NULL);
            // reset the ROI of the result
            cvResetImageROI(result);
            k++;
        }
    }

    // return the result
    return result;
}

IplImage *rotateImage90(IplImage *image)
{
    IplImage *rotatedImage = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
    CvPoint2D32f center;
    float center_val = (float)((image->width)-1)/2;
    center.x = center_val;
    center.y = center_val;
    CvMat *mapMatrix = cvCreateMat(2, 3, CV_32FC1);
    cv2DRotationMatrix(center, -90, 1.0, mapMatrix);
    cvWarpAffine(image, rotatedImage, mapMatrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
    cvReleaseMat(&mapMatrix);
    return rotatedImage;
}
