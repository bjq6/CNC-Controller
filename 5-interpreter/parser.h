#ifndef parser_h
#define parser_h

// time it should take to complete one step
#define time_to_cut 0.001 
#define BUFF_SIZE 100
#define PI 3.14159265

typedef int bool;
#define true 1
#define false 0

typedef struct {
	float x;
	float y;
	float z;
} vector;

/*	read_line takes in a line to read and returns:
 * 			> true	= 	line is valid command and the proper values are set
 * 			> false = 	line is NOT valid. Values should not be read or acted on.  
 */
//bool read_line(char * line, int * g_code, float * f_val, float * r_val, float * x_val, float * y_val, float * z_val);
bool read_line(char * line, int * g_code, float * f_val, float * r_val, vector * victor);

void read_file(char* fileName);

#endif