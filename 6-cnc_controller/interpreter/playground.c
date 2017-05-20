#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "queue.h"

int main() {

	queue q;
	init_q(&q);

	vector x = {1,2,3};
	vector y = {4,5,6};
	vector z = {7,8,9};

	insert(&q, x);
   	insert(&q, y);
   	insert(&q, z);

   	vector newb;


 	newb = removeData(&q); printf("(%f, %f, %f)\n", newb.x, newb.y, newb.z);
 	newb = removeData(&q); printf("(%f, %f, %f)\n", newb.x, newb.y, newb.z);
	newb = removeData(&q); printf("(%f, %f, %f)\n", newb.x, newb.y, newb.z);
	newb = removeData(&q); printf("(%f, %f, %f)\n", newb.x, newb.y, newb.z);
	newb = removeData(&q); printf("(%f, %f, %f)\n", newb.x, newb.y, newb.z);
	newb = removeData(&q); printf("(%f, %f, %f)\n", newb.x, newb.y, newb.z); 
	newb = removeData(&q); printf("(%f, %f, %f)\n", newb.x, newb.y, newb.z);




	return 0;
}