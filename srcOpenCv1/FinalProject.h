/*
 * hw4.h
 *
 *  Created on: Mar 10, 2013
 *      Author: lou
 */

#ifndef FINALPROJECT_H_
#define FINALPROJECT_H_

#include "cv.h"
#include "highgui.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"

/**
 * Loads an image (stored as an IplImage struct) for each
 * filename provided.
 * @param fileName		The filename corresponding to the source image
 */
IplImage* loadImage(char *fileName);

void getSizeValue(int finalValue, int *valueNeeded);

IplImage** getSubImages(IplImage *src, int numColumns, int numRows);

IplImage* stitchImages(IplImage **splitImages, int numColumns, int numRows);

IplImage** dotPattern(IplImage **splitImages, CvScalar *splitColors, int numColumns, int numRows);



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
double colorDistance(CvScalar c1, CvScalar c2);

/**
 * Finds the CvScalar in colors closest to t using the colorDistance function.
 * @param t		 	The color to look for
 * @param scolors	The colors to look through
 * @param numColors	The length of scolors
 * @returns			The index of scolors that t is closest to
 * 					(i.e., colorDistance( t, scolors[result]) <=
 * 					colorDistance( t, scolors[i]) for all i != result)
 */
int findClosest(CvScalar t, CvScalar* scolors, int numColors);
/**
 * For each image provided, computes the average color vector
 * (represented as a CvScalar object).
 *
 * @param images	The images
 * @param numImages	The length of images
 * @returns 		An numImages length array of CvScalars were rv[i] is the average color in images[i]
 */
CvScalar* getAvgColors(IplImage** images, int numImages);

#endif /* HW4_H_ */
