#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main() {

	for (int i = 1; i <= 100; i++) {
    		char buffer[32];
    		snprintf(buffer, sizeof(char) * 32, "file%i.in", i);
		FILE* file = fopen(buffer,"a+");
		int n = rand() % 100 + 1; // 1 < n < 101
		fprintf(file, "%d\n", n);
        	while (n > 0) { // genero la instancia
			int c = rand() % 100 + 1; // 1< c < 10002
			int i = rand() % 99 + 1; // 1< i < 10001
			int f =	i + 1 + rand() % 100; // i< f <10002 		
			fprintf(file, "%d %d %d\n", c, i, f);
			n --;	
		}
		fclose(file);
	}



    	return 0;
}
