/*
 * FinalProject.c
 *
 *  Created on: Mar 10, 2013
 *      Author: Chris Khedoo
 */

#include "FinalProject.h"


void getSizeValue(int finalValue, int *valueNeeded)
{
    int i;
    *valueNeeded = 0;
    for (i = 1; i <= finalValue/2; i++)
    {
        if (finalValue%i==0)
            *valueNeeded = i;
    }
}

IplImage** getSubImages(IplImage *src, int numColumns, int numRows)
{
    int cellWidth, cellHeight, y, x, i;
    IplImage **rv;
    CvSize s = cvGetSize(src);

    cellWidth = s.width / numColumns;
    cellHeight = s.height / numRows;

    rv = malloc(sizeof(IplImage*) * numColumns * numRows);
    if (rv == NULL)
    {
        fprintf(stderr, "Could not allocate rv array.\n");
        exit(0);
    }

    i = 0;
    for (y = 0; y < s.height; y += cellHeight)
    {
        for (x = 0; x < s.width; x+= cellWidth)
        {
            rv[i] = cvCreateImage(cvSize(cellWidth, cellHeight), src->depth, src->nChannels);
            if (rv[i] == NULL)
            {
                fprintf(stderr, "Could not allocate image.\n");
            }

            cvSetImageROI(src, cvRect(x, y, cellWidth, cellHeight));

            cvCopy(src, rv[i], NULL);

            cvResetImageROI(src);

            i++;
        }
    }

    return rv;
}

IplImage* stitchImages(IplImage **splitImages, int numColumns, int numRows)
{
    int i, j;
    int k = 0;

    CvSize s = cvGetSize(splitImages[0]);
    int width = s.width;
    int height = s.height;

    CvSize finalSize = cvSize(width*numColumns, height*numRows);

    if (splitImages ==  NULL)
    {
        return NULL;
    }

    IplImage *result = malloc(sizeof(IplImage) * numColumns * numRows);

    for (i = 0; i < numColumns; i++)
    {
        for (j = 0; j < numRows; j++)
        {
            CvRect r;
            r.width = width;
            r.height = height;
            r.x = width*j;
            r.y = height*i;
            cvSetImageROI(result, r);
            cvCopy(splitImages[k], result, NULL);
            cvResetImageROI(result);
            k++;
        }
    }

    return result;
}

IplImage** dotPattern(IplImage **splitImages, CvScalar *splitColors, int numColumns, int numRows)
{
    int i, j;
    int k = 0;
    CvSize s = cvGetSize(splitImages[0]);
    int width = s.width;
    int height = s.height;

    IplImage **rv = malloc(sizeof(IplImage) * numColumns * numRows);

    for (i = 0; i < numColumns; i++)
    {
        for (j = 0; j < numRows; j++)
        {
            // Center of circle
            CvPoint center = cvPoint((width+i)/2, (height+j)/2);
            cvCircle(splitImages[k], center, (width/2)-1, splitColors[k], -1, CV_AA, 0);
            rv[k] = splitImages[k];
            k++;
        }
    }

    return rv;
}

/**
 * Computes the distance between two colors (stored as CvScalars).
 *
 * Given a CvScalar c, you can access the blue, green, and red (BGR) elements
 * via c.val[0], c.val[1], and c.val[2], respectively.
 *
 * This function computes the distance between two colors as the euclidean
 * distance between the two BGR vectors.
 *
 * @see http://en.wikipedia.org/wiki/Euclidean_distance
 *
 * @param c1	The first color
 * @param c2	The second color
 * @returns		The euclidean distance between the two 3-d vectors
 */
double colorDistance(CvScalar c1, CvScalar c2)
{
    double d = 0; // the result
    int i; // an iterator

    // iterate over the dimensions and compute the sum
    for (i = 0; i < 3; i++)
        d += abs(c1.val[i] - c2.val[i]);

    // return the square root of the result.
    // If d is zero, just return 0.
    if (d == 0)
        return 0;
    else
        return sqrt(d);
}

/**
 * Finds the CvScalar in colors closest to t using the colorDistance function.
 * @param t		 	The color to look for
 * @param scolors	The colors to look through
 * @param numColors	The length of scolors
 * @returns			The index of scolors that t is closest to
 * 					(i.e., colorDistance( t, scolors[result]) <=
 * 					colorDistance( t, scolors[i]) for all i != result)
 */
int findClosest(CvScalar t, CvScalar* scolors, int numColors)
{
    int rv = 0, // return value
        i; // used to iterate
    double d, // stores the result of distance
    m = colorDistance(t, scolors[0]); // the current minmum distance

    // iterate over scolors
    for (i = 0; i < numColors; i++)
    {
        // compute the distance between t and scolors[i]
        d = colorDistance(t, scolors[i]);
        // check if the distance is less then current minimum
        if (d < m)
        {
            // if it is, store i as the current result and cache the minimum distance
            rv = i;
            m = d;
        }
    }

    // return result.
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
    rv = malloc(sizeof(CvScalar) * numImages);
    // iterate over images and compute average color
    for (i = 0; i < numImages; i++)
    {
        // for each image, compute the average color (hint: use cvAvg)
        rv[i] = cvAvg(images[i], NULL);
    }
    // return result
    return rv;
}
