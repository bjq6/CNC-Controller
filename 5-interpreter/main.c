#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "nuts_bolts.h"
#include "queue.h"

int main(){
	// File
	const char * gcode_file[]={
		"G00 X01 Y01",
		"G01 X02 Y02",
		"G02 X03 Y03",
		"End of File"
	};

	int n = 0;

    queue pos_q;
    queue speed_q;
	init_q(&pos_q);
	init_q(&speed_q);

    while ( strcmp( gcode_file[n], "End of File" ) != 0 ) {

    	// Initializations
    	int g_code = -1;
		float f_val = -1, r_val = 0;
		vector victor = { 0, 0, 0 };

		// Populate Initializations from current GCode command
    	read_line(lin, &g_code, &f_val, &r_val, &victor);

    	switch(g_code){
    		case 0: 
    		case 1: 
    			break;
    		case 2: 
    		case 3: 
    			break;
    		default: 
    			printf("Weird... I thought this would have been caught earlier.\n");
    	}

        printf("%s\n", gcode_file[n]); 
        n++;
    }
    printf("\n"); 
    
}