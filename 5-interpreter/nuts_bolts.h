#ifndef nuts_bolts_h
#define nuts_bolts_h

#include <stdint.h>

typedef int bool;
#define true 1
#define false 0

/*  	Simple fail code - just prints the failure */
void FAIL(char * status);

/*		If char c DOES exist in string, return True and pointer to first instance of char c to ptr
 * 		If char c DOES NOT exist in string, return false and do not change ptr 
 */
bool find_first(char str[], char c, int * ptr);

/*		If char c DOES exist in string, 
 *
 *			Search for character immediately after it
 *
 *			If number DOES exist after char c 		- return True and change ptr to entire number
 *			If number DOES NOT exist after char c 	- return False and do not change ptr	 
 *
 * 		If char c DOES NOT exist in string, FAIL with <failure>
 *
 *			This function assumes that character exists. This can be checked with find_first function 
 */
bool read_num(char str[], char c, float * ptr);


/*  	From grbl's nuts_bolts file for reading floats
 */
uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr);


#endif