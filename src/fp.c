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
        fd = open(fname, O_RDONLY);
        if(fd == -1){
                fprintf(stderr, "unable to open '%s'", fname);
                exit(1);
        }
        fstat(fd, &stat_buf);

        offset = 0;
        */

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
	
	char * imgDownloaded;
    char * imgEdited;

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
                //imgDownloaded = downloadImage(atoken);
                imgEdited = downloadImage(atoken);
                //2)call function requested (sets the name of the imaged edited)
                        // param: the name of the image downloaded
                        // returns 0 on success, -1 on error
                //imgEdited = blur(imgDownloaded);
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
