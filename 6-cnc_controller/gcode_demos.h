#ifndef GCODE_DEMOS_H
#define GCODE_DEMOS_H

/**
 * Three preprogrammed GCode demos
 *
 * 	1. Demonstrates linear motion
 *	2. Demonstrates rapid motion and circular interpolations
 *	3. Demonstrates a more complicated program that will trace out the letters "ECE"
 */

char *gdemo1 = "G00 X20 Y10\0G01 X40 Y40 F1\0G01 X60 Y40 F1\0G01 X60 Y60 F1\0eof";
char *gdemo2 = "G00 X50 Y50\0G03 X90 Y50 R20\0eof";
char *gdemo3 = "G0 X10 Y10\0G1 X35 Y10\0G1 X10 Y10\0G1 X10 Y35\0\
				G1 X35 Y35\0G1 X10 Y35\0G1 X10 Y60\0G1 X70 Y60\0\
				G3 X70 Y10 R25\0G1 X105 Y10\0G1 X80 Y10\0G1 X80 Y35\0\
				G1 X105 Y35\0G1 X80 Y35\0G1 X80 Y60\0G1 X105 Y60\0eof";

#endif