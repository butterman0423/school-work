#include <stdio.h>

extern char* itoascii(int);
extern char* concat_array(unsigned long int*, unsigned long int);
extern unsigned long int count_specs(char*);
extern unsigned long int pringle(char*, ...);

int main(int argc, char const *argv[]) {
	
    char* str = "Hello for %a and %%%a and %% and %a. and %a.\n";

    unsigned long int arr1[] = {123,456,7890};
    unsigned long int arr2[] = {12};
 	unsigned long int arr3[] = {0,0,0,0,5};
	unsigned long int arr4[] = {1,1};
	unsigned long int larr1 = 3;
	unsigned long int larr2 = 1;
	unsigned long int larr3 = 5;
	unsigned long int larr4 = 2;
	pringle(str, arr1, larr1, arr2, larr2, arr3, larr3, arr4, larr4);
	

    char* str2 = "Hello for %a and %%%a and %% and %a. and %a, with %A%a.\n";
	unsigned long int arr5[] = {1000, 500, 250, 0};
	unsigned long int larr5 = 4;
	pringle(str2, arr1, larr1, arr2, larr2, arr3, larr3, arr4, larr4, arr5, larr5);
    //char* newcount = itoascii(ret);
    //puts(newcount);
	
	unsigned long int arr6[] = {1, 2, 3};
	unsigned long int arr7[] = {10, 20, 30};
	unsigned long int larr6 = 3;
	unsigned long int larr7 = 3;
	char * str3 = "%a,%a,%a,%a,%a,%a";
	pringle(str3, arr1, larr1, arr2, larr2, arr3, larr3, arr4, larr4, arr5, larr5, arr6, larr6, arr7, larr7);
    return 0;
}
