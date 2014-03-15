/*
 * fp.c
 *
 *  handle_request inherited from hw2 assignment and edited as needed
 * 
 */
#include "fp.h"

/** <blur> is just a name to identified the function
 * This is the function in which we make the changes to the image
 * @param imgName the name saved on disk for the image downloaded (This is the name returned by the downloadImage() function)
 * loads the image and applies the filter
 * Returns the new name given to the image edited
 */
char* blur(char* imgName) {
	return NULL;
}

char* downloadImage(char* webPath){
	pid_t pid;
	char *const paramList[] = {"/usr/bin/wget", webPath, "-P", "./images", NULL};

	if((pid = fork()) == 0){
		setpgid(0,0);
		if(execv("/usr/bin/wget", paramList) < 0){
			printf("%s: Command not found.\n", "/usr/bin/wget");
			exit(0);
		}
	}
	wait(NULL);
	char *fileName = basename(webPath);
	printf("Image Downloaded\n");
	return fileName;
}

IplImage* loadImage(char* imgName){
	IplImage* rv;

	rv = malloc(sizeof(IplImage*));

	if(!(rv = cvLoadImage(imgName, CV_LOAD_IMAGE_UNCHANGED)))
	{
		fprintf(stderr, "File could not be loaded");
		return NULL;
	}

	return rv;
}

/**
 * This function is the big one of this assignment.
 * It takes a string representing the HTTP request, parses the
 * values from the GET variables, creates an HTTP response,
 * and fills it with the results of one of the above functions.
 *
 * For example, if you go to:
 * http://localhost:8000/?f=fib&n=10
 * You should see:
 * 55
 * In your browser.
 *
 * Note the request above will have the first line be:
 * GET /?f=fib&n=10 HTTP/1.1
 * So you'll have to use a tokenizer to pull out the "10" and "f=fib"
 */
char* handle_request(char * req) {
	char * rv; // This will be our return value
	char * res; // This will point to a location in rv where we can write the html to.
	char * ftoken; // this will be a token that we pull out of the 'path' variable indicating the function
	               // for example, if you go to http://localhost:8000/?f=fib&n=10, this will be "f=fib"
	char * atoken; // A token representing the argument to the function, i.e., "n=10"
	char * tmp; // used to point to where the arguments start

	rv = malloc(4096 * sizeof(char));// TODO: allocate a big chunk of memory for the result. Like 2048 characters

	// TODO: write the first line of the HTML header to rv.
    sprintf(rv, "HTTP/1.1 200 OK\n\n");
	// see http://www.opencalais.com/HTTPexamples for some examples
	// you only need to write the first few lines here.

	// FREEBEE: set the 'res' pointer to be in the location after the header
	res = &(rv[strlen("HTTP/1.1 200 OK\n\n")]);

    // Use strncmp, see http://www.cplusplus.com/reference/cstring/strncmp/
	if(strncmp(req, "GET", 3) == 0) // TODO: check if the request is a GET request.
    {
     	// FREEBEE: set the tmp pointer to where the "path" variable begins
		tmp = req+4;
        if(strncmp(tmp, "/", 1) == 0)
            tmp++;// TODO: increment tmp point if it points to a '/'
        if(strncmp(tmp, "?", 1) == 0)
            tmp++;	// TODO: increment tmp point if it points to a '?'
        
        ftoken = strtok(tmp , "&");	// TODO: Set the 'ftoken' variable to the first argument of the path variable.
		// Use the strtok function to do this
		// For an example, see http://www.cplusplus.com/reference/cstring/strtok/

	
        atoken = strtok(NULL, "&% ");	// TODO: set atoken to the n= argument
 	// TODO: set atoken to the n= argument
	atoken++;	// TODO: advance the atoken to remove the "n="
        
	// TODO: see if 'ftoken' is any of our functions
	if(strcmp(ftoken, "f=blur") == 0){
		//print("%s\n", downloadImage(argv[1]));
		sprintf(res, "%s", downloadImage(atoken));   // TODO: if ftoken is "f=fib", call fib and print results to res
		
		//sprintf(res, "%s", atoken);   // TODO: if ftoken is "f=fib", call fib and print results to res
        }
	/*
        if(strcmp(ftoken, "f=numFiles") == 0){
            sprintf(res, "%d", numFiles(atoken));  // TODO: if numFiles, call function and print result
        }
        if(strcmp(ftoken, "f=getFileSize") == 0){
            sprintf(res, "%ld", getFileSize(atoken));  // TODO: if getFileSize, call function and print result
        }
        if(strcmp(ftoken, "f=getEnvValue") == 0){
            sprintf(res, "%s", getEnvValue(atoken)); // TODO: if getEnvValue, call function and print result
        }
	*/
    }
	return rv; // TODO: return result
}
