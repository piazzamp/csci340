// -----------------------------------
// CSCI 340 - Operating Systems I 
// Spring 2015 (Tony Leclerc, Brent Munsell)
// utils.h header file
// Homework Assignment 1
//
// -----------------------------------

#define PI 3.14159265	// define PI constant
#define DEBUG 0		// 0 turns off debug printfs, 1 turns on debug printfs

// -----------------------------------
// C-Style data structure to hold vector
// magnitude (r) and direction (theta)
// Note: theta is in degrees
//
typedef struct {

	double r;	// magnitude value
	double theta;	// direction value (in degrees)

} v_struct;

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
int read( char* file_name, v_struct* p_vec_array );

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
double x_component( v_struct* p_vector );

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
double y_component( v_struct* p_vector );


double degtorad(double deg);
