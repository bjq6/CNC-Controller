#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "nuts_bolts.h"

bool read_line(char * line, int * g_code, float * f_val, float * r_val, vector * victor) {
	uint8_t char_counter = 0;
	char letter;
  	float value;
  	uint8_t int_value = 0;

 	// Parse through line. Populate the Command to execute, the X, Y, Z changes that need to happen
 	// and the feedrate F at which these should happen. 	
  	while (line[char_counter] != 0) {

		letter = line[char_counter];
	    if((letter < 'A') || (letter > 'Z')) { char_counter++; continue;} 
	    char_counter++; 
	    if (!read_float(line, &char_counter, &value)) { FAIL("STATUS_BAD_NUMBER_FORMAT"); }

	    int_value = trunc(value);

	    switch(letter) {

	    	//Currently only supports GCode, could be extended to support other commands
	      case 'G':
	      	switch(int_value) {
	      		case 0: //  RAPID        LINEAR    MODE
	      		case 1: //               LINEAR    MODE
	      		case 2: //  CLOCKWISE    CIRCULAR  MODE
	      		case 3: //  C-CLOCKWISE  CIRCULAR  MODE
	      			*g_code = int_value;
	      			break;
	      		default:
	      			printf("Does not support G%i codes. Sorry, try again next semester.\n",int_value);
	      	}
	      	break; // End G block
	      default:

	      	switch(letter){
	      		case 'F': *f_val = value; break;
	      		case 'R': *r_val = value; break;
	      		case 'X': victor->x = value; break;
	      		case 'Y': victor->y = value; break;
	      		case 'Z': victor->z = value; break;
	      		default:
	      			printf("Does not support '%c'Codes.\n",letter);
	      	}
	    }
	}
	return 0;
} 

void process_linear(queue * pos_q, vector * dest, vector * init){
	/* Calculates distance from current point to destination point. Calculates number of steps needed*/ 
	float dx = dest->x-init->x, dy = dest->y-init->y, dz = dest->z-init->z;
	float dist = sqrt(dx*dx + dy*dy + dz*dz);
	int num_steps = dist/STEP_SIZE;

	/*Creates a linear interpolation for each step from init to destination*/
	for (int n=0;n<num_steps+1;n++){

		vector pos_step = {init->x+(n*dx)/num_steps, init->y+(n*dy)/num_steps, init->z+(n*dz)/num_steps};

		insert(pos_q, pos_step);
	}
}

void process_circular(queue * pos_q, vector * dest, vector * init, float r, int cc){
	/* Calculates distance from current point to destination point. Calculates number of steps needed*/ 
	float dx = dest->x-init->x, dy = dest->y-init->y, dz = dest->z-init->z;
	float dist = sqrt(dx*dx + dy*dy + dz*dz);
	float theta = 2*asin(.5*dist/r);
	float arc_dist = r*theta;
	float d = dist/2;

	/*Creates a chord from init to destination - used to find origin of the circle*/
	float mid_x = (dest->x+init->x)/2;
	float mid_y = (dest->y+init->y)/2;

	/*Finds hypotenuse*/
	float h = sqrt(r*r - d*d);

	/*Number of steps and change in theta for each step*/
	int num_steps = arc_dist/STEP_SIZE;
	float theta_per_seg = theta/num_steps;

	/*Finds the origin of the circle*/
	float x_o = mid_x + cc*h*dy/dist;
	float y_o = mid_y - cc*h*dx/dist;

	float alpha = atan2(init->y-y_o, init->x-x_o);

	/*Creates a circular interpolation for each step from init to destination*/
	for (int n=0;n<num_steps+1;n++){
		float beta = alpha-cc*n*theta_per_seg;
		vector pos = {x_o + r*cos(beta), y_o + r*sin(beta)};
		insert( pos_q, pos );
	}
	
}

/*Reads and prints a file - used for testing, not in main*/
void read_file(char* fileName){
    FILE* file = fopen(fileName, "r"); 
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); 
    }

    fclose(file);
}


/*Only used in testing*/
/*
int main(){
	char lin[] = "G01 X02 Y3";
	int g_code = -1;
	float f_val = -1, r_val = 0;
	vector victor = { 0, 0, 0 };

	read_line(lin, &g_code, &f_val, &r_val, &victor);
	// printf("Input Line: %s contains: \n", lin);
	// printf("G= %i \t F=%i \t R= %i \t X= %i \t Y= %i \t Z= %i \n", g_code, f_val, r_val, x_val, y_val, z_val);

	vector coords[BUFF_SIZE];
	vector speeds[BUFF_SIZE];

	vector init = {0, 0, 0};
	vector dest = {2, 4, 0};

	// process_linear(coords, speeds, 100, 2, 4, 0, 0, 0, 0);
	process_linear(coords, speeds, 100, 2, 4, 0, 0, 0, 0);	
	
}
*/


