#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main() {

	for (int i = 1; i <= 800; i++) {
    		char buffer[32];
    		snprintf(buffer, sizeof(char) * 32, "file%i.in", i);
		FILE* file = fopen(buffer,"a+");
		int n = rand() % 100000 + 1; // 0 < n < 10001
        	int P = rand() % 10000; // sin restricción P
		fprintf(file, "%d %d\n", n, P);
        	while (n > 0) { // genero la instancia
			int z = rand() % 10000; // -1 < n < 10001
			int s = rand() % 1000;
			int c =	rand() % 100;		
			fprintf(file, "%d %d %d\n", z, s, c);
			n --;	
		}
		fclose(file);
	}



    	return 0;
}
