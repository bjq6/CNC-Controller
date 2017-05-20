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

	      case 'G':
	      	switch(int_value) {
	      		case 0: //  RAPID        LINEAR    MODE
	      		case 1: //               LINEAR    MODE
	      		case 2: //  CLOCKWISE    CIRCULAR  MODE
	      		case 3: //  C-CLOCKWISE  CIRCULAR  MODE
	      			*g_code = int_value;
	      			break;
	      		default:
	      			printf("We do not support G%i codes. Sorry, try again next semester.\n",int_value);
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
	      			printf("Didn't know what to do with letter '%c'.\n",letter);
	      	}
	    }
	}
	return 0;
} 

void process_linear(vector coords[], vector speeds[], float f, vector dest, vector init){
	float dx = dest.x-init.x, dy = dest.y-init.y, dz = dest.z-init.z;
	float dist = sqrt(dx*dx + dy*dy + dz*dz);
	int num_steps = dist/(f*time_to_cut);

	for (int n=0;n<num_steps+1;n++){

		coords[n].x = init_x+(n*dx)/num_steps; 
		coords[n].y = init_y+(n*dy)/num_steps; 
		coords[n].z = init_z+(n*dz)/num_steps;

		speeds[n].x = dx/dist*f;
		speeds[n].y = dy/dist*f;
		speeds[n].z = dz/dist*f;

		//printf("Posit (%f, %f, %f)\n", coords[n].x, coords[n].y, coords[n].z);
		//printf("Speed (%f, %f, %f)\n", speeds[n][0], speeds[n][1], speeds[n][2]);
	}
}

/*
void process_circular(vector coords[], vector speeds[], float f, vector dest, vector init){
	float dx = dest.x-init.x, dy = dest.y-init.y, dz = dest.z-init.z;
	float dist = sqrt(dx*dx + dy*dy + dz*dz);
	float theta = 2*asin(.5*dist/r);
	float arc_dist = r*theta;

	int num_steps = arc_dist/(f*time_to_cut);

	float theta_per_seg = theta/num_steps;
	float x_o = 
	float y_o = 

	for (int n=0;n<num_steps+1;n++){
		coords[n][0] = init_x+(n*dx)/num_steps; 
		coords[n][1] = init_y+(n*dy)/num_steps; 
		coords[n][2] = init_z+(n*dz)/num_steps; 

		printf("Posit (%f, %f, %f)\n", coords[n][0], coords[n][1], coords[n][2]);
	}
}
*/

void read_file(char* fileName){
    FILE* file = fopen(fileName, "r"); 
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); 
    }

    fclose(file);
}

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


