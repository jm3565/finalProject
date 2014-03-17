/*
 * fp.c
 *
 *  handle_request inherited from hw2 assignment and edited as needed
 *  Author: Juan Munoz
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

/**
 * This is the fucntion that downloads a jpg image from the web
 * Saves the image found at the given webPath in the 'images' folder
 * Returns the name of the image
 */
char* downloadImage(char* webPath){
	pid_t pid;
        char * fileName;
        char *const paramList[] = {"/usr/bin/wget", webPath, "-P", IMGFOLDER, NULL};

        if((pid = fork()) == 0){
                setpgid(0,0);
                if(execv("/usr/bin/wget", paramList) < 0){
                        printf("%s: Command not found.\n", "/usr/bin/wget");
                        exit(0);
                }
        }
        wait(NULL);
        fileName = basename(webPath);
        printf("Image Downloaded\n");
        return fileName;
}

/**
 * This is the function to load an image with OpenCV to do the transformations
 * Receives the file name of the image downloaded
 * Returns a pointer to the IplImage
 */
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

// This is the initial function supposed to send the HTTP response to the web browser
// It is not used in the final code, just here for the purpose of showing our initial 
// approach for this issue

int sendImage(char * imgEdited, int conSocket){
//char* sendImage(char * imgEdited){
        char * res;
        char * fname;
        long len, ret;
        FILE * ifp;
        char * rv;

        struct stat stat_buf;
        off_t offset = 0;
        int rc;

        fname = malloc(sizeof(char) * 128);

        strcat(fname, IMGFOLDER); /* build the path name of the file */
        strcat(fname, "/");
        strcat(fname, imgEdited);

        /*
        ifp = fopen(fname, "r+b");
        if(ifp == NULL){
                printf("%s: File not found.\n", fname);
                exit(0);
        }
        

        printf("file opened: %s", fname);

        fseek(ifp, 0, SEEK_END);
        len = ftell(ifp);
        fseek(ifp, 0, SEEK_SET);

        //len = (long)fseek(ifp, (off_t)0, SEEK_END); // lseek to the file end to find the length //
        printf("image length: %ld\n", len);     
        //(void)fseek(ifp, (off_t)0, SEEK_SET); // lseek back to the file start ready for reading //
        */

        rv = malloc(BUFSIZE * sizeof(char));// TODO: allocate a big chunk of memory for the result. Like 2048 characters
        sprintf(rv, "HTTP/1.1 200 OK\n");

        res = &(rv[strlen("HTTP/1.1 200 OK\n")]);
        sprintf(res, "Content-Length: %ld\nContent-Type: %s\n\n<html>\n<body>\n<img src=\"%s\">\n</body>\n</html>", len, "image/jpg", fname);

        printf("%s\n", rv);

        /*
        (void)write(conSocket, rv, strlen(res)); // send image data throught the socket //

        while((ret = read(ifp, rv, BUFSIZE)) > 0){
                (void)write(conSocket, rv, ret);
        }
        sleep(1); // allow socket to drain before signalling the socket is closed //
        printf("image Sent\n");
        */
        //return rv;
}


/**
 * BASE CODE OF THIS FUNCTION IS FROM HW2
 * It takes a string representing the HTTP request, parses the
 * values from the GET variables and calls the function requested,
 * and fills it with the results of one of the above functions.
 *
 * For example, if you go to:
 * http://localhost:8000/?f=blur&%=http://img1.wikia.nocookie.net/__cb20110502031006/uncyclopedia/images/c/c9/Boardroom_small.jpg
 * The Boardroom_small.jpg is downloaded to the 'images' folder
 * calls the 'blur' function to transform the image
 * sets up the HTTP response with the new image generated
 * and displays it in the browser
 */
char* handle_request(char * req) {
	char * res; // This will point to a location in rv where we can write the html to.
	char * ftoken; // this will be a token that we pull out of the 'path' variable indicating the function
	               // for example, if you go to http://localhost:8000/?f=fib&n=10, this will be "f=fib"
	char * atoken; // A token representing the argument to the function, i.e., "n=10"
	char * tmp; // used to point to where the arguments start
	
	char * imgDownloaded; // This is the name of the file downloaded from the web
    char * imgEdited; // This is the name of the file edited

/*
	rv = malloc(4096 * sizeof(char));// TODO: allocate a big chunk of memory for the result. Like 2048 characters

	// TODO: write the first line of the HTML header to rv.
    sprintf(rv, "HTTP/1.1 200 OK\n\n");
	// see http://www.opencalais.com/HTTPexamples for some examples
	// you only need to write the first few lines here.

	// FREEBEE: set the 'res' pointer to be in the location after the header
	res = &(rv[strlen("HTTP/1.1 200 OK\n\n")]);
*/	
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
                //sprintf(res, "%s", downloadImage(atoken));   // TODO: if ftoken is "f=fib", call fib and print results to res

                //sprintf(res, "%s", atoken);   // TODO: if ftoken is "f=fib", call fib and print results to res

                //1)download the image (sets fileName of downloaded image)
                        // param: the webPath of the image
                        // returns 0 on success, -1 on error
                imgDownloaded = downloadImage(atoken);
                //2)call function requested (sets the name of the imaged edited)
                        // param: the name of the image downloaded
                        // returns 0 on success, -1 on error
                imgEdited = blur(imgDownloaded);
                //printf("imgE: %s\n", imgEdited);
                //3)open the image changed and send the HTTP response
                        // param: the name of the image edited
                        // param: the partial HTTP response to be filled
                        // return 0 on success, -1 on error
                //sendImage(imgEdited, con_Socket);
                //rv = sendImage(imgEdited);
        }
    }
	return imgEdited; // TODO: return result
}
