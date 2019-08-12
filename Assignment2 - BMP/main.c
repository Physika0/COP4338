/*
 * Name: 
 * PID: 
 *
 * Description:
 * The program manipulates .bmp files with options
 * including scaling up the image (enlarging), 
 * rotating in degrees multiples of 90, vertical flipping,
 * and horizontal flipping.
 * All of these options inputted in the command line.
 *
 *
 * I affirm that this work is my own and that I did not
 * get help from anyone else or sources from the itnernet.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "bmplib.h"
#include <math.h>

/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL* original, int rows, int cols, int scale,
	    PIXEL** new, int* newrows, int* newcols)
{
	if(scale < 2)
	{
		printf("ERROR: Scale must be an integer that is 2 or greater\n");
		return -1;
	}
	
	*newcols = cols * scale;
	*newrows = rows * scale;
	
	*new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));

	int row, col, sx, sy;
	
	for(row = 0; row < rows; row++)
	{
		for(col = 0; col < cols; col++)
		{
			PIXEL* o = original + row*cols + col;

			for(sy = 0; sy < scale; sy++)
			{
				for(sx = 0; sx < scale; sx++)
				{
					PIXEL* n = (*new) + (scale * row) * *newcols 
						   + (scale * col) + (sy * *newcols) + sx;
					*n = *o;
				}
			}
		}
	}

	return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation,
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation,
	   PIXEL** new, int* newrows, int* newcols)
{

	int row, col;
	*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

	//Checks if rotation is a multiple of 90	
	if(rotation % 90 != 0)
	{
		printf("ERROR: Rotation degree must be a multiple of 90 between -270 and 270\n");
		return -1;
	}

	if(rotation == 90 || rotation == -270)
	{
		for(row = 0; row < rows; row++)
        	{
                	for(col = 0; col < cols; col++)
                	{
                        	PIXEL* o = original + (row*cols) + col;
                        	PIXEL* n = (*new) + ((cols - col - 1)*rows) + row;
                        	*n = *o;
                	}
        	}
        	*newcols = rows;
        	*newrows = cols;
	}
	else if (rotation == -90 || rotation == 270)
	{
		for(row = 0; row < rows; row++)
                {
                        for(col = 0; col < cols; col++)
                        {
                                PIXEL* o = original + (row*cols) + col;
                                PIXEL* n = (*new) + (col*rows) + (rows - row - 1);
                                *n = *o;
                        }
                }
                *newcols = rows;
                *newrows = cols;
	}
	else if(abs(rotation) == 180)
	{

		for(row = 0; row < rows; row++)
                {
                        for(col = 0; col < cols; col++)
                        {
                                PIXEL* o = original + (row*cols) + col;
                                PIXEL* n = (*new) + ((rows - row - 1)*cols) + (cols - col - 1);
                                *n = *o;
                        }
                }
                *newcols = cols;
                *newrows = rows;		

	}


	return 0;
}

/*
 * This method Vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int verticalflip (PIXEL *original, PIXEL **new, int rows, int cols)
{

	int row, col;

	if ((rows <= 0) || (cols <= 0))
	{
		printf("ERROR: Rows or columns are less than 0\n");
		return -1;
	}

	*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

	for (row=0; row < rows; row++)
	{
		for (col=0; col < cols; col++) 
		{

			PIXEL* o = original + (row * cols) + col;
			PIXEL* n = (*new) + ((rows - row - 1) * cols) + col;
			*n = *o;
		}
	}

	return 0;
}


/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols)
{
	int row, col;

	if ((rows <= 0) || (cols <= 0))
	{
		printf("ERROR: Rows or columns are less than 0\n");
		return -1;
	}

	*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

	for (row=0; row < rows; row++)
	{
		for (col=0; col < cols; col++) 
		{
			PIXEL* o = original + row*cols + col;
			PIXEL* n = (*new) + row*cols + (cols-1-col);
			*n = *o;
    		}
	}

	return 0;
}

int main(int argc, char **argv)
{
  
  	extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int sflag=0, rflag=0, fflag=0, vflag=0, oflag=0;
	static char usage[] = "usage: %s [-s scale | -r degree | -f | -v ] [-o output_file] [input_file]\n";

	int scale;
	int rDegree;
	char* outfileName = "result.bmp";
	char* infileName;

	while ((c = getopt(argc, argv, "s:r:fvo:")) != -1)
		switch (c) {
		case 's':
			sflag = 1;
			scale = atoi(optarg);
			break;
		case 'r':
			rflag = 1;
			rDegree = atoi(optarg);
			break;
		case 'f':
			fflag = 1;
			break;
		case 'v':
			vflag = 1;
			break;
		case 'o':
			oflag = 1;
         		outfileName = optarg;
			break;
		case '?':
			err = 1;
			break;
		}
	
	if ((optind+1) > argc) {	
		/* need at least one argument (change +1 to +2 for two, etc. as needeed) */

		printf("optind = %d, argc=%d\n", optind, argc);
		fprintf(stderr, "%s: missing input file name\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} else if (err) {
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}

	infileName = argv[argc-1];

	/*
	// Debugging information
	printf("sflag = %d\n", sflag);
	printf("scale = %d\n", scale);
	printf("rflag = %d\n", rflag);
	printf("rDegree = %d\n", rDegree);
	printf("fflag = %d\n", fflag);
	printf("vflag = %d\n", vflag);
	printf("outfileName = \"%s\"\n", outfileName);
	printf("infileName = \"%s\"\n", infileName);
	*/

  	int row, col;
  	PIXEL *b, *nb;

	//Order of operations: rotate (rflag), flip vertically (vflag), flip horizontally (fflag)
	//Scale (sflag) happens last
	
	if(rflag)
	{

		readFile(infileName, &row, &col, &b);
		if(rotate(b, row, col, rDegree, &nb, &row, &col) == -1)
		{
			free(b);
			free(nb);
			exit(1);
		}
		writeFile(outfileName, row, col, nb);

		if(vflag)
		{
			readFile(outfileName, &row, &col, &b);
			if(verticalflip(b, &nb, row, col) == -1)
			{
				free(b);
				free(nb);
				exit(1);
			}
			writeFile(outfileName, row, col, nb);
		}

		if(fflag)
		{
			readFile(outfileName, &row, &col, &b);
			if(flip(b, &nb, row, col) == -1)
			{
				free(b);
				free(nb);
				exit(1);
			}
			writeFile(outfileName, row, col, nb);
		}
	}
	else if(vflag)
	{
		readFile(infileName, &row, &col, &b);
		if(verticalflip(b, &nb, row, col) == -1)
		{
			free(b);
			free(nb);
			exit(1);
		}
		writeFile(outfileName, row, col, nb);

		if(fflag)
		{
			readFile(outfileName, &row, &col, &b);
			if(flip(b, &nb, row, col) == -1)
			{
				free(b);
				free(nb);
				exit(1);
			}
			writeFile(outfileName, row, col, nb);
		}
	}
	else if(fflag)
	{
		readFile(infileName, &row, &col, &b);
		if(flip(b, &nb, row, col) == -1)
		{
			free(b);
			free(nb);
			exit(1);
		}
		writeFile(outfileName, row, col, nb);
	}

	//Checks if scale flag is set and if any other flags
	//have been set to determine which file to modify
	if(sflag && (rflag || vflag || fflag))
	{
		readFile(outfileName, &row, &col, &b);
		if(enlarge(b, row, col, scale, &nb, &row, &col) == -1)
		{
			free(b);
			free(nb);
			exit(1);
		}
		writeFile(outfileName, row, col, nb);
	}
	else if(sflag)
	{
		readFile(infileName, &row, &col, &b);
                if(enlarge(b, row, col, scale, &nb, &row, &col) == -1)
		{
			free(b);
			free(nb);
			exit(1);
		}
                writeFile(outfileName, row, col, nb);
	}
	

  	free(b);
  	free(nb);
  
	exit(0);

}
