// -----------------------------------
// CSCI 340 - Operating Systems I 
// Spring 2015 (Tony Leclerc, Brent Munsell)
// utils.c implementation file
// Homework Assignment 1
//
// -----------------------------------

// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <stdio.h>	// IO functions
#include <stdlib.h> 	// atof function
#include <math.h>	// math functions
#include "utils.h"	// your own functions

// Now you put your implementation of the function prototypes here...

// tail -60 utils.h >> utils.c:
// ------------------------------------
// Function prototype for reading a
// text file that defines 10 vectors. See 
// assignment for format of this file.
// 
// Basic Step-by-Step instructions
//  1. Open file using string value assigned to file_name 
//     character pointer.
//  2. For each vector defined in the text file, 
//     read in the magnitude and direction values
//     and then split using the comma character as 
//     the delimiter. There are many ways to do this, 
//     just remember a char pointer (i.e. char*) can 
//     be treated as a character array (i.e. file_name[0] 
//     will return the char value at index position 0). 
//     Don't forget to increment a counter variable that 
//     maintains number of vectors read-in by this function.
//  3. Convert parsed magnitude and direction values to 
//     from a string to a double (hint: use atof function)
//  4. Store magnitude and direction double values in 
//     the correct index position in the v_struct array. 
//     Hint: The position of the vector in the text file 
//     is equivalent to the index position in the v_struct
//     array. That is, the direction and magnitude values 
//     stored in p_vec_array[0] would be the first vector 
//     in the vectors text file.
//
// Arguments: 	char* file_name = file name of vector text file
//	      	p_vec_array = pointer to an array of v_structs
// Return:      Number (int data type) of vectors read-in by function
//
int read( char* file_name, v_struct* p_vec_array){
	int count = 0, i;	
	char tempstr[20];
	FILE * fp = fopen(file_name, "r");

	if (fp == NULL){	
		fprintf(stderr, "failed to open the file. double check it and try again.\n");
		return -1;
	}

	// read in a line as a string with fgets() or getline()?
	while(fgets(tempstr, 20, fp) != NULL){
		// parse string
		i = 0;
		while (tempstr[i] != ',' && tempstr[i] != NULL){
			i++;
		}
		if (DEBUG){
			printf("(read) --> comma found at index %d in string: %s\n", i, tempstr);
		}
		if (tempstr[i] == ','){
			tempstr[i] = NULL;
			// break it into two null-terminated strings
			p_vec_array[count].r = atof(tempstr);
			p_vec_array[count].theta = atof(&(tempstr[i+1]));
			count++;
		}		
	}
	fclose(fp);
	return count;
}

// --------------------------------------
// Function prototype that uses the magnitude and 
// direction values in vector structure
// to compute the x component, i.e. r*cos(theta)
//
// Note(s): 
//	- theta must be converted from degrees to radians 
//	  prior to calculation.
//
// Arguments: 	p_vector = point to v_struct 
// Return: 	double that represents x component value
//
double degtorad(double deg){
	if (deg > 360){
		deg = deg - 360;
	}
	else if (deg < -360){
		deg = deg + 360;
	} // guaranteed: -720 < deg < 720

	return deg * PI / 180;
}

double x_component( v_struct* p_vector) {
	double rad, r = p_vector->r;
	rad = degtorad(p_vector->theta);
	return r * cos(rad);
}

// --------------------------------------
// Function prototype that uses the magnitude and 
// direction values in vector structure
// to compute the y component, i.e. r*sin(theta)
//
// Note(s): 
//	- theta must be converted from degrees to radians 
//	  prior to calculation.
//
// Arguments: 	p_vector = point to v_struct 
// Return: 	double that represents y component value
//
double y_component( v_struct* p_vector) {
	double rad, r = p_vector->r;
	rad = degtorad(p_vector->theta);
	return r * sin(rad);
}
