#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main() {

	int MAX = 3000;

	for (int i = 1; i <= MAX; i++) {
    		char buffer[32];
    		snprintf(buffer, sizeof(char) * 32, "file%i.in", i);
		FILE* file = fopen(buffer,"a+");
		int n = i; // 1 < n < 101
		fprintf(file, "%d\n", n);

		int c = MAX + 1; // 1< c < 10002
		int b = 2; // 1< i < 10001
		int e =	2*MAX + i + 1; // i< f <10002 		

        while (n > 0) { // genero la instancia
        	c--;
        	b++;
			e--;
			fprintf(file, "%d %d %d\n", c, b, e);
			n --;	
		}
		fclose(file);
	}



    	return 0;
}
