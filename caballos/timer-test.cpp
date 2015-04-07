#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>  
using namespace std;

typedef pair<int, int> Coord;

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
    FILE* file = fopen("tiempos.txt","w+");
    srand(time(NULL));
    // "echo '3 0' | ./caballos"
    const char *echo_base_0 = "echo '%d %d'";
    const char *echo_base_k = "echo '%d %d ";
    const char *extra_base = "%d %d ";
    const char *program_base_0 = " | ./caballos";
    const char *program_base_k = "' | ./caballos";
    
    int n_max = 4;
    int muestras = 200;

    double sums[n_max*n_max+1];
    int counts[n_max*n_max+1];
    for (int i = 0; i <= n_max*n_max; i++) {
        counts[i] = 0;
        sums[i] = 0;
    }
    for (int n = n_max; n <= n_max; n++) {
        set<Coord> posibles;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                posibles.insert(Coord(i+1,j+1));
            }
        }
        for (int k = 0; k <= n*n; k++) {
            double sum = 0;
            //fprintf(file, "%d ", n*n - k);
            for (int j = 0; j < muestras; j++) {
                // 15 muestras aleatorias para cada combinacion de n y k
                char program[512];
                cout << n << " " << k << endl;
                fprintf(file, "%d ", n*n - k);
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
                        int index = rand()%posibles.size();
                        Coord coord = posibles.
                        int f = 
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
                //sum += tiempo;
                fprintf(file, "%7.10f\n", tiempo);
            }
            //counts[n*n - k] = counts[n*n - k] + 1;
            //sums[n*n - k] = sums[n*n - k] + sum;
        }
    }

    /*for (int i = 0; i <= n_max*n_max; i++) {
        double aux = sums[i] / (counts[i] * muestras);
        //aux = log2(aux + 2);
        printf("%7.8f / %d = %7.8f\n", sums[i], counts[i], aux);
        fprintf(file, "%d %7.16f\n", i, aux);
    }*/
    fclose(file);
    return 0;
}
