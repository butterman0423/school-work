#include <stdio.h>

extern char* itoascii(int);
extern char* concat_array(unsigned long int*, unsigned long int);
extern unsigned long int count_specs(char*);
extern unsigned long int pringle(char*, ...);

int main(int argc, char const *argv[]) {
	
    // Test for task 1.
    //   Output: 1234
    
    char* s = itoascii(1234);
    puts(s);

	
    // Test for task 2
    //   Output: 10 200 30
    
    unsigned long int arr[] = {10,200,30};
    char* x = concat_array(arr, 3);
    puts(x);
	
	
    // Test for task 3
    //   Output: 3
    
    char* str = "Hello this is a test string for %a and %%%a and %% and %a.\n";
    unsigned long int c = count_specs(str);
    char* count = itoascii(c);
    puts(count);
	
	
    // Test for task 4
    //   Output: Hello this is a test string for 123 456 7890  and %%12  and %% and 0 0 0 0 5 .
    //           78

    unsigned long int arr1[] = {123,456,7890};
    unsigned long int arr2[] = {12};
    unsigned long int arr3[] = {0,0,0,0,5};
	unsigned long int ret = pringle(str, arr1, 3, arr2, 1, arr3, 5);
    char* newcount = itoascii(ret);
    puts(newcount);

    return 0;
}
