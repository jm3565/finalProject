/*
 * fp.h
 */

#include <string.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "cv.h"
#include "highgui.h"
#include <libgen.h>

/**
 * Computes the nth Fibonacci number
 * @see http://en.wikipedia.org/wiki/Fibonacci_number
 */
char* blur(char* imgName);

char* downloadImage(char* webPath);

IplImage* loadImage(char* imgName);

int sendImage(char * imgEdited, int conSocket);

/**
 * This function is the big one of this assignment.
 * It takes a string representing the HTTP request, parses the
 * values from the GET variables, creates an HTTP response,
 * and fills it with the results of one of the above functions.
 *
 * For example, if you go to:
 * http://localhost:8000/?f=fib&n=10
 * http://tux64-14.cs.drexel.edu:8000/?f=blur&%=http://www.site.com/image/path.jpg
 * You should see:
 * 55
 * In your browser.
 *
 * Note the request above will have the first line be:
 * GET /?f=fib&n=10 HTTP/1.1
 * So you'll have to use a tokenizer to pull out the "10" and "f=fib"
 */
char* handle_request(char * req);
