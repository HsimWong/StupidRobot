#include<stdio.h>
#include<math.h>
#include<trilist.h>

extern double arr_arcsin[271];
extern double arr_arccos[271];
extern double arr_arctan[271];


int o;

void tri_list_init() {

	for ( o = 0;o <= 271;o++) 
	{
		arr_arcsin[o] = asin(o/180*3.1416) ;
		arr_arccos[o] = acos(o/180*3.1416) ;
		arr_arctan[o] = atan(o/180*3.1416) ;
	} 
}