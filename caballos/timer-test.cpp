#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

clock_t startTime;
void startTimer() {
    startTime = clock();
}

void endTimer(FILE* file) {
    //cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC << " seconds." << endl;
    double tiempo = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
    fprintf(file, "%7.8f\n", tiempo);
}

double endTimer() {
    //cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC << " seconds." << endl;
    double tiempo = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
    return tiempo;
}

// Implementacion
int main() {
    FILE* file = fopen("tiempos.txt","a+");
    srand(time(NULL));
    // "echo '3 0' | ./caballos"
    const char *echo_base_0 = "echo '%d %d'";
    const char *echo_base_k = "echo '%d %d ";
    const char *extra_base = "%d %d ";
    const char *program_base_0 = " | ./caballos";
    const char *program_base_k = "' | ./caballos";

    double sums[6*6];
    int counts[6*6];
    for (int i = 0; i < 6*6; i++) {
        counts[i] = 0;
        sums[i] = 0;
    }
    for (int n = 3; n <= 6; n++) {
        for (int k = 0; k <= n*n; k++) {
            double sum = 0;
            //fprintf(file, "%d ", n*n - k);
            for (int j = 0; j < 15; j++) {
                // 15 muestras aleatorias para cada combinacion de n y k
                char program[512];
                cout << n << " " << k << endl;
                //fprintf(file, "%d ", n*n - k);
                double tiempo = 0;
                if (k == 0) {
                    sprintf(program, echo_base_0, n, k);
                    strcat(program, program_base_0);
                    startTimer();
                    system(program);
                    tiempo = endTimer();
                } else {
                    sprintf(program, echo_base_k, n, k);
                    for (int k_aux = 0; k_aux < k; k_aux++) {
                        int f = rand()%n;
                        int c = rand()%n;
                        f++;
                        c++;
                        char saux[10];
                        sprintf(saux, extra_base, f, c);
                        strcat(program, saux);
                    }
                    strcat(program, program_base_k);
                    startTimer();
                    system(program);
                    tiempo = endTimer();
                }
                sum += tiempo;
            }
            counts[n*n - k] = counts[n*n - k] + 1;
            sums[n*n - k] = sums[n*n - k] + sum;
            //fprintf(file, "%7.8f\n", sum);
        }
    }

    for (int i = 0; i < 6*6; i++) {
        double aux = sums[i] / (counts[i] * 15);
        printf("%7.8f / %d = %7.8f\n", sums[i], counts[i], aux);
        fprintf(file, "%d %7.16f\n", i, aux);
    }
    fclose(file);
    return 0;
}
