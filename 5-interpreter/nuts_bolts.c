#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include "nuts_bolts.h"

#define MAX_INT_DIGITS 8

void FAIL(char * status) {
	printf("FAIL: %s\n", status);
}

bool find_first(char str[], char c, int * ptr){

  	char * pch;
	pch = strchr(str,c);
	int i =  pch-str;
	*ptr = i;
	return (*ptr>=0);

}

bool read_num(char str[], char c, float * ptr){

	int char_loc;

	if (!find_first(str, c, &char_loc)){ 
		FAIL("FAILURE:read_num - Character does not exist in string"); 
		return 0;
	}
	else {
		int next = char_loc+1; 
		if (!isdigit(str[next])) { return 0; }

		while (isdigit(str[next])||str[next]=='.'){ next++; }
		char* substr;
		strncpy(substr, str+char_loc+1, next-char_loc-1);
		*ptr = atof(substr);
		return 1;
	}
	return 0; // not needed but there for safety
}

uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr)
{
  char *ptr = line + *char_counter;
  unsigned char c;

  // Grab first character and increment pointer. No spaces assumed in line.
  c = *ptr++;

  // Capture initial positive/minus character
  bool isnegative = false;
  if (c == '-') {
    isnegative = true;
    c = *ptr++;
  } else if (c == '+') {
    c = *ptr++;
  }

  // Extract number into fast integer. Track decimal in terms of exponent value.
  uint32_t intval = 0;
  int8_t exp = 0;
  uint8_t ndigit = 0;
  bool isdecimal = false;
  while(1) {
    c -= '0';
    if (c <= 9) {
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS) {
        if (isdecimal) { exp--; }
        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
      } else {
        if (!(isdecimal)) { exp++; }  // Drop overflow digits
      }
    } else if (c == (('.'-'0') & 0xff)  &&  !(isdecimal)) {
      isdecimal = true;
    } else {
      break;
    }
    c = *ptr++;
  }

  // Return if no digits have been read.
  if (!ndigit) { return(false); };

  // Convert integer into floating point.
  float fval;
  fval = (float)intval;

  // Apply decimal. Should perform no more than two floating point multiplications for the
  // expected range of E0 to E-4.
  if (fval != 0) {
    while (exp <= -2) {
      fval *= 0.01;
      exp += 2;
    }
    if (exp < 0) {
      fval *= 0.1;
    } else if (exp > 0) {
      do {
        fval *= 10.0;
      } while (--exp > 0);
    }
  }

  // Assign floating point value with correct sign.
  if (isnegative) {
    *float_ptr = -fval;
  } else {
    *float_ptr = fval;
  }

  *char_counter = ptr - line - 1; // Set char_counter to next statement

  return(true);
}

void read_file(char* fileName){
    FILE* file = fopen(fileName, "r"); 
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); 
    }

    fclose(file);
}
    
